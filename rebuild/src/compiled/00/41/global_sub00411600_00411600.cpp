#include "rebuild_abi.h"

/* callee at 0x00589f10 — a this-only member invoked on each element.
   Modeled as free __fastcall(objptr) (byte-identical to __fastcall). */
extern void __fastcall sub_00589f10(void* thisptr);

/* 0x00411600 — destroy/cleanup loop over an array of 4-byte elements.
   ecx = first, edx = last, plus one (unused) stack arg -> ret 4.
   For each element (stride 4) it calls the this-only member on the
   element pointer, advancing until first == last. */
void __fastcall sub_00411600(void* ecx, void* edx, int /*unused*/)
{
    unsigned char* first = (unsigned char*)ecx;
    unsigned char* last  = (unsigned char*)edx;

    if (first != last)
    {
        do
        {
            sub_00589f10((void*)first);
            first += 4;
        }
        while (first != last);
    }
}