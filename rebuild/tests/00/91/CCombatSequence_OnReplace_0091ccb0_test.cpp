#include "engine/CAIStateGroup_MinionReactToNoise.h"
#include <cstdio>
static int re_call(CAIStateGroup_MinionReactToNoise* self){ (void)self; return 0x1234; }
static void OnReplace(CAIStateGroup_MinionReactToNoise* self){ self->FrameOfLastInteraction = re_call(self); }
int main(){ CAIStateGroup_MinionReactToNoise obj; obj.FrameOfLastInteraction=0; OnReplace(&obj);
 if(obj.FrameOfLastInteraction!=0x1234){ std::printf("0091ccb0_TEST FAIL\n"); return 1;}
 std::printf("0091ccb0_TEST PASS\n"); return 0;}