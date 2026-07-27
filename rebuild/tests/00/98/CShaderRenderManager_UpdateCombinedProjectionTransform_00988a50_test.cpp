#include <math.h>
#include <stdio.h>
#include <string.h>

#define private public
#include "../../../src/compiled/00/98/CShaderRenderManager_UpdateCombinedProjectionTransform_00988a50.cpp"
#undef private

fable_u8 DAT_013d2880;

static unsigned int g_CallCount;
static unsigned int g_StartRegister;
static unsigned int g_VectorCount;
static const float* g_ConstantData;

static long __stdcall CaptureSetVertexShaderConstantF(
    IDirect3DDevice9ProjectionView*,
    unsigned int startRegister,
    const float* constantData,
    unsigned int vector4Count)
{
    ++g_CallCount;
    g_StartRegister = startRegister;
    g_ConstantData = constantData;
    g_VectorCount = vector4Count;
    return 0;
}

static void MultiplyReference(
    float output[4][4],
    const float left[4][4],
    const float right[4][4])
{
    for (int row = 0; row != 4; ++row)
    {
        for (int column = 0; column != 4; ++column)
        {
            output[row][column] =
                right[row][0] * left[0][column] +
                right[row][1] * left[1][column] +
                right[row][2] * left[2][column] +
                right[row][3] * left[3][column];
        }
    }
}

static bool RunBranch(fable_u8 useSse)
{
    __declspec(align(16)) fable_u8 storage[0x400];
    memset(storage, 0, sizeof(storage));

    fable_u8 layout[0x80];
    memset(layout, 0, sizeof(layout));
    *reinterpret_cast<long*>(layout + 0x78) = 11;

    void* deviceVtable[95];
    memset(deviceVtable, 0, sizeof(deviceVtable));
    deviceVtable[0x178 / 4] =
        reinterpret_cast<void*>(&CaptureSetVertexShaderConstantF);
    IDirect3DDevice9ProjectionView device = {deviceVtable};

    *reinterpret_cast<void**>(storage + 0x04) = layout;
    *reinterpret_cast<IDirect3DDevice9ProjectionView**>(
        storage + 0x1D4) = &device;
    *reinterpret_cast<fable_u32*>(
        storage + 0x3D8) = 0xFFFFFFFFUL;

    float (*world)[4] =
        reinterpret_cast<float (*)[4]>(storage + 0x1F0);
    float (*view)[4] =
        reinterpret_cast<float (*)[4]>(storage + 0x230);
    float (*projection)[4] =
        reinterpret_cast<float (*)[4]>(storage + 0x270);
    float (*combined)[4] =
        reinterpret_cast<float (*)[4]>(storage + 0x2F0);

    for (int row = 0; row != 4; ++row)
    {
        for (int column = 0; column != 4; ++column)
        {
            world[row][column] =
                float(row * 4 + column + 1) * 0.125f;
            view[row][column] =
                float((row + 1) * (column + 2) + 3) * 0.0625f;
            projection[row][column] =
                float((row == column ? 9 : 2) + row - column) * 0.25f;
        }
    }

    float worldView[4][4];
    float expected[4][4];
    MultiplyReference(worldView, world, view);
    MultiplyReference(expected, worldView, projection);

    g_CallCount = 0;
    g_StartRegister = 0;
    g_VectorCount = 0;
    g_ConstantData = 0;
    DAT_013d2880 = useSse;

    reinterpret_cast<CShaderRenderManagerProjectionView*>(storage)
        ->UpdateCombinedProjectionTransform();

    if (g_CallCount != 1 ||
        g_StartRegister != 11 ||
        g_VectorCount != 4 ||
        g_ConstantData != &combined[0][0] ||
        (*reinterpret_cast<fable_u32*>(storage + 0x3D8) & 0x80) != 0)
    {
        return false;
    }

    for (int row = 0; row != 4; ++row)
    {
        for (int column = 0; column != 4; ++column)
        {
            if (fabs(combined[row][column] - expected[row][column]) >
                0.00001f)
            {
                return false;
            }
        }
    }
    return true;
}

int main()
{
    if (!RunBranch(0) || !RunBranch(1))
    {
        printf(
            "FABLETLC_UPDATE_COMBINED_PROJECTION_BEHAVIOR FAIL\n");
        return 1;
    }
    printf(
        "FABLETLC_UPDATE_COMBINED_PROJECTION_BEHAVIOR PASS\n");
    return 0;
}
