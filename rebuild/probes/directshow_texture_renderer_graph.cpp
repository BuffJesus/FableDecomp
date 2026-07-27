#include <streams.h>
#include <stdio.h>

volatile LONG g_FrameCount = 0;
volatile LONG g_ChangingFrameObserved = 0;
volatile LONG g_FirstFrameHashSet = 0;
unsigned long g_FirstFrameHash = 0;

class FableTextureRendererGraphProbe : public CBaseVideoRenderer
{
public:
    explicit FableTextureRendererGraphProbe(HRESULT* result)
        : CBaseVideoRenderer(
            CLSID_NULL,
            TEXT("Fable Texture Renderer Graph Probe"),
            0,
            result),
          videoSystem160(0),
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
        return S_OK;
    }

    HRESULT DoRenderSample(IMediaSample* sample)
    {
        BYTE* bytes = 0;
        if (FAILED(sample->GetPointer(&bytes)) || bytes == 0)
            return E_FAIL;

        const long length = sample->GetActualDataLength();
        unsigned long hash = 2166136261UL;
        const long stride = length > 4096 ? length / 4096 : 1;
        for (long index = 0; index < length; index += stride)
        {
            hash ^= bytes[index];
            hash *= 16777619UL;
        }

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

    IGraphBuilder* graph = 0;
    IMediaControl* control = 0;
    IMediaEvent* event = 0;
    FableTextureRendererGraphProbe* renderer = 0;
    long completionCode = 0;
    int exitCode = 5;

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

    printf(
        "DIRECTSHOW_TEXTURE_RENDERER_GRAPH "
        "frames=%ld changing=%ld size=%lux%lu pitch=%lu event=%ld\n",
        g_FrameCount,
        g_ChangingFrameObserved,
        renderer->decodedWidth16C,
        renderer->decodedHeight170,
        renderer->sourceRowPitch174,
        completionCode);
    exitCode =
        g_FrameCount > 1 &&
        g_ChangingFrameObserved != 0 &&
        renderer->decodedWidth16C != 0 &&
        renderer->decodedHeight170 != 0
            ? 0
            : 6;

cleanup:
    if (control != 0)
        control->Stop();
    if (event != 0)
        event->Release();
    if (control != 0)
        control->Release();
    if (graph != 0)
        graph->Release();
    CoUninitialize();
    return exitCode;
}
