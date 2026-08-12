#include <cstdio>

static long g_ret = 0;
static void* g_self = 0;
static void* g_p = 0;
static float g_f = 0.0f;

struct Inner {
    char pad[0x64];
    Inner* subObj; // +0x64
    long Query(void* p, float f);
};

long Inner::Query(void* p, float f)
{
    g_self = this; g_p = p; g_f = f;
    return g_ret;
}

struct CTCTargeted {
    void* v;
    Inner* obj;
};

long __fastcall CTCTargeted_GetNumTargetPoints(CTCTargeted* self);

int main()
{
    // Case 1: subObj == null -> returns 0
    Inner inner;
    inner.subObj = 0;
    CTCTargeted t;
    t.obj = &inner;
    long r0 = CTCTargeted_GetNumTargetPoints(&t);

    // Case 2: subObj != null -> forwards to Inner::Query with (0x13bac50, 1.0f)
    Inner subObj;
    inner.subObj = &subObj;
    g_ret = 42;
    long r1 = CTCTargeted_GetNumTargetPoints(&t);

    bool ok = (r0 == 0)
        && (r1 == 42)
        && (g_self == (void*)&subObj)
        && (g_p == (void*)0x13bac50)
        && (g_f == 1.0f);

    if (ok) { printf("PASS_GNTP_OK\n"); return 0; }
    printf("FAIL r0=%ld r1=%ld p=%p f=%f\n", r0, r1, g_p, g_f);
    return 1;
}