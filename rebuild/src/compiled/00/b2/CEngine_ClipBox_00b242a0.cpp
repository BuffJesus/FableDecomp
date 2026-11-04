#include "fable_engine.h"
#include "fable_engine_internal.h"
#include "engine/CEngineCamera.h"
#include "engine/CMatrix3x4.h"
bool CEngine::ClipBox(const C3DBoundingBox& box) const
{
    CMatrix3x4 matrix;
    box.CalculateMatrix(matrix);
    return NEngineInternal::GPCamera->ClipBox(matrix);
}
