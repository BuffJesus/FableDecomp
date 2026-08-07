#include <cstdio>
static int g_base=0;
struct C { char pad[0x28]; char f28; char f29; void AddToInterface(); };
static void __fastcall base_ati(C* p){ (void)p; g_base=1; }
void C::AddToInterface(){ this->f28 = 0; this->f29 = 0; base_ati(this); }
int main(){ C o; o.f28=9; o.f29=9; o.AddToInterface();
 if(o.f28||o.f29||!g_base){ std::printf("00688a60_TEST FAIL\n"); return 1;} std::printf("00688a60_TEST PASS\n"); return 0;}