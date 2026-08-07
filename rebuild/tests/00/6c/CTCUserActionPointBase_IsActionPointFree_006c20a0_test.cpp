#include <cstdio>
struct S { char pad[0x14]; void** f14; };
static bool f(S* self){ void** p=self->f14; return *p != (void*)p; }
int main(){ void* slot; slot=&slot; S o; o.f14=(void**)&slot; if(f(&o)){ std::printf("006c20a0_TEST FAIL\n"); return 1;}
 void* other=(void*)0x5; slot=other; if(!f(&o)){ std::printf("006c20a0_TEST FAIL\n"); return 1;}
 std::printf("006c20a0_TEST PASS\n"); return 0;}