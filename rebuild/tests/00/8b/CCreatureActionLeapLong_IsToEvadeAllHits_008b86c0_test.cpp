#include <cstdio>
struct S { char pad[0x114]; unsigned char b114; };
static bool f(S* self){ return self->b114==0; }
int main(){ S o; o.b114=0; if(!f(&o)){ std::printf("008b86c0_TEST FAIL\n"); return 1;}
 o.b114=5; if(f(&o)){ std::printf("008b86c0_TEST FAIL\n"); return 1;}
 std::printf("008b86c0_TEST PASS\n"); return 0;}