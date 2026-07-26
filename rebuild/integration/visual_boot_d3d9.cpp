#include "fable_visual_d3d9.h"
#include "fable_render_state.h"
#include "fable_render2d_vertex_queue.h"
#include "fable_render_capture.h"
#include "fable_render_texture.h"
#include "fable_render_window.h"
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

struct FableD3DViewport
{
    FableD3DDword x;
    FableD3DDword y;
    FableD3DDword width;
    FableD3DDword height;
    float minZ;
    float maxZ;
};

struct FableVisualVertex
{
    float x;
    float y;
    float z;
    float rhw;
    fable_u32 diffuseColour;
    fable_u32 specularColour;
    float u;
    float v;
};

FABLE_STATIC_ASSERT(sizeof(FableVisualVertex) == 0x20);

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
typedef FableD3DResult (FABLE_STDCALL *FableD3DSetViewport)(
    FableD3DDevice9* device,
    const FableD3DViewport* viewport);

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
    const FableD3DDword kD3DFvfXyzRhwColourTexture1 = 0x1C4;

    FableD3D9* g_Direct3D = 0;
    FableD3DDevice9* g_Device = 0;
    FableD3DTexture9* g_Texture = 0;
    CRenderManagerCoreAttachTextureView g_RenderManagerCore = {};
    CDisplayManagerWindowView g_DisplayManagerWindow;
    CRenderStateManagerRealiseView g_RenderStateManager = {};
    CRenderStateInfo g_VisualRenderStates[10] = {};
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

    class VisualRender2DAdapter : public IRender2DDrawListAdapter
    {
    public:
        explicit VisualRender2DAdapter(
            const FableVisualVertex* vertices)
            : vertices_(vertices),
              succeeded_(true),
              drew_(false)
        {
            memset(textures_, 0, sizeof(textures_));
            memset(&captureManager_, 0, sizeof(captureManager_));
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
                captureManager_.captures0008[0].entry00 =
                    &captureSentinel_;
                captureManager_.captures0008[0].value04 = 0;
                captureManager_.captureCount2808 = 1;
                captureManager_.pendingRestoreCount280C = 0;
                captureManager_.captureOffset2814 = 2;
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
                ApplyStateBlock();
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
                RENDER2D_ADAPTER_ATTACH_TEXTURE)
            {
                CTextureAttachView texture = {
                    g_Texture
                };
                g_RenderManagerCore.AttachTextureToStage(
                    argument0,
                    argument1 != 0 ? &texture : 0);
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
                g_RenderStateManager.RealiseRenderState();
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
                Render2DDrawListBlockView* end = begin + 6;
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
                captureManager_.RestoreCaptureBlock();
                if (
                    captureManager_.captureCount2808 != 0 ||
                    captureManager_.pendingRestoreCount280C != 0 ||
                    captureManager_.captureOffset2814 != 1)
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

        void ApplyStateBlock()
        {
            static const fable_u32 stateParameters[10] = {
                7, 22, 27, 137,
                1, 2, 4, 5,
                5, 6
            };
            static const fable_u32 desiredValues[10] = {
                0, 1, 0, 0,
                2, 2, 2, 2,
                2, 2
            };
            static const fable_u8 stateTypes[10] = {
                1, 1, 1, 1,
                2, 2, 2, 2,
                3, 3
            };

            for (fable_u32 index = 0; index < 10; ++index)
            {
                CRenderStateInfo& state =
                    g_VisualRenderStates[index];
                state.DesiredState = desiredValues[index];
                state.StateParam = stateParameters[index];
                state.DirtyListFlag = 1;
                state.StateType = stateTypes[index];
                state.TextureStage = 0;
                g_RenderStateManager.StateUpdateList2008[index] =
                    &state;
            }
            g_RenderStateManager.StateUpdateListSize280C = 10;
        }

        const FableVisualVertex* vertices_;
        CTextureAssignmentView textures_[3];
        CRenderStateManagerCaptureView captureManager_;
        CRenderStateEntry captureSentinel_;
        bool succeeded_;
        bool drew_;
    };
}

void CDisplayManager::SetIntegerViewportEndpoint(
    const C2DBoxI& viewport)
{
    FableD3DViewport deviceViewport = {
        static_cast<FableD3DDword>(viewport.left),
        static_cast<FableD3DDword>(viewport.top),
        static_cast<FableD3DDword>(
            viewport.right - viewport.left),
        static_cast<FableD3DDword>(
            viewport.bottom - viewport.top),
        0.0f,
        1.0f
    };
    FableD3DSetViewport setViewport =
        reinterpret_cast<FableD3DSetViewport>(
            g_Device->vtable[47]);
    setViewport(g_Device, &deviceViewport);
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
    memset(
        &g_RenderManagerCore,
        0,
        sizeof(g_RenderManagerCore));
    g_RenderManagerCore.displayDevice3CF0 =
        reinterpret_cast<FableTextureDevice*>(g_Device);
    reinterpret_cast<CRenderManagerCoreWindowView*>(
        &g_RenderManagerCore)->displayManager3A3C =
            &g_DisplayManagerWindow;
    memset(
        &g_RenderStateManager,
        0,
        sizeof(g_RenderStateManager));
    g_RenderStateManager.PD3DDevice0004 =
        reinterpret_cast<FableRenderStateDevice*>(g_Device);
    memset(
        g_VisualRenderStates,
        0,
        sizeof(g_VisualRenderStates));
    for (fable_u32 stateIndex = 0;
         stateIndex < 10;
         ++stateIndex)
    {
        g_VisualRenderStates[stateIndex].CurrentState =
            0xFFFFFFFFu;
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
        {left, top, 0.0f, 1.0f, 0xFFFFFFFFu, 0, 0.0f, 0.0f},
        {right, top, 0.0f, 1.0f, 0xFFFFFFFFu, 0, 1.0f, 0.0f},
        {left, bottom, 0.0f, 1.0f, 0xFFFFFFFFu, 0, 0.0f, 1.0f},
        {left, bottom, 0.0f, 1.0f, 0xFFFFFFFFu, 0, 0.0f, 1.0f},
        {right, top, 0.0f, 1.0f, 0xFFFFFFFFu, 0, 1.0f, 0.0f},
        {right, bottom, 0.0f, 1.0f, 0xFFFFFFFFu, 0, 1.0f, 1.0f}
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

    Render2DAdapterFlush flushes[4];
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
            if (flushCount >= 4)
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
    adapterInput.firstWindowIdentity =
        reinterpret_cast<fable_u32>(&fullWindow);
    adapterInput.flushes = flushes;
    adapterInput.flushCount = flushCount;
    VisualRender2DAdapter adapter(vertices);
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

void FABLE_FASTCALL FableShutdownVisualD3D9()
{
    ReleaseObject(g_Texture);
    ReleaseObject(g_Device);
    ReleaseObject(g_Direct3D);
    memset(
        &g_RenderManagerCore,
        0,
        sizeof(g_RenderManagerCore));
    memset(
        &g_RenderStateManager,
        0,
        sizeof(g_RenderStateManager));
    memset(
        g_VisualRenderStates,
        0,
        sizeof(g_VisualRenderStates));
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
