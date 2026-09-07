#include "engine/CAIStateGroup_WifeGiveYouGift.h"  // retyped onto the PDB layout; byte parity re-verified
struct CWorld; struct CThingAICreature;
struct CAIStateGroup_WifeGiveYouGift_Methods : CAIStateGroup_WifeGiveYouGift {
    void InitStateGroup(const CWorld&, CThingAICreature&);
};
void CAIStateGroup_WifeGiveYouGift_Methods::InitStateGroup(const CWorld&, CThingAICreature&){ FrameToSpeakNext=0; GiftToGiveDef=0; DeliveredGiveSpeech=0; }