#include <cstdio>
char __fastcall fn_00548630(char* self){ if(self[0xc]) return self[0xf0]; return self[0x30]; }
int main(){ char b[0x200]={0}; b[0xc]=1; b[0xf0]=7; b[0x30]=3;
 if(fn_00548630(b)!=7){ printf("00548630_TEST FAIL\n"); return 1; }
 b[0xc]=0; if(fn_00548630(b)!=3){ printf("00548630_TEST FAIL\n"); return 1; }
 printf("00548630_TEST PASS\n"); return 0; }