// Retail 0x00B91340. Replace the reference argument with its field at +0x0C,
// then tail-forward to the manager overload at 0x00B91140.
struct CEngineSubPrimitiveBase {
    char m_leading[0x0C];
    void* m_pBoundingVolume;
};

struct CEnginePrimitiveManagerOcclusionBase {
    void RenderBoundingVolume(void* pBoundingVolume);
    void RenderBoundingVolume(CEngineSubPrimitiveBase& primitive);
};

void CEnginePrimitiveManagerOcclusionBase::RenderBoundingVolume(
    CEngineSubPrimitiveBase& primitive) {
    RenderBoundingVolume(primitive.m_pBoundingVolume);
}