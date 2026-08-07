#include <cstdio>
struct UF2 { char pad[0x2c]; int f2c; };
struct UF { char pad[8]; UF2* f8; char pad2[0x18-0xc]; int f18; };
int __fastcall fn_009fbf70(UF* self){ UF2* p=self->f8; if(p) return p->f2c - self->f18; return -1; }
int main(){ UF2 q; q.f2c=50; UF u; u.f8=&q; u.f18=20;
 if(fn_009fbf70(&u)!=30){ printf("009fbf70_TEST FAIL\n"); return 1; }
 u.f8=0; if(fn_009fbf70(&u)!=-1){ printf("009fbf70_TEST FAIL\n"); return 1; }
 printf("009fbf70_TEST PASS\n"); return 0; }