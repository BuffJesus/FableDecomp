#include "fable_render_state_manager.h"
void CRenderManagerCore::DrawCurrentBuffersAsIndexedTris(unsigned long minVertex,unsigned long vertexCount,unsigned long startIndex,unsigned long primitiveCount)
{
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitive(PT_TRIANGLE_LIST,0,minVertex,vertexCount,startIndex,primitiveCount);
}
