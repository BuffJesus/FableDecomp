#include <cstdio>
char __fastcall fn_00548650(char* self){ if(self[0xc]) return self[0xf1]; return self[0x32]; }
int main(){ char b[0x200]={0}; b[0xc]=1; b[0xf1]=7; b[0x32]=3;
 if(fn_00548650(b)!=7){ printf("00548650_TEST FAIL\n"); return 1; }
 b[0xc]=0; if(fn_00548650(b)!=3){ printf("00548650_TEST FAIL\n"); return 1; }
 printf("00548650_TEST PASS\n"); return 0; }