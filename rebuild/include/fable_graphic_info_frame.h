#pragma once
#include "fable_base_class.h"
#include "fable_iv_counted_pointer.h"
class CGraphicInfo;
class CManagedTexture;
class CBankFileAsyncData;
class CGraphicFrame;
// Original01396C64, Ego_r global signedlong4B; loader casts to file-priority enum.
extern long TextureLoadingPriority;
// Original16B owner; the nested data definition follows the graphic bank header.
class CGraphicInfoFrame : public CBaseClass {
public:
    class CFrameData;
    void BeginLoading(unsigned long,unsigned long) const;
    unsigned long GetWidth() const;
    unsigned long GetHeight() const;
    unsigned short FrameNum;
    signed char SmallMipmapLevel;
    mutable CIVCountedPointer<CFrameData> Data;
    CGraphicInfo* GraphicInfo;
private:
    // Donor private member, mutable references and signed-char mip (retail9FE550).
    void InitialiseFrame(CGraphicFrame&, CManagedTexture&, signed char);
    void ReleaseData() const;
    bool UploadToVolumeTexture(CManagedTexture&,CBankFileAsyncData&,unsigned long) const;
};
