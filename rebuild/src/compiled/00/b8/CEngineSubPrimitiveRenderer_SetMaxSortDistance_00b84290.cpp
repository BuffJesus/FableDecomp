// CEngineSubPrimitiveRenderer::SetMaxSortDistance @ 0x00b84290
#include "engine/CEngineSubPrimitiveRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
extern float g_sortDistanceNumerator; // [0x12a3a20]


void __fastcall SetMaxSortDistance(CEngineSubPrimitiveRenderer* self, float dist)
{
    self->MaxSortDistance = dist;
    self->SortDistanceMultiplier = g_sortDistanceNumerator / dist;
}