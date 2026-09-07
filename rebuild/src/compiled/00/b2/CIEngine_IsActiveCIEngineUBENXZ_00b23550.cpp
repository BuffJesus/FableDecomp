
#include "engine/CIEngine.h"  // retyped onto the PDB layout; byte parity re-verified
bool __fastcall CIEngine_IsActive(const CIEngine* self) {
  return self->Active;
}