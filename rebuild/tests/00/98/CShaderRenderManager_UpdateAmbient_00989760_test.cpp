#include <stdio.h>
#include <string.h>

#include "../../../src/compiled/00/98/CShaderRenderManager_UpdateAmbient_00989760.cpp"

static fable_u32 g_CallCount;
static FAmbientVertexShaderDevice* g_Device;
static fable_i32 g_StartRegister;
static fable_u32 g_VectorCount;
static FAmbientVector g_Ambient;

static long __stdcall CaptureSetAmbientConstants(
    FAmbientVertexShaderDevice* device,
    fable_i32 startRegister,
    const void* data,
    fable_u32 vectorCount)
{
    ++g_CallCount;
    g_Device = device;
    g_StartRegister = startRegister;
    g_VectorCount = vectorCount;
    g_Ambient = *static_cast<const FAmbientVector*>(data);
    return 0;
}

int main()
{
    void* vtable[0x5F];
    memset(vtable, 0, sizeof(vtable));
    vtable[0x5E] =
        reinterpret_cast<void*>(&CaptureSetAmbientConstants);

    FAmbientVertexShaderDevice device = {vtable};
    FAmbientConstantLayoutView layout;
    memset(&layout, 0, sizeof(layout));
    layout.ambientRegister2C = 13;

    CShaderRenderManagerUpdateAmbientView manager;
    memset(&manager, 0, sizeof(manager));
    manager.constantLayout04 = &layout;
    manager.ambientX1AC = 0.25f;
    manager.ambientY1B0 = 0.5f;
    manager.ambientZ1B4 = 0.75f;
    manager.displayDevice1D4 = &device;
    manager.shaderStateMask3D8 = 0xFFFFFFFFu;

    manager.UpdateAmbient();
    if (g_CallCount != 1 ||
        g_Device != &device ||
        g_StartRegister != 13 ||
        g_VectorCount != 1 ||
        g_Ambient.x != 0.25f ||
        g_Ambient.y != 0.5f ||
        g_Ambient.z != 0.75f ||
        g_Ambient.w != 1.0f ||
        manager.shaderStateMask3D8 != 0xFFFFFFFBu)
    {
        printf("FABLETLC_UPDATE_AMBIENT_BEHAVIOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_UPDATE_AMBIENT_BEHAVIOR PASS\n");
    return 0;
}
