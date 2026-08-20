#pragma optimize("s",on)
// CDisplayManager::CopyBackBufferToTexture @ 0x004df7ff
// __fastcall CTexture* (CDisplayManager*)
//
// The retail body forms two 1-byte stack temporaries in disjoint lifetimes
// (so both reuse the same [ebp-1] slot), takes the address of each, passes
// the pair to a __fastcall member helper (call 0x004dd12f), keeps `this` in a
// callee-saved register (esi) across the call, and returns `this` cast to
// CTexture*. VC7.1 emits two separate `lea eax,[ebp-1]` because the two temps
// are distinct source objects that happen to share the reused slot.

struct CTexture;

struct CDisplayManager {
    void Grab(char* a, char* b);
    CTexture* CopyBackBufferToTexture();
};

CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char* p;
    char* q;
    { char a; p = &a; }
    { char b; q = &b; }
    Grab(p, q);
    return (CTexture*)this;
}