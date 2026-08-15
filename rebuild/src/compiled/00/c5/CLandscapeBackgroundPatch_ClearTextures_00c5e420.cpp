// CLandscapeBackgroundPatch::ClearTextures @ 0x00c5e420

struct Tex;

// imported free-like callee via IAT [0x143fe30]; single arg, callee cleans stack
extern void (__stdcall *g_freeTex)(Tex*);

struct CLandscapeBackgroundPatch {
    void* vtbl;   // +0
    Tex*  tex;    // +4
};

void __fastcall ClearTextures(CLandscapeBackgroundPatch* self)
{
    Tex* t = self->tex;
    self->vtbl = (void*)0x12b90e8;
    if (t) {
        g_freeTex(t);
        self->tex = 0;
    }
}