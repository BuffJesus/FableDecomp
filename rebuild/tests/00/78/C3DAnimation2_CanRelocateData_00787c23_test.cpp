#include <cstdio>
struct CR { char pad[0x38]; char f38; };
int __fastcall fn_00787c23(CR* self){ int v = self->f38; return v == 0; }
int main(){ CR c; c.f38=0; if(fn_00787c23(&c)!=1){ printf("00787c23_TEST FAIL\n"); return 1; }
 c.f38=7; if(fn_00787c23(&c)!=0){ printf("00787c23_TEST FAIL\n"); return 1; }
 printf("00787c23_TEST PASS\n"); return 0; }