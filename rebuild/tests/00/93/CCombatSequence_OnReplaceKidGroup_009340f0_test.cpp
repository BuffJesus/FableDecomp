#include "engine/CAIStateGroup_MinionKidGroupTag.h"
#include <cstdio>
static int re_call(CAIStateGroup_MinionKidGroupTag* self){ (void)self; return 0x1234; }
static void OnReplaceKidGroup(CAIStateGroup_MinionKidGroupTag* self){ self->FrameLastPlayedIn = re_call(self); }
int main(){ CAIStateGroup_MinionKidGroupTag obj; obj.FrameLastPlayedIn=0; OnReplaceKidGroup(&obj);
 if(obj.FrameLastPlayedIn!=0x1234){ std::printf("009340f0_TEST FAIL\n"); return 1;}
 std::printf("009340f0_TEST PASS\n"); return 0;}