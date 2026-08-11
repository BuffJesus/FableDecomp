#include <cstdio>

static int g_checkRet = 0;
static int g_notified = 0;

struct Sub14 {
    int Check();
};
struct Target4 {
    void Notify();
};
struct CTCDParticleEmitter {
    int      m0;
    Target4* m4;
    int      m8;
    int      mC;
    int      m10;
    Sub14    m14;
    void OnDie();
};

int Sub14::Check() { return g_checkRet; }
void Target4::Notify() { g_notified++; }

void CTCDParticleEmitter::OnDie()
{
    if (this->m14.Check())
        this->m4->Notify();
}

int main()
{
    Target4 tgt;
    CTCDParticleEmitter e;
    e.m4 = &tgt;

    g_checkRet = 0; g_notified = 0;
    e.OnDie();
    if (g_notified != 0) { printf("FAIL c1\n"); return 1; }

    g_checkRet = 1; g_notified = 0;
    e.OnDie();
    if (g_notified != 1) { printf("FAIL c2\n"); return 1; }

    printf("ONDIE_OK\n");
    return 0;
}