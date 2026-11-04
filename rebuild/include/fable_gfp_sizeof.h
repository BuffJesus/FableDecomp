#pragma once
// Lionhead's memory-accounting helpers, from FableWin.pdb (debug build, nothing inlined):
//  - ??$GFPIsNull@<T>@@YI_N...@Z: whether a pointer member is null (overloads for raw pointers,
//    CIVCountedPointer<T> and CCountedPointer<T>; e.g. ??$GFPIsNull@VCAnimationData@C3DAnimationInfo@@@@YI_NABV?$CIVCountedPointer@VCAnimationData@C3DAnimationInfo@@@@@Z).
//  - ??$GFPSizeof@<T>@@YIK<T&>@Z (19 instantiations, e.g. FableWin 0x03157C70 for
//    CIVCountedPointer<C3DAnimationInfo::CAnimationData>): the memory a pointer member accounts
//    for, i.e. its own size plus, when it is set, the pointee's size (FableWin calls
//    ??$GFSizeof@<T>@@YIKAAV<T>@@@Z on *pointer, which forwards to the pointee's Sizeof()).
// Retail /O2 expands them inline in every Sizeof (C3DAnimationInfo::Sizeof 0x00A265B0:
// `Data ? Data->Sizeof() + 4 : 4`, the non-null case falling through).
// Only the overloads the recovered bodies use are modelled.
#include "rebuild_abi.h"
#include "fable_iv_counted_pointer.h"

template <class T>
inline bool FABLE_FASTCALL GFPIsNull(const CIVCountedPointer<T>& pointer)
{
    return pointer.IsNull();
}

template <class T>
inline bool FABLE_FASTCALL GFPIsNull(T* pointer)
{
    return pointer == 0;
}

template <class T>
inline unsigned long FABLE_FASTCALL GFPSizeof(T& pointer)
{
    if (!GFPIsNull(pointer))
        return pointer->Sizeof() + sizeof(pointer);
    return sizeof(pointer);
}
