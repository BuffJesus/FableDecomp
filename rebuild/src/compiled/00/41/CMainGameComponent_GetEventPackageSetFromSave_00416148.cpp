// CMainGameComponent::GetEventPackageSetFromSave @ 0x00416148 (95 bytes).
// FableWin ?GetEventPackageSetFromSave@CMainGameComponent@@IAE_NPAVCGameEventPackageSet@@@Z.
// Reads one length-prefixed compressed event-package set from the replay file
// into the scratch buffer and decompresses it into packageSet. False when the
// scratch buffer is busy.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_system_manager.h"

bool CMainGameComponent::GetEventPackageSetFromSave(CGameEventPackageSet* packageSet)
{
    void* buffer;
    if (GScratchBuffer.Lock(&buffer))
    {
        long size;
        EventPackageFile.Read(&size, 4, 0);
        EventPackageFile.Read(buffer, size, 0);
        packageSet->InitFromCompressedBuffer((const unsigned char*)buffer);
        GScratchBuffer.Unlock();
        return true;
    }
    return false;
}
