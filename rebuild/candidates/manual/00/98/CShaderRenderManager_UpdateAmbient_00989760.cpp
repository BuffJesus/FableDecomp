#include "rebuild_abi.h"

struct FAmbientVertexShaderDevice;
typedef long (__stdcall *FableSetAmbientConstantsCall)(
    FAmbientVertexShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount);

struct FAmbientVertexShaderDevice
{
    void** vtable;
};

struct FAmbientConstantLayoutView
{
    fable_u8 unknown00[0x2C];
    fable_i32 ambientRegister2C;
};

struct FAmbientVector
{
    float x;
    float y;
    float z;
    float w;
};

struct CShaderRenderManagerUpdateAmbientView
{
    fable_u8 unknown000[4];
    FAmbientConstantLayoutView* constantLayout04;
    fable_u8 unknown008[0x1A4];
    float ambientX1AC;
    float ambientY1B0;
    float ambientZ1B4;
    fable_u8 unknown1B8[0x1C];
    FAmbientVertexShaderDevice* displayDevice1D4;
    fable_u8 unknown1D8[0x200];
    fable_u32 shaderStateMask3D8;

    void UpdateAmbient();
};

void CShaderRenderManagerUpdateAmbientView::UpdateAmbient()
{
    FAmbientVector ambient;
    ambient.x = ambientX1AC;
    ambient.y = ambientY1B0;
    ambient.z = ambientZ1B4;
    ambient.w = 1.0f;
    reinterpret_cast<FableSetAmbientConstantsCall>(
        displayDevice1D4->vtable[0x5E])(
            displayDevice1D4,
            constantLayout04->ambientRegister2C,
            &ambient,
            1);
    shaderStateMask3D8 &= ~4u;
}
