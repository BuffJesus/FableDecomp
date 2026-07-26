#include <cstdio>

struct C2DBoxF { float a, b, c, d; };

int g_calls = 0;

struct CEditControlCentre {
    char pad0[0x28c];
    unsigned char flagB;
    char pad1[0x294 - 0x28d];
    unsigned char flagA;

    void h1(int, int, int){ g_calls++; }
    void h2(int, int, C2DBoxF*){ g_calls++; }
    void h3(int, C2DBoxF*){ g_calls++; }
    void h4(int, int){ g_calls++; }
    void h5(int, int){ g_calls++; }
    bool GetCurrentMask(int p1, int p2, int p3, int p4);
};

bool CEditControlCentre::GetCurrentMask(int p1, int p2, int p3, int p4)
{
    __declspec(align(16)) char storage[0x1f4];
    C2DBoxF* box = (C2DBoxF*)storage;
    if (!this->flagA)
        return false;
    h1(p1, p2, p4);
    h2(p3, p4, box);
    h3(p4, box);
    h4(p3, p4);
    h5(p3, p4);
    this->flagB = (unsigned char)p2;
    return true;
}

int main()
{
    CEditControlCentre e;
    e.flagA = 0; e.flagB = 0; g_calls = 0;
    bool r0 = e.GetCurrentMask(1, 2, 3, 4);
    if (r0 != false || g_calls != 0) { std::printf("FAIL disabled path\n"); return 1; }

    e.flagA = 1; e.flagB = 0; g_calls = 0;
    bool r1 = e.GetCurrentMask(1, 0x77, 3, 4);
    if (r1 != true || g_calls != 5 || e.flagB != 0x77) { std::printf("FAIL enabled path\n"); return 1; }

    std::printf("CEditControlCentre_00c6a5d0_TEST PASS\n");
    return 0;
}