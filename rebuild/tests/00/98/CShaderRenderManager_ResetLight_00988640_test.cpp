#include <cstdio>
#include <cstring>

struct Light { char pad[0x34]; unsigned char enabled; unsigned char dirty; };
struct Mgr {
    char pad[0x164];
    Light* lights;
    char gap[4];
    unsigned long count;
    char pad2[0x268];
    unsigned long dirtyFlags;
};

void __fastcall ResetLight(Mgr* self, unsigned long index)
{
    if (index >= self->count) return;
    Light* base = self->lights;
    unsigned char en = base[index].enabled;
    if (en == 0) return;
    base[index].enabled = 0;
    self->lights[index].dirty = 1;
    self->dirtyFlags |= 0x700;
}

int main()
{
    Light arr[4];
    memset(arr, 0, sizeof(arr));
    Mgr m; memset(&m, 0, sizeof(m));
    m.lights = arr; m.count = 4;

    // index out of range -> no change
    ResetLight(&m, 4);
    if (m.dirtyFlags != 0) { printf("BAD1\n"); return 1; }

    // disabled light -> no change
    arr[1].enabled = 0;
    ResetLight(&m, 1);
    if (arr[1].dirty != 0 || m.dirtyFlags != 0) { printf("BAD2\n"); return 1; }

    // enabled light -> clear enabled, set dirty, set flags
    arr[2].enabled = 1;
    ResetLight(&m, 2);
    if (arr[2].enabled != 0) { printf("BAD3\n"); return 1; }
    if (arr[2].dirty != 1) { printf("BAD4\n"); return 1; }
    if (m.dirtyFlags != 0x700) { printf("BAD5\n"); return 1; }

    printf("RESETLIGHT_OK\n");
    return 0;
}