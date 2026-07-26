#include <cstdio>

struct RefTarget {
    void** vtbl;
    long refcount;
    char pad[0x5f - 8];
    bool analogueLoading;
};

struct CTCWeapon {
    char pad[0xc];
    RefTarget* m_target;
};

typedef void (__fastcall *ReleaseFn)(RefTarget*);

static int g_released = 0;
static void __fastcall FakeRelease(RefTarget* t) { g_released = 1; }

bool __fastcall CTCWeapon_IsAnalogueLoading(const CTCWeapon* self)
{
    RefTarget* t = self->m_target;
    if (t)
        ++t->refcount;
    bool v = t->analogueLoading;
    long* pr = &t->refcount;
    long rc = *pr - 1;
    *pr = rc;
    if (rc == 0)
        ((ReleaseFn)t->vtbl[1])(t);
    return v;
}

int main()
{
    void* vt[2];
    vt[0] = 0;
    vt[1] = (void*)&FakeRelease;

    RefTarget t;
    t.vtbl = vt;
    t.refcount = 1;
    t.analogueLoading = true;

    CTCWeapon w;
    w.m_target = &t;

    bool r = CTCWeapon_IsAnalogueLoading(&w);
    if (r != true) { std::printf("FAIL: expected true got %d\n", (int)r); return 1; }
    if (t.refcount != 1) { std::printf("FAIL: refcount %ld\n", t.refcount); return 1; }
    if (g_released != 0) { std::printf("FAIL: released early\n"); return 1; }

    t.analogueLoading = false;
    t.refcount = 1;
    r = CTCWeapon_IsAnalogueLoading(&w);
    if (r != false) { std::printf("FAIL: expected false got %d\n", (int)r); return 1; }

    std::printf("CTCWeapon_005d9270_TEST PASS\n");
    return 0;
}