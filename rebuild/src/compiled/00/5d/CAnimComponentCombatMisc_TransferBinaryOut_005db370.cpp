#include "engine/CAnimComponentCombatMisc.h"
struct CDataOutputStream {
    void WriteEBOOL(bool value);
};
struct CAnimComponentCombatMiscMethods : CAnimComponentCombatMisc {
    void TransferBinaryOut(CDataOutputStream& output) const;
};
void CAnimComponentCombatMiscMethods::TransferBinaryOut(CDataOutputStream& output) const
{
    output.WriteEBOOL(MeleeFlourish);
    output.WriteEBOOL(MeleeKnockdown);
}