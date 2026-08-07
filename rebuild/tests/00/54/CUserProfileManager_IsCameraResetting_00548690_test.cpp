#include <cstdio>
char __fastcall fn_00548690(char* self){ if(self[0xc]) return self[0x12c]; return self[0x79]; }
int main(){ char b[0x200]={0}; b[0xc]=1; b[0x12c]=7; b[0x79]=3;
 if(fn_00548690(b)!=7){ printf("00548690_TEST FAIL\n"); return 1; }
 b[0xc]=0; if(fn_00548690(b)!=3){ printf("00548690_TEST FAIL\n"); return 1; }
 printf("00548690_TEST PASS\n"); return 0; }