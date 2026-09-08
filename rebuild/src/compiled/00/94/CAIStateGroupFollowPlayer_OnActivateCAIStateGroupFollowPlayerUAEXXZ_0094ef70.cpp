#include "engine/CAIStateGroup_FollowPlayer.h"

struct FollowPlayerProximitySource { float Method(float scale); };
extern FollowPlayerProximitySource* __fastcall GetObj(void* stateGroup);
extern float g_bias;

void __fastcall CAIStateGroup_FollowPlayer_OnActivate(CAIStateGroup_FollowPlayer* stateGroup) {
    FollowPlayerProximitySource* source = GetObj(stateGroup);
    stateGroup->Proximity = source->Method(2.6f) + g_bias;
}
