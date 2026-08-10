#include <cstdio>

float g_sortDistanceNumerator = 100.0f; // [0x12a3a20]

struct CEngineSubPrimitiveRenderer
{
    char pad[0x10];
    float maxSortDistance;
    float invMaxSortDistance;
};

void __fastcall SetMaxSortDistance(CEngineSubPrimitiveRenderer* self, float dist)
{
    self->maxSortDistance = dist;
    self->invMaxSortDistance = g_sortDistanceNumerator / dist;
}

int main()
{
    CEngineSubPrimitiveRenderer o;
    SetMaxSortDistance(&o, 4.0f);
    if (o.maxSortDistance == 4.0f && o.invMaxSortDistance == 25.0f)
        printf("00b84290_TEST PASS\n");
    else
        printf("FAIL %f %f\n", o.maxSortDistance, o.invMaxSortDistance);
    return 0;
}