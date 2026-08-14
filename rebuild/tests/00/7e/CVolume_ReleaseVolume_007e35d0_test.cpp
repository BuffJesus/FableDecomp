#include <cstdio>

static int g_released = -1;

struct IVolumeManager {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void ReleaseHandle(int handle) { g_released = handle; }
};

static IVolumeManager g_mgr;
static IVolumeManager* g_mgr_ptr = &g_mgr;
static IVolumeManager** const g_volumeManager = &g_mgr_ptr;

struct CVolume { int f0; int f4; int f8; int handle; };

void __fastcall ReleaseVolume(CVolume* self)
{
    int h = self->handle;
    if (h != 0) {
        (*g_volumeManager)->ReleaseHandle(h);
        self->handle = 0;
    }
}

int main() {
    // case: handle non-zero -> releases and zeros
    CVolume v; v.f0=1; v.f4=2; v.f8=3; v.handle=0x1234;
    ReleaseVolume(&v);
    if (g_released != 0x1234) { printf("BAD1\n"); return 1; }
    if (v.handle != 0) { printf("BAD2\n"); return 1; }

    // case: handle zero -> no release, no change
    g_released = -1;
    CVolume v2; v2.handle = 0;
    ReleaseVolume(&v2);
    if (g_released != -1) { printf("BAD3\n"); return 1; }
    if (v2.handle != 0) { printf("BAD4\n"); return 1; }

    printf("VOLPARITY_OK\n");
    return 0;
}