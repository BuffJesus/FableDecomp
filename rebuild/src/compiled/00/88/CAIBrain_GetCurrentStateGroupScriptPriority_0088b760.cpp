#include "engine/CAIBrain.h"
#include "engine/CAIStateGroupBase.h"

struct CAIBrain_Methods : CAIBrain {
    int GetCurrentStateGroupScriptPriority();
};

int CAIBrain_Methods::GetCurrentStateGroupScriptPriority()
{
    CAIStateGroupBase* state =
        *(CAIStateGroupBase**)&this->StateGroupList[0x10];
    return (state && !state->Terminated)
        ? *(int*)state->ScriptAIPriority  // EScriptAIPriority
        : 0;
}
