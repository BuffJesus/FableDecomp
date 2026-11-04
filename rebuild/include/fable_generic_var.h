#pragma once

// CGenericVar (Ego_r.pdb, 0x1C: vfptr, Type, ValString, ValSLONG, ValULONG,
// ValBool, ValFloat) and the parameter vector the console command bindings
// receive. fable_console.h only declares std::vector; retail's container is a
// 12-byte STLport vector (first/last/end of storage), modelled here as the one
// specialization the console needs. The command bindings index it directly:
// CConsole::SetConsoleTextColour 0x009EAA50 reads params[0..2].ValULONG at
// +0x10/+0x2C/+0x48 of the first element.

#include "fable_console.h"

class CGenericVar
{
public:
    // FableWin ??0CGenericVar@@QAE@XZ / ??0CGenericVar@@QAE@ABV0@@Z, retail
    // 0x009EA150 / 0x009EA360; the destructor is 0x009EA170 (scalar deleting
    // 0x009EA230).
    CGenericVar();
    CGenericVar(const CGenericVar& other);
    virtual ~CGenericVar();

    // Ego_r/FableWin virtual ?Init@CGenericVar@@UAEX...@Z overloads; retail vtable 0x0129C3E0
    // (slot 0 scalar deleting destructor 0x009EA230, then Init(CCharString) 0x009EA210,
    // Init(bool) 0x009EA1F0, Init(ULONG) 0x009EA1D0, Init(SLONG) 0x009EA1B0, Init(float) 0x009EA190).
    virtual void Init(const float& value);
    virtual void Init(const long& value);
    virtual void Init(const unsigned long& value);
    virtual void Init(const bool& value);
    virtual void Init(const CCharString& value);

    // Ego_r inline accessors.
    EGenericVarType GetType() const { return Type; }
    // Ego_r CGenericVar::GetString returns a copy (the list commands copy it).
    CCharString GetString() const { return ValString; }
    long GetSLONG() const { return ValSLONG; }
    unsigned long GetULONG() const { return ValULONG; }
    bool GetBool() const { return ValBool; }
    float GetFloat() const { return ValFloat; }

    EGenericVarType Type;                                          // +0x04
    CCharString ValString;                                         // +0x08
    long ValSLONG;                                                 // +0x0C
    unsigned long ValULONG;                                        // +0x10
    bool ValBool;                                                  // +0x14
    float ValFloat;                                                // +0x18
};
FABLE_STATIC_ASSERT(sizeof(CGenericVar) == 0x1C);

// Inline in lib_generic_var.hpp; callers such as
// CConsoleCommandParameters::GetCorrectlyTypedParameter 0x009EA6C0 expand
// them. The out-of-line copies are CGenericVar_Lifetime_009ea150.cpp.
#ifndef FABLE_GENERIC_VAR_OUT_OF_LINE
inline CGenericVar::CGenericVar() { Type = GENERIC_VAR_NULL; }
inline CGenericVar::CGenericVar(const CGenericVar& other)
    : Type(other.Type), ValString(other.ValString), ValSLONG(other.ValSLONG),
      ValULONG(other.ValULONG), ValBool(other.ValBool), ValFloat(other.ValFloat)
{
}
inline CGenericVar::~CGenericVar() {}
#endif
FABLE_STATIC_ASSERT(offsetof(CGenericVar, ValULONG) == 0x10);

namespace std
{
    template<>
    class vector<CGenericVar, allocator<CGenericVar> >
    {
    public:
        typedef unsigned int size_type;
        size_type size() const { return static_cast<size_type>(_M_finish - _M_start); }
        bool empty() const { return _M_start == _M_finish; }
        const CGenericVar& operator[](size_type index) const { return _M_start[index]; }
        const CGenericVar* begin() const { return _M_start; }
        const CGenericVar* end() const { return _M_finish; }

        CGenericVar* _M_start;
        CGenericVar* _M_finish;
        CGenericVar* _M_end_of_storage;
    };
}
FABLE_STATIC_ASSERT(sizeof(CGenericVarVector) == 0x0C);
