#include "engine/CEnginePrimitiveRenderer.h"
#include "fable_engine_primitive_handle.h"
void CEnginePrimitiveRenderer::AddPrimitive(const CEnginePrimitive& primitive)
{
    CEnginePrimitiveHandle handle;
    SetPersistentPrimitive(handle,primitive,4,true);
}
