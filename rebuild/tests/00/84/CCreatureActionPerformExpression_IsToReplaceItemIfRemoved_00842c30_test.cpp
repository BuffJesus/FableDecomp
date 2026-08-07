#include <cstdio>
struct Inner { char pad[0x86]; unsigned char f; };
struct S { char pad[0xA8]; Inner* p; };
static unsigned char IsToReplaceItemIfRemoved(S* self){ return self->p->f; }
int main(){ Inner inr; inr.f=0x5A; S obj; obj.p=&inr;
 if(IsToReplaceItemIfRemoved(&obj)!=0x5A){ std::printf("00842c30_TEST FAIL\n"); return 1;}
 std::printf("00842c30_TEST PASS\n"); return 0;}