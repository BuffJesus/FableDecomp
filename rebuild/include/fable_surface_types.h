#pragma once
#include "fable_d3d9.h"
#include "fable_d3d_format_info.h"
#include "fable_surface_multisample.h"
#include "fable_extents_i.h"
#include "fable_display_geometry.h"
class CRGBColour;
class CPixelFormatInit;
class CPixelFormat
{
public:
    CPixelFormat() : FormatIndex(-1) {}
    explicit CPixelFormat(D3DFORMAT format) { Initialise(format); }
    // Inline in retail (CDataInputStream::ReadPixelFormat 0x00994670 calls
    // Initialise 0x009E3640 on its local).
    explicit CPixelFormat(const CPixelFormatInit& init) { Initialise(init); }

    void Initialise(D3DFORMAT format);
    void Initialise(const CPixelFormatInit& init);
    // Ego_r ?GetInit@CPixelFormat@@QBE?AVCPixelFormatInit@@XZ, retail 0x009E3870.
    CPixelFormatInit GetInit() const;
    void Initialise(long r,long g,long b,long a,bool isSigned);
    bool IsSigned() const;
    bool IsEquivalent(const CPixelFormat&) const;
    bool IsCompressed() const;
    EPixelFormatType GetType() const;
    unsigned long GetRPosShift() const;
    unsigned long GetGPosShift() const;
    unsigned long GetBPosShift() const;
    unsigned long GetNoRBits() const;
    unsigned long GetNoGBits() const;
    unsigned long GetNoBBits() const;
    unsigned long GetRMask() const;
    unsigned long GetGMask() const;
    unsigned long GetBMask() const;
    // Alpha channel: bit count, position (above R, G and B) and mask. Ego_r names ?GetAMask@CPixelFormat@@QBEKXZ;
    // all three are inline in retail (InitializeStreamingFontTextureCache 0x009A3640 reads GD3DFormats directly).
    unsigned long GetNoABits() const { return GD3DFormats[FormatIndex].AlphaBits; }
    unsigned long GetAPosShift() const { return GD3DFormats[FormatIndex].BBits + GD3DFormats[FormatIndex].GBits + GD3DFormats[FormatIndex].RBits; }
    unsigned long GetAMask() const { return ((1 << GetNoABits()) - 1) << GetAPosShift(); }
    unsigned long GetRScaleShift() const;
    unsigned long GetGScaleShift() const;
    unsigned long GetBScaleShift() const;
    unsigned long GetAScaleShift() const;
    // ?GetColourDepth@CPixelFormat@@QBEKXZ (retail 0x009E3820).
    unsigned long GetColourDepth() const;
    unsigned long GetByteColourDepth() const;
    void ConvertRGBToPFDData(const CRGBColour&, unsigned char*) const;
    // ?FindFormatInfoIndexFromD3DFormat@CPixelFormat@@SIJW4_D3DFORMAT@@@Z
    // (retail 0x009E3620): index of `format` in GD3DFormats, or -1.
    static long FABLE_FASTCALL FindFormatInfoIndexFromD3DFormat(D3DFORMAT format);

    long FormatIndex;
};

class CSurfaceLockedRect
{
public:
    C2DExtentsI Dimensions;
    long Pitch;
    unsigned char* PMem;
};

class CSurface
{
public:
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    CSurface(const CSurface& source);
    virtual ~CSurface() { Uninitialise(); }

    void Uninitialise();
    void InitialiseFromTextureMipmap(IDirect3DSurface9* surface);
    void InitialiseFromPreallocatedSurface(IDirect3DSurface9* surface, bool addReference);
    CSurface& operator=(const CSurface& source);
    bool InitialiseAsDepthBuffer(const C2DExtentsI&, const CPixelFormat&, ESurfaceMultisampleType, bool);
    // Ego_r ?InitialiseAsRenderTarget@CSurface@@QAE_NABVC2DExtentsI@@ABVCPixelFormat@@W4ESurfaceMultisampleType@@_N3@Z,
    // retail 0x009F3310.
    bool InitialiseAsRenderTarget(const C2DExtentsI&, const CPixelFormat&, ESurfaceMultisampleType, bool lockable, bool unused);
    bool InitialiseAsImage(
        const C2DExtentsI& dimensions,
        const CPixelFormat& format,
        bool dynamic);

    // Ego_r ?GetRefCount@CSurface@@QBEJXZ, retail 0x009F2DA0: AddRef then return Release's count.
    long GetRefCount() const;
    C2DExtentsI GetDimensions() const;
    // Ego_r GetD3DSurface; inline in retail (CDisplayManager::SetRenderTarget 0x009BF160).
    IDirect3DSurface9* GetD3DSurface() const { return PD3DSurface; }
    CPixelFormat GetPixelFormat() const;
    unsigned long GetByteLength() const;
    // Ego_r ?GetPitch@CSurface@@QBEKXZ, retail 0x009F3390.
    unsigned long GetPitch() const;
    // Ego_r ?CopyFromMemoryAutoSwizzle@CSurface@@QAEXPAXJ@Z / ?CopyToMemoryAutoUnswizzle@CSurface@@QAEXPAXJ@Z,
    // retail 0x009F3470 / 0x009F3520.
    void CopyFromMemoryAutoSwizzle(void* source, long source_pitch);
    void CopyToMemoryAutoUnswizzle(void* dest, long dest_pitch);
    // Ego_r ?Swizzle@CSurface@@QAEXK@Z / ?Unswizzle@CSurface@@QAEXK@Z (retail 0x009F4330 /
    // 0x009F4360) and the CSurfaceLockedRect& forms (0x009F2E40 / 0x009F2E50, empty on PC).
    void Swizzle(unsigned long flags);
    void Unswizzle(unsigned long flags);
    void Swizzle(CSurfaceLockedRect& lock);
    void Unswizzle(CSurfaceLockedRect& lock);

    // ?GetWidth@CSurface@@QBEKXZ / ?GetHeight@CSurface@@QBEKXZ (FableWin
    // bodies: GetDimensions().X / .Y). Retail's MainGameComponent TU inlines
    // GetWidth but calls GetHeight out of line (COMDAT 0x00415BF0).
    unsigned long GetWidth() const { return GetDimensions().X; }
    unsigned long GetHeight() const;

    CSurfaceLockedRect Lock(unsigned long flags);
    void Unlock() const;
    void SetPixel(long x,long y,const CRGBColour&,const CSurfaceLockedRect&);

    void CopyFromMemory(const C2DBoxI*, const void*, const C2DBoxI*, const CPixelFormat&, long, unsigned long);
    void CopyFromMemory(const C2DBoxI*, const void*, const C2DBoxI*, long);

    void CopyFromSurface(
        const C2DBoxI* destinationBox,
        const CSurface& source,
        const C2DBoxI* sourceBox,
        unsigned long filter);

private:
    bool CanDoProperSignedConversion(
        const C2DBoxI* destinationBox,
        const C2DBoxI* sourceBox,
        const CPixelFormat& format);
    void CopyFromUnsignedDataToSignedSurface(
        const void* data,
        const CPixelFormat& format,
        long pitch);

public:
    IDirect3DSurface9* PD3DSurface;
    unsigned long AllocationSource;
    void* PAllocatedMemory;
};

enum ESurfacePool { SURFACE_POOL_DEFAULT_LOCATION=0, SURFACE_POOL_MANAGED=1, SURFACE_POOL_SYSTEM_MEM=2 };

class CTexture
{
public:
    CTexture() : PD3DTexture(0), ByteLength(0), AllocationSource(0) {}
    CTexture(const C2DExtentsI& dimensions,long levels,const CPixelFormat& format,unsigned long flags,ESurfacePool pool)
        : PD3DTexture(0), AllocationSource(0) { Initialise(dimensions,levels,format,flags,pool,false,false); }
    explicit CTexture(IDirect3DTexture9* texture) : PD3DTexture(0), AllocationSource(0)
    { InitialiseFromPreallocatedTexture(texture); }
    CTexture(const CTexture& source); // retail 009F9D00
    const CTexture& operator=(const CTexture& source); // retail 009FA1C0
    ~CTexture() { Uninitialise(); }
    void Uninitialise();
    // Ego_r ?Swizzle@CTexture@@QAEXJJ@Z / ?Unswizzle@CTexture@@QAEXJJ@Z and the
    // (long, CSurfaceLockedRect&) forms, retail 0x009FA0C0 / 0x009FA100 / 0x009FA140 / 0x009FA180.
    void Swizzle(long level, long flags);
    void Unswizzle(long level, long flags);
    void Swizzle(long level, CSurfaceLockedRect& lock);
    void Unswizzle(long level, CSurfaceLockedRect& lock);
    // Ego_r ?SwizzleAll@CTexture@@QAEXJ@Z / ?UnswizzleAll@CTexture@@QAEXJ@Z, retail 0x009F9FA0 / 0x009FA030.
    void SwizzleAll(long flags);
    void UnswizzleAll(long flags);
    bool IsBusy() const;
    // Ego_r/FableWin ?GetRefCount@CTexture@@QAEJXZ, retail 0x009F9D60 (AddRef, then Release's count).
    long GetRefCount();
    // Ego_r/FableWin ?GenerateMipmaps@CTexture@@QAEXKK@Z, retail 0x009F9E60 (D3DXFilterTexture).
    void GenerateMipmaps(unsigned long filter, unsigned long sourceLevel);
    // Ego_r ?Lock@CTexture@@QAE?AVCSurfaceLockedRect@@KK@Z (retail 0x009FA450) and the level unlock (retail
    // 0x009F9DE0; GFUpdateFontTextureCache 0x009A23E0 locks and unlocks level 0).
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);
    void Unlock(unsigned long level);
    bool InitialiseFromPreallocatedTexture(IDirect3DTexture9* texture);
    bool Initialise(const C2DExtentsI& dimensions, long levels, const CPixelFormat& format, unsigned long flags, ESurfacePool pool, bool dynamic, bool automaticMipmaps);
    static C2DExtentsI FABLE_FASTCALL GetNextValidDimensions(const C2DExtentsI& dimensions,const CPixelFormat& format);
    // Some retail callers retain the original out-of-line 9F9DF0 copy.
#ifdef FABLE_TEXTURE_OUT_OF_LINE_GET_NO_LEVELS
    long GetNoLevels() const;
#else
    long GetNoLevels() const { return PD3DTexture->GetLevelCount(); }
#endif
    CPixelFormat GetPixelFormat() const;
    long GetLevelByteLength(long level) const;
    C2DExtentsI GetDimensions(unsigned long level=0) const; // retail 009F9DB0
    unsigned long GetWidth(unsigned long level=0) const {return GetDimensions(level).X;}
    unsigned long GetHeight(unsigned long level=0) const {return GetDimensions(level).Y;}
    CSurface GetSurfaceLevel(long level) const;

    void CopyFromTexture(const CTexture& source, unsigned long filter);

    IDirect3DTexture9* PD3DTexture;
    union
    {
        struct { unsigned long ByteLength : 28; unsigned long AllocationSource : 4; };
        // Whole-word reads/stores are observable across CalcByteLength callbacks.
        unsigned long LengthAndAllocation;
    };
private:
    void CalcByteLength();
};

FABLE_STATIC_ASSERT(sizeof(CSurface)==16);
FABLE_STATIC_ASSERT(sizeof(CTexture)==8);

// Retail 0x009F9E00 (out of line, CTexture_GetSurfaceLevel_009f9e00.cpp);
// SwizzleAll / UnswizzleAll 0x009F9FA0 / 0x009FA030 expand it, so their TU
// defines FABLE_TEXTURE_GET_SURFACE_LEVEL_INLINE for an inline copy.
#ifdef FABLE_TEXTURE_GET_SURFACE_LEVEL_INLINE
inline CSurface CTexture::GetSurfaceLevel(long level) const
{
    IDirect3DSurface9* d3dSurface;
    PD3DTexture->GetSurfaceLevel(level, &d3dSurface);

    CSurface surface;
    surface.InitialiseFromTextureMipmap(d3dSurface);
    return surface;
}
#endif
