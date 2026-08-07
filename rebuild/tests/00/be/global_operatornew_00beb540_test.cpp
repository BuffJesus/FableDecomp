#include "rebuild_abi.h"
#include <cstdio>
struct G { int a; int Helper(int x); };
int G::Helper(int x){ return this->a + x; }
static G gobj; G* g_beb540=&gobj;
extern "C" int FABLE_FASTCALL New_beb540(int arg){ return g_beb540->Helper(arg); }
int main(){ gobj.a=10; if(New_beb540(7)!=17){std::printf("B1");return 1;} std::printf("RB_beb540_OK\n"); return 0; }