#include "engine/CGameScriptInterface.h"  // retyped onto the PDB layout; byte parity re-verified

class CScriptThing;

struct CCharString {
    void* m_pRep;
    CCharString(const char* text, int length);
    ~CCharString();
};

struct CGameScriptReactionSlot {
    virtual void SetReactionEnabled(
        const void* entity, const CCharString* componentName, int enabled) = 0;
};

typedef void (CGameScriptReactionSlot::*ReactionSlotMFP)(
    const void* entity, const CCharString* componentName, int enabled);

extern const char kName[];

struct CGameScriptInterface_Methods : CGameScriptInterface {
    void EntitySetOpinionReactionsEnabled(
        const CScriptThing& entity, int enabled) const;
};

void CGameScriptInterface_Methods::EntitySetOpinionReactionsEnabled(
    const CScriptThing& entity, int enabled) const
{
    CCharString componentName(kName, -1);
    CGameScriptReactionSlot* target = (CGameScriptReactionSlot*)this;
    ReactionSlotMFP method;
    *(void**)&method = ((void**)this->__vftable)[0x938 / 4];
    (target->*method)((const void*)&entity, &componentName, enabled);
}
