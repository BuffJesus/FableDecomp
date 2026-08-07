#include <cstdio>
struct Inner { char pad[5]; char b5; };
struct S { char pad[0x2c]; Inner* f2c; };
static char f(S* self){ if(!self->f2c) return 0; return self->f2c->b5; }
int main(){ Inner inr; inr.b5=7; S o; o.f2c=&inr; if(f(&o)!=7){ std::printf("00cb7940_TEST FAIL\n"); return 1;}
 o.f2c=0; if(f(&o)!=0){ std::printf("00cb7940_TEST FAIL\n"); return 1;}
 std::printf("00cb7940_TEST PASS\n"); return 0;}