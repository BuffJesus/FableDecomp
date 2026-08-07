#include "rebuild_abi.h"
// _Cons_val<...> @ 0x00b847b0
// ecx=this; eax=[esp+4]=n; lea eax,[eax*3+3]; edx=this+eax*8; [esp+4]=edx; jmp helper
struct Pair24 { char raw[24]; };
struct ConsAlloc {
    void Helper(Pair24 *last);   // __fastcall(this, last)
    void ConsVal(unsigned n);    // __fastcall(this, n)
};
void ConsAlloc::ConsVal(unsigned n)
{
    Helper((Pair24*)this + (n + 1));
}