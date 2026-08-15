// CLandscapeBackgroundPatch::ClearTextures  0x00c739a0
struct Texture;

// __stdcall import via IAT: push arg, call [__imp_], callee cleans (no add esp).
// dllimport forces the indirect `ff15 [import]` form (6 bytes) not `e8 rel32`.
extern "C" __declspec(dllimport) void __stdcall ReleaseTexture(Texture* t);

struct CLandscapeBackgroundPatch {
    void** vtbl;      // +0
    Texture* texture; // +4
};

extern void* const CLandscapeBackgroundPatch_vtbl;

void __fastcall ClearTextures(CLandscapeBackgroundPatch* self)
{
    Texture* t = self->texture;
    self->vtbl = (void**)&CLandscapeBackgroundPatch_vtbl;
    if (t) {
        ReleaseTexture(t);
        self->texture = 0;
    }
}