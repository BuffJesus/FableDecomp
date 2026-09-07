
#include "engine/CMemoryFile.h"  // retyped onto the PDB layout; byte parity re-verified
long __fastcall CMemoryFile_GetPosition(const CMemoryFile* self) {
    return self->Position;
}