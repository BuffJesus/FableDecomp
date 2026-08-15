struct CLandscapeBackgroundPatch {
    char pad[0xc0];
    void* tex;   // +0xc0
};

// call [0x14404b0]: global stdcall function pointer, one arg, callee-cleaned
extern void (__stdcall *g_releaseTexture)(void*);

void __fastcall ClearTextures(CLandscapeBackgroundPatch* self)
{
    void* t = self->tex;
    if (t) {
        g_releaseTexture(t);
        self->tex = 0;
    }
}