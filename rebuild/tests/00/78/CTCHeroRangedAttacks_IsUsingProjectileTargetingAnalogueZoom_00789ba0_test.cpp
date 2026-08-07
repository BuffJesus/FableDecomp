#include <cstdio>
struct Inner { char pad[0x136]; unsigned char b136; };
struct S { char pad[0x10]; Inner* f10; };
static bool f(S* self){ return self->f10->b136 ? true : false; }
int main(){ Inner inr; inr.b136=1; S o; o.f10=&inr; if(!f(&o)){ std::printf("00789ba0_TEST FAIL\n"); return 1;}
 inr.b136=0; if(f(&o)){ std::printf("00789ba0_TEST FAIL\n"); return 1;}
 std::printf("00789ba0_TEST PASS\n"); return 0;}