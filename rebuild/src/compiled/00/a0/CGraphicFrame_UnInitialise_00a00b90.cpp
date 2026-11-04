#define FABLE_GRAPHIC_FRAME_INLINE_UNINITIALISE
#define FABLE_INLINE_PROVIDER_ANCHOR
#include "fable_graphic_frame.h"
void CGraphicFrame::UnInitialise() {
    CGraphicFrameData* data=FrameData;
    if(data) {
        if(--data->ReferenceCount==0) {
            data->~CGraphicFrameData();
            operator delete(data);
        }
        FrameData=0;
    }
}
void FableGraphicFrameUninitialiseProviderAnchor(CGraphicFrame& frame){frame.UnInitialise();}
