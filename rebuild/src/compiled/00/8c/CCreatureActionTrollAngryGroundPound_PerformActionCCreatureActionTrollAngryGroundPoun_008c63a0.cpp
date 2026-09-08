#include "engine/CCreatureAction_TrollAngryGroundPound.h"

struct CTrollGroundPoundState {
    unsigned char padding[0x1c8];
    unsigned char flags;
};

struct CTrollGroundPoundActionState {
    CTrollGroundPoundState* GetState();
};

struct CCreatureAction_TrollAngryGroundPound_Methods : CCreatureAction_TrollAngryGroundPound {
    void DoBase();
    void PerformAction();
};

void CCreatureAction_TrollAngryGroundPound_Methods::PerformAction()
{
    this->DoBase();
    CTrollGroundPoundActionState* actionState = (CTrollGroundPoundActionState*)&this->sub;
    CTrollGroundPoundState* state = actionState->GetState();
    state->flags |= 0x40;
}
