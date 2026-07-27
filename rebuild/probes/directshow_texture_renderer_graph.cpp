#include <streams.h>
#include <d3d9.h>
#include <stdio.h>
#include <string.h>

#include "fable_video_system.h"

volatile LONG g_FrameCount = 0;
volatile LONG g_ChangingFrameObserved = 0;
volatile LONG g_FirstFrameHashSet = 0;
volatile LONG g_FramePublished = 0;
volatile LONG g_FrameConsumed = 0;
unsigned long g_FirstFrameHash = 0;
CVideoSysRecoveredLayout g_VideoSystem;
IDirect3D9* g_Direct3D = 0;
IDirect3DDevice9* g_Direct3DDevice = 0;
IDirect3DTexture9* g_DecodedTexture = 0;
IDirect3DTexture9* g_PresentationTexture = 0;
unsigned long g_DestinationRowPitch = 0;
unsigned long g_CurrentFrameHash = 0;

bool FABLE_FASTCALL WriteDecodedTexture(
    void* context,
    fable_u32 textureFormat,
    const fable_u8* source,
    fable_u32 sourceRowPitch,
    fable_u32 width,
    fable_u32 height)
{
    IDirect3DTexture9* const texture =
        static_cast<IDirect3DTexture9*>(context);
    D3DLOCKED_RECT locked;
    if (
        texture == 0 ||
        FAILED(texture->LockRect(0, &locked, 0, 0)))
    {
        return false;
    }

    g_DestinationRowPitch =
        static_cast<unsigned long>(locked.Pitch);
    const bool converted = FableConvertVideoRgb24Frame(
        textureFormat,
        source,
        sourceRowPitch,
        static_cast<fable_u8*>(locked.pBits),
        g_DestinationRowPitch,
        width,
        height);

    unsigned long hash = 2166136261UL;
    if (converted)
    {
        const unsigned long rowBytes = width * 4;
        const unsigned long sampleStride =
            rowBytes * height > 4096
                ? rowBytes * height / 4096
                : 1;
        unsigned long sampled = 0;
        for (fable_u32 y = 0; y < height; ++y)
        {
            const fable_u8* const row =
                static_cast<const fable_u8*>(locked.pBits) +
                y * g_DestinationRowPitch;
            for (fable_u32 x = 0; x < rowBytes; ++x)
            {
                if ((sampled++ % sampleStride) == 0)
                {
                    hash ^= row[x];
                    hash *= 16777619UL;
                }
            }
        }
    }
    const HRESULT unlockResult = texture->UnlockRect(0);
    if (!converted || FAILED(unlockResult))
        return false;
    g_CurrentFrameHash = hash;
    return true;
}

bool ConsumePublishedTexture(
    unsigned long width,
    unsigned long height)
{
    if (
        WaitForSingleObjectEx(
            g_VideoSystem.frameReadyEvent7C,
            100,
            TRUE) != WAIT_OBJECT_0)
    {
        return false;
    }

    IDirect3DTexture9* const source =
        static_cast<IDirect3DTexture9*>(
            g_VideoSystem.decoderTexture80);
    if (source == 0 || g_PresentationTexture == 0)
        return false;

    EnterCriticalSection(
        reinterpret_cast<LPCRITICAL_SECTION>(
            g_VideoSystem.textureCriticalSection64));
    D3DLOCKED_RECT sourceLocked;
    D3DLOCKED_RECT destinationLocked;
    HRESULT sourceResult =
        source->LockRect(0, &sourceLocked, 0, D3DLOCK_READONLY);
    HRESULT destinationResult = E_FAIL;
    if (SUCCEEDED(sourceResult))
    {
        destinationResult =
            g_PresentationTexture->LockRect(
                0,
                &destinationLocked,
                0,
                0);
    }

    if (SUCCEEDED(sourceResult) && SUCCEEDED(destinationResult))
    {
        const unsigned long rowBytes = width * 4;
        for (unsigned long y = 0; y < height; ++y)
        {
            memcpy(
                static_cast<unsigned char*>(
                    destinationLocked.pBits) +
                    y * destinationLocked.Pitch,
                static_cast<const unsigned char*>(
                    sourceLocked.pBits) +
                    y * sourceLocked.Pitch,
                rowBytes);
        }
        g_PresentationTexture->UnlockRect(0);
    }
    if (SUCCEEDED(sourceResult))
        source->UnlockRect(0);
    LeaveCriticalSection(
        reinterpret_cast<LPCRITICAL_SECTION>(
            g_VideoSystem.textureCriticalSection64));

    return
        SUCCEEDED(sourceResult) &&
        SUCCEEDED(destinationResult);
}

class FableTextureRendererGraphProbe : public CBaseVideoRenderer
{
public:
    explicit FableTextureRendererGraphProbe(HRESULT* result)
        : CBaseVideoRenderer(
            CLSID_NULL,
            TEXT("Fable Texture Renderer Graph Probe"),
            0,
            result),
          videoSystem160(&g_VideoSystem),
          texture164(0),
          textureFormat168(0),
          decodedWidth16C(0),
          decodedHeight170(0),
          sourceRowPitch174(0),
          ownsTexture178(0),
          unknown17C(0)
    {
    }

    HRESULT CheckMediaType(const CMediaType* mediaType)
    {
        if (*mediaType->FormatType() != FORMAT_VideoInfo)
            return E_INVALIDARG;
        if (
            *mediaType->Type() != MEDIATYPE_Video ||
            *mediaType->Subtype() != MEDIASUBTYPE_RGB24)
        {
            return E_FAIL;
        }
        return S_OK;
    }

    HRESULT SetMediaType(const CMediaType* mediaType)
    {
        const HRESULT baseResult =
            CBaseVideoRenderer::SetMediaType(mediaType);
        if (FAILED(baseResult))
            return baseResult;

        const VIDEOINFOHEADER* videoInfo =
            reinterpret_cast<const VIDEOINFOHEADER*>(
                mediaType->Format());
        decodedWidth16C =
            static_cast<unsigned long>(
                videoInfo->bmiHeader.biWidth);
        const long signedHeight = videoInfo->bmiHeader.biHeight;
        decodedHeight170 =
            static_cast<unsigned long>(
                signedHeight < 0 ? -signedHeight : signedHeight);
        sourceRowPitch174 = (decodedWidth16C + 1) * 3 & ~3UL;
        textureFormat168 = 0x15;
        if (g_DecodedTexture != 0)
        {
            g_DecodedTexture->Release();
            g_DecodedTexture = 0;
        }
        if (g_PresentationTexture != 0)
        {
            g_PresentationTexture->Release();
            g_PresentationTexture = 0;
        }
        const HRESULT textureResult =
            g_Direct3DDevice->CreateTexture(
                decodedWidth16C,
                decodedHeight170,
                1,
                0,
                D3DFMT_A8R8G8B8,
                D3DPOOL_MANAGED,
                &g_DecodedTexture,
                0);
        if (FAILED(textureResult))
            return textureResult;
        const HRESULT presentationResult =
            g_Direct3DDevice->CreateTexture(
                decodedWidth16C,
                decodedHeight170,
                1,
                0,
                D3DFMT_A8R8G8B8,
                D3DPOOL_MANAGED,
                &g_PresentationTexture,
                0);
        if (FAILED(presentationResult))
            return presentationResult;
        texture164 = g_DecodedTexture;
        return S_OK;
    }

    HRESULT DoRenderSample(IMediaSample* sample)
    {
        BYTE* bytes = 0;
        if (FAILED(sample->GetPointer(&bytes)) || bytes == 0)
            return E_FAIL;

        const long length = sample->GetActualDataLength();
        if (
            length <
                static_cast<long>(
                    sourceRowPitch174 * decodedHeight170) ||
            !FablePublishDecodedVideoFrame(
                g_VideoSystem,
                texture164,
                &WriteDecodedTexture,
                g_DecodedTexture,
                textureFormat168,
                bytes,
                sourceRowPitch174,
                decodedWidth16C,
                decodedHeight170))
        {
            return E_FAIL;
        }
        InterlockedExchange(&g_FramePublished, 1);
        if (!ConsumePublishedTexture(
                decodedWidth16C,
                decodedHeight170))
        {
            return E_FAIL;
        }
        InterlockedExchange(&g_FrameConsumed, 1);

        const unsigned long hash = g_CurrentFrameHash;

        if (InterlockedCompareExchange(
                &g_FirstFrameHashSet,
                1,
                0) == 0)
        {
            g_FirstFrameHash = hash;
        }
        else if (hash != g_FirstFrameHash)
        {
            InterlockedExchange(&g_ChangingFrameObserved, 1);
        }
        InterlockedIncrement(&g_FrameCount);
        return S_OK;
    }

    void* videoSystem160;
    void* texture164;
    unsigned long textureFormat168;
    unsigned long decodedWidth16C;
    unsigned long decodedHeight170;
    unsigned long sourceRowPitch174;
    unsigned char ownsTexture178;
    unsigned char padding179[3];
    unsigned long unknown17C;
};

int main(int argumentCount, char** arguments)
{
    if (argumentCount != 2)
        return 2;

    WCHAR moviePath[1024];
    if (MultiByteToWideChar(
            CP_ACP,
            0,
            arguments[1],
            -1,
            moviePath,
            sizeof(moviePath) / sizeof(moviePath[0])) == 0)
    {
        return 3;
    }

    HRESULT result = CoInitialize(0);
    if (FAILED(result))
        return 4;

    memset(&g_VideoSystem, 0, sizeof(g_VideoSystem));
    InitializeCriticalSection(
        reinterpret_cast<LPCRITICAL_SECTION>(
            g_VideoSystem.textureCriticalSection64));
    g_VideoSystem.frameReadyEvent7C =
        CreateEvent(0, FALSE, FALSE, 0);
    if (g_VideoSystem.frameReadyEvent7C == 0)
    {
        DeleteCriticalSection(
            reinterpret_cast<LPCRITICAL_SECTION>(
                g_VideoSystem.textureCriticalSection64));
        CoUninitialize();
        return 5;
    }

    g_Direct3D = Direct3DCreate9(D3D_SDK_VERSION);
    if (g_Direct3D == 0)
    {
        CloseHandle(g_VideoSystem.frameReadyEvent7C);
        DeleteCriticalSection(
            reinterpret_cast<LPCRITICAL_SECTION>(
                g_VideoSystem.textureCriticalSection64));
        CoUninitialize();
        return 6;
    }
    D3DPRESENT_PARAMETERS present;
    memset(&present, 0, sizeof(present));
    present.Windowed = TRUE;
    present.SwapEffect = D3DSWAPEFFECT_DISCARD;
    present.hDeviceWindow = GetDesktopWindow();
    const HRESULT deviceResult = g_Direct3D->CreateDevice(
        D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        present.hDeviceWindow,
        D3DCREATE_SOFTWARE_VERTEXPROCESSING |
            D3DCREATE_MULTITHREADED,
        &present,
        &g_Direct3DDevice);
    if (FAILED(deviceResult))
    {
        g_Direct3D->Release();
        g_Direct3D = 0;
        CloseHandle(g_VideoSystem.frameReadyEvent7C);
        DeleteCriticalSection(
            reinterpret_cast<LPCRITICAL_SECTION>(
                g_VideoSystem.textureCriticalSection64));
        CoUninitialize();
        return 7;
    }

    IGraphBuilder* graph = 0;
    IMediaControl* control = 0;
    IMediaEvent* event = 0;
    FableTextureRendererGraphProbe* renderer = 0;
    long completionCode = 0;
    int exitCode = 8;

    result = CoCreateInstance(
        CLSID_FilterGraph,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IGraphBuilder,
        reinterpret_cast<void**>(&graph));
    if (FAILED(result))
        goto cleanup;

    renderer = new FableTextureRendererGraphProbe(&result);
    if (renderer == 0 || FAILED(result))
        goto cleanup;
    result = graph->AddFilter(
        renderer,
        L"Fable Texture Renderer");
    if (FAILED(result))
        goto cleanup;

    result = graph->RenderFile(moviePath, 0);
    if (FAILED(result))
        goto cleanup;
    result = graph->QueryInterface(
        IID_IMediaControl,
        reinterpret_cast<void**>(&control));
    if (FAILED(result))
        goto cleanup;
    result = graph->QueryInterface(
        IID_IMediaEvent,
        reinterpret_cast<void**>(&event));
    if (FAILED(result))
        goto cleanup;
    result = control->Run();
    if (FAILED(result))
        goto cleanup;
    result = event->WaitForCompletion(30000, &completionCode);
    if (FAILED(result))
        goto cleanup;

    const DWORD frameReady = WaitForSingleObject(
        g_VideoSystem.frameReadyEvent7C,
        0);
    const bool texturePublished =
        renderer->texture164 != 0 &&
        renderer->texture164 ==
            g_VideoSystem.decoderTexture80;
    printf(
        "DIRECTSHOW_TEXTURE_RENDERER_GRAPH "
        "frames=%ld changing=%ld published=%ld consumed=%ld ready=%lu "
        "texture=%d size=%lux%lu source-pitch=%lu "
        "destination-pitch=%lu event=%ld\n",
        g_FrameCount,
        g_ChangingFrameObserved,
        g_FramePublished,
        g_FrameConsumed,
        frameReady,
        texturePublished ? 1 : 0,
        renderer->decodedWidth16C,
        renderer->decodedHeight170,
        renderer->sourceRowPitch174,
        g_DestinationRowPitch,
        completionCode);
    exitCode =
        g_FrameCount > 1 &&
        g_ChangingFrameObserved != 0 &&
        g_FramePublished != 0 &&
        g_FrameConsumed != 0 &&
        frameReady == WAIT_TIMEOUT &&
        texturePublished &&
        renderer->decodedWidth16C != 0 &&
        renderer->decodedHeight170 != 0
            ? 0
            : 9;

cleanup:
    if (control != 0)
        control->Stop();
    if (event != 0)
        event->Release();
    if (control != 0)
        control->Release();
    if (graph != 0)
        graph->Release();
    if (g_PresentationTexture != 0)
        g_PresentationTexture->Release();
    if (g_DecodedTexture != 0)
        g_DecodedTexture->Release();
    if (g_Direct3DDevice != 0)
        g_Direct3DDevice->Release();
    if (g_Direct3D != 0)
        g_Direct3D->Release();
    g_DecodedTexture = 0;
    g_PresentationTexture = 0;
    g_Direct3DDevice = 0;
    g_Direct3D = 0;
    CloseHandle(g_VideoSystem.frameReadyEvent7C);
    DeleteCriticalSection(
        reinterpret_cast<LPCRITICAL_SECTION>(
            g_VideoSystem.textureCriticalSection64));
    CoUninitialize();
    return exitCode;
}
