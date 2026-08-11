#include <stdio.h>

struct IResource;
struct IResVtbl {
    void (__stdcall *v0)(IResource*);
    void (__stdcall *v1)(IResource*);
    void (__stdcall *Release)(IResource*);
};
struct IResource {
    IResVtbl* vtbl;
};

struct CVolume {
    void* pad0;
    IResource* res;
    void ReleaseVolume();
};

static int g_released = 0;
static IResource* g_last = 0;
static void __stdcall MyRelease(IResource* r) { g_released++; g_last = r; }

int main() {
    IResVtbl vt;
    vt.v0 = 0; vt.v1 = 0; vt.Release = MyRelease;
    IResource res;
    res.vtbl = &vt;

    CVolume v;
    v.pad0 = 0;
    v.res = &res;

    v.ReleaseVolume();
    bool ok = (g_released == 1) && (g_last == &res) && (v.res == 0);

    CVolume v2;
    v2.pad0 = 0;
    v2.res = 0;
    v2.ReleaseVolume();
    ok = ok && (g_released == 1) && (v2.res == 0);

    if (ok) printf("RELEASEVOLUME_OK\n");
    else printf("RELEASEVOLUME_FAIL rel=%d\n", g_released);
    return ok ? 0 : 1;
}