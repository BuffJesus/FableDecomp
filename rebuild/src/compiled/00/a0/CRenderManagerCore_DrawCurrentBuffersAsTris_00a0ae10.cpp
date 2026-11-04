#include "fable_render_state_manager.h"
void CRenderManagerCore::DrawCurrentBuffersAsTris(unsigned long startVertex,unsigned long primitiveCount)
{
    RealiseRenderState();
    PD3DDevice->DrawPrimitive(PT_TRIANGLE_LIST,startVertex,primitiveCount);
}
