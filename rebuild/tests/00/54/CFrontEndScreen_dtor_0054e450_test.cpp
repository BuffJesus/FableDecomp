#include <cstdio>
#include <cstring>
extern "C" void __fastcall CFrontEndScreen_dtor_0054e450(void*,void*);
extern "C" void* const FableFrontEndScreenDestroyVtable[]={(void*)1};
extern "C" void* const FableFrontEndScreenDestroyInterface4[]={(void*)2};
extern "C" void* const FableFrontEndScreenDestroyInterface18[]={(void*)3};
struct Control{long refs;void* callback;void* payload;};
static int g_callback,g_free,g_base;static void*g_payload,*g_freed;static bool g_baseState;
static void __fastcall ReleasePayload(void*p,void*){++g_callback;g_payload=p;}
extern "C" void __cdecl FableFrontEndScreenFreeControl(void*p){++g_free;g_freed=p;}
extern "C" void __fastcall FableFrontEndScreenDestroyBase(void*o,void*){++g_base;unsigned char*b=(unsigned char*)o;g_baseState=*(void**)b==FableFrontEndScreenDestroyVtable&&*(void**)(b+4)==FableFrontEndScreenDestroyInterface4&&*(void**)(b+0x18)==FableFrontEndScreenDestroyInterface18&&*(unsigned long*)(b+0x160)==0&&*(unsigned long*)(b+0x164)==0;}
static bool Run(int refs,bool present){unsigned char o[0x170];Control c;std::memset(o,0xcd,sizeof(o));c.refs=refs;c.callback=(void*)ReleasePayload;c.payload=(void*)0x12345678;*(void**)(o+0x160)=(void*)0x87654321;*(Control**)(o+0x164)=present?&c:0;g_callback=g_free=g_base=0;g_payload=g_freed=0;g_baseState=false;CFrontEndScreen_dtor_0054e450(o,0);return g_base==1&&g_baseState&&(!present?(g_callback==0&&g_free==0):(refs==2?(c.refs==1&&g_callback==0&&g_free==0):(c.refs==0&&g_callback==1&&g_payload==c.payload&&g_free==1&&g_freed==&c)));}
int main(){bool a=Run(0,false),b=Run(2,true),c=Run(1,true);if(!a||!b||!c){std::printf("FSE2_0054e450_TEST FAIL %d%d%d\n",a,b,c);return 1;}std::puts("FSE2_0054e450_TEST PASS");return 0;}
