#pragma once

#include "engine/CEngineInternalPrimitiveBase.h"

// Retail 00B8FDF0 is thiscall. The unused EDX argument adapts that ABI without
// adding a method to the generated POD header. No ownership/allocation occurs.
void __fastcall FablePrimitiveAddToList(
    CEngineInternalPrimitiveBase* primitive, int unusedEdx,
    CEngineInternalPrimitiveBase** firstPrimitive);

void __fastcall FablePrimitiveRemoveFromList(
    CEngineInternalPrimitiveBase* primitive, int unusedEdx);
