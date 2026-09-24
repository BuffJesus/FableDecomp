#include "fable_ui_manager_singleton.h"

extern "C" void* CFrontEndManager_GetInstance_0041e5f2()
{
    if (!FableFrontEndManagerInstance)
    {
        void* storage = FableFrontEndManagerAllocate(0xD0);
        FableFrontEndManagerInstance = storage ? FableFrontEndManagerConstruct(storage) : 0;
    }
    return FableFrontEndManagerInstance;
}

void* __cdecl FableUiGetManager()
{
    return CFrontEndManager_GetInstance_0041e5f2();
}
