#include <cstdio>
static int g_base=0;
struct C { char pad[0x28]; int f28; int f2c; void Initialise(); };
static void __fastcall base_init(C* p){ (void)p; g_base=1; }
void C::Initialise(){ this->f28 = 0; this->f2c = 0; base_init(this); }
int main(){ C o; o.f28=9; o.f2c=9; o.Initialise();
 if(o.f28||o.f2c||!g_base){ std::printf("0096a250_TEST FAIL\n"); return 1;} std::printf("0096a250_TEST PASS\n"); return 0;}