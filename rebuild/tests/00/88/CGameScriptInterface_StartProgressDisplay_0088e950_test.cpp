#include <cstdio>
static int gA,gB,gC,gD; static char gS; static char gE;
void __fastcall h_88e950(char self, char edx, int a, int b, int c, int d){ gS=self; gE=edx; gA=a; gB=b; gC=c; gD=d; }
void __fastcall fn_0088e950(){ h_88e950((char)0, (char)0, 0, 1, 0, 0); }
int main(){ fn_0088e950(); if(gS!=0||gE!=0||gA!=0||gB!=1||gC!=0||gD!=0){ printf("0088e950_TEST FAIL\n"); return 1; }
 printf("0088e950_TEST PASS\n"); return 0; }