// CTCHeroStats::GetMoralityMax @ 0x0057a67e (retail 18 bytes)
// retail hex: e8 4d c3 10 00  8b 80 dc 00 00 00  8b 80 d0 00 00 00  c3
//
// Disasm:
//   call   0x10c352              ; __fastcall helper, this(ecx) passed through -> ptr in eax
//   mov    eax,[eax+0xdc]        ; eax = *(ptr + 0xDC)
//   mov    eax,[eax+0xd0]        ; eax = *(that + 0xD0)  -> return value
//   ret
//
// VC7.1 model: member accessor as __fastcall with object pointer first (arrives ecx).
// The single call takes ecx(this) with no stack args, so it is another this-only
// accessor on the same object. We model it as a helper returning a pointer, then two
// chained pointer derefs at fixed offsets. cl /O2 emits the tail as a call + two movs.
//
// Engine callee is relocation-masked; declared extern so it need not resolve.

// Forward object type. Only used as an opaque "this".
struct CTCHeroStats;

// The this-only helper the accessor calls (e.g. returns some owned sub-object pointer).
// __fastcall with the object in ecx == thiscall for a this-only member.
extern void * __fastcall CTCHeroStats_GetHelper(CTCHeroStats *self);

long __fastcall CTCHeroStats__GetMoralityMax(CTCHeroStats *self)
{
    // call helper (this passed in ecx) -> pointer in eax
    char *p = (char *)CTCHeroStats_GetHelper(self);   // call 0x10c352
    char *q = *(char **)(p + 0xDC);                    // mov eax,[eax+0xdc]
    return *(long *)(q + 0xD0);                         // mov eax,[eax+0xd0]; ret
}