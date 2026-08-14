// CDisplayManager::CopyBackBufferToTexture @ 0x0057986e
// Forwards to the __fastcall container-init helper (0x00579681) passing two
// pointers into a single reused stack byte, then returns the this pointer as a
// CTexture*. The two out-parameters are addresses of two short-lived char locals
// that the compiler coalesces onto the same stack slot ([ebp-1]).
struct CTexture;

struct CDisplayManager {
    // __fastcall helper @ 0x00579681, two char* out-params
    CTexture *InitContainerSlot(char *a, char *b);
    CTexture *CopyBackBufferToTexture();
};

#pragma optimize("s", on)
CTexture *CDisplayManager::CopyBackBufferToTexture()
{
    char *pa;
    char *pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    InitContainerSlot(pa, pb);
    return reinterpret_cast<CTexture *>(this);
}