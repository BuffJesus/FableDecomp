#include <cstdio>
#include <cmath>

struct B {
    char pad[8];
    float f8;
};

static B g_b;

struct A {
    B* GetB() { return &g_b; }
};

static A g_a;

struct CTCLook {
    char pad[0xb0];
    float fB0;

    A* GetA() { return &g_a; }

    float UpdateLookTimer()
    {
        A* a = this->GetA();
        B* b = a->GetB();
        return b->f8 + this->fB0;
    }
};

int main()
{
    CTCLook look;
    look.fB0 = 1.5f;
    g_b.f8 = 2.25f;

    float r = look.UpdateLookTimer();
    if (std::fabs(r - 3.75f) < 0.0001f) {
        std::printf("LOOKTIMER_OK r=%f\n", r);
        return 0;
    }
    std::printf("LOOKTIMER_FAIL r=%f\n", r);
    return 1;
}