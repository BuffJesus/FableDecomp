#pragma optimize("s",on)
// CDisplayManager::CopyBackBufferToTexture @ 0x004df84e (__fastcall, returns CTexture&)
// Forwards two scratch byte flags to the texture-format handler at 0x004dd1f2, then
// returns *this. The two flags live in disjoint scopes so the optimizer overlays them
// on the same stack slot while still emitting a separate lea for each argument.
struct CTexture;

struct CDisplayManager {
    CTexture& CopyBackBufferToTexture();
    void Handler(char* a, char* b);   // 0x004dd1f2, __fastcall(this, a, b)
};

CTexture& CDisplayManager::CopyBackBufferToTexture()
{
    char* p;
    char* q;
    { char b; q = &b; }
    { char a; p = &a; }
    this->Handler(q, p);
    return *reinterpret_cast<CTexture*>(this);
}