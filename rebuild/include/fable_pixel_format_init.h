#pragma once
#include "rebuild_abi.h"
// Original Ego_r PDB: six bytes; Type and Linear share byte zero.
class CPixelFormatInit
{
public:
 unsigned char Type : 4;
 unsigned char Linear : 1;
 unsigned char ColourDepth;
 unsigned char RBits;
 unsigned char GBits;
 unsigned char BBits;
 unsigned char ABits;
};
FABLE_STATIC_ASSERT(sizeof(CPixelFormatInit)==6);
