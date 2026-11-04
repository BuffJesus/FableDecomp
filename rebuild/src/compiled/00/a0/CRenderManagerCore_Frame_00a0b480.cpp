#include "fable_render_state.h"
double FABLE_FASTCALL GFGetTime();

bool CRenderManagerCore::BeginRender()
{
    if(!Rendering) {
        LastRenderEndTime=(float)GFGetTime();
        CurrentVertexFormat=0;PSelectedIndexBuffer=0;
        FableRenderStateDevice* device=reinterpret_cast<FableRenderStateDevice*>(PD3DDevice);
        device->vtable->SetStreamSource(device,0,0,0,0);
        NoLastFrameZSortedPolys=NoZSortedPolys;
        NoLastFrameNonZSortedPolys=NoNonZSortedPolys;
        NoLastFramePolys=NoPolys;
        NoZSortedPolys=0;NoNonZSortedPolys=0;NoPolys=0;
        NoTrisDrawn=0;NoVerticesDrawn=0;NoDrawPrimCalls=0;
        Rendering=true;return true;
    }
    return false;
}

void CRenderManagerCore::EndRender()
{
    if(Rendering==true) {
        double elapsed=GFGetTime()-LastRenderEndTime;
        NoRenderStateChanges=0;
        NoTrisPerSecond=(long)(NoTrisDrawn/elapsed);
        Rendering=false;
    }
}
