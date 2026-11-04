// lib_generic_var, retail 0x009EA190-0x009EA22A: the five virtual CGenericVar::Init
// overloads (Ego_r / FableWin ?Init@CGenericVar@@UAEX...@Z). Each stores the value and
// then tags the variant type. Only reached through the vtable 0x0129C3E0 (no direct
// callers). Needs the virtual Init declarations in fable_generic_var.h.
#include "fable_generic_var.h"

// Retail 009EA190 (vtable slot 0x0129C3F8).
void CGenericVar::Init(const float& value)
{
    ValFloat = value;
    Type = GENERIC_VAR_FLOAT;
}

// Retail 009EA1B0 (vtable slot 0x0129C3F4).
void CGenericVar::Init(const long& value)
{
    ValSLONG = value;
    Type = GENERIC_VAR_SLONG;
}

// Retail 009EA1D0 (vtable slot 0x0129C3F0).
void CGenericVar::Init(const unsigned long& value)
{
    ValULONG = value;
    Type = GENERIC_VAR_ULONG;
}

// Retail 009EA1F0 (vtable slot 0x0129C3EC).
void CGenericVar::Init(const bool& value)
{
    ValBool = value;
    Type = GENERIC_VAR_BOOL;
}

// Retail 009EA210 (vtable slot 0x0129C3E8).
void CGenericVar::Init(const CCharString& value)
{
    ValString = value;
    Type = GENERIC_VAR_STRING;
}
