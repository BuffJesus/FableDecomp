#include "fable_render_state_manager.h"
void CRenderManagerCore::SetVertexFormat(unsigned long vertexFormat)
{
    PD3DDevice->SetFVF(vertexFormat);
}
