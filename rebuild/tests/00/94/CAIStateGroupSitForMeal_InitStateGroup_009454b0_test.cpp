#include <cstdio>
struct CWorld{}; struct CThingAICreature{};
struct S { char pad[0x3c]; int f3c; unsigned char b40; char pad2[3]; int f44; void Init(const CWorld&, CThingAICreature&); };
void S::Init(const CWorld&, CThingAICreature&){ b40=0; f3c=-1; f44=-1; }
int main(){ S o; o.b40=9;o.f3c=0;o.f44=0; CWorld w; CThingAICreature t; o.Init(w,t);
 if(o.b40||o.f3c!=-1||o.f44!=-1){ std::printf("009454b0_TEST FAIL\n"); return 1;}
 std::printf("009454b0_TEST PASS\n"); return 0;}