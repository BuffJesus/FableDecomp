#include <cstdio>
struct Inner { char pad[0x85]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
static unsigned char IsToHaveEmptyHands(S* self){ return self->p->f; }
int main(){ Inner inr; inr.f=0x5A; S obj; obj.p=&inr;
 if(IsToHaveEmptyHands(&obj)!=0x5A){ std::printf("00842c20_TEST FAIL\n"); return 1;}
 std::printf("00842c20_TEST PASS\n"); return 0;}