#include <stdio.h>

// Independent behaviour model of 0x00643d5d:
// returns (long)((float)(unsigned char)key * this->inner->scale).

struct Inner {
    char  pad[0x2a0];
    float scale;
};
struct Outer {
    int    m0;
    Inner* inner;
};

static float g_last = -1.0f;
static long ftol2_model(float v) { g_last = v; return (long)v; }

static long model(Outer* self, int key)
{
    Inner* p = self->inner;
    unsigned char b = (unsigned char)key;
    float f = (float)(int)b;
    return ftol2_model(f * p->scale);
}

int main()
{
    Inner inr;
    inr.scale = 2.5f;
    Inner* pinr = &inr;

    Outer obj;
    obj.m0 = 0;
    obj.inner = pinr;
    Outer* pobj = &obj;

    long r1 = model(pobj, 0x104);   /* low byte 4 -> 4 * 2.5 = 10 */
    int ok1 = (r1 == 10) && (g_last == 10.0f);

    long r2 = model(pobj, 0x100);   /* low byte 0 -> 0.0 */
    int ok2 = (r2 == 0) && (g_last == 0.0f);

    long r3 = model(pobj, 3);       /* 3 * 2.5 = 7.5 -> ftol truncates to 7 */
    int ok3 = (r3 == 7);

    if (ok1 && ok2 && ok3) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL r1=%ld r2=%ld r3=%ld g=%f\n", r1, r2, r3, g_last);
    }
    return 0;
}