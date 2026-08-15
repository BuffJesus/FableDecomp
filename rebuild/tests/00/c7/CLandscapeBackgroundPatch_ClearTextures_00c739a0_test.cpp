#include <cstdio>

struct Texture { int alive; };

static int g_released = 0;
static void ReleaseTexture(Texture* t) { if (t) { t->alive = 0; g_released++; } }

static void* g_vtbl_marker = (void*)0x12345678;

struct CLandscapeBackgroundPatch {
    void** vtbl;
    Texture* texture;
};

static void ClearTextures(CLandscapeBackgroundPatch* self)
{
    Texture* t = self->texture;
    self->vtbl = (void**)&g_vtbl_marker;
    if (t) { ReleaseTexture(t); }
    self->texture = 0;
}

int main()
{
    Texture tex; tex.alive = 1;
    CLandscapeBackgroundPatch o;
    o.vtbl = 0; o.texture = &tex;
    ClearTextures(&o);
    bool ok1 = (o.vtbl == (void**)&g_vtbl_marker);
    bool ok2 = (o.texture == 0);
    bool ok3 = (g_released == 1);
    bool ok4 = (tex.alive == 0);

    // null-texture path: no release, no crash
    CLandscapeBackgroundPatch o2;
    o2.vtbl = 0; o2.texture = 0;
    g_released = 0;
    ClearTextures(&o2);
    bool ok5 = (o2.texture == 0) && (g_released == 0) && (o2.vtbl == (void**)&g_vtbl_marker);

    if (ok1 && ok2 && ok3 && ok4 && ok5) printf("CLEARTEX_OK\n");
    else printf("FAIL\n");
    return 0;
}