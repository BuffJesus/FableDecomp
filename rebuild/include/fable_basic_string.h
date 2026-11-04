#pragma once

#include "rebuild_abi.h"

// Lionhead's CBasicString<T> (Ego_r.pdb: 13 bytes for <char>, packed): the
// text buffer, its length, a 31-bit buffer size and the UseFastExtend bit in
// the following byte. CCharStringData wraps one (CCharString's shared data).
//
// Retail <char> instantiation: CBasicString(const char*) 0x009A0590 and
// SetString(const char*, unsigned long) 0x009A0300 are out of line;
// CBasicString(const char*, unsigned long) is inline (CCharString::
// AllocStringData 0x0099EA60 expands it before calling SetString).
#pragma pack(push, 1)
template <class T>
class CBasicString
{
public:
    // FableWin ??0?$CBasicString@D@@QAE@PBD@Z.
    CBasicString(const T* text);

    // FableWin ??0?$CBasicString@D@@QAE@PBDK@Z.
    CBasicString(const T* text, unsigned long length)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(true)
    {
        SetString(text, length);
    }

    // FableWin ??0?$CBasicString@D@@QAE@KD@Z: `length` copies of `character`
    // (CCharString::MakeOwnCopyOfDataOrEmptyString 0x0099EB40 builds (0, ' ')).
    CBasicString(unsigned long length, T character)
        : PData(0), StringLength(0), DataLength(0), UseFastExtend(true)
    {
        SetString(length, character);
    }

    // Fixture storage only: retail declares no default constructor, and no
    // reconstructed code uses this one.
    CBasicString()
    {
    }

protected:
    // FableWin ?SetString@?$CBasicString@D@@IAEXPBDK@Z.
    void SetString(const T* text, unsigned long length);
    // Ego_r ?SetString@?$CBasicString@D@@IAEXKD@Z, retail <char> 0x009A0390.
    void SetString(unsigned long length, T character);

public:
    // FableWin ?Append@?$CBasicString@D@@QAEXPBDK@Z, retail <char> 0x009A04E0
    // (CCharString::operator+= 0x0099F0A0).
    void Append(const T* text, unsigned long length);
    void Append(T character, unsigned long length);

protected:
    // FableWin ?ExtendData@?$CBasicString@D@@IAEXK@Z, retail <char> 0x009A01C0:
    // grow the buffer to hold at least the given string length.
    void ExtendData(unsigned long length);

    // FableWin ?AllocateDataFromStringLength@?$CBasicString@D@@IAEKKPAPAD@Z.
    unsigned long AllocateDataFromStringLength(unsigned long length, T** data);
    // FableWin ?AllocateDataFromDataLength@?$CBasicString@D@@IAEXKPAPAD@Z.
    void AllocateDataFromDataLength(unsigned long dataLength, T** data);
    // FableWin ?ClearStringEnd@?$CBasicString@D@@IAEXPADK@Z.
    void ClearStringEnd(T* data, unsigned long length);
    // FableWin ?Align@?$CBasicString@D@@KIKK@Z.
    static unsigned long __fastcall Align(unsigned long length);
    // FableWin ?CopyAndTerminate@?$CBasicString@D@@KIXPADPBDK@Z.
    static void __fastcall CopyAndTerminate(T* dest, const T* src, unsigned long length);
    // FableWin ?SetStringLength@?$CBasicString@D@@IAEXJ@Z: the length, its terminator
    // and the zeroed tail of the aligned buffer.
    void SetStringLength(long length);
    // FableWin ?Compare@?$CBasicString@D@@KIJPBD0@Z, retail <char> 0x00411570:
    // signed strcmp. The body is below the class (operator< expands it at
    // 0x00429950; map<CCharString, long>::_M_lower_bound 0x0047547D calls it).
    static long __fastcall Compare(const T* left, const T* right);

public:
    // FableWin ??M?$CBasicString@D@@QBE_NABV0@@Z, retail <char> 0x00429950
    // (out of line in map<CCharString, long>::operator[] 0x00519230, with Compare
    // expanded); map<CCharString, long>::_M_lower_bound 0x0047547D expands it
    // to a Compare call and setl.
    FABLE_UNIT_INLINE bool operator<(const CBasicString& other) const
    {
        return Compare(PData, other.PData) < 0;
    }
    // Ego_r ?Truncate@?$CBasicString@D@@QAEXK@Z, retail <char> 0x009A0080.
    void Truncate(unsigned long length);
    // FableWin ?Erase@?$CBasicString@D@@QAEXKK@Z, retail <char> 0x009A0130.
    void Erase(unsigned long start, unsigned long count);
    // Ego_r ?Insert@?$CBasicString@D@@QAEXKPAD@Z, retail <char> 0x009A06B0.
    void Insert(unsigned long pos, T* text);
    // FableWin ??8?$CBasicString@D@@QBE_NPBD@Z, retail 0x004115A0.
    bool operator==(const T* other) const;
    // FableWin ??8?$CBasicString@D@@QBE_NABV0@@Z; inline in retail
    // (CSymbolMap::FindFirstSymbolWithVal 0x009E50C0: length, then Compare).
    bool operator==(const CBasicString& other) const
    {
        if (StringLength != other.StringLength)
            return false;
        return Compare(PData, other.PData) == 0;
    }

public:

    T* PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool UseFastExtend : 1;
};
#pragma pack(pop)


// Signed strcmp: -1, 0 or 1. A fixture of the landed instantiation defines
// FABLE_BASIC_STRING_DECLARATIONS_ONLY to call it instead of a COMDAT copy.
#ifndef FABLE_BASIC_STRING_DECLARATIONS_ONLY
// This unit boundary reproduces retail's out-of-line Compare call in the
// map lower-bound instantiation without changing comparison semantics.
#ifdef FABLE_BASIC_STRING_COMPARE_NOINLINE
# define FABLE_COMPARE_ATTRIBUTE __declspec(noinline)
#else
# define FABLE_COMPARE_ATTRIBUTE
#endif
template <class T>
FABLE_COMPARE_ATTRIBUTE long __fastcall CBasicString<T>::Compare(const T* left, const T* right)
{
    for (;;)
    {
        T l = *left;
        if (l == 0 && *right == 0)
            return 0;
        T r = *right;
        if (l < r)
            return -1;
        if (l > r)
            return 1;
        ++left;
        ++right;
    }
}
#undef FABLE_COMPARE_ATTRIBUTE
#endif

FABLE_STATIC_ASSERT(sizeof(CBasicString<char>) == 0x0D);
