#include "rebuild_abi.h"

struct FableShaderDevice;
typedef long (__stdcall *FableSetVSConstantsCall)(
    FableShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount);

struct FableShaderDevice
{
    void** vtable;
};

struct CShaderRenderManagerSetVSConstantsView
{
    fable_u8 unknown000[0x1D4];
    FableShaderDevice* displayDevice1D4;

    void SetVSConstants(
        fable_i32 startRegister,
        const void* data,
        fable_u32 vectorCount);
};

void CShaderRenderManagerSetVSConstantsView::SetVSConstants(
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount)
{
    if (vectorCount != 0)
    {
        reinterpret_cast<FableSetVSConstantsCall>(
            displayDevice1D4->vtable[0x5E])(
                displayDevice1D4,
                startRegister,
                data,
                vectorCount);
    }
}
