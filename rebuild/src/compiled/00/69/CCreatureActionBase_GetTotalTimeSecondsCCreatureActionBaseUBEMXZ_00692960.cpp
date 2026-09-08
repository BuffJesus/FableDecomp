#include "engine/CCreatureActionBase.h"

struct CreaturePointerStorage;
struct TimeConverter { float Convert(int ticks); };
extern TimeConverter* __fastcall GetHelperFromSubB(CreaturePointerStorage* creature);

struct CCreatureActionBaseVirtuals {
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void v4(); virtual void v5(); virtual void v6();
    virtual int GetTotalTimeTicks() const;
};

float __fastcall CCreatureActionBase_GetTotalTimeSeconds(const CCreatureActionBase* action) {
    TimeConverter* converter = GetHelperFromSubB(
        reinterpret_cast<CreaturePointerStorage*>(const_cast<unsigned char*>(action->PCreature)));
    const CCreatureActionBaseVirtuals* virtuals =
        reinterpret_cast<const CCreatureActionBaseVirtuals*>(action);
    return converter->Convert(virtuals->GetTotalTimeTicks());
}
