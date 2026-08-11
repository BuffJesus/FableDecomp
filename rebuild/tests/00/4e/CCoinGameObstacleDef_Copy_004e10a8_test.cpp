#include <cstdio>

struct CDefClassBase;

struct CDefBase {
    void Inherit(CDefClassBase const *src);
};

struct CCoinGameObstacleDef {
    char pad[0x28];
    unsigned long f28;
    void __fastcall Copy(int /*edx*/, CDefClassBase const *src);
};

static int g_baseCalled = 0;
void CDefBase::Inherit(CDefClassBase const *src) { g_baseCalled = 1; }

void __fastcall CCoinGameObstacleDef::Copy(int /*edx*/, CDefClassBase const *src)
{
    ((CDefBase *)this)->Inherit(src);
    this->f28 = ((CCoinGameObstacleDef const *)src)->f28;
}

int main()
{
    CCoinGameObstacleDef a, b;
    a.f28 = 0;
    b.f28 = 0xDEADBEEF;
    a.Copy(0, (CDefClassBase const *)&b);
    if (g_baseCalled == 1 && a.f28 == 0xDEADBEEF)
        printf("004e10a8_TEST PASS\n");
    else
        printf("FAIL %d %08lx\n", g_baseCalled, a.f28);
    return 0;
}