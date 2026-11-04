#pragma once

// FableWin/Ego_r CGameEventDispatch: four 16-byte member pointers, 87 rows.
// Pointer types precede owner definitions, preserving retail's storage model.
#pragma pointers_to_members(full_generality, virtual_inheritance)
class CWorld;
class CDisplayEngine;
class CGameEvent;
class CGameEventDispatch {
public:
    typedef void (CWorld::*Action)(const CGameEvent*);
    typedef void (CDisplayEngine::*Feedback)(const CGameEvent*);
    Action ActionFunction;
    Action LocalActionFunction;
    Feedback FeedbackFunction;
    Feedback ConfirmFunction;
};
#include "fable_game.h"
#include "fable_display_engine.h"
FABLE_STATIC_ASSERT(sizeof(CGameEventDispatch)==64);
FABLE_STATIC_ASSERT(offsetof(CGameEventDispatch,FeedbackFunction)==32);
FABLE_STATIC_ASSERT(offsetof(CGameEventDispatch,ConfirmFunction)==48);

inline const CGameEventDispatch* CMainGameComponent::PeekEventDispatchTableEntry(long type) const {
    return &EventDispatch[type];
}
