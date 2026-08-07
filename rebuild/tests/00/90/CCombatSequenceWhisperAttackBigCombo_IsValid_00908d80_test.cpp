#include <cstdio>
struct Inner { char pad[0x6c]; unsigned char b6c; };
struct S { char pad[0x20]; Inner* f20; };
static unsigned char f(S* self){ unsigned char b=self->f20->b6c; b>>=5; b=~b; b&=1; return b; }
int main(){ Inner inr; inr.b6c=0x00; S o; o.f20=&inr; if(f(&o)!=1){ std::printf("00908d80_TEST FAIL\n"); return 1;}
 inr.b6c=0x20; if(f(&o)!=0){ std::printf("00908d80_TEST FAIL\n"); return 1;}
 std::printf("00908d80_TEST PASS\n"); return 0;}