#include "fable_visual_d3d9.h"
#include "fable_render_state.h"
#include "fable_render2d_vertex_queue.h"
#include "fable_render_capture.h"
#include "fable_render_texture.h"
#include "fable_render_window.h"
#include "fable_shader_render_manager.h"
#include "fable_state_block_sold.h"
#include "fable_texture_lifecycle.h"
#include "render2d_batch_plan.h"
#include "render2d_draw_list_adapter.h"

#include <string.h>

typedef long FableD3DResult;
typedef unsigned long FableD3DDword;
typedef unsigned int FableD3DUint;
typedef int FableD3DBool;

struct FableD3D9
{
    void** vtable;
};

struct FableD3DDevice9
{
    void** vtable;
};

struct FableD3DTexture9
{
    void** vtable;
};

struct FableD3DPresentParameters
{
    FableD3DUint backBufferWidth;
    FableD3DUint backBufferHeight;
    FableD3DUint backBufferFormat;
    FableD3DUint backBufferCount;
    FableD3DUint multiSampleType;
    FableD3DDword multiSampleQuality;
    FableD3DUint swapEffect;
    void* deviceWindow;
    FableD3DBool windowed;
    FableD3DBool enableAutoDepthStencil;
    FableD3DUint autoDepthStencilFormat;
    FableD3DDword flags;
    FableD3DUint fullScreenRefreshRate;
    FableD3DUint presentationInterval;
};

struct FableD3DLockedRectangle
{
    fable_i32 pitch;
    void* bits;
};

typedef FableRender2DSpriteVertex FableVisualVertex;

typedef FableD3DResult (FABLE_STDCALL *FableD3DRelease)(
    void* object);
typedef FableD3DResult (FABLE_STDCALL *FableD3DCreateDevice)(
    FableD3D9* direct3D,
    FableD3DUint adapter,
    FableD3DUint deviceType,
    void* focusWindow,
    FableD3DDword behaviorFlags,
    FableD3DPresentParameters* parameters,
    FableD3DDevice9** device);
typedef FableD3DResult (FABLE_STDCALL *FableD3DReset)(
    FableD3DDevice9* device,
    FableD3DPresentParameters* parameters);
typedef FableD3DResult (FABLE_STDCALL *FableD3DCreateTexture)(
    FableD3DDevice9* device,
    FableD3DUint width,
    FableD3DUint height,
    FableD3DUint levels,
    FableD3DDword usage,
    FableD3DUint format,
    FableD3DUint pool,
    FableD3DTexture9** texture,
    void** sharedHandle);
typedef FableD3DResult (FABLE_STDCALL *FableD3DLockTexture)(
    FableD3DTexture9* texture,
    FableD3DUint level,
    FableD3DLockedRectangle* locked,
    const void* rectangle,
    FableD3DDword flags);
typedef FableD3DResult (FABLE_STDCALL *FableD3DUnlockTexture)(
    FableD3DTexture9* texture,
    FableD3DUint level);
typedef FableD3DResult (FABLE_STDCALL *FableD3DClear)(
    FableD3DDevice9* device,
    FableD3DDword rectangleCount,
    const void* rectangles,
    FableD3DDword flags,
    FableD3DDword color,
    float depth,
    FableD3DDword stencil);
typedef FableD3DResult (FABLE_STDCALL *FableD3DBeginScene)(
    FableD3DDevice9* device);
typedef FableD3DResult (FABLE_STDCALL *FableD3DEndScene)(
    FableD3DDevice9* device);
typedef FableD3DResult (FABLE_STDCALL *FableD3DPresent)(
    FableD3DDevice9* device,
    const void* sourceRectangle,
    const void* destinationRectangle,
    void* destinationWindow,
    const void* dirtyRegion);
typedef FableD3DResult (FABLE_STDCALL *FableD3DSetTexture)(
    FableD3DDevice9* device,
    FableD3DDword stage,
    FableD3DTexture9* texture);
typedef FableD3DResult (FABLE_STDCALL *FableD3DSetDwordState)(
    FableD3DDevice9* device,
    FableD3DDword state,
    FableD3DDword value);
typedef FableD3DResult (FABLE_STDCALL *FableD3DSetStageState)(
    FableD3DDevice9* device,
    FableD3DDword stage,
    FableD3DDword state,
    FableD3DDword value);
typedef FableD3DResult (FABLE_STDCALL *FableD3DSetFvf)(
    FableD3DDevice9* device,
    FableD3DDword fvf);
typedef FableD3DResult (FABLE_STDCALL *FableD3DDrawPrimitiveUp)(
    FableD3DDevice9* device,
    FableD3DUint primitiveType,
    FableD3DUint primitiveCount,
    const void* vertices,
    FableD3DUint stride);
extern "C" __declspec(dllimport) FableD3D9* FABLE_STDCALL
Direct3DCreate9(FableD3DUint sdkVersion);
extern "C" __declspec(dllimport) FableD3DDword FABLE_STDCALL
GetTickCount();

CShaderRenderManager g_CShaderRenderManager_013BC470;

fable_u32 DAT_01396f6c = 2;
fable_u32 DAT_01396f68 = 1;
fable_u32 DAT_01396e04 = 4;
fable_u32 DAT_01396ef0 = 2;
fable_u32 DAT_01396eec = 0;
fable_u32 DAT_01396e7c = 4;
fable_u32 DAT_01396e70 = 1;
fable_u32 DAT_01396e5c = 2;
fable_u32 DAT_01396fb0 = 3;
fable_u32 DAT_01396fa4 = 3;
fable_u32 DAT_01396fa0 = 2;
fable_u32 DAT_01396e58 = 1;
fable_u32 DAT_01396e64 = 1;
fable_u32 DAT_01396dd0 = 3;

namespace
{
    const FableD3DUint kD3DSdkVersion = 32;
    const FableD3DUint kD3DDeviceTypeHal = 1;
    const FableD3DDword kD3DSoftwareVertexProcessing = 0x20;
    const FableD3DUint kD3DSwapEffectDiscard = 1;
    const FableD3DUint kD3DFormatA8R8G8B8 = 21;
    const FableD3DUint kD3DFormatD16 = 80;
    const FableD3DUint kD3DPoolManaged = 1;
    const FableD3DDword kD3DClearTarget = 1;
    const FableD3DDword kD3DClearZBuffer = 2;
    const FableD3DDword kD3DFvfXyzRhwColourTexture1 = 0x1C4;
    const fable_i32 kMainMenuRowYOffsets[7] = {
        0, 30, 60, 120, 180, 210, 240
    };
    const fable_u32 kDetailRowCounts[3] = {10, 3, 10};
    const fable_u32 kDetailValueCounts[3][10] = {
        {2, 2, 2, 10, 2, 16, 2, 2, 2, 2},
        {11, 11, 11, 0, 0, 0, 0, 0, 0, 0},
        {3, 4, 3, 4, 4, 4, 18, 2, 4, 3}
    };
    const fable_i32 kDetailRowY[3][10] = {
        {90, 120, 150, 180, 210, 240, 270, 300, 330, 360},
        {130, 190, 250, 0, 0, 0, 0, 0, 0, 0},
        {90, 120, 150, 180, 210, 240, 270, 300, 330, 360}
    };
    const fable_u32 kRedefineKeyValueCount = 55;
    const fable_u32 kRedefineDefaultKeyValues[9] = {
        31, 27, 9, 12, 1, 2, 3, 3, 3
    };

    FableD3D9* g_Direct3D = 0;
    FableD3DDevice9* g_Device = 0;
    void* g_DeviceWindow = 0;
    FableD3DTexture9* g_Texture = 0;
    FableD3DTexture9* g_TitleTexture = 0;
    FableD3DTexture9* g_ForestTexture = 0;
    FableD3DTexture9* g_SunbeamTexture = 0;
    FableD3DTexture9* g_MenuTexture = 0;
    FableD3DTexture9* g_CoastalTexture = 0;
    FableD3DTexture9* g_CoastalSunbeamTexture = 0;
    FableD3DTexture9* g_OptionsTexture = 0;
    FableD3DTexture9* g_HelpersTexture = 0;
    FableD3DTexture9* g_TitleSegmentTexture = 0;
    FableD3DTexture9* g_ButtonLeftTexture = 0;
    FableD3DTexture9* g_ButtonMiddleTexture = 0;
    FableD3DTexture9* g_ButtonRightTexture = 0;
    FableUiCountedComponent g_TitleRuleComponentStorage[3] = {};
    FableUiComponentLifetimeCounters g_TitleRuleLifetime = {};
    FableUiGeneratedComponentVector g_TitleRuleComponents = {
        g_TitleRuleComponentStorage,
        0,
        3,
        &g_TitleRuleLifetime
    };
    FableUiCountedComponent g_OptionsButtonComponentStorage[3] = {};
    FableUiComponentLifetimeCounters g_OptionsButtonLifetime = {};
    FableUiGeneratedComponentVector g_OptionsButtonComponents = {
        g_OptionsButtonComponentStorage,
        0,
        3,
        &g_OptionsButtonLifetime
    };
    FableUiCountedComponent g_MainMenuBigButtonComponentStorage[3] = {};
    FableUiComponentLifetimeCounters g_MainMenuBigButtonLifetime = {};
    FableUiGeneratedComponentVector g_MainMenuBigButtonComponents = {
        g_MainMenuBigButtonComponentStorage,
        0,
        3,
        &g_MainMenuBigButtonLifetime
    };
    FableUiRuntimeListChild g_MainMenuRowChildren[7] = {};
    FableUiRuntimeListChild g_OptionsRowChildren[4] = {};
    CRenderManagerCoreAttachTextureView g_RenderManagerCore = {};
    fable_u32 g_DisplayManagerWindowStorage[0x214 / 4] = {};
    fable_u8 g_RenderStateManagerStorage[0x3A3C] = {};
    fable_u8 g_RenderSystemStorage[0x0C] = {};
    fable_u8 g_SystemManagerStorage[0x64] = {};
    fable_i32 g_ArtworkWidth = 0;
    fable_i32 g_ArtworkHeight = 0;
    fable_i32 g_TitleWidth = 0;
    fable_i32 g_TitleHeight = 0;
    fable_i32 g_MenuWidth = 0;
    fable_i32 g_MenuHeight = 0;
    fable_i32 g_OptionsWidth = 0;
    fable_i32 g_OptionsHeight = 0;
    fable_i32 g_TitleSegmentWidth = 0;
    fable_i32 g_TitleSegmentHeight = 0;
    FableD3DDword g_AnimationStartTick = 0;
    FableD3DDword g_ForestTransitionStart = 0;
    FableD3DDword g_SunbeamTransitionStart = 0;
    fable_u32 g_ForestFrame = 0;
    fable_u32 g_NextForestFrame = 1;
    fable_u32 g_SunbeamFrame = 0;
    fable_u32 g_NextSunbeamFrame = 1;
    fable_u32 g_AnimationRandomState = 0x4C494F4Eu;
    fable_u32 g_MainMenuSelection = 0;
    fable_u32 g_OptionsSelection = 0;
    fable_u32 g_SaveSelection = 0;
    fable_u32 g_DetailScreen = 0;
    fable_u32 g_DetailOptionValues[3][10] = {};
    fable_u32 g_RedefineHover = 0;
    fable_u32 g_RedefineResetHover = 0;
    fable_u32 g_RedefineSelection = 0;
    fable_u32 g_RedefineKeyValues[9] = {};
    fable_u32 g_QuitHover = 0;
    bool g_MainMenuActive = false;
    bool g_OptionsMenuActive = false;
    bool g_SaveMenuActive = false;
    bool g_OptionsBackHovered = false;
    bool g_QuitPromptActive = false;
    bool g_Presented = false;

    bool Failed(FableD3DResult result)
    {
        return result < 0;
    }

    CRenderStateManagerRealiseView& RenderStateManager()
    {
        return *reinterpret_cast<CRenderStateManagerRealiseView*>(
            g_RenderStateManagerStorage);
    }

    CRenderStateManagerCaptureView& CaptureManager()
    {
        return *reinterpret_cast<CRenderStateManagerCaptureView*>(
            g_RenderStateManagerStorage);
    }

    CRenderStateInfo& VisualState(fable_u32 offset)
    {
        return *reinterpret_cast<CRenderStateInfo*>(
            g_RenderStateManagerStorage + offset);
    }

    void InitialiseVisualState(
        fable_u32 offset,
        fable_u32 baselineValue,
        fable_u32 stateParameter,
        fable_u8 stateType,
        fable_u8 textureStage)
    {
        CRenderStateInfo& state = VisualState(offset);
        state.CurrentState = 0xFFFFFFFFu;
        state.DesiredState = baselineValue;
        state.BookmarkMask = 0;
        state.StateParam = stateParameter;
        state.DirtyListFlag = 0;
        state.StateType = stateType;
        state.TextureStage = textureStage;
        state.padding13 = 0;
    }

    void InitialiseSoldStateMetadata()
    {
        InitialiseVisualState(0x2890, 1, 22, 1, 0);
        InitialiseVisualState(0x282C, 2, 8, 1, 0);
        InitialiseVisualState(0x2818, 1, 9, 1, 0);
        InitialiseVisualState(0x2E44, 0, 7, 3, 0);
        InitialiseVisualState(0x2EE4, 1, 1, 3, 0);
        InitialiseVisualState(0x2F84, 1, 2, 3, 0);
        InitialiseVisualState(0x2944, 1, 28, 1, 0);
        InitialiseVisualState(0x2A20, 0, 137, 1, 0);

        InitialiseVisualState(0x291C, 5, 19, 1, 0);
        InitialiseVisualState(0x2930, 6, 20, 1, 0);
        InitialiseVisualState(0x2868, 0, 7, 1, 0);
        InitialiseVisualState(0x28B8, 1, 27, 1, 0);
        InitialiseVisualState(0x28CC, 1, 15, 1, 0);
        InitialiseVisualState(0x2854, 0, 14, 1, 0);
        InitialiseVisualState(0x287C, 8, 23, 1, 0);

        InitialiseVisualState(0x3344, 0, 2, 2, 0);
        InitialiseVisualState(0x33E4, 2, 3, 2, 0);
        InitialiseVisualState(0x3484, 0, 5, 2, 0);
        InitialiseVisualState(0x3524, 2, 6, 2, 0);
        InitialiseVisualState(0x3204, 2, 1, 2, 0);
        InitialiseVisualState(0x32A4, 2, 4, 2, 0);
        InitialiseVisualState(0x3218, 4, 1, 2, 1);
        InitialiseVisualState(0x32B8, 4, 4, 2, 1);

        InitialiseVisualState(0x30C4, 1, 6, 3, 0);
        InitialiseVisualState(0x3164, 1, 5, 3, 0);
    }

    void RequestTrackedVisualState(
        fable_u32 offset,
        fable_u32 requestedValue)
    {
        CRenderStateInfo& state = VisualState(offset);
        if (state.DesiredState == requestedValue)
            return;

        CRenderStateManagerCaptureView& manager =
            CaptureManager();
        const fable_u32 activeCaptureMask =
            manager.captureOffset2814;
        if ((state.BookmarkMask & activeCaptureMask) == 0)
        {
            state.BookmarkMask |= activeCaptureMask;
            CRenderStateCapture& capture =
                manager.captures0008[
                    manager.captureCount2808++];
            capture.entry00 =
                reinterpret_cast<CRenderStateEntry*>(&state);
            capture.value04 = state.DesiredState;
        }

        state.DesiredState = requestedValue;
        if (state.DirtyListFlag == 0)
        {
            state.DirtyListFlag = 1;
            manager.pendingRestores2008[
                manager.pendingRestoreCount280C++] =
                    reinterpret_cast<CRenderStateEntry*>(&state);
        }
    }

    bool ResolveTrackedVisualStateValue(
        fable_u32 valueKind,
        fable_u32 encodedValue,
        fable_u32& resolvedValue)
    {
        if (
            valueKind == RENDER2D_STATE_VALUE_LITERAL ||
            valueKind == RENDER2D_STATE_VALUE_ARGUMENT)
        {
            resolvedValue = encodedValue;
            return true;
        }
        if (valueKind != RENDER2D_STATE_VALUE_GLOBAL)
            return false;

        switch (encodedValue)
        {
        case 0x01396FB0:
            resolvedValue = DAT_01396fb0;
            return true;
        case 0x01396FA4:
            resolvedValue = DAT_01396fa4;
            return true;
        case 0x01396FA0:
            resolvedValue = DAT_01396fa0;
            return true;
        case 0x01396E58:
            resolvedValue = DAT_01396e58;
            return true;
        case 0x01396E64:
            resolvedValue = DAT_01396e64;
            return true;
        case 0x01396DD0:
            resolvedValue = DAT_01396dd0;
            return true;
        case 0x01396E04:
            resolvedValue = DAT_01396e04;
            return true;
        }
        return false;
    }

    template <typename T>
    void ReleaseObject(T*& object)
    {
        if (object != 0)
        {
            void** vtable = object->vtable;
            reinterpret_cast<FableD3DRelease>(vtable[2])(object);
            object = 0;
        }
    }

    bool UploadArtwork(
        FableD3DTexture9*& texture,
        fable_i32 artworkWidth,
        fable_i32 artworkHeight,
        fable_i32 artworkPitch,
        fable_u32 artworkBitsPerPixel,
        const void* artworkPixels,
        bool preserveAlpha,
        bool unpremultiplyAlpha)
    {
        if (
            artworkWidth <= 0 ||
            artworkHeight == 0 ||
            artworkPixels == 0 ||
            (artworkBitsPerPixel != 24 &&
             artworkBitsPerPixel != 32))
        {
            return false;
        }

        const fable_i32 absoluteHeight =
            artworkHeight < 0
                ? -artworkHeight
                : artworkHeight;
        FableD3DCreateTexture createTexture =
            reinterpret_cast<FableD3DCreateTexture>(
                g_Device->vtable[23]);
        if (Failed(createTexture(
                g_Device,
                artworkWidth,
                absoluteHeight,
                1,
                0,
                kD3DFormatA8R8G8B8,
                kD3DPoolManaged,
                &texture,
                0)))
        {
            return false;
        }

        FableD3DLockedRectangle locked = {};
        FableD3DLockTexture lockTexture =
            reinterpret_cast<FableD3DLockTexture>(
                texture->vtable[19]);
        if (Failed(lockTexture(texture, 0, &locked, 0, 0)))
        {
            return false;
        }

        const fable_u8* source =
            static_cast<const fable_u8*>(artworkPixels);
        const fable_i32 sourceBytesPerPixel =
            artworkBitsPerPixel / 8;
        for (fable_i32 y = 0; y != absoluteHeight; ++y)
        {
            const fable_i32 sourceY =
                artworkHeight > 0
                    ? absoluteHeight - 1 - y
                    : y;
            const fable_u8* sourceRow =
                source + sourceY * artworkPitch;
            fable_u32* destinationRow =
                reinterpret_cast<fable_u32*>(
                    static_cast<fable_u8*>(locked.bits) +
                    y * locked.pitch);
            for (fable_i32 x = 0; x != artworkWidth; ++x)
            {
                const fable_u8* pixel =
                    sourceRow + x * sourceBytesPerPixel;
                const fable_u32 alpha =
                    preserveAlpha && artworkBitsPerPixel == 32
                        ? static_cast<fable_u32>(pixel[3]) << 24
                        : 0xFF000000u;
                fable_u32 blue = pixel[0];
                fable_u32 green = pixel[1];
                fable_u32 red = pixel[2];
                if (
                    unpremultiplyAlpha &&
                    preserveAlpha &&
                    artworkBitsPerPixel == 32 &&
                    pixel[3] != 0 &&
                    pixel[3] != 255)
                {
                    blue = blue * 255 / pixel[3];
                    green = green * 255 / pixel[3];
                    red = red * 255 / pixel[3];
                    if (blue > 255) blue = 255;
                    if (green > 255) green = 255;
                    if (red > 255) red = 255;
                }
                destinationRow[x] =
                    alpha |
                    (red << 16) |
                    (green << 8) |
                    blue;
            }
        }

        FableD3DUnlockTexture unlockTexture =
            reinterpret_cast<FableD3DUnlockTexture>(
                texture->vtable[20]);
        if (Failed(unlockTexture(texture, 0)))
        {
            return false;
        }

        return true;
    }

    void AppendVisualQuad(
        FableVisualVertex* vertices,
        fable_u32& vertexCount,
        FableRender2DPlanRecord* records,
        fable_u32& recordCount,
        FableD3DTexture9* texture,
        float left,
        float top,
        float right,
        float bottom,
        float u0,
        float v0,
        float u1,
        float v1,
        fable_u32 diffuseColour)
    {
        FableVisualVertex quad[6] = {
            {left, top, 0.0f, 1.0f, diffuseColour, 0, u0, v0},
            {right, top, 0.0f, 1.0f, diffuseColour, 0, u1, v0},
            {left, bottom, 0.0f, 1.0f, diffuseColour, 0, u0, v1},
            {left, bottom, 0.0f, 1.0f, diffuseColour, 0, u0, v1},
            {right, top, 0.0f, 1.0f, diffuseColour, 0, u1, v0},
            {right, bottom, 0.0f, 1.0f, diffuseColour, 0, u1, v1}
        };
        memcpy(vertices + vertexCount, quad, sizeof(quad));
        vertexCount += 6;

        FableRender2DPlanRecord& first = records[recordCount++];
        memset(&first, 0, sizeof(first));
        first.textureIdentity =
            reinterpret_cast<fable_u32>(texture);
        first.payload.normal.stateBlock = 1;
        records[recordCount++] = first;
    }

    fable_u32 ChooseNextAnimationFrame(
        fable_u32 currentFrame,
        fable_u32 frameCount)
    {
        g_AnimationRandomState =
            g_AnimationRandomState * 1664525u + 1013904223u;
        fable_u32 selected =
            g_AnimationRandomState % (frameCount - 1);
        if (selected >= currentFrame)
            ++selected;
        return selected;
    }

    fable_u32 DetailControlAtlasIndex(
        fable_u32 screenIndex,
        fable_u32 row,
        fable_u32 value)
    {
        fable_u32 index = 0;
        for (fable_u32 screen = 0; screen != screenIndex; ++screen)
        {
            for (
                fable_u32 priorRow = 0;
                priorRow != kDetailRowCounts[screen];
                ++priorRow)
            {
                index += kDetailValueCounts[screen][priorRow];
            }
        }
        for (fable_u32 priorRow = 0; priorRow != row; ++priorRow)
            index += kDetailValueCounts[screenIndex][priorRow];
        return index + value;
    }

    bool InitialiseTitleRuleComponents(
        fable_i32 segmentWidth,
        fable_i32 segmentHeight)
    {
        if (
            segmentWidth <= 0 ||
            segmentHeight <= 0 ||
            640 % segmentWidth != 0)
        {
            return false;
        }

        memset(
            g_TitleRuleComponentStorage,
            0,
            sizeof(g_TitleRuleComponentStorage));
        memset(
            &g_TitleRuleLifetime,
            0,
            sizeof(g_TitleRuleLifetime));
        g_TitleRuleComponents.size = 0;

        FableUiTableRuntimeInput input = {};
        input.geometry.availableSpriteMask =
            (1UL << 0) |
            (1UL << 1) |
            (1UL << 4);
        input.geometry.horizontalRepeatCount =
            static_cast<unsigned long>(640 / segmentWidth);
        input.geometry.horizontalEdgeSize.x =
            static_cast<float>(segmentWidth);
        input.geometry.horizontalEdgeSize.y =
            static_cast<float>(segmentHeight);
        input.geometry.cornerSize[0] =
            input.geometry.horizontalEdgeSize;
        input.geometry.cornerSize[1] =
            input.geometry.horizontalEdgeSize;

        const unsigned int spriteKeys[3] = {0, 1, 4};
        for (unsigned int index = 0; index != 3; ++index)
        {
            const unsigned int spriteKey = spriteKeys[index];
            input.sprite[spriteKey].definitionId = 122;
            input.sprite[spriteKey].size =
                input.geometry.horizontalEdgeSize;
        }
        return FableConstructUiTableComponents(
            &input,
            &g_TitleRuleComponents);
    }

    bool InitialiseButtonComponents(
        fable_i32 totalWidth,
        FableUiCountedComponent* storage,
        FableUiComponentLifetimeCounters* lifetime,
        FableUiGeneratedComponentVector* components,
        fable_i32 leftWidth,
        fable_i32 leftHeight,
        fable_i32 middleWidth,
        fable_i32 middleHeight,
        fable_i32 rightWidth,
        fable_i32 rightHeight)
    {
        const fable_i32 innerWidth =
            totalWidth - leftWidth - rightWidth;
        if (
            storage == 0 ||
            lifetime == 0 ||
            components == 0 ||
            leftWidth <= 0 ||
            leftHeight <= 0 ||
            middleWidth <= 0 ||
            middleHeight <= 0 ||
            rightWidth <= 0 ||
            rightHeight <= 0 ||
            innerWidth <= 0 ||
            innerWidth % middleWidth != 0)
        {
            return false;
        }

        memset(
            storage,
            0,
            sizeof(FableUiCountedComponent) * 3);
        memset(
            lifetime,
            0,
            sizeof(*lifetime));
        components->size = 0;

        FableUiTableRuntimeInput input = {};
        input.geometry.availableSpriteMask =
            (1UL << 0) |
            (1UL << 1) |
            (1UL << 4);
        input.geometry.horizontalRepeatCount =
            static_cast<unsigned long>(innerWidth / middleWidth);
        input.geometry.horizontalEdgeSize.x =
            static_cast<float>(middleWidth);
        input.geometry.horizontalEdgeSize.y =
            static_cast<float>(middleHeight);
        input.geometry.cornerSize[0].x =
            static_cast<float>(leftWidth);
        input.geometry.cornerSize[0].y =
            static_cast<float>(leftHeight);
        input.geometry.cornerSize[1].x =
            static_cast<float>(rightWidth);
        input.geometry.cornerSize[1].y =
            static_cast<float>(rightHeight);

        input.sprite[0].definitionId = 129;
        input.sprite[0].size = input.geometry.cornerSize[0];
        input.sprite[1].definitionId = 130;
        input.sprite[1].size = input.geometry.cornerSize[1];
        input.sprite[4].definitionId = 131;
        input.sprite[4].size =
            input.geometry.horizontalEdgeSize;
        return FableConstructUiTableComponents(
            &input,
            components);
    }

    void InitialiseMainMenuRowStates()
    {
        memset(
            g_MainMenuRowChildren,
            0,
            sizeof(g_MainMenuRowChildren));
        for (unsigned int child = 0; child != 7; ++child)
        {
            FableUiRuntimeListChild& row =
                g_MainMenuRowChildren[child];
            row.states.stateMask = 0x1F;
            for (unsigned int state = 0; state != 5; ++state)
            {
                CUIStateRecoveredLayout& retailState =
                    row.states.state[state];
                retailState.stateChangeFlag = 7;
                retailState.position.x =
                    state == 2 ? 0.0f : 120.0f;
                row.states.state[state].zoom.x = 1.0f;
                row.states.state[state].zoom.y = 1.0f;
                retailState.colour.red = 255;
                retailState.colour.green = 255;
                retailState.colour.blue = 255;
                retailState.colour.alpha =
                    state == 2 ? 0 : 255;
                retailState.updateTime =
                    state == 2 ? 0.2f : -1.0f;
            }
            row.currentState = child == 0 ? 3 : 0;
            row.currentColour =
                row.states.state[row.currentState].colour;
        }
    }

    void ApplyMainMenuRowState(
        unsigned int child,
        unsigned long state)
    {
        if (
            child >= 7 ||
            state >= 7 ||
            (
                g_MainMenuRowChildren[child].states.stateMask &
                (1u << state)
            ) == 0)
        {
            return;
        }
        FableUiRuntimeListChild& row =
            g_MainMenuRowChildren[child];
        const CUIStateRecoveredLayout& retailState =
            row.states.state[state];
        row.currentState = state;
        // The row atlas is authored at the retail state-0 text position
        // x=120, so live CUIState positions are applied as deltas.
        row.currentPosition.x =
            retailState.position.x - 120.0f;
        row.currentPosition.y =
            retailState.position.y;
        row.currentColour = retailState.colour;
    }

    void InitialiseOptionsRowStates()
    {
        memset(
            g_OptionsRowChildren,
            0,
            sizeof(g_OptionsRowChildren));
        for (unsigned int child = 0; child != 4; ++child)
        {
            FableUiRuntimeListChild& row =
                g_OptionsRowChildren[child];
            row.states.stateMask = 0x1F;
            for (unsigned int state = 0; state != 5; ++state)
            {
                CUIStateRecoveredLayout& retailState =
                    row.states.state[state];
                retailState.stateChangeFlag = 7;
                retailState.position.x =
                    state == 2 ? 0.0f : 120.0f;
                retailState.zoom.x = 1.0f;
                retailState.zoom.y = 1.0f;
                retailState.colour.red = 255;
                retailState.colour.green = 255;
                retailState.colour.blue = 255;
                retailState.colour.alpha =
                    state == 2 ? 0 : 255;
                retailState.updateTime =
                    state == 2 ? 0.2f : -1.0f;
            }
            row.currentState = child == 0 ? 3 : 0;
            row.currentColour =
                row.states.state[row.currentState].colour;
        }
    }

    void ApplyOptionsRowState(
        unsigned int child,
        unsigned long state)
    {
        if (
            child >= 4 ||
            state >= 7 ||
            (
                g_OptionsRowChildren[child].states.stateMask &
                (1u << state)
            ) == 0)
        {
            return;
        }
        FableUiRuntimeListChild& row =
            g_OptionsRowChildren[child];
        const CUIStateRecoveredLayout& retailState =
            row.states.state[state];
        row.currentState = state;
        row.currentPosition.x =
            retailState.position.x - 120.0f;
        row.currentPosition.y =
            retailState.position.y;
        row.currentColour = retailState.colour;
    }

    class VisualRender2DAdapter : public IRender2DDrawListAdapter
    {
    public:
        explicit VisualRender2DAdapter(
            const FableVisualVertex* vertices,
            fable_u32 vertexCount)
            : vertices_(vertices),
              vertexCount_(vertexCount),
              attachedTexture_(0),
              succeeded_(true),
              drew_(false),
              realisedStateBlock_(false)
        {
            memset(textures_, 0, sizeof(textures_));
            memset(&captureSentinel_, 0, sizeof(captureSentinel_));
            captureSentinel_.captureType11 = 4;
        }

        virtual void Invoke(
            fable_u32 eventKind,
            fable_u32 argument0,
            fable_u32 argument1,
            fable_u32 argument2,
            fable_u32 argument3)
        {
            if (!succeeded_)
                return;

            if (eventKind == RENDER2D_ADAPTER_BEGIN_CAPTURE)
            {
                CRenderStateManagerCaptureView& captureManager =
                    CaptureManager();
                captureManager.captures0008[0].entry00 =
                    &captureSentinel_;
                captureManager.captures0008[0].value04 = 0;
                captureManager.captureCount2808 = 1;
                captureManager.pendingRestoreCount280C = 0;
                captureManager.captureOffset2814 = 2;
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_REQUEST_TRACKED_STATE)
            {
                fable_u32 requestedValue = 0;
                if (!ResolveTrackedVisualStateValue(
                        argument1,
                        argument2,
                        requestedValue))
                {
                    succeeded_ = false;
                }
                else
                {
                    RequestTrackedVisualState(
                        argument0,
                        requestedValue);
                }
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_INITIALISE_NULL_TEXTURE)
            {
                if (argument0 >= 1 && argument0 <= 3)
                {
                    CTexturePreallocatedView* texture =
                        reinterpret_cast<CTexturePreallocatedView*>(
                            &textures_[argument0 - 1]);
                    if (
                        !texture->InitialiseFromPreallocatedTexture(0) ||
                        texture->texture00 != 0 ||
                        texture->ByteLength != 0)
                    {
                        succeeded_ = false;
                    }
                }
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_ASSIGN_TEXTURE)
            {
                if (
                    argument0 >= 1 && argument0 <= 3 &&
                    argument1 >= 1 && argument1 <= 3)
                {
                    textures_[argument0 - 1] =
                        textures_[argument1 - 1];
                }
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_UNINITIALISE_TEXTURE)
            {
                if (argument0 >= 1 && argument0 <= 3)
                {
                    reinterpret_cast<
                        CTextureUninitialiseView*>(
                            &textures_[argument0 - 1])->
                        Uninitialise();
                }
            }
            else if (eventKind == RENDER2D_ADAPTER_APPLY_STATE_BLOCK)
            {
                CStateBlockFunctionSoldView sold;
                sold.Apply();
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_APPLY_VERTEX_SHADER)
            {
                FableD3DSetFvf setFvf =
                    reinterpret_cast<FableD3DSetFvf>(
                        g_Device->vtable[89]);
                Record(setFvf(
                    g_Device,
                    kD3DFvfXyzRhwColourTexture1));
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_UPDATE_PIXEL_SHADER)
            {
                CShaderRenderManagerUpdatePixelShaderView& shader =
                    *reinterpret_cast<
                        CShaderRenderManagerUpdatePixelShaderView*>(
                            &g_CShaderRenderManager_013BC470);
                shader.UpdatePixelShader();
                if ((shader.shaderStateMask3D8 & 1) != 0)
                    succeeded_ = false;
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_RESET_WORLD_TRANSFORM)
            {
                CShaderRenderManager_ResetWorldTransform(
                    &g_CShaderRenderManager_013BC470);
                const fable_u8* shaderBytes =
                    reinterpret_cast<const fable_u8*>(
                        &g_CShaderRenderManager_013BC470);
                const float* worldTransform =
                    reinterpret_cast<const float*>(
                        shaderBytes + 0x1F0);
                if (
                    shaderBytes[0x1E8] != 1 ||
                    worldTransform[0] != 1.0f ||
                    worldTransform[5] != 1.0f ||
                    worldTransform[10] != 1.0f ||
                    worldTransform[15] != 1.0f ||
                    (
                        g_CShaderRenderManager_013BC470.
                            updateFlags3D8 &
                        0xC990) != 0xC990)
                {
                    succeeded_ = false;
                }
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_ATTACH_TEXTURE)
            {
                FableD3DTexture9* selectedTexture = 0;
                if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_Texture))
                {
                    selectedTexture = g_Texture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_TitleTexture))
                {
                    selectedTexture = g_TitleTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_ForestTexture))
                {
                    selectedTexture = g_ForestTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_SunbeamTexture))
                {
                    selectedTexture = g_SunbeamTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_MenuTexture))
                {
                    selectedTexture = g_MenuTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_CoastalTexture))
                {
                    selectedTexture = g_CoastalTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(
                        g_CoastalSunbeamTexture))
                {
                    selectedTexture = g_CoastalSunbeamTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_OptionsTexture))
                {
                    selectedTexture = g_OptionsTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(g_HelpersTexture))
                {
                    selectedTexture = g_HelpersTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(
                        g_TitleSegmentTexture))
                {
                    selectedTexture = g_TitleSegmentTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(
                        g_ButtonLeftTexture))
                {
                    selectedTexture = g_ButtonLeftTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(
                        g_ButtonMiddleTexture))
                {
                    selectedTexture = g_ButtonMiddleTexture;
                }
                else if (
                    argument1 ==
                    reinterpret_cast<fable_u32>(
                        g_ButtonRightTexture))
                {
                    selectedTexture = g_ButtonRightTexture;
                }
                CTextureAttachView texture = {selectedTexture};
                attachedTexture_ = selectedTexture;
                g_RenderManagerCore.AttachTextureToStage(
                    argument0,
                    selectedTexture != 0 ? &texture : 0);
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_DRAW_PRIMITIVE_UP)
            {
                FableD3DDrawPrimitiveUp drawPrimitiveUp =
                    reinterpret_cast<FableD3DDrawPrimitiveUp>(
                        g_Device->vtable[83]);
                const fable_u8* vertexBytes =
                    reinterpret_cast<const fable_u8*>(vertices_);
                Record(drawPrimitiveUp(
                    g_Device,
                    argument0,
                    argument1,
                    vertexBytes + argument2,
                    argument3));
                drew_ = succeeded_;
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_SET_WINDOW)
            {
                const C2DBoxF* window =
                    reinterpret_cast<const C2DBoxF*>(argument0);
                if (window != 0)
                {
                    reinterpret_cast<
                        CRenderManagerCoreWindowView*>(
                            &g_RenderManagerCore)->
                        SetAWindow(*window);
                }
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_REALISE_RENDER_STATE)
            {
                if (
                    !realisedStateBlock_ &&
                    RenderStateManager().
                        StateUpdateListSize280C != 25)
                {
                    succeeded_ = false;
                }
                RenderStateManager().RealiseRenderState();
                FableD3DSetDwordState setRenderState =
                    reinterpret_cast<FableD3DSetDwordState>(
                        g_Device->vtable[57]);
                if (
                    attachedTexture_ != 0 &&
                    attachedTexture_ != g_Texture)
                {
                    Record(setRenderState(g_Device, 19, 5));
                    Record(setRenderState(g_Device, 20, 6));
                    Record(setRenderState(g_Device, 27, 1));
                }
                else
                {
                    Record(setRenderState(g_Device, 27, 0));
                }
                realisedStateBlock_ = true;
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_CLEAR_VERTEX_QUEUE)
            {
                Render2DDrawListBlockView controller = {};
                Render2DDrawListBlockView* begin =
                    reinterpret_cast<
                        Render2DDrawListBlockView*>(
                            const_cast<FableVisualVertex*>(
                                vertices_));
                Render2DDrawListBlockView* end =
                    begin + vertexCount_;
                controller.begin00 = begin;
                controller.end04 = end;
                controller.CopyBlock(begin, end);
                if (controller.end04 != begin)
                    succeeded_ = false;
            }
            else if (
                eventKind ==
                RENDER2D_ADAPTER_RESTORE_CAPTURE)
            {
                CRenderStateManagerCaptureView& captureManager =
                    CaptureManager();
                captureManager.RestoreCaptureBlock();
                if (captureManager.pendingRestoreCount280C != 25)
                    succeeded_ = false;
                RenderStateManager().RealiseRenderState();
                if (
                    captureManager.captureCount2808 != 0 ||
                    captureManager.pendingRestoreCount280C != 0 ||
                    captureManager.captureOffset2814 != 1)
                {
                    succeeded_ = false;
                }
            }
        }

        bool Succeeded() const
        {
            return succeeded_ && drew_;
        }

    private:
        void Record(FableD3DResult result)
        {
            if (Failed(result))
                succeeded_ = false;
        }

        const FableVisualVertex* vertices_;
        fable_u32 vertexCount_;
        FableD3DTexture9* attachedTexture_;
        CTextureAssignmentView textures_[3];
        CRenderStateEntry captureSentinel_;
        bool succeeded_;
        bool drew_;
        bool realisedStateBlock_;
    };
}

void* FABLE_FASTCALL GFGetSystemManager()
{
    return g_SystemManagerStorage;
}

bool FABLE_FASTCALL FableInitialiseVisualD3D9(
    void* window,
    fable_i32 backBufferWidth,
    fable_i32 backBufferHeight,
    fable_i32 artworkWidth,
    fable_i32 artworkHeight,
    fable_i32 artworkPitch,
    fable_u32 artworkBitsPerPixel,
    const void* artworkPixels,
    fable_i32 titleWidth,
    fable_i32 titleHeight,
    fable_i32 titlePitch,
    fable_u32 titleBitsPerPixel,
    const void* titlePixels,
    fable_i32 forestWidth,
    fable_i32 forestHeight,
    fable_i32 forestPitch,
    fable_u32 forestBitsPerPixel,
    const void* forestPixels,
    fable_i32 sunbeamWidth,
    fable_i32 sunbeamHeight,
    fable_i32 sunbeamPitch,
    fable_u32 sunbeamBitsPerPixel,
    const void* sunbeamPixels,
    fable_i32 menuWidth,
    fable_i32 menuHeight,
    fable_i32 menuPitch,
    fable_u32 menuBitsPerPixel,
    const void* menuPixels,
    fable_i32 coastalWidth,
    fable_i32 coastalHeight,
    fable_i32 coastalPitch,
    fable_u32 coastalBitsPerPixel,
    const void* coastalPixels,
    fable_i32 coastalSunbeamWidth,
    fable_i32 coastalSunbeamHeight,
    fable_i32 coastalSunbeamPitch,
    fable_u32 coastalSunbeamBitsPerPixel,
    const void* coastalSunbeamPixels,
    fable_i32 optionsWidth,
    fable_i32 optionsHeight,
    fable_i32 optionsPitch,
    fable_u32 optionsBitsPerPixel,
    const void* optionsPixels,
    fable_i32 helpersWidth,
    fable_i32 helpersHeight,
    fable_i32 helpersPitch,
    fable_u32 helpersBitsPerPixel,
    const void* helpersPixels,
    fable_i32 titleSegmentWidth,
    fable_i32 titleSegmentHeight,
    fable_i32 titleSegmentPitch,
    fable_u32 titleSegmentBitsPerPixel,
    const void* titleSegmentPixels,
    fable_i32 buttonLeftWidth,
    fable_i32 buttonLeftHeight,
    fable_i32 buttonLeftPitch,
    fable_u32 buttonLeftBitsPerPixel,
    const void* buttonLeftPixels,
    fable_i32 buttonMiddleWidth,
    fable_i32 buttonMiddleHeight,
    fable_i32 buttonMiddlePitch,
    fable_u32 buttonMiddleBitsPerPixel,
    const void* buttonMiddlePixels,
    fable_i32 buttonRightWidth,
    fable_i32 buttonRightHeight,
    fable_i32 buttonRightPitch,
    fable_u32 buttonRightBitsPerPixel,
    const void* buttonRightPixels)
{
    FableShutdownVisualD3D9();
    g_DeviceWindow = window;
    g_Direct3D = Direct3DCreate9(kD3DSdkVersion);
    if (g_Direct3D == 0)
    {
        g_DeviceWindow = 0;
        return false;
    }

    FableD3DPresentParameters parameters = {};
    parameters.backBufferWidth = backBufferWidth;
    parameters.backBufferHeight = backBufferHeight;
    parameters.backBufferCount = 1;
    parameters.swapEffect = kD3DSwapEffectDiscard;
    parameters.deviceWindow = window;
    parameters.windowed = 1;
    parameters.enableAutoDepthStencil = 1;
    parameters.autoDepthStencilFormat = kD3DFormatD16;

    FableD3DCreateDevice createDevice =
        reinterpret_cast<FableD3DCreateDevice>(
            g_Direct3D->vtable[16]);
    if (Failed(createDevice(
            g_Direct3D,
            0,
            kD3DDeviceTypeHal,
            window,
            kD3DSoftwareVertexProcessing,
            &parameters,
            &g_Device)) ||
        g_Device == 0 ||
        !UploadArtwork(
            g_Texture,
            artworkWidth,
            artworkHeight,
            artworkPitch,
            artworkBitsPerPixel,
            artworkPixels,
            false,
            false) ||
        (
            titlePixels != 0 &&
            !UploadArtwork(
                g_TitleTexture,
                titleWidth,
                titleHeight,
                titlePitch,
                titleBitsPerPixel,
                titlePixels,
                true,
                true)
        ) ||
        (
            forestPixels != 0 &&
            !UploadArtwork(
                g_ForestTexture,
                forestWidth,
                forestHeight,
                forestPitch,
                forestBitsPerPixel,
                forestPixels,
                true,
                false)
        ) ||
        (
            sunbeamPixels != 0 &&
            !UploadArtwork(
                g_SunbeamTexture,
                sunbeamWidth,
                sunbeamHeight,
                sunbeamPitch,
                sunbeamBitsPerPixel,
                sunbeamPixels,
                true,
                false)
        ) ||
        (
            menuPixels != 0 &&
            !UploadArtwork(
                g_MenuTexture,
                menuWidth,
                menuHeight,
                menuPitch,
                menuBitsPerPixel,
                menuPixels,
                true,
                true)
        ) ||
        (
            coastalPixels != 0 &&
            !UploadArtwork(
                g_CoastalTexture,
                coastalWidth,
                coastalHeight,
                coastalPitch,
                coastalBitsPerPixel,
                coastalPixels,
                true,
                false)
        ) ||
        (
            coastalSunbeamPixels != 0 &&
            !UploadArtwork(
                g_CoastalSunbeamTexture,
                coastalSunbeamWidth,
                coastalSunbeamHeight,
                coastalSunbeamPitch,
                coastalSunbeamBitsPerPixel,
                coastalSunbeamPixels,
                true,
                false)
        ) ||
        (
            optionsPixels != 0 &&
            !UploadArtwork(
                g_OptionsTexture,
                optionsWidth,
                optionsHeight,
                optionsPitch,
                optionsBitsPerPixel,
                optionsPixels,
                true,
                true)
        ) ||
        (
            helpersPixels != 0 &&
            !UploadArtwork(
                g_HelpersTexture,
                helpersWidth,
                helpersHeight,
                helpersPitch,
                helpersBitsPerPixel,
                helpersPixels,
                true,
                true)
        ) ||
        (
            titleSegmentPixels != 0 &&
            !UploadArtwork(
                g_TitleSegmentTexture,
                titleSegmentWidth,
                titleSegmentHeight,
                titleSegmentPitch,
                titleSegmentBitsPerPixel,
                titleSegmentPixels,
                true,
                true)
        ) ||
        (
            buttonLeftPixels != 0 &&
            !UploadArtwork(
                g_ButtonLeftTexture,
                buttonLeftWidth,
                buttonLeftHeight,
                buttonLeftPitch,
                buttonLeftBitsPerPixel,
                buttonLeftPixels,
                true,
                true)
        ) ||
        (
            buttonMiddlePixels != 0 &&
            !UploadArtwork(
                g_ButtonMiddleTexture,
                buttonMiddleWidth,
                buttonMiddleHeight,
                buttonMiddlePitch,
                buttonMiddleBitsPerPixel,
                buttonMiddlePixels,
                true,
                true)
        ) ||
        (
            buttonRightPixels != 0 &&
            !UploadArtwork(
                g_ButtonRightTexture,
                buttonRightWidth,
                buttonRightHeight,
                buttonRightPitch,
                buttonRightBitsPerPixel,
                buttonRightPixels,
                true,
                true)
        ))
    {
        FableShutdownVisualD3D9();
        return false;
    }
    g_ArtworkWidth = artworkWidth;
    g_ArtworkHeight =
        artworkHeight < 0 ? -artworkHeight : artworkHeight;
    if (g_TitleTexture != 0)
    {
        g_TitleWidth = titleWidth;
        g_TitleHeight =
            titleHeight < 0 ? -titleHeight : titleHeight;
    }
    if (g_MenuTexture != 0)
    {
        g_MenuWidth = menuWidth;
        g_MenuHeight =
            menuHeight < 0 ? -menuHeight : menuHeight;
    }
    if (
        (g_ForestTexture == 0) != (g_SunbeamTexture == 0) ||
        (
            g_ForestTexture != 0 &&
            (
                forestWidth != 640 ||
                (forestHeight != 1920 && forestHeight != -1920) ||
                sunbeamWidth != 640 ||
                (sunbeamHeight != 1440 && sunbeamHeight != -1440)
            )
        ))
    {
        FableShutdownVisualD3D9();
        return false;
    }
    if (
        (g_MenuTexture == 0) != (g_CoastalTexture == 0) ||
        (g_MenuTexture == 0) != (g_CoastalSunbeamTexture == 0) ||
        (
            g_MenuTexture != 0 &&
            (
                (g_MenuWidth != 640 && g_MenuWidth != 1280) ||
                g_MenuHeight != 3360 ||
                coastalWidth != 640 ||
                (coastalHeight != 1920 && coastalHeight != -1920) ||
                coastalSunbeamWidth != 640 ||
                (
                    coastalSunbeamHeight != 1440 &&
                    coastalSunbeamHeight != -1440
                )
            )
        ))
    {
        FableShutdownVisualD3D9();
        return false;
    }
    if (
        (g_OptionsTexture == 0) != (g_HelpersTexture == 0) ||
        (g_OptionsTexture == 0) != (g_TitleSegmentTexture == 0) ||
        (g_OptionsTexture == 0) != (g_ButtonLeftTexture == 0) ||
        (g_OptionsTexture == 0) != (g_ButtonMiddleTexture == 0) ||
        (g_OptionsTexture == 0) != (g_ButtonRightTexture == 0) ||
        (
            g_OptionsTexture != 0 &&
            (
                optionsWidth != 1664 ||
                (optionsHeight != 3840 && optionsHeight != -3840) ||
                helpersWidth != 640 ||
                (helpersHeight != 2880 && helpersHeight != -2880) ||
                titleSegmentWidth != 8 ||
                (
                    titleSegmentHeight != 64 &&
                    titleSegmentHeight != -64
                ) ||
                buttonLeftWidth != 64 ||
                (
                    buttonLeftHeight != 64 &&
                    buttonLeftHeight != -64
                ) ||
                buttonMiddleWidth != 8 ||
                (
                    buttonMiddleHeight != 64 &&
                    buttonMiddleHeight != -64
                ) ||
                buttonRightWidth != 64 ||
                (
                    buttonRightHeight != 64 &&
                    buttonRightHeight != -64
                )
            )
        ))
    {
        FableShutdownVisualD3D9();
        return false;
    }
    if (g_OptionsTexture != 0)
    {
        g_OptionsWidth = optionsWidth;
        g_OptionsHeight =
            optionsHeight < 0 ? -optionsHeight : optionsHeight;
        g_TitleSegmentWidth = titleSegmentWidth;
        g_TitleSegmentHeight =
            titleSegmentHeight < 0
                ? -titleSegmentHeight
                : titleSegmentHeight;
        if (!InitialiseTitleRuleComponents(
                g_TitleSegmentWidth,
                g_TitleSegmentHeight) ||
            !InitialiseButtonComponents(
                280,
                g_OptionsButtonComponentStorage,
                &g_OptionsButtonLifetime,
                &g_OptionsButtonComponents,
                buttonLeftWidth,
                buttonLeftHeight < 0
                    ? -buttonLeftHeight
                    : buttonLeftHeight,
                buttonMiddleWidth,
                buttonMiddleHeight < 0
                    ? -buttonMiddleHeight
                    : buttonMiddleHeight,
                buttonRightWidth,
                buttonRightHeight < 0
                    ? -buttonRightHeight
                    : buttonRightHeight) ||
            !InitialiseButtonComponents(
                400,
                g_MainMenuBigButtonComponentStorage,
                &g_MainMenuBigButtonLifetime,
                &g_MainMenuBigButtonComponents,
                buttonLeftWidth,
                buttonLeftHeight < 0
                    ? -buttonLeftHeight
                    : buttonLeftHeight,
                buttonMiddleWidth,
                buttonMiddleHeight < 0
                    ? -buttonMiddleHeight
                    : buttonMiddleHeight,
                buttonRightWidth,
                buttonRightHeight < 0
                    ? -buttonRightHeight
                    : buttonRightHeight))
        {
            FableShutdownVisualD3D9();
            return false;
        }
    }
    g_MainMenuSelection = 0;
    InitialiseMainMenuRowStates();
    g_OptionsSelection = 0;
    g_SaveSelection = 0;
    InitialiseOptionsRowStates();
    g_DetailScreen = 0;
    memset(g_DetailOptionValues, 0, sizeof(g_DetailOptionValues));
    g_RedefineHover = 0;
    g_RedefineResetHover = 0;
    g_RedefineSelection = 0;
    memcpy(
        g_RedefineKeyValues,
        kRedefineDefaultKeyValues,
        sizeof(g_RedefineKeyValues));
    g_QuitHover = 0;
    g_MainMenuActive = false;
    g_OptionsMenuActive = false;
    g_SaveMenuActive = false;
    g_OptionsBackHovered = false;
    g_QuitPromptActive = false;
    g_AnimationStartTick = 0;
    memset(
        &g_RenderManagerCore,
        0,
        sizeof(g_RenderManagerCore));
    g_RenderManagerCore.displayDevice3CF0 =
        reinterpret_cast<FableTextureDevice*>(g_Device);
    memset(
        g_DisplayManagerWindowStorage,
        0,
        sizeof(g_DisplayManagerWindowStorage));
    fable_u8* displayManagerBytes =
        reinterpret_cast<fable_u8*>(
            g_DisplayManagerWindowStorage);
    *reinterpret_cast<FableD3DDevice9**>(
        displayManagerBytes + 0x58) = g_Device;
    *reinterpret_cast<fable_i32*>(
        displayManagerBytes + 0x194) = backBufferWidth;
    *reinterpret_cast<fable_i32*>(
        displayManagerBytes + 0x198) = backBufferHeight;
    memset(
        &g_CShaderRenderManager_013BC470,
        0,
        sizeof(g_CShaderRenderManager_013BC470));
    CShaderRenderManagerUpdatePixelShaderView& shaderManager =
        *reinterpret_cast<
            CShaderRenderManagerUpdatePixelShaderView*>(
                &g_CShaderRenderManager_013BC470);
    shaderManager.displayDevice1D4 =
        reinterpret_cast<FablePixelShaderDevice*>(g_Device);
    shaderManager.currentPixelShader1E4 = 0;
    reinterpret_cast<CRenderManagerCoreWindowView*>(
        &g_RenderManagerCore)->displayManager3A3C =
            reinterpret_cast<CDisplayManagerWindowView*>(
                g_DisplayManagerWindowStorage);
    memset(
        g_RenderStateManagerStorage,
        0,
        sizeof(g_RenderStateManagerStorage));
    memset(g_RenderSystemStorage, 0, sizeof(g_RenderSystemStorage));
    memset(g_SystemManagerStorage, 0, sizeof(g_SystemManagerStorage));
    *reinterpret_cast<fable_u8**>(
        g_SystemManagerStorage + 0x60) =
            g_RenderSystemStorage;
    *reinterpret_cast<fable_u8**>(
        g_RenderSystemStorage + 0x08) =
            g_RenderStateManagerStorage;
    RenderStateManager().PD3DDevice0004 =
        reinterpret_cast<FableRenderStateDevice*>(g_Device);
    CaptureManager().captureOffset2814 = 1;
    InitialiseSoldStateMetadata();
    return true;
}

bool FABLE_FASTCALL FableRenderVisualD3D9(
    fable_i32 clientWidth,
    fable_i32 clientHeight)
{
    if (
        g_Device == 0 ||
        g_Texture == 0 ||
        clientWidth <= 0 ||
        clientHeight <= 0)
    {
        return false;
    }

    // The retail frontend maps its 640x480 view area independently onto the
    // active display width and height. Its artwork supplies the dark side
    // regions visible in widescreen; applying another 4:3 aspect fit here
    // incorrectly shrinks the entire press-start composition.
    const fable_i32 drawWidth = clientWidth;
    const fable_i32 drawHeight = clientHeight;
    const float left = -0.5f;
    const float top = -0.5f;
    const float right = left + drawWidth;
    const float bottom = top + drawHeight;

    const float designScaleX =
        static_cast<float>(drawWidth) /
        static_cast<float>(g_ArtworkWidth);
    const float designScaleY =
        static_cast<float>(drawHeight) /
        static_cast<float>(g_ArtworkHeight);
    FableD3DTexture9* overlayTexture = g_TitleTexture;
    fable_i32 overlayWidth = g_TitleWidth;
    fable_i32 overlayHeight = g_TitleHeight;
    fable_u32 overlayFrame = 0;
    fable_u32 overlayFrameCount = 1;
    if (g_MainMenuActive)
    {
        overlayTexture = g_MenuTexture;
        overlayWidth = 640;
        overlayHeight = 480;
        overlayFrame = g_MainMenuSelection;
        overlayFrameCount = 7;
    }
    else if (g_OptionsMenuActive)
    {
        overlayTexture = g_OptionsTexture;
        overlayWidth = 640;
        overlayHeight = 480;
        overlayFrame = g_OptionsSelection;
        overlayFrameCount = 8;
    }
    else if (g_SaveMenuActive)
    {
        overlayTexture = g_OptionsTexture;
        overlayWidth = 640;
        overlayHeight = 480;
        overlayFrame = 4 + g_SaveSelection;
        overlayFrameCount = 8;
    }
    else if (g_DetailScreen != 0)
    {
        overlayTexture = g_OptionsTexture;
        overlayWidth = 640;
        overlayHeight = 480;
        overlayFrame = 3 + g_DetailScreen;
        overlayFrameCount = 8;
    }
    else if (g_QuitPromptActive)
    {
        overlayTexture = 0;
        overlayWidth = 0;
        overlayHeight = 0;
    }
    const bool titleUsesDesignCanvas =
        overlayWidth == g_ArtworkWidth &&
        overlayHeight == g_ArtworkHeight;
    const float titleLeft =
        left + (titleUsesDesignCanvas ? 0.0f : 70.0f) * designScaleX;
    const float titleTop =
        top + (titleUsesDesignCanvas ? 0.0f : 30.0f) * designScaleY;
    const float titleRight =
        titleLeft + overlayWidth * designScaleX;
    const float titleBottom =
        titleTop + overlayHeight * designScaleY;

    FableVisualVertex vertices[128];
    FableRender2DPlanRecord records[48];
    fable_u32 vertexCount = 0;
    fable_u32 recordCount = 0;
    const bool coastalBackground =
        g_MainMenuActive || g_DetailScreen == 2;
    FableD3DTexture9* backgroundTexture =
        coastalBackground ? g_CoastalTexture : g_ForestTexture;
    FableD3DTexture9* sunbeamTexture =
        coastalBackground
            ? g_CoastalSunbeamTexture
            : g_SunbeamTexture;
    if (
        !g_QuitPromptActive &&
        backgroundTexture != 0 &&
        sunbeamTexture != 0)
    {
        if (g_AnimationStartTick == 0)
        {
            g_AnimationStartTick = GetTickCount();
            g_ForestTransitionStart = 0;
            g_SunbeamTransitionStart = 0;
            g_ForestFrame = 0;
            g_SunbeamFrame = 0;
            g_AnimationRandomState = 0x4C494F4Eu;
            g_NextForestFrame =
                ChooseNextAnimationFrame(g_ForestFrame, 4);
            g_NextSunbeamFrame =
                ChooseNextAnimationFrame(g_SunbeamFrame, 3);
        }
        const FableD3DDword elapsed =
            GetTickCount() - g_AnimationStartTick;

        const FableD3DDword forestDurations[4] = {
            8000, 8000, 8000, 2000
        };
        while (
            elapsed - g_ForestTransitionStart >=
            forestDurations[g_ForestFrame])
        {
            g_ForestTransitionStart +=
                forestDurations[g_ForestFrame];
            g_ForestFrame = g_NextForestFrame;
            g_NextForestFrame =
                ChooseNextAnimationFrame(g_ForestFrame, 4);
        }
        const FableD3DDword forestPhase =
            elapsed - g_ForestTransitionStart;
        const fable_u32 forestAlpha =
            forestPhase * 255 /
            forestDurations[g_ForestFrame];
        const float forestV0 =
            static_cast<float>(g_ForestFrame) / 4.0f;
        const float forestV1 =
            static_cast<float>(g_ForestFrame + 1) / 4.0f;
        const float nextForestV0 =
            static_cast<float>(g_NextForestFrame) / 4.0f;
        const float nextForestV1 =
            static_cast<float>(g_NextForestFrame + 1) / 4.0f;
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            backgroundTexture,
            left, top, right, bottom,
            0.0f, forestV0, 1.0f, forestV1,
            0xFFFFFFFFu);
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            backgroundTexture,
            left, top, right, bottom,
            0.0f, nextForestV0, 1.0f, nextForestV1,
            (forestAlpha << 24) | 0x00FFFFFFu);

        while (elapsed - g_SunbeamTransitionStart >= 2000)
        {
            g_SunbeamTransitionStart += 2000;
            g_SunbeamFrame = g_NextSunbeamFrame;
            g_NextSunbeamFrame =
                ChooseNextAnimationFrame(g_SunbeamFrame, 3);
        }
        const FableD3DDword sunbeamPhase =
            elapsed - g_SunbeamTransitionStart;
        const fable_u32 sunbeamAlpha =
            sunbeamPhase * 255 / 2000;
        const float sunbeamV0 =
            static_cast<float>(g_SunbeamFrame) / 3.0f;
        const float sunbeamV1 =
            static_cast<float>(g_SunbeamFrame + 1) / 3.0f;
        const float nextSunbeamV0 =
            static_cast<float>(g_NextSunbeamFrame) / 3.0f;
        const float nextSunbeamV1 =
            static_cast<float>(g_NextSunbeamFrame + 1) / 3.0f;
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            sunbeamTexture,
            left, top, right, bottom,
            0.0f, sunbeamV0, 1.0f, sunbeamV1,
            ((255 - sunbeamAlpha) << 24) | 0x00FFFFFFu);
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            sunbeamTexture,
            left, top, right, bottom,
            0.0f, nextSunbeamV0, 1.0f, nextSunbeamV1,
            (sunbeamAlpha << 24) | 0x00FFFFFFu);
    }
    else if (!g_QuitPromptActive)
    {
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            g_Texture,
            left, top, right, bottom,
            0.0f, 0.0f, 1.0f, 1.0f,
            0xFFFFFFFFu);
    }
    if (
        (g_OptionsMenuActive || g_SaveMenuActive ||
         g_DetailScreen != 0) &&
        g_TitleSegmentTexture != 0 &&
        g_TitleRuleComponents.size != 0)
    {
        const FableUiRender2DBinding titleRuleBinding = {
            122,
            reinterpret_cast<fable_u32>(
                g_TitleSegmentTexture),
            0.0f,
            0.0f,
            1.0f,
            1.0f,
            0xFFFFFFFFu
        };
        const FableUiVector2 clipMinimum = {left, top};
        const FableUiVector2 clipMaximum = {right, bottom};
        FableUiRender2DAppendTarget titleRuleTarget = {
            vertices,
            128,
            &vertexCount,
            records,
            48,
            &recordCount,
            &clipMinimum,
            &clipMaximum
        };
        if (!FableAppendUiGeneratedComponentsToRender2D(
                &g_TitleRuleComponents,
                &titleRuleBinding,
                1,
                left,
                top + 35.0f * designScaleY,
                designScaleX,
                designScaleY,
                &titleRuleTarget))
        {
            return false;
        }
    }
    const FableUiGeneratedComponentVector* selectedButtonComponents = 0;
    float selectedButtonX = 0.0f;
    float selectedButtonY = 0.0f;
    if (g_MainMenuActive && g_MainMenuSelection < 7)
    {
        selectedButtonComponents =
            g_MainMenuSelection == 0
                ? &g_MainMenuBigButtonComponents
                : &g_OptionsButtonComponents;
        selectedButtonX =
            g_MainMenuSelection == 0 ? 120.0f : 180.0f;
        selectedButtonY =
            static_cast<float>(
                193 + kMainMenuRowYOffsets[g_MainMenuSelection]);
    }
    else if (g_OptionsMenuActive && g_OptionsSelection < 4)
    {
        selectedButtonComponents = &g_OptionsButtonComponents;
        selectedButtonX = 180.0f;
        selectedButtonY =
            static_cast<float>(143 + g_OptionsSelection * 30);
    }
    if (
        selectedButtonComponents != 0 &&
        g_ButtonLeftTexture != 0 &&
        g_ButtonMiddleTexture != 0 &&
        g_ButtonRightTexture != 0 &&
        selectedButtonComponents->size != 0)
    {
        const FableUiRender2DBinding buttonBindings[3] = {
            {
                129,
                reinterpret_cast<fable_u32>(
                    g_ButtonLeftTexture),
                0.0f, 0.0f, 1.0f, 1.0f,
                0xFFFFFFFFu
            },
            {
                130,
                reinterpret_cast<fable_u32>(
                    g_ButtonRightTexture),
                0.0f, 0.0f, 1.0f, 1.0f,
                0xFFFFFFFFu
            },
            {
                131,
                reinterpret_cast<fable_u32>(
                    g_ButtonMiddleTexture),
                0.0f, 0.0f, 1.0f, 1.0f,
                0xFFFFFFFFu
            }
        };
        const FableUiVector2 clipMinimum = {left, top};
        const FableUiVector2 clipMaximum = {right, bottom};
        FableUiRender2DAppendTarget buttonTarget = {
            vertices,
            128,
            &vertexCount,
            records,
            48,
            &recordCount,
            &clipMinimum,
            &clipMaximum
        };
        if (!FableAppendUiGeneratedComponentsToRender2D(
                selectedButtonComponents,
                buttonBindings,
                3,
                left + selectedButtonX * designScaleX,
                top + selectedButtonY * designScaleY,
                designScaleX,
                designScaleY,
                &buttonTarget))
        {
            return false;
        }
    }
    if (overlayTexture != 0)
    {
        const bool saveAtlasColumn =
            g_SaveMenuActive &&
            overlayTexture == g_OptionsTexture;
        const float overlayU0 =
            saveAtlasColumn
                ? 1024.0f /
                    static_cast<float>(g_OptionsWidth)
                : 0.0f;
        const float overlayU1 =
            saveAtlasColumn
                ? 1664.0f /
                    static_cast<float>(g_OptionsWidth)
                : (
                    overlayTexture == g_OptionsTexture
                        ? 640.0f /
                            static_cast<float>(g_OptionsWidth)
                        : (
                            overlayTexture == g_MenuTexture
                                ? 640.0f /
                                    static_cast<float>(g_MenuWidth)
                                : 1.0f
                        )
                );
        const float overlayV0 =
            static_cast<float>(overlayFrame) /
            static_cast<float>(overlayFrameCount);
        const float overlayV1 =
            static_cast<float>(overlayFrame + 1) /
            static_cast<float>(overlayFrameCount);
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            overlayTexture,
            titleLeft, titleTop, titleRight, titleBottom,
            overlayU0,
            overlayV0,
            overlayU1,
            overlayV1,
            0xFFFFFFFFu);
    }
    if (
        g_MainMenuActive &&
        g_MenuTexture != 0 &&
        g_MenuWidth == 1280)
    {
        for (fable_u32 row = 0; row != 7; ++row)
        {
            const FableUiRuntimeListChild& rowChild =
                g_MainMenuRowChildren[row];
            const float rowOffsetX =
                rowChild.currentPosition.x * designScaleX;
            const float rowOffsetY =
                rowChild.currentPosition.y * designScaleY;
            const fable_u32 rowColour =
                (static_cast<fable_u32>(
                    rowChild.currentColour.alpha) << 24) |
                (static_cast<fable_u32>(
                    rowChild.currentColour.red) << 16) |
                (static_cast<fable_u32>(
                    rowChild.currentColour.green) << 8) |
                static_cast<fable_u32>(
                    rowChild.currentColour.blue);
            AppendVisualQuad(
                vertices, vertexCount, records, recordCount,
                g_MenuTexture,
                left + rowOffsetX,
                top + rowOffsetY,
                right + rowOffsetX,
                bottom + rowOffsetY,
                0.5f,
                static_cast<float>(row) / 7.0f,
                1.0f,
                static_cast<float>(row + 1) / 7.0f,
                rowColour);
        }
    }
    if (
        g_OptionsMenuActive &&
        g_OptionsTexture != 0 &&
        g_OptionsWidth == 1664)
    {
        for (fable_u32 row = 0; row != 4; ++row)
        {
            const FableUiRuntimeListChild& rowChild =
                g_OptionsRowChildren[row];
            const float rowOffsetX =
                rowChild.currentPosition.x * designScaleX;
            const float rowOffsetY =
                rowChild.currentPosition.y * designScaleY;
            const fable_u32 rowColour =
                (static_cast<fable_u32>(
                    rowChild.currentColour.alpha) << 24) |
                (static_cast<fable_u32>(
                    rowChild.currentColour.red) << 16) |
                (static_cast<fable_u32>(
                    rowChild.currentColour.green) << 8) |
                static_cast<fable_u32>(
                    rowChild.currentColour.blue);
            AppendVisualQuad(
                vertices, vertexCount, records, recordCount,
                g_OptionsTexture,
                left + rowOffsetX,
                top + rowOffsetY,
                right + rowOffsetX,
                bottom + rowOffsetY,
                1024.0f /
                    static_cast<float>(g_OptionsWidth),
                static_cast<float>(row) / 8.0f,
                1664.0f /
                    static_cast<float>(g_OptionsWidth),
                static_cast<float>(row + 1) / 8.0f,
                rowColour);
        }
    }
    if (
        g_DetailScreen >= 1 &&
        g_DetailScreen <= 3 &&
        g_OptionsTexture != 0)
    {
        const fable_u32 screenIndex = g_DetailScreen - 1;
        for (
            fable_u32 row = 0;
            row != kDetailRowCounts[screenIndex];
            ++row)
        {
            const fable_u32 tileIndex = DetailControlAtlasIndex(
                screenIndex,
                row,
                g_DetailOptionValues[screenIndex][row]);
            const fable_u32 tileColumn = 0;
            const fable_u32 tileRow = tileIndex;
            const float controlLeft =
                left + 300.0f * designScaleX;
            const float controlTop =
                top +
                static_cast<float>(
                    kDetailRowY[screenIndex][row] - 3) *
                designScaleY;
            AppendVisualQuad(
                vertices, vertexCount, records, recordCount,
                g_OptionsTexture,
                controlLeft,
                controlTop,
                controlLeft + 200.0f * designScaleX,
                controlTop + 30.0f * designScaleY,
                (640.0f + tileColumn * 200.0f) /
                    static_cast<float>(g_OptionsWidth),
                (tileRow * 30.0f) /
                    static_cast<float>(g_OptionsHeight),
                (840.0f + tileColumn * 200.0f) /
                    static_cast<float>(g_OptionsWidth),
                ((tileRow + 1) * 30.0f) /
                    static_cast<float>(g_OptionsHeight),
                0xFFFFFFFFu);
        }
    }
    if (
        g_DetailScreen == 4 &&
        g_RedefineHover >= 1 &&
        g_RedefineHover <= 9 &&
        g_HelpersTexture != 0)
    {
        const fable_u32 row = g_RedefineHover - 1;
        const float rowTop =
            top + static_cast<float>(110 + row * 26) * designScaleY;
        const float stripTop =
            static_cast<float>(5 * 480 + row * 35);
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            g_HelpersTexture,
            left + 8.0f * designScaleX,
            rowTop,
            left + 596.0f * designScaleX,
            rowTop + 35.0f * designScaleY,
            0.0f,
            stripTop / 2880.0f,
            588.0f / 640.0f,
            (stripTop + 35.0f) / 2880.0f,
            0xFFFFFFFFu);
    }
    if (
        g_DetailScreen == 4 &&
        g_OptionsTexture != 0)
    {
        for (fable_u32 row = 0; row != 9; ++row)
        {
            const fable_u32 keyValue =
                g_RedefineSelection == row + 1
                    ? 7
                    : g_RedefineKeyValues[row];
            const float keyTop =
                static_cast<float>(keyValue * 26);
            const float rowTop =
                top + static_cast<float>(115 + row * 26) * designScaleY;
            AppendVisualQuad(
                vertices, vertexCount, records, recordCount,
                g_OptionsTexture,
                left + 420.0f * designScaleX,
                rowTop,
                left + 596.0f * designScaleX,
                rowTop + 26.0f * designScaleY,
                840.0f / static_cast<float>(g_OptionsWidth),
                keyTop / static_cast<float>(g_OptionsHeight),
                1016.0f / static_cast<float>(g_OptionsWidth),
                (keyTop + 26.0f) /
                    static_cast<float>(g_OptionsHeight),
                0xFFFFFFFFu);
        }
    }
    if (
        g_DetailScreen == 4 &&
        g_RedefineResetHover >= 1 &&
        g_RedefineResetHover <= 2 &&
        g_HelpersTexture != 0)
    {
        const fable_u32 column = g_RedefineResetHover - 1;
        const float atlasTop = static_cast<float>(5 * 480 + 320);
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            g_HelpersTexture,
            left + static_cast<float>(column * 320) * designScaleX,
            top + 389.0f * designScaleY,
            left + static_cast<float>((column + 1) * 320) * designScaleX,
            top + 453.0f * designScaleY,
            static_cast<float>(column * 320) / 640.0f,
            atlasTop / 2880.0f,
            static_cast<float>((column + 1) * 320) / 640.0f,
            (atlasTop + 64.0f) / 2880.0f,
            0xFFFFFFFFu);
    }
    if ((g_OptionsMenuActive || g_SaveMenuActive) &&
        g_HelpersTexture != 0)
    {
        const fable_u32 helperFrame =
            g_OptionsBackHovered ? 1 : 0;
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            g_HelpersTexture,
            left, top, right, bottom,
            0.0f,
            static_cast<float>(helperFrame) / 6.0f,
            1.0f,
            static_cast<float>(helperFrame + 1) / 6.0f,
            0xFFFFFFFFu);
    }
    else if (g_QuitPromptActive && g_HelpersTexture != 0)
    {
        const fable_u32 helperFrame = 2 + g_QuitHover;
        AppendVisualQuad(
            vertices, vertexCount, records, recordCount,
            g_HelpersTexture,
            left, top, right, bottom,
            0.0f,
            static_cast<float>(helperFrame) / 6.0f,
            1.0f,
            static_cast<float>(helperFrame + 1) / 6.0f,
            0xFFFFFFFFu);
    }
    FableRender2DPlanEvent planEvents[68];
    FableRender2DPlanOutput plan = {
        planEvents,
        68,
        0,
        false
    };
    FableBuildRender2DBatchPlan(records, recordCount, plan);
    if (plan.overflow)
    {
        return false;
    }

    FableD3DClear clear =
        reinterpret_cast<FableD3DClear>(
            g_Device->vtable[43]);
    FableD3DBeginScene beginScene =
        reinterpret_cast<FableD3DBeginScene>(
            g_Device->vtable[41]);
    FableD3DEndScene endScene =
        reinterpret_cast<FableD3DEndScene>(
            g_Device->vtable[42]);
    FableD3DPresent present =
        reinterpret_cast<FableD3DPresent>(
            g_Device->vtable[17]);
    if (Failed(clear(
            g_Device,
            0,
            0,
            kD3DClearTarget | kD3DClearZBuffer,
            0xFF000000u,
            1.0f,
            0)) ||
        Failed(beginScene(g_Device)))
    {
        return false;
    }

    Render2DAdapterFlush flushes[8];
    fable_u32 flushCount = 0;
    fable_u32 currentTexture = 0;
    for (fable_u32 eventIndex = 0;
         eventIndex < plan.count;
         ++eventIndex)
    {
        const FableRender2DPlanEvent& event =
            plan.events[eventIndex];
        if (event.kind == FABLE_RENDER2D_PLAN_BIND_TEXTURE)
        {
            currentTexture = event.argument0;
        }
        else if (event.kind == FABLE_RENDER2D_PLAN_FLUSH)
        {
            if (flushCount >= 8)
            {
                endScene(g_Device);
                return false;
            }
            Render2DAdapterFlush& flush =
                flushes[flushCount++];
            flush.textureIdentity = currentTexture;
            flush.primitiveType = event.argument0;
            flush.primitiveCount = event.argument1;
            flush.vertexIndex = event.argument2;
        }
    }

    Render2DAdapterInput adapterInput;
    memset(&adapterInput, 0, sizeof(adapterInput));
    C2DBoxF fullWindow = {
        0.0f,
        0.0f,
        static_cast<float>(clientWidth),
        static_cast<float>(clientHeight)
    };
    adapterInput.entryVertexShadersEnabled = true;
    adapterInput.worldTransformNeedsReset = true;
    g_CShaderRenderManager_013BC470.updateFlags3D8 |= 1;
    adapterInput.dirtyFlagsAfterShaderApply =
        g_CShaderRenderManager_013BC470.updateFlags3D8;
    adapterInput.firstWindowIdentity =
        reinterpret_cast<fable_u32>(&fullWindow);
    adapterInput.flushes = flushes;
    adapterInput.flushCount = flushCount;
    VisualRender2DAdapter adapter(
        vertices,
        vertexCount);
    FableDriveRender2DDrawListAdapter(
        adapterInput,
        adapter);
    const FableD3DResult endResult = endScene(g_Device);
    if (!adapter.Succeeded() || Failed(endResult))
    {
        return false;
    }
    if (Failed(present(g_Device, 0, 0, 0, 0)))
    {
        return false;
    }
    g_Presented = true;
    return true;
}

bool FABLE_FASTCALL FablePresentVisualD3D9Black()
{
    if (g_Device == 0)
        return false;

    FableD3DClear clear =
        reinterpret_cast<FableD3DClear>(
            g_Device->vtable[43]);
    FableD3DPresent present =
        reinterpret_cast<FableD3DPresent>(
            g_Device->vtable[17]);
    if (Failed(clear(
            g_Device,
            0,
            0,
            kD3DClearTarget | kD3DClearZBuffer,
            0xFF000000u,
            1.0f,
            0)) ||
        Failed(present(g_Device, 0, 0, 0, 0)))
    {
        return false;
    }

    g_Presented = true;
    return true;
}

bool FABLE_FASTCALL FableResizeVisualD3D9(
    fable_i32 backBufferWidth,
    fable_i32 backBufferHeight)
{
    if (
        g_Device == 0 ||
        g_DeviceWindow == 0 ||
        backBufferWidth <= 0 ||
        backBufferHeight <= 0)
    {
        return false;
    }

    FableD3DPresentParameters parameters = {};
    parameters.backBufferWidth = backBufferWidth;
    parameters.backBufferHeight = backBufferHeight;
    parameters.backBufferCount = 1;
    parameters.swapEffect = kD3DSwapEffectDiscard;
    parameters.deviceWindow = g_DeviceWindow;
    parameters.windowed = 1;
    parameters.enableAutoDepthStencil = 1;
    parameters.autoDepthStencilFormat = kD3DFormatD16;

    FableD3DReset reset =
        reinterpret_cast<FableD3DReset>(
            g_Device->vtable[16]);
    if (Failed(reset(g_Device, &parameters)))
        return false;

    fable_u8* displayManagerBytes =
        reinterpret_cast<fable_u8*>(
            g_DisplayManagerWindowStorage);
    *reinterpret_cast<fable_i32*>(
        displayManagerBytes + 0x194) = backBufferWidth;
    *reinterpret_cast<fable_i32*>(
        displayManagerBytes + 0x198) = backBufferHeight;
    RenderStateManager().PD3DDevice0004 =
        reinterpret_cast<FableRenderStateDevice*>(g_Device);
    CaptureManager().captureCount2808 = 0;
    CaptureManager().pendingRestoreCount280C = 0;
    CaptureManager().captureOffset2814 = 1;
    InitialiseSoldStateMetadata();
    g_Presented = false;
    return true;
}

void FABLE_FASTCALL FableShutdownVisualD3D9()
{
    FableReleaseUiGeneratedComponents(
        &g_MainMenuBigButtonComponents);
    FableReleaseUiGeneratedComponents(
        &g_OptionsButtonComponents);
    FableReleaseUiGeneratedComponents(&g_TitleRuleComponents);
    ReleaseObject(g_ButtonRightTexture);
    ReleaseObject(g_ButtonMiddleTexture);
    ReleaseObject(g_ButtonLeftTexture);
    ReleaseObject(g_TitleSegmentTexture);
    ReleaseObject(g_HelpersTexture);
    ReleaseObject(g_OptionsTexture);
    ReleaseObject(g_CoastalSunbeamTexture);
    ReleaseObject(g_CoastalTexture);
    ReleaseObject(g_MenuTexture);
    ReleaseObject(g_SunbeamTexture);
    ReleaseObject(g_ForestTexture);
    ReleaseObject(g_TitleTexture);
    ReleaseObject(g_Texture);
    ReleaseObject(g_Device);
    ReleaseObject(g_Direct3D);
    g_DeviceWindow = 0;
    memset(
        &g_RenderManagerCore,
        0,
        sizeof(g_RenderManagerCore));
    memset(
        g_RenderStateManagerStorage,
        0,
        sizeof(g_RenderStateManagerStorage));
    memset(g_RenderSystemStorage, 0, sizeof(g_RenderSystemStorage));
    memset(g_SystemManagerStorage, 0, sizeof(g_SystemManagerStorage));
    g_ArtworkWidth = 0;
    g_ArtworkHeight = 0;
    g_TitleWidth = 0;
    g_TitleHeight = 0;
    g_MenuWidth = 0;
    g_MenuHeight = 0;
    g_OptionsWidth = 0;
    g_OptionsHeight = 0;
    g_TitleSegmentWidth = 0;
    g_TitleSegmentHeight = 0;
    g_AnimationStartTick = 0;
    g_ForestTransitionStart = 0;
    g_SunbeamTransitionStart = 0;
    g_MainMenuSelection = 0;
    memset(
        g_MainMenuRowChildren,
        0,
        sizeof(g_MainMenuRowChildren));
    g_OptionsSelection = 0;
    g_SaveSelection = 0;
    memset(
        g_OptionsRowChildren,
        0,
        sizeof(g_OptionsRowChildren));
    g_DetailScreen = 0;
    memset(g_DetailOptionValues, 0, sizeof(g_DetailOptionValues));
    g_QuitHover = 0;
    g_MainMenuActive = false;
    g_OptionsMenuActive = false;
    g_SaveMenuActive = false;
    g_OptionsBackHovered = false;
    g_QuitPromptActive = false;
    g_Presented = false;
}

void FABLE_FASTCALL FableSetVisualFrontendMainMenu(bool active)
{
    if (g_MainMenuActive == active)
        return;
    if (active &&
        (
            g_MenuTexture == 0 ||
            g_CoastalTexture == 0 ||
            g_CoastalSunbeamTexture == 0
        ))
    {
        return;
    }
    g_MainMenuActive = active;
    if (active)
    {
        g_MainMenuSelection = 0;
        InitialiseMainMenuRowStates();
        g_OptionsMenuActive = false;
        g_SaveMenuActive = false;
        g_DetailScreen = 0;
        g_QuitPromptActive = false;
    }
    // Both compiled swapping widgets activate from frame zero.
    g_AnimationStartTick = 0;
}

void FABLE_FASTCALL FableSetVisualFrontendMainMenuSelection(
    fable_u32 selection)
{
    if (!g_MainMenuActive || selection >= 7)
        return;
    if (selection == g_MainMenuSelection)
        return;
    ApplyMainMenuRowState(g_MainMenuSelection, 4);
    ApplyMainMenuRowState(selection, 3);
    g_MainMenuSelection = selection;
}

bool FABLE_FASTCALL FableScrollVisualFrontendMainMenu(
    bool scrollDown,
    fable_u32* selected,
    fable_u32* soundRequest)
{
    if (soundRequest != 0)
        *soundRequest = FableUiFrontEndSoundNone;
    if (!g_MainMenuActive || selected == 0)
        return false;
    const fable_u32 previousSelection =
        g_MainMenuSelection;
    FableUiFrontEndListScrollPlan plan = {};
    const bool moved = FableApplyUiFrontEndListScroll(
            scrollDown,
            true,
            0,
            g_MainMenuRowChildren,
            7,
            7,
            static_cast<long>(g_MainMenuSelection),
            &plan);
    if (soundRequest != 0)
        *soundRequest = static_cast<fable_u32>(plan.soundRequest);
    if (!moved)
    {
        return false;
    }
    g_MainMenuSelection =
        static_cast<fable_u32>(plan.selectedChild);
    ApplyMainMenuRowState(previousSelection, 4);
    ApplyMainMenuRowState(g_MainMenuSelection, 3);
    *selected = g_MainMenuSelection;
    return true;
}

void FABLE_FASTCALL FableSetVisualFrontendOptionsMenu(bool active)
{
    if (g_OptionsMenuActive == active)
        return;
    if (active &&
        (
            g_OptionsTexture == 0 ||
            g_HelpersTexture == 0
        ))
    {
        return;
    }
    g_OptionsMenuActive = active;
    if (active)
    {
        g_MainMenuActive = false;
        g_SaveMenuActive = false;
        g_DetailScreen = 0;
        g_QuitPromptActive = false;
        g_OptionsSelection = 0;
        InitialiseOptionsRowStates();
        g_OptionsBackHovered = false;
    }
    g_AnimationStartTick = 0;
}

void FABLE_FASTCALL FableSetVisualFrontendOptionsSelection(
    fable_u32 selection)
{
    if (!g_OptionsMenuActive || selection >= 4)
        return;
    if (selection == g_OptionsSelection)
        return;
    ApplyOptionsRowState(g_OptionsSelection, 4);
    ApplyOptionsRowState(selection, 3);
    g_OptionsSelection = selection;
}

void FABLE_FASTCALL FableSetVisualFrontendOptionsBackHovered(bool hovered)
{
    if (!g_OptionsMenuActive && !g_SaveMenuActive)
        return;
    g_OptionsBackHovered = hovered;
}

void FABLE_FASTCALL FableSetVisualFrontendSaveMenu(bool active)
{
    if (g_SaveMenuActive == active)
        return;
    if (active &&
        (
            g_OptionsTexture == 0 ||
            g_HelpersTexture == 0
        ))
    {
        return;
    }
    g_SaveMenuActive = active;
    if (active)
    {
        g_MainMenuActive = false;
        g_OptionsMenuActive = false;
        g_DetailScreen = 0;
        g_QuitPromptActive = false;
        g_SaveSelection = 0;
        g_OptionsBackHovered = false;
    }
    g_AnimationStartTick = 0;
}

void FABLE_FASTCALL FableSetVisualFrontendSaveSelection(
    fable_u32 selection)
{
    if (!g_SaveMenuActive || selection >= 4)
        return;
    g_SaveSelection = selection;
}

void FABLE_FASTCALL FableSetVisualFrontendDetailScreen(fable_u32 screen)
{
    if (screen >= 5 || (screen != 0 && g_OptionsTexture == 0))
        return;
    if (g_DetailScreen == screen)
        return;
    g_DetailScreen = screen;
    g_RedefineHover = 0;
    g_RedefineResetHover = 0;
    g_RedefineSelection = 0;
    if (screen != 0)
    {
        g_MainMenuActive = false;
        g_OptionsMenuActive = false;
        g_SaveMenuActive = false;
        g_QuitPromptActive = false;
    }
    g_AnimationStartTick = 0;
}

void FABLE_FASTCALL FableSetVisualFrontendDetailOptionValue(
    fable_u32 screen,
    fable_u32 row,
    fable_u32 value)
{
    if (screen < 1 || screen > 3)
        return;
    const fable_u32 screenIndex = screen - 1;
    if (
        row >= kDetailRowCounts[screenIndex] ||
        value >= kDetailValueCounts[screenIndex][row])
    {
        return;
    }
    g_DetailOptionValues[screenIndex][row] = value;
}

void FABLE_FASTCALL FableSetVisualFrontendRedefineHover(fable_u32 hover)
{
    if (g_DetailScreen != 4 || hover > 9)
        return;
    g_RedefineHover = hover;
}

void FABLE_FASTCALL FableSetVisualFrontendRedefineResetHover(
    fable_u32 hover)
{
    if (g_DetailScreen != 4 || hover > 2)
        return;
    g_RedefineResetHover = hover;
}

void FABLE_FASTCALL FableSetVisualFrontendRedefineSelection(
    fable_u32 selection)
{
    if (g_DetailScreen != 4 || selection > 9)
        return;
    g_RedefineSelection = selection;
}

void FABLE_FASTCALL FableSetVisualFrontendRedefineKey(
    fable_u32 row,
    fable_u32 keyValue)
{
    if (
        g_DetailScreen != 4 ||
        row >= 9 ||
        keyValue >= kRedefineKeyValueCount ||
        keyValue == 7)
    {
        return;
    }
    g_RedefineKeyValues[row] = keyValue;
}

void FABLE_FASTCALL FableSetVisualFrontendQuitPrompt(bool active)
{
    if (g_QuitPromptActive == active)
        return;
    if (active && g_HelpersTexture == 0)
        return;
    g_QuitPromptActive = active;
    if (active)
    {
        g_MainMenuActive = false;
        g_OptionsMenuActive = false;
        g_SaveMenuActive = false;
        g_DetailScreen = 0;
        g_QuitHover = 0;
    }
    g_AnimationStartTick = 0;
}

void FABLE_FASTCALL FableSetVisualFrontendQuitHover(fable_u32 hover)
{
    if (!g_QuitPromptActive || hover >= 3)
        return;
    g_QuitHover = hover;
}

bool FABLE_FASTCALL FableIsVisualD3D9Active()
{
    return g_Device != 0 && g_Texture != 0;
}

bool FABLE_FASTCALL FableWasVisualD3D9Presented()
{
    return g_Presented;
}
