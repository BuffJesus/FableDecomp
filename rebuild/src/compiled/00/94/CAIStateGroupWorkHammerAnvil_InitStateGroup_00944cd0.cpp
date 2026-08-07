struct CWorld; struct CThingAICreature;
struct S { char pad[0x14]; unsigned char b14; char pad2[0x2f]; int f44; int f48; void Init(const CWorld&, CThingAICreature&); };
void S::Init(const CWorld&, CThingAICreature&){ f44=0; b14=0; f48=0; }