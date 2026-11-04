#include <stdio.h>
#include <string.h>

#include "fable_shader_ambient.h"

static fable_u32 g_CallCount;
static FAmbientVertexShaderDevice* g_Device;
static fable_i32 g_StartRegister;
static fable_u32 g_VectorCount;
static FAmbientVector g_Ambient;
static long g_Result;

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
    return g_Result;
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

    // Failed device writes still clear the dirty bit in retail. Values are
    // forwarded without clamping; unrelated bytes must remain unchanged.
    unsigned char expected[sizeof(manager)];
    manager.ambientX1AC = -3.5f;
    manager.ambientY1B0 = 17.0f;
    manager.ambientZ1B4 = -0.0f;
    layout.ambientRegister2C = -7;
    manager.shaderStateMask3D8 = 0xA5A50004u;
    memcpy(expected, &manager, sizeof(manager));
    reinterpret_cast<CShaderRenderManagerUpdateAmbientView*>(expected)->shaderStateMask3D8 &= ~4u;
    g_Result = -1;
    manager.UpdateAmbient();
    if (g_CallCount != 2 || g_StartRegister != -7 || g_VectorCount != 1 ||
        g_Device != &device || g_Ambient.x != -3.5f || g_Ambient.y != 17.0f ||
        memcmp(&g_Ambient.z, &manager.ambientZ1B4, sizeof(float)) ||
        g_Ambient.w != 1.0f || memcmp(expected, &manager, sizeof(manager))) return 2;
    // The caller may invoke this even when the dirty bit is already clear.
    manager.UpdateAmbient();
    if (g_CallCount != 3 || memcmp(expected, &manager, sizeof(manager))) return 3;

    printf("FABLETLC_UPDATE_AMBIENT_BEHAVIOR PASS\n");
    return 0;
}
