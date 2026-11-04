#include "engine/CEnginePrimitiveRenderer.h"
#include "fable_engine_primitive_manager.h"
void CEnginePrimitiveRenderer::RenderNonsortedPrimitives()
{
    for(unsigned i=0;i<PrimitiveManagers.size();++i)
        PrimitiveManagers[i].Data->RenderNonsortedPrimitives();
}
