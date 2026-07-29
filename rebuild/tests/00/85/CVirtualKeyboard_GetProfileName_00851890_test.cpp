#include <cstdio>
#include <cstring>
extern "C" void* __fastcall CVirtualKeyboard_GetProfileName_00851890(void*,void*,void*);
struct Keyboard{unsigned char pad[0xc];void* edit;};
struct Edit{void** vtable;};
struct Output{long start,end;};
static void* g_vtable[0x23c/4];static Edit g_edit;static const unsigned short* g_text;static int g_destroy;
static void __fastcall GetValue(void*,void*,void*out){*(const unsigned short**)out=g_text;}
extern "C" long __fastcall FableProfileNameLength(void*o,void*){const unsigned short*p=*(const unsigned short**)o;long n=0;while(p[n])++n;return n;}
extern "C" const unsigned short* __fastcall FableProfileNameData(void*o,void*){return *(const unsigned short**)o;}
extern "C" void __fastcall FableProfileNameSubstring(void*,void*,void*out,long s,long e){((Output*)out)->start=s;((Output*)out)->end=e;}
extern "C" void __fastcall FableProfileNameDestroy(void*,void*){++g_destroy;}
static bool Run(const unsigned short*t,long s,long e){Keyboard k;Output o={-1,-1};std::memset(&k,0,sizeof(k));std::memset(g_vtable,0,sizeof(g_vtable));g_vtable[0x238/4]=(void*)GetValue;g_edit.vtable=g_vtable;k.edit=&g_edit;g_text=t;g_destroy=0;void*r=CVirtualKeyboard_GetProfileName_00851890(&k,0,&o);return r==&o&&o.start==s&&o.end==e&&g_destroy==1;}
int main(){static const unsigned short a[]={0};static const unsigned short b[]={' ',' ','A','l','i','c','e',' ',' ',0};static const unsigned short c[]={' ',' ',' ',0};static const unsigned short d[]={'B','o','b',0};bool x=Run(a,0,0),y=Run(b,2,7),z=Run(c,3,3),w=Run(d,0,3);if(!x||!y||!z||!w){std::printf("FSE2_00851890_TEST FAIL %d%d%d%d\n",x,y,z,w);return 1;}std::puts("FSE2_00851890_TEST PASS");return 0;}
