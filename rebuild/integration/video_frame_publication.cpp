#include "fable_video_system.h"

extern "C"
{
    __declspec(dllimport) void FABLE_STDCALL EnterCriticalSection(
        void* criticalSection);
    __declspec(dllimport) void FABLE_STDCALL LeaveCriticalSection(
        void* criticalSection);
    __declspec(dllimport) int FABLE_STDCALL SetEvent(void* eventHandle);
}

bool FABLE_FASTCALL FablePublishDecodedVideoFrame(
    CVideoSysRecoveredLayout& videoSystem,
    void* decodedTexture,
    FableDecodedVideoFrameWriter writer,
    void* writerContext,
    fable_u32 textureFormat,
    const fable_u8* source,
    fable_u32 sourceRowPitch,
    fable_u32 width,
    fable_u32 height)
{
    EnterCriticalSection(videoSystem.textureCriticalSection64);
    const bool converted =
        writer != 0 &&
        writer(
            writerContext,
            textureFormat,
            source,
            sourceRowPitch,
            width,
            height);
    if (converted)
    {
        videoSystem.decoderTexture80 = decodedTexture;
        if (videoSystem.frameReadyEvent7C != 0)
            SetEvent(videoSystem.frameReadyEvent7C);
    }
    LeaveCriticalSection(videoSystem.textureCriticalSection64);
    return converted;
}
