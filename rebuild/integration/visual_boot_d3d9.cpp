#include "fable_visual_d3d9.h"
#include "render2d_batch_plan.h"

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

struct FableVisualVertex
{
    float x;
    float y;
    float z;
    float rhw;
    float u;
    float v;
};

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

namespace
{
    const FableD3DUint kD3DSdkVersion = 32;
    const FableD3DUint kD3DDeviceTypeHal = 1;
    const FableD3DDword kD3DSoftwareVertexProcessing = 0x20;
    const FableD3DUint kD3DSwapEffectDiscard = 1;
    const FableD3DUint kD3DFormatA8R8G8B8 = 21;
    const FableD3DUint kD3DPoolManaged = 1;
    const FableD3DDword kD3DClearTarget = 1;
    const FableD3DDword kD3DFvfXyzRhwTexture1 = 0x104;

    FableD3D9* g_Direct3D = 0;
    FableD3DDevice9* g_Device = 0;
    FableD3DTexture9* g_Texture = 0;
    fable_i32 g_ArtworkWidth = 0;
    fable_i32 g_ArtworkHeight = 0;
    bool g_Presented = false;

    bool Failed(FableD3DResult result)
    {
        return result < 0;
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
        fable_i32 artworkWidth,
        fable_i32 artworkHeight,
        fable_i32 artworkPitch,
        fable_u32 artworkBitsPerPixel,
        const void* artworkPixels)
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
                &g_Texture,
                0)))
        {
            return false;
        }

        FableD3DLockedRectangle locked = {};
        FableD3DLockTexture lockTexture =
            reinterpret_cast<FableD3DLockTexture>(
                g_Texture->vtable[19]);
        if (Failed(lockTexture(g_Texture, 0, &locked, 0, 0)))
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
                destinationRow[x] =
                    0xFF000000u |
                    (static_cast<fable_u32>(pixel[2]) << 16) |
                    (static_cast<fable_u32>(pixel[1]) << 8) |
                    static_cast<fable_u32>(pixel[0]);
            }
        }

        FableD3DUnlockTexture unlockTexture =
            reinterpret_cast<FableD3DUnlockTexture>(
                g_Texture->vtable[20]);
        if (Failed(unlockTexture(g_Texture, 0)))
        {
            return false;
        }

        g_ArtworkWidth = artworkWidth;
        g_ArtworkHeight = absoluteHeight;
        return true;
    }
}

bool FABLE_FASTCALL FableInitialiseVisualD3D9(
    void* window,
    fable_i32 backBufferWidth,
    fable_i32 backBufferHeight,
    fable_i32 artworkWidth,
    fable_i32 artworkHeight,
    fable_i32 artworkPitch,
    fable_u32 artworkBitsPerPixel,
    const void* artworkPixels)
{
    FableShutdownVisualD3D9();
    g_Direct3D = Direct3DCreate9(kD3DSdkVersion);
    if (g_Direct3D == 0)
    {
        return false;
    }

    FableD3DPresentParameters parameters = {};
    parameters.backBufferWidth = backBufferWidth;
    parameters.backBufferHeight = backBufferHeight;
    parameters.backBufferCount = 1;
    parameters.swapEffect = kD3DSwapEffectDiscard;
    parameters.deviceWindow = window;
    parameters.windowed = 1;

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
            artworkWidth,
            artworkHeight,
            artworkPitch,
            artworkBitsPerPixel,
            artworkPixels))
    {
        FableShutdownVisualD3D9();
        return false;
    }
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

    fable_i32 drawWidth = clientWidth;
    fable_i32 drawHeight =
        clientWidth * g_ArtworkHeight / g_ArtworkWidth;
    if (drawHeight > clientHeight)
    {
        drawHeight = clientHeight;
        drawWidth =
            clientHeight * g_ArtworkWidth / g_ArtworkHeight;
    }
    const float left =
        static_cast<float>((clientWidth - drawWidth) / 2) - 0.5f;
    const float top =
        static_cast<float>((clientHeight - drawHeight) / 2) - 0.5f;
    const float right = left + drawWidth;
    const float bottom = top + drawHeight;

    FableVisualVertex vertices[6] = {
        {left, top, 0.0f, 1.0f, 0.0f, 0.0f},
        {right, top, 0.0f, 1.0f, 1.0f, 0.0f},
        {left, bottom, 0.0f, 1.0f, 0.0f, 1.0f},
        {left, bottom, 0.0f, 1.0f, 0.0f, 1.0f},
        {right, top, 0.0f, 1.0f, 1.0f, 0.0f},
        {right, bottom, 0.0f, 1.0f, 1.0f, 1.0f}
    };
    FableRender2DPlanRecord records[2];
    memset(records, 0, sizeof(records));
    records[0].textureIdentity =
        reinterpret_cast<fable_u32>(g_Texture);
    records[0].payload.normal.stateBlock = 1;
    records[1] = records[0];
    FableRender2DPlanEvent planEvents[8];
    FableRender2DPlanOutput plan = {
        planEvents,
        8,
        0,
        false
    };
    FableBuildRender2DBatchPlan(records, 2, plan);
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
            kD3DClearTarget,
            0xFF000000u,
            1.0f,
            0)) ||
        Failed(beginScene(g_Device)))
    {
        return false;
    }

    FableD3DSetTexture setTexture =
        reinterpret_cast<FableD3DSetTexture>(
            g_Device->vtable[65]);
    FableD3DSetDwordState setRenderState =
        reinterpret_cast<FableD3DSetDwordState>(
            g_Device->vtable[57]);
    FableD3DSetStageState setTextureStageState =
        reinterpret_cast<FableD3DSetStageState>(
            g_Device->vtable[67]);
    FableD3DSetStageState setSamplerState =
        reinterpret_cast<FableD3DSetStageState>(
            g_Device->vtable[69]);
    FableD3DSetFvf setFvf =
        reinterpret_cast<FableD3DSetFvf>(
            g_Device->vtable[89]);
    FableD3DDrawPrimitiveUp drawPrimitiveUp =
        reinterpret_cast<FableD3DDrawPrimitiveUp>(
            g_Device->vtable[83]);

    setRenderState(g_Device, 7, 0);
    setRenderState(g_Device, 22, 1);
    setRenderState(g_Device, 27, 0);
    setRenderState(g_Device, 137, 0);
    setTextureStageState(g_Device, 0, 1, 2);
    setTextureStageState(g_Device, 0, 2, 2);
    setTextureStageState(g_Device, 0, 4, 2);
    setTextureStageState(g_Device, 0, 5, 2);
    setSamplerState(g_Device, 0, 5, 2);
    setSamplerState(g_Device, 0, 6, 2);
    setFvf(g_Device, kD3DFvfXyzRhwTexture1);
    FableD3DResult drawResult = 0;
    for (fable_u32 eventIndex = 0;
         eventIndex < plan.count;
         ++eventIndex)
    {
        const FableRender2DPlanEvent& event =
            plan.events[eventIndex];
        if (event.kind == FABLE_RENDER2D_PLAN_BIND_TEXTURE)
        {
            drawResult = setTexture(g_Device, 0, g_Texture);
        }
        else if (event.kind == FABLE_RENDER2D_PLAN_FLUSH)
        {
            drawResult = drawPrimitiveUp(
                g_Device,
                event.argument0,
                event.argument1,
                vertices + event.argument2,
                sizeof(FableVisualVertex));
        }
        if (Failed(drawResult))
        {
            endScene(g_Device);
            return false;
        }
    }
    const FableD3DResult endResult = endScene(g_Device);
    if (Failed(drawResult) || Failed(endResult))
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

void FABLE_FASTCALL FableShutdownVisualD3D9()
{
    ReleaseObject(g_Texture);
    ReleaseObject(g_Device);
    ReleaseObject(g_Direct3D);
    g_ArtworkWidth = 0;
    g_ArtworkHeight = 0;
    g_Presented = false;
}

bool FABLE_FASTCALL FableIsVisualD3D9Active()
{
    return g_Device != 0 && g_Texture != 0;
}

bool FABLE_FASTCALL FableWasVisualD3D9Presented()
{
    return g_Presented;
}
