// CMeshDataBank::FreeDeviceSpecificResources, retail 0x00A24000 (Ego_r b60250,
// ?FreeDeviceSpecificResources@CMeshDataBank@@QAEXXZ, MeshDataBank.cpp): vtable slot 1 of every
// entry in MeshesAndAnimations (+0x380).
#include "engine/CMeshDataBank.h"

// Only caller 0x00A09DA3 (walks every mesh bank): slot 1 of every entry.
void CMeshDataBank::FreeDeviceSpecificResources()
{
    for (unsigned long index = 0; index < MeshesAndAnimations.size(); ++index)
    {
        CMeshDataBankEntry* entry = MeshesAndAnimations[index];
        if (entry)
            entry->FreeDeviceSpecificResources();
    }
}
