#pragma once
#include <stddef.h>
#include "fable_display_geometry.h"
// Original PDB C3DBoxI24B : C2DBoxI16B, unsigned coordinates.
class C3DBoxI : public C2DBoxI
{
public:
 unsigned int TLZ;
 unsigned int BRZ;
};
FABLE_STATIC_ASSERT(sizeof(C2DBoxI)==16);
FABLE_STATIC_ASSERT(sizeof(C3DBoxI)==24);
FABLE_STATIC_ASSERT(offsetof(C3DBoxI,TLZ)==16);
FABLE_STATIC_ASSERT(offsetof(C3DBoxI,BRZ)==20);
