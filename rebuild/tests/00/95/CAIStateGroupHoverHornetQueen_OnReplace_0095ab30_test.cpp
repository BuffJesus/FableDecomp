#include <cstdio>

struct Sub50 {
    void Method(unsigned int a, unsigned int b);
};

struct CAIStateGroup_HoverHornetQueen {
    char pad[0x50];
    Sub50 m50;
    unsigned int m54;
    void OnReplaceBase();
    void OnReplace();
};

static int g_base = 0;
static unsigned int g_a = 0, g_b = 0;

void CAIStateGroup_HoverHornetQueen::OnReplaceBase() { g_base = 1; }
void Sub50::Method(unsigned int a, unsigned int b) { g_a = a; g_b = b; }

void CAIStateGroup_HoverHornetQueen::OnReplace()
{
    OnReplaceBase();
    unsigned int a = *(unsigned int*)((char*)this + 0x50);
    unsigned int b = *(unsigned int*)((char*)this + 0x54);
    m50.Method(a, b);
}

int main()
{
    CAIStateGroup_HoverHornetQueen o;
    *(unsigned int*)((char*)&o + 0x50) = 0x11112222u;
    o.m54 = 0x33334444u;
    o.OnReplace();
    if (g_base == 1 && g_a == 0x11112222u && g_b == 0x33334444u)
        printf("HORNETQUEEN_ONREPLACE_OK\n");
    else
        printf("FAIL b=%d a=%08x b=%08x\n", g_base, g_a, g_b);
    return (g_base == 1 && g_a == 0x11112222u && g_b == 0x33334444u) ? 0 : 1;
}