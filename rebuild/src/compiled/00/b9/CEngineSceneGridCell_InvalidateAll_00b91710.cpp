// Retail 0x00B91710. Invalidate the three PDB-named cached values on this
// grid cell and each parent, stopping once an already-invalid ancestor is reached.
enum { CACHE_VALID_MASK = 0x07 };
struct CEngineSceneGridCell {
    CEngineSceneGridCell* Parent;
    char m_between04And44[0x40];
    unsigned char m_validFlags; // bits: BoundingBoxValid, TotalCountValid, RenderMaskValid
    void InvalidateAll();
};
void CEngineSceneGridCell::InvalidateAll() {
    CEngineSceneGridCell* cell = this;
    do {
        unsigned char flags = cell->m_validFlags;
        if ((flags & CACHE_VALID_MASK) == 0) return;
        cell->m_validFlags = flags & 0xF8;
        cell = cell->Parent;
    } while (cell != 0);
}