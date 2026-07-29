#include <cstdio>
#include <cstring>
extern "C" void __fastcall CFrontEndScreen_Initialise_0054e4b0(void*,void*);
extern "C" const char FableFrontEndScreenAcceptName[]="UI_ACCEPT";
struct Name{const char*text;};struct Screen{void**vtable;};
enum E{kBase=1,kConstruct,kLookup,kPublish};static E g_e[4];static int g_n;static void*g_result,*g_published,*g_screen;static const char*g_name;
static void A(E e){g_e[g_n++]=e;}
extern "C" void __fastcall FableFrontEndScreenInitialiseBase(void*o,void*){A(kBase);g_screen=o;}
extern "C" void __fastcall FableFrontEndScreenConstructAcceptName(void*n,void*,const char*t,long l){A(kConstruct);if(l==-1)((Name*)n)->text=t;}
static void* __fastcall Lookup(void*o,void*,Name n){A(kLookup);if(o==g_screen)g_name=n.text;return g_result;}
extern "C" void __fastcall FableFrontEndScreenPublishAccept(void*o,void*p){A(kPublish);if(o==g_screen)g_published=p;}
static bool Run(bool found){unsigned char o[0x170];void*vt[4]={0};std::memset(o,0,sizeof(o));vt[3]=(void*)Lookup;*(void**)o=vt;*(unsigned long*)(o+0x30)=0x12345678;g_n=0;g_result=found?(void*)0x55667788:0;g_published=0;g_screen=0;g_name=0;CFrontEndScreen_Initialise_0054e4b0(o,0);static const E a[]={kBase,kConstruct,kLookup,kPublish};return g_n==(found?4:3)&&std::memcmp(g_e,a,g_n*sizeof(E))==0&&std::strcmp(g_name,"UI_ACCEPT")==0&&*(unsigned long*)(o+0x15c)==0x12345678&&*(void**)(o+0x168)==g_result&&g_published==(found?g_result:0);}
int main(){bool a=Run(false),b=Run(true);if(!a||!b){std::printf("FSE2_0054e4b0_TEST FAIL %d%d\n",a,b);return 1;}std::puts("FSE2_0054e4b0_TEST PASS");return 0;}
