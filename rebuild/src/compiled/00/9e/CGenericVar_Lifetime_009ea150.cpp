// lib_generic_var, retail 0x009EA150-0x009EA360: CGenericVar construction and
// destruction. The default constructor sets only the type (the value fields
// stay uninitialised); the copy is member by member.
#define FABLE_GENERIC_VAR_OUT_OF_LINE
#include "fable_generic_var.h"

// Retail 009EA150.
CGenericVar::CGenericVar() { Type = GENERIC_VAR_NULL; }

// Retail 009EA360.
CGenericVar::CGenericVar(const CGenericVar& other)
    : Type(other.Type), ValString(other.ValString), ValSLONG(other.ValSLONG),
      ValULONG(other.ValULONG), ValBool(other.ValBool), ValFloat(other.ValFloat)
{
}

// Retail 009EA170 (scalar deleting destructor 009EA230).
CGenericVar::~CGenericVar()
{
}
