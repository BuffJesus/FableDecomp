#include <cstdio>
struct CWorld{}; struct CThingAICreature{};
struct S { char pad[0x14]; unsigned char b14; char pad2[0x2f]; int f44; int f48; void Init(const CWorld&, CThingAICreature&); };
void S::Init(const CWorld&, CThingAICreature&){ f44=0; b14=0; f48=0; }
int main(){ S o; o.b14=9;o.f44=9;o.f48=9; CWorld w; CThingAICreature t; o.Init(w,t);
 if(o.b14||o.f44||o.f48){ std::printf("00944cd0_TEST FAIL\n"); return 1;}
 std::printf("00944cd0_TEST PASS\n"); return 0;}