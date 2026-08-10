// CEngineSubPrimitiveRenderer::SetMaxSortDistance @ 0x00b84290
extern float g_sortDistanceNumerator; // [0x12a3a20]

struct CEngineSubPrimitiveRenderer
{
    char pad[0x10];
    float maxSortDistance; // +0x10
    float invMaxSortDistance; // +0x14
};

void __fastcall SetMaxSortDistance(CEngineSubPrimitiveRenderer* self, float dist)
{
    self->maxSortDistance = dist;
    self->invMaxSortDistance = g_sortDistanceNumerator / dist;
}