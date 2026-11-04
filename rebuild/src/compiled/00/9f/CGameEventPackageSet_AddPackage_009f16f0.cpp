#include "fable_game.h"

void CGameEventPackageSet::AddPackage(const CGameEventPackage* package)
{
    Packages[NoPackages] = *package;
    ++NoPackages;
}
