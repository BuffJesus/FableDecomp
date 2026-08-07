#include <cstdio>
static int g=0; static void __fastcall vb_helper(void* p){ (void)p; g=1; }
struct S { char pad[0x204]; int f204; };
static void Dtor(S* self){ self->f204 = 1; vb_helper(self); }
int main(){ S o; o.f204=0; Dtor(&o); if(o.f204!=1||!g){ std::printf("0057462c_TEST FAIL\n"); return 1;}
 std::printf("0057462c_TEST PASS\n"); return 0;}