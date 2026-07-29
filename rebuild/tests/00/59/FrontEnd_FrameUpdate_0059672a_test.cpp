#include <cstdio>
#include <cstring>
extern "C" void __fastcall FrontEnd_FrameUpdate_0059672a(void*,void*,void*);
enum Event{kGet=1,kDispatch,kEmbedded,kState};
struct Embedded{void** vtable;int calls;};
struct Argument{unsigned long header;Embedded embedded;};
struct Manager{void** vtable;};
static Event g_events[4];static int g_count;static Manager g_manager;
static void* g_managerVtable[3];static void* g_embeddedVtable[7];
static void* g_dispatched;static void* g_expectedOwner;static void* g_state;
static void* g_argumentValue;static void* g_argumentAddress;
static void Add(Event e){g_events[g_count++]=e;}
extern "C" void* __cdecl FableFrontEndFrameGetManager(){Add(kGet);return &g_manager;}
static void __fastcall Dispatch(void* m,void*,void* p){Add(kDispatch);if(m==&g_manager)g_dispatched=p;}
static void __fastcall EmbeddedUpdate(void* p,void*){Add(kEmbedded);++((Embedded*)p)->calls;}
extern "C" void __fastcall FableFrontEndFrameUpdateState(void* s,void*,void* a){Add(kState);g_state=s;g_argumentAddress=a;g_argumentValue=*(void**)a;}
int main(){
 unsigned char owner[0x40];Argument arg;std::memset(owner,0,sizeof(owner));std::memset(&arg,0,sizeof(arg));
 std::memset(g_managerVtable,0,sizeof(g_managerVtable));std::memset(g_embeddedVtable,0,sizeof(g_embeddedVtable));
 g_managerVtable[2]=(void*)Dispatch;g_embeddedVtable[6]=(void*)EmbeddedUpdate;g_manager.vtable=g_managerVtable;arg.embedded.vtable=g_embeddedVtable;
 g_count=0;g_dispatched=g_state=g_argumentValue=g_argumentAddress=0;g_expectedOwner=owner;
 FrontEnd_FrameUpdate_0059672a(owner,0,&arg);
 static const Event expected[]={kGet,kDispatch,kEmbedded,kState};
 bool pass=g_count==4&&std::memcmp(g_events,expected,sizeof(expected))==0&&g_dispatched==&arg.embedded&&arg.embedded.calls==1&&g_state==owner+0x20&&g_argumentAddress!=0&&g_argumentValue==&arg;
 if(!pass){std::printf("FSE2_0059672a_TEST FAIL count=%d dispatch=%d calls=%d state=%d arg=%d\n",g_count,g_dispatched==&arg.embedded?1:0,arg.embedded.calls,g_state==owner+0x20?1:0,g_argumentValue==&arg?1:0);return 1;}
 std::puts("FSE2_0059672a_TEST PASS");return 0;
}
