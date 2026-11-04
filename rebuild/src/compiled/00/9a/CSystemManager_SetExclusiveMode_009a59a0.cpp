// Retail 009A59A0. Both passes and the final false result are present in retail.
#include "fable_system_manager.h"

extern "C" __declspec(dllimport) void* FABLE_STDCALL GetCurrentProcess();
extern "C" __declspec(dllimport) int FABLE_STDCALL SetPriorityClass(void*, unsigned long);

bool CSystemManager::SetExclusiveMode(bool exclusive)
{
    const bool previous = ExclusiveMode;
    if (!PreventExclusiveModeChanges)
    {
        void* process = GetCurrentProcess();
        for (int attempt = 0; attempt < 2; ++attempt)
        {
            ExclusiveMode = attempt == 0 ? exclusive : previous;
            SetPriorityClass(process, 0x20);
            if (!PInputManager.Data || PInputManager.Data->SetExclusiveMode(ExclusiveMode))
                PDisplayManager.Data->SetExclusiveMode(ExclusiveMode);
        }
    }
    return false;
}
