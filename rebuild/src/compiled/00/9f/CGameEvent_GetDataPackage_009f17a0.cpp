// CGameEvent::GetDataPackage @ 0x009F17A0
// (FableWin ?GetDataPackage@CGameEvent@@QBEXPAVCGameEventDataPackage@@@Z).
//
// Copies the written payload into a read cursor and rewinds it.

#include "fable_game_event.h"

#include <string.h>

void CGameEvent::GetDataPackage(CGameEventDataPackage* package) const
{
    memcpy(package->Data, Data, EndPos);
    package->Length = EndPos;
    package->StartPos = 0;
    package->EndPos = 0;
}