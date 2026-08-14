#pragma optimize("s",on)
// CDisplayManager::CopyBackBufferToTexture @ 0x004e76df  (__fastcall, returns this)
// Grabs the address of two short-lived scratch bytes (which the compiler places in
// the same 1-byte frame slot) and forwards both to a texture-format helper, then
// returns the display manager itself.

struct CTexture;

struct CDisplayManager {
    CTexture* CopyBackBufferToTexture();
    void Helper(char* a, char* b);   // __fastcall @ 0x004dd170 (TextureFormat_Handler_01)
};

CTexture* CDisplayManager::CopyBackBufferToTexture()
{
    char* p1;
    char* p2;
    { char scratch; p1 = &scratch; }
    { char scratch; p2 = &scratch; }
    this->Helper(p1, p2);
    return (CTexture*)this;
}