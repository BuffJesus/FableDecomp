// CCameraCollisionDef::Copy(CDefClassBase const*)
#include "engine/CCameraCollisionDef.h"  // retyped onto the PDB layout; byte parity re-verified
struct CDefClassBase;


// Base-class Copy, called via rel32.
extern void __fastcall CDefClassBase_Copy(void* self, void* edx, const CDefClassBase* src);

void __fastcall CCameraCollisionDef_Copy(CCameraCollisionDef* self, void* edx, const CDefClassBase* src)
{
    CDefClassBase_Copy(self, edx, src);
    self->CameraCollisionMesh = ((const CCameraCollisionDef*)src)->CameraCollisionMesh;
}