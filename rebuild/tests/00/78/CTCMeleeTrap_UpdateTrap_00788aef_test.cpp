#include <cstdio>

struct SubObj {
    char pad[0x44];
    float f44;
};

void __stdcall MeleeTrapHelper(float a, float b);

struct CTCMeleeTrap {
    char pad[0x54];
    SubObj* m54;
    void UpdateTrap();
};

float g_someConst = 2.5f;

static float g_a = 0.0f, g_b = 0.0f;

void __stdcall MeleeTrapHelper(float a, float b)
{
    g_a = a;
    g_b = b;
}

int main()
{
    SubObj s;
    s.f44 = 7.5f;
    CTCMeleeTrap t;
    t.m54 = &s;
    t.UpdateTrap();
    if (g_a == 2.5f && g_b == 7.5f)
        printf("MELEETRAP_OK\n");
    else
        printf("FAIL a=%f b=%f\n", g_a, g_b);
    return (g_a == 2.5f && g_b == 7.5f) ? 0 : 1;
}