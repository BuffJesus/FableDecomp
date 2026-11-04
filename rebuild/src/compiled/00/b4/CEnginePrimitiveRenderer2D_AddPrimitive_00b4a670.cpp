#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineInternalPrimitiveBase.h"
void CEnginePrimitiveRenderer2D::AddPrimitive(CEngineInternalPrimitiveBase& primitive)
{
    if(primitive.EnginePrimitiveFlags&0x400)
        primitive.AddToList(&First2DDisplacementPrim);
    else
        primitive.AddToList(&First2DPrim);
}
