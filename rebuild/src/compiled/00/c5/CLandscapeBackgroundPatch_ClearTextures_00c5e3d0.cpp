// CLandscapeBackgroundPatch::ClearTextures  @ retail 0x00c5e3d0
// void __fastcall (modeled as __fastcall self in ecx)

// Global function pointer released via call dword ptr [0x143fe30].
// push eax; call [mem]; no stack cleanup at call site -> callee cleans up (stdcall).
extern void (__stdcall *g_ReleaseTexture)(int handle);

struct CLandscapeBackgroundPatch
{
    int field_0;      // +0x00
    int textureHandle; // +0x04

    void ClearTextures();
};

void CLandscapeBackgroundPatch::ClearTextures()
{
    int h = this->textureHandle;
    if (h != -1)
    {
        g_ReleaseTexture(h);
        this->textureHandle = -1;
    }
}