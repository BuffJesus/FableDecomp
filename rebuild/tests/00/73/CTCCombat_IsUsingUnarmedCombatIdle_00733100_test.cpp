#include <cstdio>

unsigned char g_flag = 0;

static int g_check1 = 0, g_check2 = 0, g_check3 = 0;
static int g_getcall = 0;

struct CObj {
    unsigned char buf[0x300];
    CObj* Get(int key) { (void)key; return this; }
    CObj* Get2() { return this; }
    bool  Check3() { return g_check3 != 0; }
    bool  Check() {
        g_getcall++;
        if (g_getcall == 1) return g_check1 != 0;
        if (g_getcall == 2) return g_check2 != 0;
        return false;
    }
};

struct CTCCombat {
    void*  pad0;
    CObj*  obj;
    char   pad2[0x14];
    int    count;
};

bool __fastcall CTCCombat_IsUsingUnarmedCombatIdle(CTCCombat* self)
{
    if (self->obj->Get(0x1b)->Check())
        return false;
    if (self->obj->Get(0x2f)->Check())
        return false;
    if (self->obj->Check3())
        return false;
    if (self->obj->Get2()->buf[0x217])
        return true;
    if (g_flag)
        return false;
    return self->count > 0;
}

int main() {
    static CObj o;
    CTCCombat c; c.obj = &o; c.count = 0;

    g_getcall = 0; g_check1 = 1; g_check2 = 0; g_check3 = 0; o.buf[0x217] = 0; g_flag = 0;
    if (CTCCombat_IsUsingUnarmedCombatIdle(&c)) { std::printf("FAIL c1\n"); return 1; }

    g_getcall = 0; g_check1 = 0; g_check2 = 0; g_check3 = 1; o.buf[0x217] = 0; g_flag = 0;
    if (CTCCombat_IsUsingUnarmedCombatIdle(&c)) { std::printf("FAIL c3\n"); return 1; }

    g_getcall = 0; g_check1 = 0; g_check2 = 0; g_check3 = 0; o.buf[0x217] = 5; g_flag = 0;
    if (!CTCCombat_IsUsingUnarmedCombatIdle(&c)) { std::printf("FAIL cbyte\n"); return 1; }

    g_getcall = 0; o.buf[0x217] = 0; g_flag = 1;
    if (CTCCombat_IsUsingUnarmedCombatIdle(&c)) { std::printf("FAIL cflag\n"); return 1; }

    g_getcall = 0; g_flag = 0; c.count = 3;
    if (!CTCCombat_IsUsingUnarmedCombatIdle(&c)) { std::printf("FAIL ccount+\n"); return 1; }

    g_getcall = 0; c.count = 0;
    if (CTCCombat_IsUsingUnarmedCombatIdle(&c)) { std::printf("FAIL ccount0\n"); return 1; }

    std::printf("CTCCombat_00733100_TEST PASS\n");
    return 0;
}