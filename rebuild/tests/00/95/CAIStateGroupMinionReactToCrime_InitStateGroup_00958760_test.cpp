#include <cstdio>
struct CWorld{}; struct CThingAICreature{};
struct S { char pad[0x14]; int f14; unsigned char b18; unsigned char b19; void Init(const CWorld&, CThingAICreature&); };
void S::Init(const CWorld&, CThingAICreature&){ b18=0; f14=0; b19=0; }
int main(){ S o; o.f14=9;o.b18=9;o.b19=9; CWorld w; CThingAICreature t; o.Init(w,t);
 if(o.f14||o.b18||o.b19){ std::printf("00958760_TEST FAIL\n"); return 1;}
 std::printf("00958760_TEST PASS\n"); return 0;}