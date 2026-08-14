// CDisplayManager::CopyBackBufferToTexture @ 0x004df956
// Returns the self pointer (esi) after forwarding two scratch byte-locals to a helper.
// Retail keeps the EBP frame and reuses one stack slot ([ebp-1]) for both locals,
// re-materialising the address (two lea) rather than CSEing it -> size-optimised codegen.
#pragma optimize("s",on)

struct CTexture;

struct CDisplayManager {
    CTexture* helper(char* a, char* b);   // @ 0x4dd405, __fastcall member
    CTexture* CopyBackBufferToTexture();
};

CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char* pa;
    char* pb;
    { char a; pa = &a; }
    { char b; pb = &b; }
    this->helper(pa, pb);
    return (CTexture*)this;
}