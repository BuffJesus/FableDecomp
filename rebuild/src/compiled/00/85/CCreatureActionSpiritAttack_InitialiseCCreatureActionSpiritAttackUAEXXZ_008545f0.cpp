#include "engine/CCreatureAction_SpiritAttack.h"

extern int g_spiritInt;
extern float g_spiritMul;
extern "C" int __cdecl SpiritHelper();

struct CCreatureAction_SpiritAttack_Methods : CCreatureAction_SpiritAttack {
    virtual void Initialise();
};
void CCreatureAction_SpiritAttack_Methods::Initialise() {
    SpiritHelper();
    CCreatureAction_SpiritAttack* action =
        reinterpret_cast<CCreatureAction_SpiritAttack*>(this);
    action->field10 = (int)((float)g_spiritInt * g_spiritMul);
}
