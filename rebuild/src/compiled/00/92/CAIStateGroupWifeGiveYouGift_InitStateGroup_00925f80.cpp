struct CWorld; struct CThingAICreature;
struct CAIStateGroup_WifeGiveYouGift {
    char pad[0x14]; int f14; int f18; unsigned char b1c;
    void InitStateGroup(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_WifeGiveYouGift::InitStateGroup(const CWorld&, CThingAICreature&){ f14=0; f18=0; b1c=0; }