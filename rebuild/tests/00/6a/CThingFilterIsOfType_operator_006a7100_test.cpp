#include <cstdio>
struct F { int f0; int op(unsigned char* a2){ int x=(unsigned char)a2[0x10]; return x==this->f0; } };
int __fastcall fn_006a7100(F* self, int _edx, unsigned char* a2){ return self->op(a2); }
int main(){ F f; f.f0=5; unsigned char b[0x20]={0}; b[0x10]=5;
 if(fn_006a7100(&f,0,b)!=1){ printf("006a7100_TEST FAIL\n"); return 1; }
 b[0x10]=6; if(fn_006a7100(&f,0,b)!=0){ printf("006a7100_TEST FAIL\n"); return 1; }
 printf("006a7100_TEST PASS\n"); return 0; }