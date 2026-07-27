#include "fable_video_system.h"

#include <stdio.h>
#include <string.h>

extern "C"
{
    __declspec(dllimport) void FABLE_STDCALL InitializeCriticalSection(
        void* criticalSection);
    __declspec(dllimport) void FABLE_STDCALL DeleteCriticalSection(
        void* criticalSection);
    __declspec(dllimport) void* FABLE_STDCALL CreateEventA(
        void* securityAttributes,
        int manualReset,
        int initialState,
        const char* name);
    __declspec(dllimport) unsigned long FABLE_STDCALL WaitForSingleObject(
        void* handle,
        unsigned long milliseconds);
    __declspec(dllimport) int FABLE_STDCALL CloseHandle(void* handle);
}

namespace
{
    struct WriterContext
    {
        CVideoSysRecoveredLayout* videoSystem;
        fable_u8* destination;
        fable_u32 destinationRowPitch;
        bool observedUnpublishedState;
    };

    bool FABLE_FASTCALL WriteFrame(
        void* opaqueContext,
        fable_u32 textureFormat,
        const fable_u8* source,
        fable_u32 sourceRowPitch,
        fable_u32 width,
        fable_u32 height)
    {
        WriterContext* const context =
            static_cast<WriterContext*>(opaqueContext);
        context->observedUnpublishedState =
            context->videoSystem->decoderTexture80 == 0 &&
            WaitForSingleObject(
                context->videoSystem->frameReadyEvent7C,
                0) == 0x102;
        return FableConvertVideoRgb24Frame(
            textureFormat,
            source,
            sourceRowPitch,
            context->destination,
            context->destinationRowPitch,
            width,
            height);
    }
}

int main()
{
    const fable_u8 source[] = {
        0x10, 0x20, 0x30,
        0x40, 0x50, 0x60
    };
    fable_u8 destination[8];
    CVideoSysRecoveredLayout videoSystem;
    memset(&videoSystem, 0, sizeof(videoSystem));
    memset(destination, 0, sizeof(destination));

    InitializeCriticalSection(videoSystem.textureCriticalSection64);
    videoSystem.frameReadyEvent7C =
        CreateEventA(0, 0, 0, 0);
    if (videoSystem.frameReadyEvent7C == 0)
    {
        DeleteCriticalSection(videoSystem.textureCriticalSection64);
        return 1;
    }

    void* const texture = reinterpret_cast<void*>(0x12345678);
    WriterContext writerContext = {
        &videoSystem,
        destination,
        8,
        false
    };
    const bool published = FablePublishDecodedVideoFrame(
        videoSystem,
        texture,
        &WriteFrame,
        &writerContext,
        0x15,
        source,
        6,
        2,
        1);
    const unsigned long eventResult = WaitForSingleObject(
        videoSystem.frameReadyEvent7C,
        0);
    if (
        !published ||
        !writerContext.observedUnpublishedState ||
        videoSystem.decoderTexture80 != texture ||
        eventResult != 0 ||
        destination[0] != 0x10 ||
        destination[1] != 0x20 ||
        destination[2] != 0x30 ||
        destination[3] != 0xFF ||
        destination[4] != 0x40 ||
        destination[5] != 0x50 ||
        destination[6] != 0x60 ||
        destination[7] != 0xFF)
    {
        CloseHandle(videoSystem.frameReadyEvent7C);
        DeleteCriticalSection(videoSystem.textureCriticalSection64);
        return 2;
    }

    videoSystem.decoderTexture80 = 0;
    if (
        FablePublishDecodedVideoFrame(
            videoSystem,
            texture,
            &WriteFrame,
            &writerContext,
            0x18,
            source,
            6,
            2,
            1) ||
        videoSystem.decoderTexture80 != 0 ||
        WaitForSingleObject(
            videoSystem.frameReadyEvent7C,
            0) != 0x102)
    {
        CloseHandle(videoSystem.frameReadyEvent7C);
        DeleteCriticalSection(videoSystem.textureCriticalSection64);
        return 3;
    }

    CloseHandle(videoSystem.frameReadyEvent7C);
    DeleteCriticalSection(videoSystem.textureCriticalSection64);
    printf("FABLETLC_VIDEO_FRAME_PUBLICATION PASS\n");
    return 0;
}
