#include <stdio.h>
static void* g_seen = 0;
struct CEngineSubPrimitiveBase {
    char m_leading[0x0C];
    void* m_pBoundingVolume;
};
struct CEnginePrimitiveManagerOcclusionBase {
    void RenderBoundingVolume(void* pBoundingVolume);
    void RenderBoundingVolume(CEngineSubPrimitiveBase& primitive);
};
void CEnginePrimitiveManagerOcclusionBase::RenderBoundingVolume(void* value) { g_seen = value; }
void CEnginePrimitiveManagerOcclusionBase::RenderBoundingVolume(
    CEngineSubPrimitiveBase& primitive) { RenderBoundingVolume(primitive.m_pBoundingVolume); }
int main() {
    CEnginePrimitiveManagerOcclusionBase manager;
    CEngineSubPrimitiveBase primitive;
    primitive.m_pBoundingVolume = (void*)0x12345678;
    manager.RenderBoundingVolume(primitive);
    if (g_seen != primitive.m_pBoundingVolume) return 1;
    printf("RENDER_BOUNDING_VOLUME PASS\n");
    return 0;
}