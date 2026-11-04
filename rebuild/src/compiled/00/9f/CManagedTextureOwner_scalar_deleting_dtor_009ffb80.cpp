// ??_GCManagedTextureOwner@@UAEPAXI@Z @ 0x009FFB80: slot 0 of the
// CManagedTextureOwner vtable 0x0129C848 (RTTI COL 0x01356808). The class has
// an implicit destructor, so the scalar deleting destructor calls the
// standalone ~CResource 0x009FC550 directly. Retail emits this vtable with the
// CGraphicInfoFrame::CFrameData constructor (GraphicDataBank.cpp), whose base
// subobject constructor is CManagedTextureOwner's inline one; that unit calls
// the out-of-line ~CResource body.
#define FABLE_RESOURCE_DESTRUCTOR_DECLARATION_ONLY
#include "fable_graphic_info.h"
CGraphicInfoFrame::CFrameData::CFrameData(const CGraphicInfoFrame* frame)
    : GraphicInfoFrame(frame) {
    CurrentMipmapLevel=-1;
    TouchedMipmapLevel=127;
    TouchedFrame=frame->GraphicInfo->Bank->ResourceList.CurrentFrame-1;
    unsigned long size=sizeof(*this)+Frame.Sizeof();
    if(!LoadingManagedTexture.Data.IsNull())size+=LoadingManagedTexture.Sizeof();
    SetResourceSize(size);
    frame->GraphicInfo->Bank->AddResource(*this);
}
