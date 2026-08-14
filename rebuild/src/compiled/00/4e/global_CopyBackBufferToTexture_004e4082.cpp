// CopyBackBufferToTexture @ 0x004e4082  CDisplayManager  __fastcall -> CTexture*
//
// Forwarder: builds two transient bool flags (default state), hands their
// addresses to the texture-format worker at 0x4dd487, then returns `this`
// (the CDisplayManager, reinterpreted as the produced CTexture*). The two
// flags have non-overlapping scopes, so VC7.1 packs them into the single
// [ebp-1] stack byte and emits an independent `lea` per argument.
#pragma optimize("y",off)   // retail kept the ebp frame (leave epilog)
#pragma optimize("s",on)    // retail favored size here (leave, no CSE fold)

struct CTexture;
struct CDisplayManager;

// worker @ 0x4dd487 (TextureFormat_Handler_14): __stdcall(bool*, bool*)
void __stdcall CopyBackBuffer_FormatStep(bool* a, bool* b);

CTexture* __fastcall CopyBackBufferToTexture(CDisplayManager* self, void* /*edx*/)
{
    bool* a;
    bool* b;
    { bool flagA; a = &flagA; }
    { bool flagB; b = &flagB; }
    CopyBackBuffer_FormatStep(a, b);
    return (CTexture*)self;
}