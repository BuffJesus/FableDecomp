#include <cstdio>

struct C;

struct A {
    char pad1c[0x1c];
    struct BSrc* f1c;
};

struct BSrc {
    C* GetB();
};

struct C {
    C* GetC();
    void Do(int arg);
};

struct D {
    void Do(int arg);
};

struct Wrapper {
    C* p;
    bool Check();
};

struct GlobalGui {
    void Notify();
};

A* g_A;
GlobalGui* g_pGlobalGui;

static int  g_do_called   = 0;
static int  g_do_arg      = 0;
static int  g_notify      = 0;
static bool g_check_ret   = true;
static C*   g_check_seen  = 0;
static C*   g_theC        = 0;

C* BSrc::GetB()          { return g_theC; }
C* C::GetC()             { return (C*)this; }
void C::Do(int a)        { g_do_called++; g_do_arg = a; }
void D::Do(int a)        { g_do_called++; g_do_arg = a; }
void GlobalGui::Notify() { g_notify++; }
bool Wrapper::Check()    { g_check_seen = p; return g_check_ret; }

void __fastcall SavedConfirmed();

int main()
{
    static C theC;
    static BSrc theB;
    static A theA;
    static GlobalGui theGui;

    g_theC = &theC;
    theA.f1c = &theB;
    g_A = &theA;
    g_pGlobalGui = &theGui;

    g_check_ret  = true;
    g_do_called  = 0;
    g_do_arg     = 0;
    g_notify     = 0;
    g_check_seen = 0;

    SavedConfirmed();

    if (g_check_seen != &theC) { std::printf("FAIL check_seen\n"); return 1; }
    if (g_do_called != 1)      { std::printf("FAIL do_called\n"); return 1; }
    if (g_do_arg != 0xC6)      { std::printf("FAIL do_arg %d\n", g_do_arg); return 1; }
    if (g_notify != 1)         { std::printf("FAIL notify\n"); return 1; }

    g_check_ret  = false;
    g_do_called  = 0;
    g_notify     = 0;

    SavedConfirmed();

    if (g_do_called != 0) { std::printf("FAIL inactive do\n"); return 1; }
    if (g_notify != 0)    { std::printf("FAIL inactive notify\n"); return 1; }

    std::printf("SAVEDCONFIRMED_0047DB73_TEST PASS\n");
    return 0;
}