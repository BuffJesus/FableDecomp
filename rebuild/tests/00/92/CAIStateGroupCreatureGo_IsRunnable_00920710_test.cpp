#include <cstdio>
struct RN { void* f0; char* f4; };
static void* gN; static char gRet;
char __fastcall h_920710(void* nested){ gN=nested; return gRet; }
char __fastcall fn_00920710(RN* self){ char* p4 = self->f4; void* nested = *(void**)(p4 + 0x18); return h_920710(nested) == 0; }
int main(){ char blk[0x20]; *(void**)(blk+0x18)=(void*)0x55; RN r; r.f4=blk;
 gRet=0; if(fn_00920710(&r)!=1||gN!=(void*)0x55){ printf("00920710_TEST FAIL\n"); return 1; }
 gRet=1; if(fn_00920710(&r)!=0){ printf("00920710_TEST FAIL\n"); return 1; }
 printf("00920710_TEST PASS\n"); return 0; }