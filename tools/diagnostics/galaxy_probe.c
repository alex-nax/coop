#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

typedef struct { const char *client_id,*client_secret,*config_path,*storage_path; void *allocator,*thread_factory; const char *host; uint16_t port; } InitOptions;
typedef struct { void **vtable; } Listener;
static HMODULE galaxy;
static unsigned successes,failures;
static void destroyed(Listener *self) { (void)self; puts("Unexpected listener destruction"); }
static void auth_success(Listener *self) { (void)self; successes++; puts("OnAuthSuccess received"); }
static void auth_failure(Listener *self,uint32_t reason) { (void)self; failures++; printf("OnAuthFailure received reason=%u\n",reason); }
static void auth_lost(Listener *self) { (void)self; puts("OnAuthLost received"); }
static void ticket_success(Listener *self) { (void)self; puts("OnEncryptedAppTicketRetrieveSuccess received"); }
static void ticket_failure(Listener *self,uint32_t reason) { (void)self; printf("OnEncryptedAppTicketRetrieveFailure reason=%u\n",reason); }
static LONG WINAPI report_exception(EXCEPTION_POINTERS *e) {
    printf("EXCEPTION %08lx at Galaxy+%llx RAX=%llx RDX=%llx accessed=%llx\n",e->ExceptionRecord->ExceptionCode,
        (unsigned long long)((uintptr_t)e->ExceptionRecord->ExceptionAddress-(uintptr_t)galaxy),
        e->ContextRecord->Rax,e->ContextRecord->Rdx,(unsigned long long)e->ExceptionRecord->ExceptionInformation[1]);
    return EXCEPTION_EXECUTE_HANDLER;
}
int main(int argc,char **argv) {
    setvbuf(stdout,NULL,_IONBF,0);
    if(argc!=2 || (strcmp(argv[1],"new") && strcmp(argv[1],"old") && strcmp(argv[1],"compat"))) return 2;
    unsigned shift=!strcmp(argv[1],"compat");
    SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX);
    SetUnhandledExceptionFilter(report_exception);
    galaxy=LoadLibraryA("Galaxy64.dll");
    if(!galaxy) { printf("LoadLibrary error=%lu\n",GetLastError()); return 3; }
    void (*init)(const InitOptions *),(*tick)(void),(*shutdown_sdk)(void);
    void *(*get_user)(void);
    FARPROC address=GetProcAddress(galaxy,"?Init@api@galaxy@@YAXAEBUInitOptions@12@@Z"); memcpy(&init,&address,sizeof(init));
    address=GetProcAddress(galaxy,"?ProcessData@api@galaxy@@YAXXZ"); memcpy(&tick,&address,sizeof(tick));
    address=GetProcAddress(galaxy,"?Shutdown@api@galaxy@@YAXXZ"); memcpy(&shutdown_sdk,&address,sizeof(shutdown_sdk));
    address=GetProcAddress(galaxy,"?User@api@galaxy@@YAPEAVIUser@12@XZ"); memcpy(&get_user,&address,sizeof(get_user));
    if(!init || !tick || !shutdown_sdk || !get_user) return 4;
    InitOptions options={"58641165360420707","diagnostic-only",".",NULL,NULL,NULL,NULL,0};
    puts("Initializing isolated emulator"); init(&options);
    address=GetProcAddress(galaxy,"?Stats@api@galaxy@@YAPEAVIStats@12@XZ");
    void *(*get_stats)(void); memcpy(&get_stats,&address,sizeof(get_stats));
    void **stats_table=*(void ***)get_stats();
    for(unsigned i=0;i<32;i++) printf("STATS_SLOT %u %llx\n",i,(unsigned long long)((uintptr_t)stats_table[i]-(uintptr_t)galaxy));
    void *user=get_user(); if(!user) return 5;
    void **vtable=*(void ***)user;
    void *callbacks[]={(void *)destroyed,(void *)auth_success,(void *)auth_failure,(void *)auth_lost};
    Listener listener={callbacks};
    printf("SignInGalaxy implementation RVA=%llx using %s signature; listener=%p\n",(unsigned long long)((uintptr_t)vtable[7]-(uintptr_t)galaxy),argv[1],(void *)&listener);
    if(strcmp(argv[1],"old")) ((void (*)(void *,bool,uint32_t,Listener *))vtable[7])(user,false,15,&listener);
    else ((void (*)(void *,bool,Listener *))vtable[7])(user,false,&listener);
    for(unsigned i=0;i<100 && !successes && !failures;i++) { tick(); Sleep(10); }
    bool logged_on=((bool (*)(void *))vtable[24+shift])(user);
    printf("RESULT successes=%u failures=%u IsLoggedOn=%d\n",successes,failures,logged_on);
    if(shift) {
        void *stats=get_stats(); uint64_t user_id=0; bool unlocked=false; uint32_t unlock_time=0;
        ((void (*)(void *,const char *))stats_table[11])(stats,"COOP_ABI_PROBE");
        ((void (*)(void *,const char *,bool *,uint32_t *,uint64_t *))stats_table[10])(stats,"COOP_ABI_PROBE",&unlocked,&unlock_time,&user_id);
        printf("STATS_RESULT GetAchievement unlocked=%d\n",unlocked);
        ((void (*)(void *,const char *))stats_table[12])(stats,"COOP_ABI_PROBE");
        if(!unlocked) return 7;
    }
    void *ticket_callbacks[]={(void *)destroyed,(void *)ticket_success,(void *)ticket_failure};
    Listener ticket_listener={ticket_callbacks};
    ((void (*)(void *,const void *,uint32_t,Listener *))vtable[25+shift])(user,NULL,0,&ticket_listener);
    for(unsigned i=0;i<50;i++) { tick(); Sleep(10); }
    unsigned char ticket[1000]={0}; uint32_t ticket_size=sizeof(ticket);
    ((void (*)(void *,void *,uint32_t,uint32_t *))vtable[26+shift])(user,ticket,sizeof(ticket),&ticket_size);
    printf("GetEncryptedAppTicket length=%u\n",ticket_size);
    shutdown_sdk();
    return successes==1 && failures==0 && logged_on?0:6;
}
