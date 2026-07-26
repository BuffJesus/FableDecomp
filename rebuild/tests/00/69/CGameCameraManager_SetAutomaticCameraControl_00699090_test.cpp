#include <cstdio>

static int g_ctor = 0, g_dtor = 0, g_inner = 0, g_extra = 0;
static int g_lastEdx = 0;

struct ScopedGuard {
    void* p;
    ScopedGuard(const char* s, int n) { p = (void*)s; (void)n; g_ctor++; }
    ~ScopedGuard() { g_dtor++; }
};

struct Inner { char x; };

struct Sub100 {
    Inner* get(ScopedGuard*);
};

static Inner g_innerObj;
Inner* Sub100::get(ScopedGuard*) { return &g_innerObj; }

static const char kStrA[] = "A";
static const char kStrB[] = "B";

struct CGameCameraManager {
    char pad[0x100];
    Sub100 sub100;
    char pad1[0x2c - sizeof(Sub100)];
    char flag12c;
    char flag12d;
    char pad2[6];
    int  h134;
    void Extra() { g_extra++; }
    void SetAutomaticCameraControl();
};

void __fastcall Inner_call_edx(Inner*, int edx) { g_inner++; g_lastEdx = edx; }

void CGameCameraManager::SetAutomaticCameraControl()
{
    if (this->flag12c == 0) {
        this->flag12c = 0;
        ScopedGuard g(kStrA, -1);
        int h = this->h134;
        Inner* p = this->sub100.get(&g);
        Inner_call_edx(p, h);
        return;
    }
    {
        ScopedGuard g(kStrB, -1);
        int h = this->h134;
        Inner* p = this->sub100.get(&g);
        Inner_call_edx(p, h);
    }
    this->Extra();
    this->flag12d = 0;
    this->flag12c = 0;
}

int main()
{
    CGameCameraManager m;
    for (int i = 0; i < (int)sizeof(m); i++) ((char*)&m)[i] = 0;
    m.h134 = 0x1234;

    m.flag12c = 0;
    m.SetAutomaticCameraControl();
    if (!(g_ctor == 1 && g_dtor == 1 && g_inner == 1 && g_extra == 0 &&
          g_lastEdx == 0x1234 && m.flag12c == 0)) {
        std::printf("FAIL branch0\n"); return 1;
    }

    m.flag12c = 1; m.flag12d = 7;
    m.SetAutomaticCameraControl();
    if (!(g_ctor == 2 && g_dtor == 2 && g_inner == 2 && g_extra == 1 &&
          m.flag12c == 0 && m.flag12d == 0)) {
        std::printf("FAIL branch1\n"); return 1;
    }

    std::printf("CGameCameraManager_00699090_TEST PASS\n");
    return 0;
}