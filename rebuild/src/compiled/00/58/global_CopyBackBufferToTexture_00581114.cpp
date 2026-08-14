// CopyBackBufferToTexture @ 0x00581114  __fastcall  CDisplayManager*
// This module was built with frame pointers. The function takes two 1-byte scratch
// locals in separate nested scopes (so they share one reused stack slot at [ebp-1]),
// captures their addresses, and passes both to a free __stdcall blit helper (callee
// cleans the stack -- no `add esp` after the call), then returns the this pointer as
// a CTexture*. #pragma optimize("s") gives the retail size-tuned prologue/epilogue
// (`push ecx` frame slot + `leave`) with `this` preserved in esi across the call, and
// the two same-slot addresses are materialized with a fresh `lea` each (no CSE) because
// they arrive through two distinct pointer temporaries.
#pragma optimize("s", on)
class CTexture;
class CDisplayManager;

void __stdcall CopyBackBuffer_Blit(char *dst, char *src);

CTexture * __fastcall CopyBackBufferToTexture(CDisplayManager *self)
{
    char *dstPtr;
    char *srcPtr;
    { char dst; dstPtr = &dst; }
    { char src; srcPtr = &src; }
    CopyBackBuffer_Blit(dstPtr, srcPtr);
    return (CTexture *)self;
}