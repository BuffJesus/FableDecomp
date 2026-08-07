#include <cstdio>
struct S { unsigned int (__fastcall** vtbl)(S*); char pad[0x18-4]; unsigned int f18; int M(); };
int S::M(){ unsigned int r=this->vtbl[0x28/4](this); return (unsigned int)r < this->f18 ? 1 : 0; }
static unsigned int g_ret=0;
static unsigned int __fastcall vfn(S* s){ return g_ret; }
int main(){ unsigned int(__fastcall* tbl[0x10])(S*)={0}; tbl[0x28/4]=vfn;
 S o; o.vtbl=tbl; o.f18=10; g_ret=5; if(o.M()!=1){ std::printf("009915c0_TEST FAIL\n"); return 1;}
 g_ret=20; if(o.M()!=0){ std::printf("009915c0_TEST FAIL\n"); return 1;}
 std::printf("009915c0_TEST PASS\n"); return 0;}