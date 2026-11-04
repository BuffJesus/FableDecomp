#include "fable_render_state_manager.h"

void CRenderManagerCore::DrawAsIndexedTris(const void* vertices,long stride,long vertexCount,void* indices,long primitiveCount)
{
    RealiseRenderState();
    PD3DDevice->DrawIndexedPrimitiveUP(PT_TRIANGLE_LIST,0,vertexCount,primitiveCount,indices,D3DFMT_INDEX16,vertices,stride);
}
