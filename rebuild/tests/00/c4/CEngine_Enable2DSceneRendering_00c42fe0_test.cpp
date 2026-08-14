#include <cstdio>

struct CEngine {
    char pad0[0x14];
    unsigned int flags;   // +0x14
    char pad1[0x08];
    float value;          // +0x20
};

void __fastcall Enable2DSceneRendering(CEngine *self, float v)
{
    if (self->value != v) {
        self->value = v;
        self->flags |= 4;
    }
}

int main()
{
    CEngine e;
    e.flags = 0;
    e.value = 1.0f;

    Enable2DSceneRendering(&e, 1.0f);
    if (e.flags != 0) { printf("FAIL nochange\n"); return 1; }
    if (e.value != 1.0f) { printf("FAIL val1\n"); return 1; }

    Enable2DSceneRendering(&e, 2.5f);
    if (e.value != 2.5f) { printf("FAIL val2\n"); return 1; }
    if ((e.flags & 4) == 0) { printf("FAIL flag\n"); return 1; }

    e.flags = 0x11;
    Enable2DSceneRendering(&e, 9.0f);
    if (e.flags != 0x15) { printf("FAIL preserve\n"); return 1; }
    if (e.value != 9.0f) { printf("FAIL val3\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}