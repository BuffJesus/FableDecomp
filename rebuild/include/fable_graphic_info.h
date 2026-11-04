#pragma once
#include "fable_base_class.h"
#include "fable_iv_counted_pointer.h"
#include "engine/CManagedTextureOwner.h"
#include "fable_managed_texture.h"
#include "fable_counted_pointer.h"
#include "engine/CGraphicFrame.h"
#include "fable_graphic_info_frame.h"
class CBankFileAsyncData;
#include "engine/CGraphicDataBank.h"
class CGraphicDataBank;
class CGraphicInfoFrame;
// Ego_r CGraphicInfo is44B; its first28B are the bank's graphic header.
class CGraphicInfo {
public:
    void UnloadFrame(unsigned long);
    CGraphicDataBank::CGraphicHeader Header;
    CGraphicDataBank::ETextureType TextureType;
    CGraphicDataBank* Bank;
    unsigned long Index;
    CGraphicInfoFrame* Frames;
};
class CGraphicInfoFrame::CFrameData : public CManagedTextureOwner {
    public:
        CFrameData(const CGraphicInfoFrame*);
        virtual ~CFrameData();
        virtual bool IsResourceLoaded() const;
        virtual void OnResourceLoaded();
        virtual void ReleaseTexture();
        virtual unsigned long GetUnloadableMemory() const;
        virtual unsigned long GetDebugMemoryInUse() const;
        virtual void ResetDebugMemoryInUse();
        void BeginLoading(unsigned long,unsigned long);
        bool UploadFrame(bool);
        bool StopLoading();
        bool FreeTextureMemory(bool);
        unsigned long Sizeof() const;
        void ResetTexture();
        CGraphicFrame Frame;
        CManagedTexture ManagedTexture;
        CManagedTexture LoadingManagedTexture;
        CCountedPointer<CBankFileAsyncData> FileData;
        signed char CurrentMipmapLevel,LoadingMipmapLevel,TouchedMipmapLevel;
        unsigned long TouchedFrame;
        const CGraphicInfoFrame* GraphicInfoFrame;
};
inline unsigned long CGraphicInfoFrame::GetWidth() const {return GraphicInfo->Header.FrameWidth;}
inline unsigned long CGraphicInfoFrame::GetHeight() const {return GraphicInfo->Header.FrameHeight;}
FABLE_STATIC_ASSERT(sizeof(CGraphicInfo)==44);
FABLE_STATIC_ASSERT(offsetof(CGraphicInfo,Frames)==40);
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame)==16);
FABLE_STATIC_ASSERT(sizeof(CGraphicInfoFrame::CFrameData)==68);
FABLE_STATIC_ASSERT(offsetof(CGraphicInfoFrame,GraphicInfo)==12);
__forceinline const CGraphicInfo* CGraphicDataBank::PeekGraphic(unsigned long index) const {
    return index<GraphicList.size()?&GraphicList[index]:0;
}
__forceinline const CGraphicInfoFrame* CGraphicDataBank::PeekGraphicInfoFrame(unsigned long index,unsigned long frame) const {
    if(PeekGraphic(index))return &PeekGraphic(index)->Frames[frame];
    return 0;
}
