struct CWorld; struct CThingAICreature;
struct S { char pad[0x14]; int f14; unsigned char b18; unsigned char b19; void Init(const CWorld&, CThingAICreature&); };
void S::Init(const CWorld&, CThingAICreature&){ b18=0; f14=0; b19=0; }