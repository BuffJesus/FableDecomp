#include "fable_game.h"

const CGameEventPackage* CGameEventPackageSet::GetPackage(unsigned long index) const
{
    return &Packages[index];
}
