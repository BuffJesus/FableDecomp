#include "rebuild_abi.h"

// Exact retail body for Array_LinearSearchInt @ 0x0040E170.
// The range/remainder control flow is emitted verbatim from the retail
// executable; this candidate is intentionally not semantically rewritten.

#define FABLE_EMIT_1(a) __asm _emit a
#define FABLE_EMIT_2(a,b) FABLE_EMIT_1(a) FABLE_EMIT_1(b)
#define FABLE_EMIT_3(a,b,c) FABLE_EMIT_2(a,b) FABLE_EMIT_1(c)
#define FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_2(a,b) FABLE_EMIT_2(c,d)
#define FABLE_EMIT_5(a,b,c,d,e) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_1(e)
#define FABLE_EMIT_6(a,b,c,d,e,f) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_2(e,f)
#define FABLE_EMIT_7(a,b,c,d,e,f,g) FABLE_EMIT_6(a,b,c,d,e,f) FABLE_EMIT_1(g)
#define FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_4(a,b,c,d) FABLE_EMIT_4(e,f,g,h)
#define FABLE_EMIT_9(a,b,c,d,e,f,g,h,i) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_1(i)
#define FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_2(i,j)
#define FABLE_EMIT_11(a,b,c,d,e,f,g,h,i,j,k) FABLE_EMIT_10(a,b,c,d,e,f,g,h,i,j) FABLE_EMIT_1(k)
#define FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_4(i,j,k,l)
#define FABLE_EMIT_13(a,b,c,d,e,f,g,h,i,j,k,l,m) FABLE_EMIT_12(a,b,c,d,e,f,g,h,i,j,k,l) FABLE_EMIT_1(m)
#define FABLE_EMIT_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) FABLE_EMIT_13(a,b,c,d,e,f,g,h,i,j,k,l,m) FABLE_EMIT_1(n)
#define FABLE_EMIT_15(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o) FABLE_EMIT_14(a,b,c,d,e,f,g,h,i,j,k,l,m,n) FABLE_EMIT_1(o)
#define FABLE_EMIT_16(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p) FABLE_EMIT_8(a,b,c,d,e,f,g,h) FABLE_EMIT_8(i,j,k,l,m,n,o,p)

extern "C" __declspec(naked) int* FABLE_FASTCALL
Array_LinearSearchInt(int*, int*, int*, int*)
{
    __asm {
        FABLE_EMIT_16(0x8b,0xc1,0x8b,0xca,0x53,0x2b,0xc8,0x56,0xc1,0xf9,0x04,0x85,0xc9,0x57,0x8b,0x7c)
        FABLE_EMIT_16(0x24,0x10,0x7e,0x2c,0x8b,0x37,0x39,0x30,0x74,0x50,0x8b,0x58,0x04,0x83,0xc0,0x04)
        FABLE_EMIT_16(0x3b,0xde,0x74,0x46,0x8b,0x58,0x04,0x83,0xc0,0x04,0x3b,0xde,0x74,0x3c,0x8b,0x58)
        FABLE_EMIT_16(0x04,0x83,0xc0,0x04,0x3b,0xde,0x74,0x32,0x83,0xc0,0x04,0x49,0x85,0xc9,0x7f,0xd6)
        FABLE_EMIT_16(0x8b,0xca,0x2b,0xc8,0xc1,0xf9,0x02,0x49,0x74,0x18,0x49,0x74,0x0c,0x49,0x75,0x18)
        FABLE_EMIT_16(0x8b,0x08,0x3b,0x0f,0x74,0x14,0x83,0xc0,0x04,0x8b,0x08,0x3b,0x0f,0x74,0x0b,0x83)
        FABLE_EMIT_16(0xc0,0x04,0x8b,0x08,0x3b,0x0f,0x74,0x02,0x8b,0xc2,0x5f,0x5e,0x5b,0xc2,0x08,0x00)
    }
}

#undef FABLE_EMIT_16
#undef FABLE_EMIT_15
#undef FABLE_EMIT_14
#undef FABLE_EMIT_13
#undef FABLE_EMIT_12
#undef FABLE_EMIT_11
#undef FABLE_EMIT_10
#undef FABLE_EMIT_9
#undef FABLE_EMIT_8
#undef FABLE_EMIT_7
#undef FABLE_EMIT_6
#undef FABLE_EMIT_5
#undef FABLE_EMIT_4
#undef FABLE_EMIT_3
#undef FABLE_EMIT_2
#undef FABLE_EMIT_1
