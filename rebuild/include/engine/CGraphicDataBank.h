#pragma once
#include "fable_game_text_bank.h"
#include "fable_carray.h"
#include "fable_surface_types.h"
#include "fable_graphic_info_frame.h"
// Donor base identities; retail bank operations address the first base at zero.
// Only the inherited bank/resource prefix is modelled, not allocation size.
class CTextureManager;
class CGraphicInfo;
class CGraphicInfoFrame;
class CGraphicFrame;
class CManagedTexture;
class CBankFileAsyncData;
class CGraphicDataBank : public CBankFileAsync, public CResourceBank {
public:
    // Ego_r.pdb original enum values; shared with CGraphicInfo::TextureType.
    enum ETextureType {
        TEXTURE_TYPE_OPAQUE=0, TEXTURE_TYPE_ALPHA=1,
        TEXTURE_TYPE_BOOLEAN_ALPHA=2, TEXTURE_TYPE_INTERPOLATED_ALPHA=3,
        TEXTURE_TYPE_BUMP=4, TEXTURE_TYPE_BUMP_ALPHA=5,
        TEXTURE_TYPE_UNCOMPRESSED_OPAQUE=6, TEXTURE_TYPE_UNCOMPRESSED_ALPHA=7,
        TEXTURE_TYPE_UNCOMPRESSED_BUMP=8, TEXTURE_TYPE_UNCOMPRESSED_BUMP_ALPHA=9,
        TEXTURE_TYPE_SIGNED=10, TEXTURE_TYPE_COUNT=11
    };
    // Original28B header from Ego_r.pdb; no independent header lifetime is inferred.
    class CGraphicHeader {public:
        unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;
        CPixelFormat PixelFormat;
        unsigned char TransparencyType,MipmapLevels;
        bool IsFlatAnimatedGraphic : 1;
        unsigned long FrameDataSize;
        // DIA SymTagArrayType/count1, and native loader reads Header+24 directly.
        unsigned long CompressedMipmapSizes[1];
    };
    void ChangeScene();
    void SetReduceSizeFrameDelay(unsigned long);
    const CGraphicInfo* PeekGraphic(unsigned long index) const;
    const CGraphicInfoFrame* PeekGraphicInfoFrame(unsigned long index,unsigned long frame) const;
    CArray<CGraphicInfo> GraphicList; // donor +254; retail +1E0
    unsigned char UnrecoveredGraphicState[0x268-0x1ec];
    unsigned long UnloadFrameDelay;
    unsigned long ReduceSizeFrameDelay;
    unsigned long TexturePointersValidForFrames;
    long MinAvailablePreloadMemory;
    long AvailableMemory;
    bool FreezeGraphics;
    unsigned char Padding27d[3];
    CCountedPointer<CTextureManager> TextureManager;
    // Donor array count11; retail BeginLoading9FDE80 indexes +288 by type*4.
    // The subsequent blank-texture array/complete bank size remain unmodelled.
    CPixelFormat PixelFormats[TEXTURE_TYPE_COUNT];
protected:
    virtual unsigned long GetDataFormatVersion();
private:
    friend class CGraphicInfoFrame;
    friend class CGraphicInfoFrame::CFrameData;
    bool LoadVolumeGraphic(CManagedTexture&,CBankFileAsyncData&,const CGraphicHeader&,bool,unsigned long,bool);
    bool LoadGraphicFrame(CGraphicFrame&,CManagedTexture&,CBankFileAsyncData&,CGraphicInfo*,bool,unsigned long,ETextureType,bool);
};
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBank,UnloadFrameDelay)==0x268);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBank,GraphicList)==0x1e0);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBank,ReduceSizeFrameDelay)==0x26c);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBank,TextureManager)==0x280);
FABLE_STATIC_ASSERT(offsetof(CGraphicDataBank,PixelFormats)==0x288);
#include "fable_graphic_info.h"
