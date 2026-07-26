#include <stdio.h>
#include <string.h>

#include "../../../src/compiled/00/98/CShaderRenderManager_SetVertexShaderConstantLayout_0098b5e0.cpp"

void* g_CShaderConstantRangeVtable[1] = {0};

static int g_DestroyCalls = 0;
static int g_AllocateCalls = 0;
static fable_u32 g_AllocateCount = 0;
static fable_u32 g_AllocateSize = 0;
static int g_ClearCalls = 0;
static C4DVectorView* g_ClearDestination = 0;
static fable_u32 g_ClearCount = 0;
static int g_ResizeCalls = 0;
static fable_u32 g_ResizeCount = 0;
static C4DVectorView g_ResizeStorage[16];

void CShaderConstantRangeView::Destroy()
{
    ++g_DestroyCalls;
}

bool CShaderConstantRangeView::operator!=(
    const CShaderConstantRangeView& other) const
{
    return
        offset04 != other.offset04 ||
        count08 != other.count08;
}

void C4DVectorVectorView::resize(
    fable_u32 count,
    const C4DVectorView& value)
{
    ++g_ResizeCalls;
    g_ResizeCount = count;
    for (fable_u32 index = 0; index < count; ++index)
    {
        g_ResizeStorage[index] = value;
    }
    begin00 = g_ResizeStorage;
    end04 = g_ResizeStorage + count;
    capacity08 = g_ResizeStorage + 16;
}

void NVectorMath::GFClear4DVectors(
    C4DVectorView* vectors,
    fable_u32 count,
    const C4DVectorView& value)
{
    ++g_ClearCalls;
    g_ClearDestination = vectors;
    g_ClearCount = count;
    for (fable_u32 index = 0; index < count; ++index)
    {
        vectors[index] = value;
    }
}

void CShaderRenderManagerSetLayoutView::AllocateLights(
    fable_u32 count,
    fable_u32 lightSize)
{
    ++g_AllocateCalls;
    g_AllocateCount = count;
    g_AllocateSize = lightSize;
    allocatedLightCount170 = count;
    allocatedLightSize174 = lightSize;
}

static void ResetCalls()
{
    g_DestroyCalls = 0;
    g_AllocateCalls = 0;
    g_AllocateCount = 0;
    g_AllocateSize = 0;
    g_ClearCalls = 0;
    g_ClearDestination = 0;
    g_ClearCount = 0;
    g_ResizeCalls = 0;
    g_ResizeCount = 0;
}

int main()
{
    CShaderRenderManagerSetLayoutView manager;
    CVertexShaderConstantLayoutView oldLayout;
    CVertexShaderConstantLayoutView newLayout;
    CVertexShaderConstantLayoutView* layouts[1];
    CLightInfoView lights[3];
    C4DVectorView lightBuffer[12];
    C4DVectorView existingGlobals[2];

    memset(&manager, 0, sizeof(manager));
    memset(&oldLayout, 0, sizeof(oldLayout));
    memset(&newLayout, 0, sizeof(newLayout));
    manager.currentLayout04 = &oldLayout;
    manager.currentLayoutIndex08 = 7;
    manager.updateFlags3D8 = 0x40000000;
    manager.SetVertexShaderConstantLayout(-1);
    if (
        manager.currentLayout04 != &manager.nullLayout18 ||
        manager.currentLayoutIndex08 != -1 ||
        manager.updateFlags3D8 != 0x40000000)
    {
        return 1;
    }

    ResetCalls();
    memset(&manager, 0, sizeof(manager));
    layouts[0] = &newLayout;
    manager.layouts0C.begin00 = layouts;
    manager.layouts0C.end04 = layouts + 1;
    manager.layouts0C.capacity08 = layouts + 1;
    manager.SetVertexShaderConstantLayout(0);
    if (
        manager.currentLayout04 != &newLayout ||
        manager.currentLayoutIndex08 != 0 ||
        manager.updateFlags3D8 != 0 ||
        g_DestroyCalls != 0 ||
        g_AllocateCalls != 0 ||
        g_ClearCalls != 0 ||
        g_ResizeCalls != 0)
    {
        return 1;
    }

    ResetCalls();
    memset(&manager, 0, sizeof(manager));
    memset(&oldLayout, 0, sizeof(oldLayout));
    memset(&newLayout, 0, sizeof(newLayout));
    memset(lights, 0, sizeof(lights));
    memset(lightBuffer, 0xCD, sizeof(lightBuffer));

    oldLayout.worldMatrix8C.offset04 = 1;
    newLayout.worldMatrix8C.offset04 = 2;
    oldLayout.fogTransform80.offset04 = 3;
    newLayout.fogTransform80.offset04 = 4;
    oldLayout.ambient28.offset04 = 5;
    newLayout.ambient28.offset04 = 6;
    oldLayout.fogColour34.offset04 = 7;
    newLayout.fogColour34.offset04 = 8;
    oldLayout.cameraPos40.offset04 = 9;
    newLayout.cameraPos40.offset04 = 10;
    oldLayout.lightSize4C = 2;
    newLayout.lightSize4C = 4;
    oldLayout.lightArray50.offset04 = 11;
    newLayout.lightArray50.offset04 = 12;
    newLayout.lightArray50.count08 = 12;
    oldLayout.lightGlobals5C.offset04 = 13;
    oldLayout.lightGlobals5C.count08 = 2;
    newLayout.lightGlobals5C.offset04 = 14;
    newLayout.lightGlobals5C.count08 = 5;
    oldLayout.combinedProjectionMatrix74.offset04 = 15;
    newLayout.combinedProjectionMatrix74.offset04 = 16;

    layouts[0] = &newLayout;
    manager.currentLayout04 = &oldLayout;
    manager.currentLayoutIndex08 = 9;
    manager.layouts0C.begin00 = layouts;
    manager.layouts0C.end04 = layouts + 1;
    manager.layouts0C.capacity08 = layouts + 1;
    manager.lights164 = lights;
    manager.lightBuffer168 = lightBuffer;
    manager.lightGlobals194.begin00 = existingGlobals;
    manager.lightGlobals194.end04 = existingGlobals + 2;
    manager.lightGlobals194.capacity08 = existingGlobals + 2;
    manager.updateFlags3D8 = 0x80000000;

    manager.SetVertexShaderConstantLayout(0);

    const fable_u32 expectedFlags =
        0x80000000u |
        4u |
        0x10u |
        0x6000u |
        0x20000u |
        8u |
        0x700u |
        0x400u |
        0x80u;
    if (
        manager.currentLayout04 != &newLayout ||
        manager.currentLayoutIndex08 != 0 ||
        manager.updateFlags3D8 != expectedFlags ||
        manager.lightCount16C != 3 ||
        g_DestroyCalls != 1 ||
        g_AllocateCalls != 1 ||
        g_AllocateCount != 3 ||
        g_AllocateSize != 4 ||
        !lights[0].needsUpdate35 ||
        !lights[1].needsUpdate35 ||
        !lights[2].needsUpdate35 ||
        g_ClearCalls != 1 ||
        g_ClearDestination != lightBuffer ||
        g_ClearCount != 12 ||
        lightBuffer[0].x != 0.0f ||
        lightBuffer[0].y != 0.0f ||
        lightBuffer[0].z != 0.0f ||
        lightBuffer[0].w != 1.0f ||
        g_ResizeCalls != 1 ||
        g_ResizeCount != 5 ||
        g_ResizeStorage[0].x != 0.0f ||
        g_ResizeStorage[0].y != 0.0f ||
        g_ResizeStorage[0].z != 0.0f ||
        g_ResizeStorage[0].w != 1.0f)
    {
        return 1;
    }

    printf("FABLETLC_SET_VS_LAYOUT_BEHAVIOR PASS\n");
    return 0;
}
