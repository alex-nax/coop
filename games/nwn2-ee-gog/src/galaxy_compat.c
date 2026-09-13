#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct { void **vtable; void *real; } UserProxy;
static void *client_table[38],*server_table[38];
static UserProxy client={client_table,NULL},server={server_table,NULL};
static SRWLOCK lock=SRWLOCK_INIT;
static HMODULE backend;
static void log_message(const char *message) {
    HANDLE f=CreateFileA("galaxy-compat.log",FILE_APPEND_DATA,FILE_SHARE_READ|FILE_SHARE_WRITE,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
    if(f!=INVALID_HANDLE_VALUE) { DWORD n; WriteFile(f,message,(DWORD)lstrlenA(message),&n,NULL); CloseHandle(f); }
}
static LONG WINAPI exception_log(EXCEPTION_POINTERS *e) {
    if(e->ExceptionRecord->ExceptionCode!=EXCEPTION_ACCESS_VIOLATION) return EXCEPTION_CONTINUE_SEARCH;
    char message[600]; uintptr_t pointer=0,slots[4]={0}; SIZE_T got;
    ReadProcessMemory(GetCurrentProcess(),(void *)e->ContextRecord->Rax,&pointer,sizeof(pointer),&got);
    ReadProcessMemory(GetCurrentProcess(),(void *)pointer,slots,sizeof(slots),&got);
    snprintf(message,sizeof(message),"AV backend+%llx RAX=%llx RDX=%llx RCX=%llx accessed=%llx pointer=%p slots=%p,%p,%p,%p gamebase=%p\r\n",
        (unsigned long long)((uintptr_t)e->ExceptionRecord->ExceptionAddress-(uintptr_t)backend),e->ContextRecord->Rax,e->ContextRecord->Rdx,e->ContextRecord->Rcx,
        (unsigned long long)e->ExceptionRecord->ExceptionInformation[1],(void *)pointer,(void *)slots[0],(void *)slots[1],(void *)slots[2],(void *)slots[3],(void *)GetModuleHandleA(NULL));
    log_message(message);
    return EXCEPTION_CONTINUE_SEARCH;
}
static void unsupported(void) {
    log_message("Unsupported Galaxy interface method called\r\n");
    RaiseException(0xe0470001,EXCEPTION_NONCONTINUABLE,0,NULL);
}
static void sign_in(UserProxy *self,bool online,uint32_t timeout,void *listener) {
    uintptr_t vtable=0,slots[4]={0}; SIZE_T got;
    ReadProcessMemory(GetCurrentProcess(),listener,&vtable,sizeof(vtable),&got);
    ReadProcessMemory(GetCurrentProcess(),(void *)vtable,slots,sizeof(slots),&got);
    char message[300];
    snprintf(message,sizeof(message),"SignInGalaxy timeout=%u listener=%p vtable=%p slots=%p,%p,%p,%p\r\n",timeout,listener,(void *)vtable,(void *)slots[0],(void *)slots[1],(void *)slots[2],(void *)slots[3]);
    log_message(message);
    void **table=*(void ***)self->real;
    ((void (*)(void *,bool,void *))table[7])(self->real,online,listener);
}
#define FORWARD(name,offset) __attribute__((naked)) static void name(void) { __asm__("mov 8(%rcx), %rcx\nmov (%rcx), %rax\njmp *" #offset "(%rax)"); }
FORWARD(f0,0) FORWARD(f1,8) FORWARD(f2,16) FORWARD(f3,24)
FORWARD(f4,32) FORWARD(f5,40) FORWARD(f6,48) FORWARD(f8,64)
FORWARD(f7,56)
FORWARD(f9,72) FORWARD(f10,80) FORWARD(f11,88) FORWARD(f12,96)
FORWARD(f13,104) FORWARD(f14,112) FORWARD(f15,120) FORWARD(f16,128)
FORWARD(f17,136) FORWARD(f18,144) FORWARD(f19,152) FORWARD(f20,160)
FORWARD(f21,168) FORWARD(f22,176) FORWARD(f23,184) FORWARD(f24,192)
FORWARD(f25,200) FORWARD(f26,208) FORWARD(f27,216) FORWARD(f28,224)
FORWARD(f29,232) FORWARD(f30,240)
FORWARD(f31,248) FORWARD(f32,256) FORWARD(f33,264) FORWARD(f34,272)
static void *wrap_user(bool game_server) {
    AcquireSRWLockExclusive(&lock);
    if(!backend) { backend=LoadLibraryA("NemirtingasGalaxy64.dll"); AddVectoredExceptionHandler(0,exception_log); }
    if(!backend) { ReleaseSRWLockExclusive(&lock); unsupported(); return NULL; }
    FARPROC address=GetProcAddress(backend,game_server?"?GameServerUser@api@galaxy@@YAPEAVIUser@12@XZ":"?User@api@galaxy@@YAPEAVIUser@12@XZ");
    void *(*get_user)(void); memcpy(&get_user,&address,sizeof(get_user));
    if(!get_user) { ReleaseSRWLockExclusive(&lock); unsupported(); return NULL; }
    void *real=get_user();
    UserProxy *proxy=game_server?&server:&client;
    if(real && !proxy->real) {
        void *initial[]={f0,f1,f2,f3,f4,f5,f6,sign_in,f8,f9,f10,f11,f12,f13,f14,unsupported,
            f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,f25,f26,unsupported,unsupported,
            f27,f28,f29,unsupported,unsupported,unsupported,unsupported,f30};
        memcpy(proxy->vtable,initial,sizeof(initial));
    }
    proxy->real=real;
    ReleaseSRWLockExclusive(&lock);
    return real?proxy:NULL;
}
void *compat_user(void) { return wrap_user(false); }
void *compat_server_user(void) { return wrap_user(true); }
void *compat_stats(void) {
    static void *table[]={f0,f1,f2,f3,f4,f5,f6,unsupported,unsupported,unsupported,
        f7,f8,f9,f10,f11,f12,f13,f14,f15,f16,f17,f18,f19,f20,f21,f22,f23,f24,
        f25,f26,f27,f28,f29,f30,f31,f32,f33,f34};
    static UserProxy proxy={table,NULL};
    AcquireSRWLockExclusive(&lock);
    if(!backend) { backend=LoadLibraryA("NemirtingasGalaxy64.dll"); AddVectoredExceptionHandler(0,exception_log); }
    FARPROC address=GetProcAddress(backend,"?Stats@api@galaxy@@YAPEAVIStats@12@XZ");
    void *(*get_stats)(void); memcpy(&get_stats,&address,sizeof(get_stats));
    proxy.real=get_stats?get_stats():NULL;
    ReleaseSRWLockExclusive(&lock);
    return proxy.real?&proxy:NULL;
}
