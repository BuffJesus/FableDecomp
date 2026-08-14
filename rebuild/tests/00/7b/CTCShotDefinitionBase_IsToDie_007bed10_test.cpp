#include <cstdio>

struct Shot { char pad[0x60]; int TimeToDie; };

static int g_now = 0;
extern "C" int __cdecl GetCurrentGameTime(void) { return g_now; }

int __fastcall IsToDie(Shot *self)
{
    int t = self->TimeToDie;
    if (t > 0) {
        if (GetCurrentGameTime() > self->TimeToDie)
            return 1;
    }
    return 0;
}

int main()
{
    Shot s;
    bool ok = true;

    // TimeToDie <= 0 -> false, no call effect
    s.TimeToDie = 0;  g_now = 100; if (IsToDie(&s) != 0) ok = false;
    s.TimeToDie = -5; g_now = 100; if (IsToDie(&s) != 0) ok = false;

    // TimeToDie > 0, now > TimeToDie -> true
    s.TimeToDie = 10; g_now = 20;  if (IsToDie(&s) != 1)  ok = false;

    // TimeToDie > 0, now == TimeToDie -> false (jle)
    s.TimeToDie = 10; g_now = 10;  if (IsToDie(&s) != 0) ok = false;

    // TimeToDie > 0, now < TimeToDie -> false
    s.TimeToDie = 10; g_now = 5;   if (IsToDie(&s) != 0) ok = false;

    if (ok) printf("BEHAVIOR_PASS\n");
    else    printf("BEHAVIOR_FAIL\n");
    return 0;
}