// CLandscapeBackgroundPatch::ClearTextures  0x00c73800
// void __fastcall(this) ; harness rewrites __fastcall->__fastcall

typedef void (__stdcall *FreeFn)(void*);
extern FreeFn g_freeFn;  // global function pointer at fixed address (0x143fbfc)

struct CLandscapeBackgroundPatch
{
    void* m_field0;    // +0x00
    void* m_textures;  // +0x04

    void ClearTextures();
};

void CLandscapeBackgroundPatch::ClearTextures()
{
    void* p = this->m_textures;
    if (p != 0)
    {
        g_freeFn(p);
        this->m_textures = 0;
    }
}