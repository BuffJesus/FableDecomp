#pragma once

#include "rebuild_abi.h"
#include "fable_string.h"

struct CTextureRendererRecoveredLayout;

// Retail PC layouts recovered from Fable.exe. These views deliberately use
// opaque pointers/byte ranges so they stay valid with the VC7.1 Tier-0 build
// without importing the DirectShow or Win32 headers.
struct CVideoSysRecoveredLayout
{
    void* graphBuilder00;
    void* mediaControl04;
    void* mediaPosition08;
    void* graphInterface0C;
    void* mediaEvent10;
    void* basicAudio14;
    void* graphInterface18;
    void* graphInterface1C;
    CTextureRendererRecoveredLayout* textureRenderer20;
    fable_u32 loopPolicy24;
    fable_u32 playbackState28;
    fable_u8 flags2C[4];
    fable_u8 movieSubtitles30[0x34];
    fable_u8 textureCriticalSection64[0x18];
    void* frameReadyEvent7C;
    void* decoderTexture80;
    fable_u32 decodedHeight84;
    fable_u32 decodedWidth88;

    void WaitForState(fable_u32 desiredState);
    bool AttemptToPlay();
    bool Pause();
    bool Stop();
    void* GetTexture() const;
};

struct CTextureRendererRecoveredLayout
{
    fable_u8 directShowBase000[0x160];
    CVideoSysRecoveredLayout* videoSystem160;
    void* texture164;
    fable_u32 textureFormat168;
    fable_u32 decodedWidth16C;
    fable_u32 decodedHeight170;
    fable_u32 sourceRowPitch174;
    fable_u32 ownsTexture178;
    fable_u32 unknown17C;
};

// The recovered tail of NUISystem::CMovie. The complete inherited prefix is
// intentionally opaque until the changing-state/UI bases are reconstructed.
struct CMovieRecoveredTail
{
    fable_u8 inherited000[0x15C];
    bool isPlaying15C;
    fable_u8 padding15D[3];
    fable_u8 textureOwner160[0x14];
    CWideString movieName174;
    fable_u8 playbackInfo178[0x1C];
    fable_u8 attemptToPlay194;
    fable_u8 alignment195[3];

    void SetMovie(CWideString& movie);
    bool IsPlaying() const;
};

enum CVideoSysRecoveredPlaybackState
{
    CVideoSysPlaybackPlaying = 1,
    CVideoSysPlaybackPaused = 2,
    CVideoSysPlaybackStopped = 3
};

enum FableVideoSystemRetailAddress
{
    FableRetailCMovieChangeState = 0x00547C90,
    FableRetailCMovieUpdate = 0x00547E40,
    FableRetailCMovieDraw = 0x00547F60,
    FableRetailCMovieSetMovie = 0x00548510,
    FableRetailCMovieIsPlaying = 0x00548520,
    FableRetailCVideoSysWaitForState = 0x00A3B0F0,
    FableRetailCVideoSysAttemptToPlay = 0x00A3B1A0,
    FableRetailCVideoSysPause = 0x00A3B1C0,
    FableRetailCVideoSysStop = 0x00A3B1F0,
    FableRetailCVideoSysGetTexture = 0x00A3B320,
    FableRetailCVideoSysProcess = 0x00A3B430,
    FableRetailCTextureRendererDoRenderSample = 0x00A3B730,
    FableRetailCVideoSysInit = 0x00A3B9D0,
    FableRetailCVideoSysDestructor = 0x00A3BC20,
    FableRetailCVideoSysConstructor = 0x00A3BC70
};

// Recovered inner conversion from CTextureRenderer::DoRenderSample
// @ 0x00A3B730. Source pixels are DirectShow RGB24 memory order (B, G, R).
bool FABLE_FASTCALL FableConvertVideoRgb24Frame(
    fable_u32 textureFormat,
    const fable_u8* source,
    fable_u32 sourceRowPitch,
    fable_u8* destination,
    fable_u32 destinationRowPitch,
    fable_u32 width,
    fable_u32 height);

typedef bool (FABLE_FASTCALL *FableDecodedVideoFrameWriter)(
    void* context,
    fable_u32 textureFormat,
    const fable_u8* source,
    fable_u32 sourceRowPitch,
    fable_u32 width,
    fable_u32 height);

// Reconstructed publication shell around the recovered conversion loop.
// The writer performs lock -> convert -> unlock while the CVideoSys critical
// section is held; only after it returns does this publish the texture and
// signal the frame-ready event, matching DoRenderSample's observed order.
bool FABLE_FASTCALL FablePublishDecodedVideoFrame(
    CVideoSysRecoveredLayout& videoSystem,
    void* decodedTexture,
    FableDecodedVideoFrameWriter writer,
    void* writerContext,
    fable_u32 textureFormat,
    const fable_u8* source,
    fable_u32 sourceRowPitch,
    fable_u32 width,
    fable_u32 height);

FABLE_STATIC_ASSERT(sizeof(CVideoSysRecoveredLayout) == 0x8C);
FABLE_STATIC_ASSERT(sizeof(CTextureRendererRecoveredLayout) == 0x180);
FABLE_STATIC_ASSERT(sizeof(CMovieRecoveredTail) == 0x198);
