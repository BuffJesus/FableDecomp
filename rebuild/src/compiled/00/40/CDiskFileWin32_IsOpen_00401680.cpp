
#include "engine/CDiskFileWin32.h"  // retyped onto the PDB layout; byte parity re-verified
bool __fastcall CDiskFileWin32_IsOpen(const CDiskFileWin32* self)
{
    return self->Opened;
}