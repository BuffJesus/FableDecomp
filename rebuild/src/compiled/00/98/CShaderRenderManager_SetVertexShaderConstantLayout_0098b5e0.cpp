#include "rebuild_abi.h"

// Retail constant-layout transition and light-buffer invalidation view.
struct CShaderConstantRangeView
{
    void* vtable00;
    fable_i32 offset04;
    fable_u32 count08;

    void Destroy();
    bool operator!=(const CShaderConstantRangeView& other) const;
};

extern void* g_CShaderConstantRangeVtable[];

struct CVertexShaderConstantLayoutView
{
    void* vtable00;
    CShaderConstantRangeView reserved04;
    CShaderConstantRangeView user10;
    CShaderConstantRangeView compression1C;
    CShaderConstantRangeView ambient28;
    CShaderConstantRangeView fogColour34;
    CShaderConstantRangeView cameraPos40;
    fable_u32 lightSize4C;
    CShaderConstantRangeView lightArray50;
    CShaderConstantRangeView lightGlobals5C;
    CShaderConstantRangeView lightAttenuations68;
    CShaderConstantRangeView combinedProjectionMatrix74;
    CShaderConstantRangeView fogTransform80;
    CShaderConstantRangeView worldMatrix8C;
    fable_u8 unknown98[0xB4];
};

FABLE_STATIC_ASSERT(sizeof(CVertexShaderConstantLayoutView) == 0x14C);

struct CVertexShaderConstantLayoutVectorView
{
    CVertexShaderConstantLayoutView** begin00;
    CVertexShaderConstantLayoutView** end04;
    CVertexShaderConstantLayoutView** capacity08;
};

struct C4DVectorView
{
    float x;
    float y;
    float z;
    float w;
};

union CSetLayoutLocal
{
    CShaderConstantRangeView range;
    C4DVectorView vector;
};

struct C4DVectorVectorView
{
    C4DVectorView* begin00;
    C4DVectorView* end04;
    C4DVectorView* capacity08;

    fable_u32 size() const
    {
        return static_cast<fable_u32>(end04 - begin00);
    }

    void resize(fable_u32 count, const C4DVectorView& value);
};

struct CLightInfoView
{
    fable_u8 unknown00[0x35];
    bool needsUpdate35;
    fable_u8 unknown36[2];
};

FABLE_STATIC_ASSERT(sizeof(CLightInfoView) == 0x38);

namespace NVectorMath
{
    void FABLE_CDECL GFClear4DVectors(
        C4DVectorView* vectors,
        fable_u32 count,
        const C4DVectorView& value);
}

struct CShaderRenderManagerSetLayoutView
{
    void* vtable00;
    CVertexShaderConstantLayoutView* currentLayout04;
    fable_i32 currentLayoutIndex08;
    CVertexShaderConstantLayoutVectorView layouts0C;
    CVertexShaderConstantLayoutView nullLayout18;
    CLightInfoView* lights164;
    C4DVectorView* lightBuffer168;
    fable_u32 lightCount16C;
    fable_u32 allocatedLightCount170;
    fable_u32 allocatedLightSize174;
    fable_u8 lightPool178[0x1C];
    C4DVectorVectorView lightGlobals194;
    fable_u8 unknown1A0[0x238];
    fable_u32 updateFlags3D8;

    void AllocateLights(fable_u32 count, fable_u32 lightSize);
    void SetVertexShaderConstantLayout(fable_i32 index);
};

FABLE_STATIC_ASSERT(
    sizeof(CShaderRenderManagerSetLayoutView) == 0x3DC);

void CShaderRenderManagerSetLayoutView::
SetVertexShaderConstantLayout(fable_i32 index)
{
    CSetLayoutLocal local;

    if (index == -1)
    {
        currentLayoutIndex08 = -1;
        currentLayout04 = &nullLayout18;
        return;
    }

    CVertexShaderConstantLayoutView* newLayout =
        layouts0C.begin00[index];

    if (currentLayout04 != 0)
    {
        updateFlags3D8 |= 4;

        if (
            currentLayout04->worldMatrix8C.offset04 !=
                newLayout->worldMatrix8C.offset04 ||
            currentLayout04->worldMatrix8C.count08 !=
                newLayout->worldMatrix8C.count08)
        {
            updateFlags3D8 |= 0x10;
        }

        if (
            currentLayout04->fogTransform80.offset04 !=
                newLayout->fogTransform80.offset04 ||
            currentLayout04->fogTransform80.count08 !=
                newLayout->fogTransform80.count08)
        {
            updateFlags3D8 |= 0x6000;
        }

        if (
            currentLayout04->ambient28.offset04 !=
                newLayout->ambient28.offset04 ||
            currentLayout04->ambient28.count08 !=
                newLayout->ambient28.count08)
        {
            updateFlags3D8 |= 4;
        }

        if (
            currentLayout04->fogColour34.offset04 !=
                newLayout->fogColour34.offset04 ||
            currentLayout04->fogColour34.count08 !=
                newLayout->fogColour34.count08)
        {
            updateFlags3D8 |= 0x20000;
        }

        if (
            currentLayout04->cameraPos40.offset04 !=
                newLayout->cameraPos40.offset04 ||
            currentLayout04->cameraPos40.count08 !=
                newLayout->cameraPos40.count08)
        {
            updateFlags3D8 |= 8;
        }

        const fable_u32 lightSize = newLayout->lightSize4C;
        if (
            currentLayout04->lightSize4C != lightSize ||
            currentLayout04->lightArray50.offset04 !=
                newLayout->lightArray50.offset04 ||
            currentLayout04->lightArray50.count08 !=
                newLayout->lightArray50.count08 ||
            currentLayout04->lightGlobals5C !=
                newLayout->lightGlobals5C)
        {
            if (static_cast<fable_i32>(lightSize) > 0)
            {
                {
                    local.range.offset04 =
                        newLayout->lightArray50.offset04;
                    local.range.count08 =
                        newLayout->lightArray50.count08;
                    local.range.vtable00 =
                        g_CShaderConstantRangeVtable;
                    lightCount16C =
                        local.range.count08 / lightSize;
                    local.range.Destroy();
                }

                if (
                    lightCount16C > allocatedLightCount170 ||
                    lightSize > allocatedLightSize174)
                {
                    AllocateLights(lightCount16C, lightSize);
                }

                for (
                    fable_u32 light = 0;
                    light < lightCount16C;
                    ++light)
                {
                    lights164[light].needsUpdate35 = true;
                }

                local.vector.x = 0.0f;
                local.vector.y = 0.0f;
                local.vector.z = 0.0f;
                local.vector.w = 1.0f;
                NVectorMath::GFClear4DVectors(
                    lightBuffer168,
                    lightCount16C * lightSize,
                    local.vector);
            }
            updateFlags3D8 |= 0x700;
        }

        const CShaderConstantRangeView& currentLightGlobals =
            currentLayout04->lightGlobals5C;
        if (
            currentLightGlobals.offset04 !=
                newLayout->lightGlobals5C.offset04 ||
            currentLightGlobals.count08 !=
                newLayout->lightGlobals5C.count08)
        {
            fable_u32 count =
                newLayout->lightGlobals5C.count08;
            if (count <= lightGlobals194.size())
            {
                count = lightGlobals194.size();
            }

            local.vector.x = 0.0f;
            local.vector.y = 0.0f;
            local.vector.z = 0.0f;
            local.vector.w = 1.0f;
            lightGlobals194.resize(count, local.vector);
            updateFlags3D8 |= 0x400;
        }

        const CShaderConstantRangeView&
            currentCombinedProjectionMatrix =
                currentLayout04->combinedProjectionMatrix74;
        if (
            currentCombinedProjectionMatrix.offset04 !=
                newLayout->combinedProjectionMatrix74.offset04 ||
            currentCombinedProjectionMatrix.count08 !=
                newLayout->combinedProjectionMatrix74.count08)
        {
            updateFlags3D8 |= 0x80;
        }
    }

    currentLayout04 = newLayout;
    currentLayoutIndex08 = index;
}
