#include <stdio.h>

// Independent behaviour model of CDrawMiniMap::GetRingAlpha.
static float g_scale = 2.0f;
struct Obj { float v; };

static float getval(Obj* o) { return o->v; }

static unsigned char GetRingAlpha_model(char* self)
{
    if (*(unsigned char*)(self + 0x10c) == 0 &&
        *(unsigned char*)(self + 0x100) != 0)
    {
        int v = (int)(getval((Obj*)(self + 0x138)) * g_scale);
        return (unsigned char)(0xff - v);
    }
    return (unsigned char)(int)(getval((Obj*)(self + 0x100)) * g_scale);
}

int main()
{
    char buf[0x200];
    // Case A: f10c==0, f100!=0 -> inverted path (0xff - v)
    for (int i = 0; i < 0x200; i++) buf[i] = 0;
    buf[0x100] = 1;               // f100 != 0
    buf[0x10c] = 0;               // f10c == 0
    *(float*)(buf + 0x138) = 10.0f; // v=10*2=20 -> 0xff-20 = 235
    unsigned char a = GetRingAlpha_model(buf);

    // Case B: f10c!=0 -> tail path uses subobject at +0x100
    for (int i = 0; i < 0x200; i++) buf[i] = 0;
    buf[0x10c] = 1;
    *(float*)(buf + 0x100) = 5.0f; // 5*2=10
    unsigned char b = GetRingAlpha_model(buf);

    // Case C: f100==0 (and f10c==0) -> tail path, reads +0x100 float (=0) -> 0
    for (int i = 0; i < 0x200; i++) buf[i] = 0;
    *(float*)(buf + 0x100) = 0.0f;
    unsigned char c = GetRingAlpha_model(buf);

    if (a == 235 && b == 10 && c == 0)
        printf("BEHAVIOR_OK\n");
    else
        printf("BEHAVIOR_FAIL a=%d b=%d c=%d\n", a, b, c);
    return 0;
}