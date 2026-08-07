#include <cstdio>
static int gA,gZ,gB;
int __stdcall h_00594de2(int a, int z, int b){ gA=a; gZ=z; gB=b; return 77; }
int __stdcall fn_00594de2(int a1, int a2){ return h_00594de2(a1, 0, a2); }
int main(){ if(fn_00594de2(11,22)!=77||gA!=11||gZ!=0||gB!=22){ printf("00594de2_TEST FAIL\n"); return 1; }
 printf("00594de2_TEST PASS\n"); return 0; }