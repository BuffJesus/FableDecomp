#include <cstdio>

static int g_logged = -999;

struct CObj { int state; };
struct CObj2 {
    int state;
    char Check1(CObj* a) { return this->state != 42 ? 1 : 0; }
    int  GetState() { return this->state; }
};
struct CSub {
    char DoProcess(CObj* a, int b, int c) { return (char)(a->state + b + c); }
};
struct CWeapon {
    char pad[0x54];
    char Check1(CObj* a) { return a->state != 42 ? 1 : 0; }
};
struct CLogger {
    void LogErr(int code) { g_logged = code; }
};

CLogger g_logger;

char __fastcall Weapon_ProcessAnimFrameIfReady(CWeapon* self, int edx, CObj* a, int b, int c)
{
    if (self->Check1(a))
    {
        int s = ((CObj2*)a)->GetState();
        if (s < 0)
        {
            g_logger.LogErr(s);
            return 0;
        }
        return ((CSub*)((char*)self + 0x54))->DoProcess(a, b, c);
    }
    return 0;
}

int main()
{
    CWeapon w; CObj o;
    // not ready path: Check1 false -> return 0
    o.state = 42;
    char r0 = Weapon_ProcessAnimFrameIfReady(&w, 0, &o, 1, 2);
    if (r0 != 0) { std::printf("FAIL notready\n"); return 1; }

    // error path (GetState<0) -> log + return 0
    o.state = -5;
    g_logged = -999;
    char r1 = Weapon_ProcessAnimFrameIfReady(&w, 0, &o, 1, 2);
    if (r1 != 0 || g_logged != -5) { std::printf("FAIL errpath\n"); return 1; }

    // ok path -> DoProcess(state+b+c)
    o.state = 10;
    char r2 = Weapon_ProcessAnimFrameIfReady(&w, 0, &o, 3, 4);
    if (r2 != (char)17) { std::printf("FAIL okpath %d\n", (int)r2); return 1; }

    std::printf("_global_00c43f80_TEST PASS\n");
    return 0;
}