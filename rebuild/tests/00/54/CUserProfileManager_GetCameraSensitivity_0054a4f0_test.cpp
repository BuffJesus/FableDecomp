#include <cstdio>
float __fastcall fn_0054a4f0(char* self){ if(self[0xc]) return *(float*)(self+0xe8); return *(float*)(self+0x28); }
int main(){ char b[0x200]={0}; b[0xc]=1; *(float*)(b+0xe8)=2.5f; *(float*)(b+0x28)=1.5f;
 if(fn_0054a4f0(b)!=2.5f){ printf("0054a4f0_TEST FAIL\n"); return 1; }
 b[0xc]=0; if(fn_0054a4f0(b)!=1.5f){ printf("0054a4f0_TEST FAIL\n"); return 1; }
 printf("0054a4f0_TEST PASS\n"); return 0; }