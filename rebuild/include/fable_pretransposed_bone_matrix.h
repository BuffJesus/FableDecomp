#pragma once
#include "rebuild_abi.h"
// Ego_r Data is a float[3][4]; retail uses48-byte elements.
class CPreTransposedBoneMatrix {
public:
    CPreTransposedBoneMatrix() {}
    float Data[3][4];
};
FABLE_STATIC_ASSERT(sizeof(CPreTransposedBoneMatrix)==48);
