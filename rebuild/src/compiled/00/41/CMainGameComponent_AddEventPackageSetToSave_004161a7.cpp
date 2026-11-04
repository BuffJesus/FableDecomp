// CMainGameComponent::AddEventPackageSetToSave @ 0x004161A7 (91 bytes).
// FableWin ?AddEventPackageSetToSave@CMainGameComponent@@IAEXPBVCGameEventPackageSet@@@Z.
// Compresses packageSet into the scratch buffer and appends it to the record
// file as a length-prefixed block. Does nothing when the scratch buffer is busy.
// MainGameComponent.cpp is size-optimised in retail.
#pragma optimize("s", on)

#include "fable_game.h"
#include "fable_system_manager.h"

void CMainGameComponent::AddEventPackageSetToSave(const CGameEventPackageSet* packageSet)
{
    void* buffer;
    if (GScratchBuffer.Lock(&buffer))
    {
        long size = packageSet->CompressIntoBuffer((unsigned char*)buffer);
        EventPackageFile.Write(&size, 4, 0);
        EventPackageFile.Write(buffer, size, 0);
        GScratchBuffer.Unlock();
    }
}
