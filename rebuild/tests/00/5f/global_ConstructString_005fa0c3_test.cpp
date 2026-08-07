#include <cstdio>
static int gA,gZ,gB;
int __stdcall h_005fa0c3(int a, int z, int b){ gA=a; gZ=z; gB=b; return 77; }
int __stdcall fn_005fa0c3(int a1, int a2){ return h_005fa0c3(a1, 0, a2); }
int main(){ if(fn_005fa0c3(11,22)!=77||gA!=11||gZ!=0||gB!=22){ printf("005fa0c3_TEST FAIL\n"); return 1; }
 printf("005fa0c3_TEST PASS\n"); return 0; }