#include <cstdio>
typedef void*(__stdcall* PFN)(int,unsigned int,int,int);
static int a0,a3; static unsigned int a1; static int a2; static char buf[4];
static void* __stdcall vp(int x0,unsigned int x1,int x2,int x3){ a0=x0;a1=x1;a2=x2;a3=x3; return buf; }
PFN g_vp=vp;
void* __fastcall op_new(unsigned int size){ return g_vp(0, size, 0x3000, 4); }
int main(){ void* p=op_new(64);
 if(p!=buf||a0!=0||a1!=64||a2!=0x3000||a3!=4){ std::printf("00a66730_TEST FAIL\n"); return 1;}
 std::printf("00a66730_TEST PASS\n"); return 0;}