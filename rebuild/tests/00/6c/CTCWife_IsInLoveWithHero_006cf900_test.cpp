#include <stdio.h>

struct CTCWife {
    char pad0[0x10];
    int state;
    bool SomeCondition();
    int IsInLoveWithHero();
};

static bool g_cond = true;
bool CTCWife::SomeCondition() { return g_cond; }

int CTCWife::IsInLoveWithHero()
{
    if (this->SomeCondition())
    {
        if (this->state == 0xc)
            return 1;
    }
    return 0;
}

int main()
{
    CTCWife w;
    w.state = 0xc;
    g_cond = true;
    bool r1 = w.IsInLoveWithHero();

    w.state = 0xc;
    g_cond = false;
    bool r2 = w.IsInLoveWithHero();

    w.state = 5;
    g_cond = true;
    bool r3 = w.IsInLoveWithHero();

    if (r1 && !r2 && !r3) {
        printf("WIFE_LOVE_OK\n");
        return 0;
    }
    printf("WIFE_LOVE_FAIL\n");
    return 1;
}