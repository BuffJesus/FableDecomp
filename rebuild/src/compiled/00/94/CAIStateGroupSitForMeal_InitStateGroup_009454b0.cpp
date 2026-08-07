struct CWorld; struct CThingAICreature;
struct S { char pad[0x3c]; int f3c; unsigned char b40; char pad2[3]; int f44; void Init(const CWorld&, CThingAICreature&); };
void S::Init(const CWorld&, CThingAICreature&){ b40=0; f3c=-1; f44=-1; }