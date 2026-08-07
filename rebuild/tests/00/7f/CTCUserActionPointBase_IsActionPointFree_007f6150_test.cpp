#include <cstdio>
struct S { char pad[0x48]; void** f48; };
static bool f(S* self){ void** p=self->f48; return *p != (void*)p; }
int main(){ void* slot; slot=&slot; S o; o.f48=(void**)&slot; if(f(&o)){ std::printf("007f6150_TEST FAIL\n"); return 1;}
 void* other=(void*)0x5; slot=other; if(!f(&o)){ std::printf("007f6150_TEST FAIL\n"); return 1;}
 std::printf("007f6150_TEST PASS\n"); return 0;}