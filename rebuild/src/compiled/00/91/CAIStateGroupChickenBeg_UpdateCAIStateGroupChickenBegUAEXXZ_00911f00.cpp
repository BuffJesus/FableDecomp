#include "engine/CAIStateGroup_ChickenBeg.h"

struct ChickenBegStageA; struct ChickenBegStageB; struct ChickenBegStageC;
ChickenBegStageA* __fastcall f1(void* stateGroup);
ChickenBegStageB* __fastcall f2(ChickenBegStageA* stage);
ChickenBegStageC* __fastcall f3(ChickenBegStageB* stage);
void __fastcall f4(void* stateGroup);

void __fastcall CAIStateGroup_ChickenBeg_Update(CAIStateGroup_ChickenBeg* stateGroup) {
    f3(f2(f1(stateGroup)));
    f4(stateGroup);
}
