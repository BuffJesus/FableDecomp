#include "engine/CEngineSubPrimitiveRenderer.h"
void CEngineSubPrimitiveRenderer::ClearType(ESubPrimitiveType type)
{
    DeleteList(SubPrimitiveLists[type].Elements);
}
