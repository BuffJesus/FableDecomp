#include "engine/CEngineSubPrimitiveRenderer.h"
unsigned long CEngineSubPrimitiveRenderer::GetCount(ESubPrimitiveType type) const
{
    return SubPrimitiveLists[type].Elements.size();
}
