#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct CVertexShaderConstantLayout
{
    fable_u32 marker;
};

struct CVertexShaderConstantLayoutVectorView
{
    CVertexShaderConstantLayout** begin00;
    CVertexShaderConstantLayout** end04;
    CVertexShaderConstantLayout** capacity08;

    fable_u32 size() const
    {
        return static_cast<fable_u32>(end04 - begin00);
    }

    void resize(
        fable_u32 count,
        CVertexShaderConstantLayout* const& value);
};

struct CShaderRenderManagerRegisterLayoutView
{
    fable_u8 unknown00[0x0C];
    CVertexShaderConstantLayoutVectorView constantLayouts0C;

    void RegisterVertexShaderConstantLayout(
        fable_i32 index,
        CVertexShaderConstantLayout* layout);
};

static CVertexShaderConstantLayout* g_ResizeStorage[8];
static int g_ResizeCalls = 0;
static fable_u32 g_ResizeCount = 0;
static CVertexShaderConstantLayout* g_ResizeValue =
    reinterpret_cast<CVertexShaderConstantLayout*>(1);

void CVertexShaderConstantLayoutVectorView::resize(
    fable_u32 count,
    CVertexShaderConstantLayout* const& value)
{
    ++g_ResizeCalls;
    g_ResizeCount = count;
    g_ResizeValue = value;
    for (fable_u32 index = 0; index < count; ++index)
    {
        g_ResizeStorage[index] = value;
    }
    begin00 = g_ResizeStorage;
    end04 = g_ResizeStorage + count;
    capacity08 = g_ResizeStorage + 8;
}

int main()
{
    CVertexShaderConstantLayout first = {1};
    CVertexShaderConstantLayout second = {2};
    CVertexShaderConstantLayout third = {3};
    CVertexShaderConstantLayout* initial[2] = {&first, &second};
    CShaderRenderManagerRegisterLayoutView manager;
    memset(&manager, 0, sizeof(manager));
    manager.constantLayouts0C.begin00 = initial;
    manager.constantLayouts0C.end04 = initial + 2;
    manager.constantLayouts0C.capacity08 = initial + 2;

    manager.RegisterVertexShaderConstantLayout(1, &third);
    if (g_ResizeCalls != 0 || initial[1] != &third)
    {
        return 1;
    }

    manager.constantLayouts0C.end04 =
        manager.constantLayouts0C.begin00 + 1;
    manager.RegisterVertexShaderConstantLayout(3, &second);
    if (
        g_ResizeCalls != 1 ||
        g_ResizeCount != 4 ||
        g_ResizeValue != 0 ||
        g_ResizeStorage[3] != &second)
    {
        return 1;
    }

    printf("FABLETLC_REGISTER_VS_LAYOUT_BEHAVIOR PASS\n");
    return 0;
}
