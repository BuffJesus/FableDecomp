#pragma optimize("s",on)
#include "rebuild_abi.h"

// 0x004711f5  make_heap<CThing**,CThingCompare_BestForTargeting2>
// __fastcall(first(ecx), last(edx), comp(stack)) -> void, ret 4
//
// Retail is the MSVC _Make_heap threshold-split: if the (byte) range rounded
// down to a multiple of the 8-byte element size exceeds 0x80 (16 elements),
// recurse on the first 16-element prefix then adjust-down the remainder;
// otherwise build the whole heap in one shot.

struct CThingHeapElem { char _[8]; };  // 8-byte element (and ~7 / +0x80 stride)

// fn1: make_heap over [first,last)      __fastcall(first, last, comp)
extern void FABLE_FASTCALL heap_build_44(CThingHeapElem* first,
                                         CThingHeapElem* last,
                                         void* comp);
// fn2: adjust-down builder              __fastcall(first, last, int, comp)
// (ecx=first, edx=last, then push 0, push comp)
extern void FABLE_FASTCALL heap_adjust_45(CThingHeapElem* first,
                                          CThingHeapElem* last,
                                          int hole,
                                          void* comp);

void FABLE_FASTCALL make_heap_004711f5(CThingHeapElem* first,
                                       CThingHeapElem* last,
                                       void* comp)
{
    if ((fable_i32)(((fable_u32)((char*)last - (char*)first)) & 0xfffffff8u) > 0x80)
    {
        CThingHeapElem* mid = (CThingHeapElem*)((char*)first + 0x80);
        heap_build_44(first, mid, comp);
        heap_adjust_45(mid, last, 0, comp);
    }
    else
    {
        heap_build_44(first, last, comp);
    }
}