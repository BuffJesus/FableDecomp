#pragma once
#include "rebuild_abi.h"
// Ego_r CBankFileDataTypeCount: two unsigned-long fields, size8.
class CBankFileDataTypeCount
{
public:
    unsigned long DataType;
    unsigned long Count;
};
FABLE_STATIC_ASSERT(sizeof(CBankFileDataTypeCount) == 8);
