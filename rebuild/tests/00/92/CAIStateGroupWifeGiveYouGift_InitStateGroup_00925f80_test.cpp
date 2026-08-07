#include <cstdio>
struct CWorld{}; struct CThingAICreature{};
struct CAIStateGroup_WifeGiveYouGift { char pad[0x14]; int f14; int f18; unsigned char b1c; void InitStateGroup(const CWorld&, CThingAICreature&); };
void CAIStateGroup_WifeGiveYouGift::InitStateGroup(const CWorld&, CThingAICreature&){ f14=0; f18=0; b1c=0; }
int main(){ CAIStateGroup_WifeGiveYouGift o; o.f14=9;o.f18=9;o.b1c=9; CWorld w; CThingAICreature t; o.InitStateGroup(w,t);
 if(o.f14||o.f18||o.b1c){ std::printf("00925f80_TEST FAIL\n"); return 1;}
 std::printf("00925f80_TEST PASS\n"); return 0;}