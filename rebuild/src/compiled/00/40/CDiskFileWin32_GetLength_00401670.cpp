
#include "engine/CDiskFileWin32.h"  // retyped onto the PDB layout; byte parity re-verified
long __fastcall CDiskFileWin32_GetLength(const CDiskFileWin32* self) {
    return self->Length;
}