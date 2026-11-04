// Retail 0x00B91710. Invalidate the three PDB-named cached values on this
// grid cell and each parent, stopping once an already-invalid ancestor is reached.
#include "engine/CEngineSceneGridCell.h"
enum { CACHE_VALID_MASK = 0x07 };
void CEngineSceneGridCell::InvalidateAll() {
    CEngineSceneGridCell* cell = this;
    do {
        unsigned char flags = cell->Flags;
        if ((flags & CACHE_VALID_MASK) == 0) return;
        cell->Flags = flags & 0xF8;
        cell = cell->Parent;
    } while (cell != 0);
}
