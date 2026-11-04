#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CMatrix3x4.h"
#include "fable_engine_internal.h"
#include "fable_shader_render_manager.h"
#include "fable_display_viewport.h"
void CEnginePrimitiveRenderer::ResetWorldTransform()
{
    CMatrix3x4 matrix;
    matrix.E12=matrix.E13=matrix.E21=matrix.E23=matrix.E31=matrix.E32=matrix.E41=matrix.E42=matrix.E43=0.0f;
    matrix.E11=matrix.E22=matrix.E33=1.0f;
    NEngineInternal::GPShaderRenderManager->SetWorldTransform(matrix);
    NEngineInternal::GPRenderManager->SetWorldTransform(matrix);
}
