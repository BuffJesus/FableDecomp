#include "engine/CEngineSubPrimitiveRenderer.h"
#include <cstdio>

float g_sortDistanceNumerator = 100.0f; // [0x12a3a20]


void __fastcall SetMaxSortDistance(CEngineSubPrimitiveRenderer* self, float dist)
{
    self->MaxSortDistance = dist;
    self->SortDistanceMultiplier = g_sortDistanceNumerator / dist;
}

int main()
{
    CEngineSubPrimitiveRenderer o;
    SetMaxSortDistance(&o, 4.0f);
    if (o.MaxSortDistance == 4.0f && o.SortDistanceMultiplier == 25.0f)
        printf("00b84290_TEST PASS\n");
    else
        printf("FAIL %f %f\n", o.MaxSortDistance, o.SortDistanceMultiplier);
    return 0;
}