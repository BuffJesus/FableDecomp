// CGameEventPackage::GetEvent @ 0x009F16D0
// (FableWin ?GetEvent@CGameEventPackage@@QBEPBVCGameEvent@@K@Z).

#include "fable_game_event.h"

const CGameEvent* CGameEventPackage::GetEvent(unsigned long index) const
{
    return &Events[index];
}
