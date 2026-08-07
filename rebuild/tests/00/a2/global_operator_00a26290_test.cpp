#include <cstdio>
struct OP { int h1(); int h2(int t); };
static int gT; int OP::h1(){ return 4; } int OP::h2(int t){ gT=t; return t+1; }
int __fastcall fn_00a26290(OP* self){ int t=self->h1(); return self->h2(t); }
int main(){ OP o; if(fn_00a26290(&o)!=5||gT!=4){ printf("00a26290_TEST FAIL\n"); return 1; }
 printf("00a26290_TEST PASS\n"); return 0; }