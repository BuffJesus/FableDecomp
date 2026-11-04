#include <stdlib.h>
#include "fable_graphic_frame.h"
extern "C" void UnusedBoundary(){abort();}
#pragma comment(linker,"/alternatename:?InitialiseAsSurfaceSection@CGraphicFrameData@@QAE_NABV?$CCountedPointer@VCTexture@@@@MMJJ_N1@Z=_UnusedBoundary")
// This program is a link-omission check only. Never execute the ABI-less call.
int main(){
    CGraphicFrame* frame=0;
    CCountedPointer<CTexture>* texture=0;
    return frame->InitialiseAsSurfaceSection(*texture,0,0,1,1,false,false)?0:1;
}
