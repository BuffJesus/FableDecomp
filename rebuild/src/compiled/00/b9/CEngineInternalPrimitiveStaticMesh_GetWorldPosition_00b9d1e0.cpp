#include "engine/CEngineInternalPrimitiveStaticMesh.h"

bool CEngineInternalPrimitiveStaticMesh::GetWorldPosition(
    C3DVector& worldPosition)
{
    worldPosition = ObjectTransform.GetTranslation();
    return true;
}