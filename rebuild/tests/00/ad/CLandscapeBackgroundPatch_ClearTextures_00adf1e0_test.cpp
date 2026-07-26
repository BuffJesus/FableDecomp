#include <cstdio>

const double g_clearDouble = 8.0;

static int g_released = 0;
static int g_relHandle = -999;
static void* g_relSelf = 0;

struct TextureOwner {
    void Release(int handle) {
        g_released++;
        g_relHandle = handle;
        g_relSelf = this;
    }
};

void __fastcall CLandscapeBackgroundPatch_ClearTextures(void* selfv)
{
    unsigned char* p = (unsigned char*)selfv;
    double dv = g_clearDouble;
    *(char*)(p + 0x10) = 0;
    *(double*)(p + 0x50) = dv;
    *(int*)(p + 0x30) = 0;
    *(int*)(p + 0x1c) = 0;
    *(int*)(p + 0x20) = 0;
    *(int*)(p + 0x24) = 0;
    *(int*)(p + 0x28) = 0;
    *(int*)(p + 0x2c) = 0;
    int h = *(int*)(p + 0x74);
    *(unsigned int*)(p + 0x0c) = 0x3d888889u;
    *(unsigned int*)(p + 0x14) = 0x3f800000u;
    *(unsigned int*)(p + 0x48) = 0x3e800000u;
    if (h != -1) {
        ((TextureOwner*)(*(void**)(p + 0x78)))->Release(h);
        *(int*)(p + 0x74) = -1;
    }
}

int main() {
    unsigned char buf[0x90];
    for (int i = 0; i < 0x90; i++) buf[i] = 0xAB;

    void* fakeOwner = (void*)0x12345678;
    *(int*)(buf + 0x74) = 42;
    *(void**)(buf + 0x78) = fakeOwner;
    CLandscapeBackgroundPatch_ClearTextures(buf);

    if (g_released != 1) { std::printf("FAIL: release count %d\n", g_released); return 1; }
    if (g_relHandle != 42) { std::printf("FAIL: handle %d\n", g_relHandle); return 1; }
    if (g_relSelf != fakeOwner) { std::printf("FAIL: self ptr\n"); return 1; }
    if (*(int*)(buf + 0x74) != -1) { std::printf("FAIL: i74 not -1\n"); return 1; }
    if (buf[0x10] != 0) { std::printf("FAIL: b10\n"); return 1; }
    if (*(double*)(buf + 0x50) != 8.0) { std::printf("FAIL: d50\n"); return 1; }
    if (*(int*)(buf + 0x1c) != 0 || *(int*)(buf + 0x20) != 0 || *(int*)(buf + 0x24) != 0 ||
        *(int*)(buf + 0x28) != 0 || *(int*)(buf + 0x2c) != 0 || *(int*)(buf + 0x30) != 0) {
        std::printf("FAIL: zero fields\n"); return 1;
    }
    if (*(unsigned int*)(buf + 0x0c) != 0x3d888889u) { std::printf("FAIL: f0c\n"); return 1; }
    if (*(unsigned int*)(buf + 0x14) != 0x3f800000u) { std::printf("FAIL: f14\n"); return 1; }
    if (*(unsigned int*)(buf + 0x48) != 0x3e800000u) { std::printf("FAIL: f48\n"); return 1; }

    g_released = 0;
    *(int*)(buf + 0x74) = -1;
    CLandscapeBackgroundPatch_ClearTextures(buf);
    if (g_released != 0) { std::printf("FAIL: released on -1\n"); return 1; }

    std::printf("CLandscapeBackgroundPatch_00adf1e0_TEST PASS\n");
    return 0;
}