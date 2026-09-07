
#include "engine/CMemoryFile.h"  // retyped onto the PDB layout; byte parity re-verified
long __fastcall CMemoryFile_GetLength(const CMemoryFile* self)
{
    return self->Length;
}