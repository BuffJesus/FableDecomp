#include "fable_system_manager.h"

class CMeshDataBank
{
public:
    void FreeDeviceSpecificResources();
};

void CMeshBankManager::OnPreDeviceReset()
{
    for (_STL::list<CMeshDataBankInfo>::iterator it = MeshDataBanks.begin();
         it != MeshDataBanks.end(); ++it)
        it->MeshDataBank->FreeDeviceSpecificResources();
}
