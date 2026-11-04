#include <math.h>
#include <stdio.h>
#include <string.h>
#include "fable_shader_render_manager.h"
#include "fable_shader_constant_layout.h"
#include "fable_matrix4x4_simd.h"

bool GEnableSIMD;

static unsigned int g_CallCount;
static long g_StartRegister;
static unsigned long g_VectorCount;
static const void* g_ConstantData;
static void* g_Device;

static HRESULT __stdcall CaptureSetVertexShaderConstantF(IDirect3DDevice9* device,long startRegister,const void* constantData,unsigned long vector4Count)
{
    ++g_CallCount;
    g_Device = device;
    g_StartRegister = startRegister;
    g_ConstantData = constantData;
    g_VectorCount = vector4Count;
    return 0;
}

static void MultiplyReference(float output[4][4],const float left[4][4],const float right[4][4])
{
    for (int row = 0; row != 4; ++row)
        for (int column = 0; column != 4; ++column)
            output[row][column] = right[row][0] * left[0][column] + right[row][1] * left[1][column] +
                right[row][2] * left[2][column] + right[row][3] * left[3][column];
}

static bool RunBranch(bool useSse)
{
    __declspec(align(16)) unsigned char storage[sizeof(CShaderRenderManager)];
    memset(storage, 0, sizeof(storage));
    CShaderRenderManager& manager = *reinterpret_cast<CShaderRenderManager*>(storage);
    unsigned char layoutStorage[sizeof(CVertexShaderConstantLayout)];
    memset(layoutStorage, 0, sizeof(layoutStorage));
    CVertexShaderConstantLayout& layout = *reinterpret_cast<CVertexShaderConstantLayout*>(layoutStorage);
    layout.CombinedProjectionMatrix.Offset = 11;
    void* deviceVtable[95];
    memset(deviceVtable, 0, sizeof(deviceVtable));
    deviceVtable[94] = reinterpret_cast<void*>(&CaptureSetVertexShaderConstantF);
    void* device[1] = {deviceVtable};
    manager.CurrentVSConstantLayout = &layout;
    manager.D3DDevice = reinterpret_cast<IDirect3DDevice9*>(device);
    manager.updateFlags3D8 = 0xFFFFFFFFUL;
    for (int row = 0; row != 4; ++row)
        for (int column = 0; column != 4; ++column)
        {
            manager.WorldMatrix.Data[row][column] = float(row * 4 + column + 1) * 0.125f;
            manager.ViewMatrix.Data[row][column] = float((row + 1) * (column + 2) + 3) * 0.0625f;
            manager.ProjectionMatrix.Data[row][column] = float((row == column ? 9 : 2) + row - column) * 0.25f;
        }
    float worldView[4][4];
    float expected[4][4];
    MultiplyReference(worldView, manager.WorldMatrix.Data, manager.ViewMatrix.Data);
    MultiplyReference(expected, worldView, manager.ProjectionMatrix.Data);
    g_CallCount = 0;
    GEnableSIMD = useSse;
    manager.UpdateCombinedProjectionTransform();
    if (g_CallCount != 1 || g_Device != device || g_StartRegister != 11 || g_VectorCount != 4 ||
        g_ConstantData != &manager.CombinedProjectionMatrix || manager.updateFlags3D8 != 0xFFFFFF7FUL)
        return false;
    for (int row = 0; row != 4; ++row)
        for (int column = 0; column != 4; ++column)
            if (fabs(manager.CombinedProjectionMatrix.Data[row][column] - expected[row][column]) > 0.00001f)
                return false;
    return true;
}

int main()
{
    if (!RunBranch(false) || !RunBranch(true))
    {
        printf("FABLETLC_UPDATE_COMBINED_PROJECTION_BEHAVIOR FAIL\n");
        return 1;
    }
    printf("FABLETLC_UPDATE_COMBINED_PROJECTION_BEHAVIOR PASS\n");
    return 0;
}
