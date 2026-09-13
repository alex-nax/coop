#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct { uintptr_t rva; BYTE expected, original; int active, terminal; const char *name; } Probe;
static Probe probes[] = {
    {0x5f6060,0x48,0,0,0,"GOG multiplayer preparation"},
    {0x5f60a7,0xff,0,0,0,"GOG IUser virtual slot 0xc8 before call"},
    {0x5f60ad,0x84,0,0,0,"GOG IUser virtual slot 0xc8 returned"},
    {0x5f60b1,0xc7,0,0,1,"GOG online check rejected: message 235403"},
    {0x5f69ed,0xc7,0,0,1,"Platform authentication timeout: message 235403"},
    {0x5f75af,0xc7,0,0,1,"Platform authentication callback rejected: message 235403"},
    {0x5fe50d,0x84,0,0,0,"AML setup returned"},
    {0x5fe521,0x41,0,0,1,"AML setup rejected: message 235403"},
    {0x5fe64f,0x84,0,0,0,"AML login start returned"},
    {0x5fe663,0x41,0,0,1,"AML login start rejected: message 235403"},
    {0x5fe818,0x41,0,0,1,"AML login request rejected: message 235403"},
    {0x10ce75,0xc7,0,0,1,"Service callback A: message 235403"},
    {0x11b5f6,0xc7,0,0,1,"Service callback B: message 235403"},
    {0x11ba96,0xc7,0,0,1,"Service callback C: message 235403"}
};
static HANDLE process;
static uintptr_t base;
static FILE *logfile;

static int read_at(uintptr_t address, void *data, SIZE_T size) {
    SIZE_T received=0;
    return ReadProcessMemory(process,(void *)address,data,size,&received) && received==size;
}
static int write_byte(uintptr_t address, BYTE value) {
    DWORD old=0,ignored=0; SIZE_T written=0;
    if(!VirtualProtectEx(process,(void *)address,1,PAGE_EXECUTE_READWRITE,&old)) return 0;
    BOOL ok=WriteProcessMemory(process,(void *)address,&value,1,&written);
    VirtualProtectEx(process,(void *)address,1,old,&ignored);
    FlushInstructionCache(process,(void *)address,1);
    return ok && written==1;
}
static void restore(void) {
    for(size_t i=0;i<sizeof(probes)/sizeof(probes[0]);i++) if(probes[i].active) {
        if(write_byte(base+probes[i].rva,probes[i].original)) probes[i].active=0;
        else fprintf(logfile,"ERROR restoring RVA %llx: %lu\n",(unsigned long long)probes[i].rva,GetLastError());
    }
}
static void snapshot(CONTEXT *c) {
    fprintf(logfile,"RAX=%llx RCX=%llx RDX=%llx RDI=%llx R14=%llx AL=%u\n",c->Rax,c->Rcx,c->Rdx,c->Rdi,c->R14,(unsigned)(c->Rax&255));
    uintptr_t core=0,eos=0;
    read_at(base+0x1dc43c8,&core,sizeof(core));
    read_at(base+0x1db6328,&eos,sizeof(eos));
    fprintf(logfile,"AML core pointer=%llx EOS platform pointer=%llx\n",(unsigned long long)core,(unsigned long long)eos);
    uintptr_t stack[64]={0};
    if(read_at(c->Rsp,stack,sizeof(stack))) {
        fprintf(logfile,"Candidate game return addresses:");
        for(unsigned i=0;i<64;i++) if(stack[i]>=base+0x1000 && stack[i]<base+0x1800000) fprintf(logfile," stack+%x=nwn2+%llx",i*8,(unsigned long long)(stack[i]-base));
        fputc('\n',logfile);
    }
}
int main(int argc,char **argv) {
    int verify_only=(argc==4 && strcmp(argv[1],"--verify")==0);
    int bypass_once=(argc==4 && strcmp(argv[1],"--bypass-once")==0);
    if(argc!=3 && !verify_only && !bypass_once) return 2;
    int shifted=verify_only || bypass_once;
    DWORD pid=(DWORD)strtoul(argv[shifted?2:1],NULL,10);
    logfile=fopen(argv[shifted?3:2],"w"); if(!logfile) return 3;
    setvbuf(logfile,NULL,_IONBF,0);
    process=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);
    if(!process) { fprintf(logfile,"OpenProcess failed: %lu\n",GetLastError()); return 4; }
    char path[MAX_PATH]; DWORD pathlen=MAX_PATH;
    if(!QueryFullProcessImageNameA(process,0,path,&pathlen) || !strstr(path,"\\content\\NWN2EEGOG\\Instance") || _stricmp(path+strlen(path)-8,"nwn2.exe")) {
        fprintf(logfile,"Refusing unexpected target\n"); CloseHandle(process); return 5;
    }
    fprintf(logfile,"Target PID %lu %s\n",pid,path);
    if(verify_only) {
        HANDLE modules=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE|TH32CS_SNAPMODULE32,pid);
        MODULEENTRY32 item={0}; item.dwSize=sizeof(item);
        if(modules!=INVALID_HANDLE_VALUE && Module32First(modules,&item)) do {
            if(_stricmp(item.szModule,"nwn2.exe")==0) base=(uintptr_t)item.modBaseAddr;
            if(_stricmp(item.szModule,"Galaxy64.dll")==0 || _stricmp(item.szModule,"EOSSDK-Win64-Shipping.dll")==0)
                fprintf(logfile,"Module %s base=%llx\n",item.szExePath,(unsigned long long)(uintptr_t)item.modBaseAddr);
        } while(Module32Next(modules,&item));
        if(modules!=INVALID_HANDLE_VALUE) CloseHandle(modules);
        BOOL attached=FALSE; int good=base && CheckRemoteDebuggerPresent(process,&attached) && !attached;
        fprintf(logfile,"Debugger attached=%d\n",attached);
        for(size_t i=0;i<sizeof(probes)/sizeof(probes[0]);i++) {
            BYTE actual=0; int ok=read_at(base+probes[i].rva,&actual,1) && actual==probes[i].expected;
            fprintf(logfile,"Probe +%llx restored=%d\n",(unsigned long long)probes[i].rva,ok);
            if(!ok) good=0;
        }
        fprintf(logfile,"Verification passed=%d\n",good);
        CloseHandle(process); fclose(logfile); return good?0:7;
    }
    if(!DebugActiveProcess(pid)) { fprintf(logfile,"Attach failed: %lu\n",GetLastError()); CloseHandle(process); return 6; }
    DebugSetProcessKillOnExit(FALSE);
    int installed=0,finished=0,exited=0,initial=1,timeout_break=0;
    ULONGLONG start=GetTickCount64();
    while(!finished) {
        if(!timeout_break && GetTickCount64()-start>=180000) {
            timeout_break=1;
            if(!DebugBreakProcess(process)) { fprintf(logfile,"Timeout interrupt failed: %lu\n",GetLastError()); break; }
        }
        DEBUG_EVENT e={0};
        if(!WaitForDebugEvent(&e,200)) continue;
        DWORD status=DBG_CONTINUE;
        if(e.dwDebugEventCode==CREATE_PROCESS_DEBUG_EVENT) {
            base=(uintptr_t)e.u.CreateProcessInfo.lpBaseOfImage;
            fprintf(logfile,"Image base=%llx\n",(unsigned long long)base);
            if(e.u.CreateProcessInfo.hFile) CloseHandle(e.u.CreateProcessInfo.hFile);
            CloseHandle(e.u.CreateProcessInfo.hThread);
            CloseHandle(e.u.CreateProcessInfo.hProcess);
        } else if(e.dwDebugEventCode==CREATE_THREAD_DEBUG_EVENT) {
            CloseHandle(e.u.CreateThread.hThread);
        } else if(e.dwDebugEventCode==LOAD_DLL_DEBUG_EVENT) {
            if(e.u.LoadDll.hFile) CloseHandle(e.u.LoadDll.hFile);
        } else if(e.dwDebugEventCode==EXIT_PROCESS_DEBUG_EVENT) {
            fprintf(logfile,"Game exited: %lu\n",e.u.ExitProcess.dwExitCode); exited=finished=1;
        } else if(e.dwDebugEventCode==EXCEPTION_DEBUG_EVENT) {
            DWORD code=e.u.Exception.ExceptionRecord.ExceptionCode;
            uintptr_t address=(uintptr_t)e.u.Exception.ExceptionRecord.ExceptionAddress;
            if(code==EXCEPTION_BREAKPOINT && timeout_break) {
                fprintf(logfile,"Timeout; removing probes while stopped.\n"); finished=1;
            } else if(code==EXCEPTION_BREAKPOINT && initial) {
                initial=0;
                for(size_t i=0;i<sizeof(probes)/sizeof(probes[0]);i++) {
                    if(!read_at(base+probes[i].rva,&probes[i].original,1) || probes[i].original!=probes[i].expected) {
                        fprintf(logfile,"Instruction mismatch at %llx: expected %02x got %02x\n",(unsigned long long)probes[i].rva,probes[i].expected,probes[i].original); finished=1; break;
                    }
                }
                if(!finished) for(size_t i=0;i<sizeof(probes)/sizeof(probes[0]);i++) {
                    if(!write_byte(base+probes[i].rva,0xcc)) { finished=1; break; }
                    probes[i].active=1;
                }
                if(!finished) { installed=1; fprintf(logfile,"READY: Select Multiplayer on the right-hand game.\n"); }
            } else if(code==EXCEPTION_BREAKPOINT && installed) {
                int found=0;
                for(size_t i=0;i<sizeof(probes)/sizeof(probes[0]);i++) if(address==base+probes[i].rva) {
                    found=1;
                    HANDLE thread=OpenThread(THREAD_GET_CONTEXT|THREAD_SET_CONTEXT,FALSE,e.dwThreadId);
                    CONTEXT c={0}; c.ContextFlags=CONTEXT_FULL;
                    if(thread && GetThreadContext(thread,&c)) {
                        fprintf(logfile,"HIT +%llx: %s\n",(unsigned long long)probes[i].rva,probes[i].name);
                        snapshot(&c);
                        if(probes[i].rva==0x5f60a7) {
                            uintptr_t target=0; read_at(c.Rdx+0xc8,&target,sizeof(target));
                            fprintf(logfile,"IUser slot 0xc8 target=%llx\n",(unsigned long long)target);
                        }
                        if(write_byte(address,probes[i].original)) {
                            probes[i].active=0; c.Rip=address;
                            if(bypass_once && probes[i].rva==0x5f60ad) {
                                c.Rax=(c.Rax & ~255ULL)|1;
                                fprintf(logfile,"ONE-TIME TEST: set IsLoggedOn return AL=1 in this thread; executable file unchanged.\n");
                            }
                            if(!SetThreadContext(thread,&c)) fprintf(logfile,"ERROR SetThreadContext: %lu\n",GetLastError());
                        }
                    }
                    if(thread) CloseHandle(thread);
                    if(probes[i].terminal) finished=1;
                    break;
                }
                if(!found) status=DBG_EXCEPTION_NOT_HANDLED;
            } else {
                fprintf(logfile,"Exception %08lx at %llx firstChance=%lu\n",code,(unsigned long long)address,e.u.Exception.dwFirstChance);
                status=DBG_EXCEPTION_NOT_HANDLED;
            }
        }
        if(finished && !exited) restore();
        ContinueDebugEvent(e.dwProcessId,e.dwThreadId,status);
    }
    if(!exited) { restore(); fprintf(logfile,"Detached=%d; game left running.\n",DebugActiveProcessStop(pid)); }
    CloseHandle(process); fclose(logfile); return 0;
}
