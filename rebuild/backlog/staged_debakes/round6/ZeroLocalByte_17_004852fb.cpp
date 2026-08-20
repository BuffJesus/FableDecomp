#pragma optimize("s",on)
// One-byte local zeroed through the intrinsic memset path and read straight back.
// VC7.1 emits `push edi; xor eax,eax; lea edi,[ebp-1]; stosb; mov al,[ebp-1]`.
// The return type is `char`, NOT `bool` — a bool return adds the `neg/sbb/neg`
// normalisation and the bytes no longer match. __fastcall this=ecx, no args.
#include <string.h>
#pragma intrinsic(memset)
struct T { char AddChildPrimitive(); };
char T::AddChildPrimitive() {
    volatile char flag;
    memset((void*)&flag, 0, 1);
    return flag;
}
