#include "engine/CAIBrain.h"

struct CAIStateGroupScriptPriorityView {
    unsigned char padding0[8];
    int priority;
    unsigned char padding1[4];
    char disabled;
};

struct CAIBrain_Methods : CAIBrain {
    int GetCurrentStateGroupScriptPriority();
};

int CAIBrain_Methods::GetCurrentStateGroupScriptPriority()
{
    CAIStateGroupScriptPriorityView* state =
        *(CAIStateGroupScriptPriorityView**)&this->StateGroupList[0x10];
    return (state && !state->disabled) ? state->priority : 0;
}
