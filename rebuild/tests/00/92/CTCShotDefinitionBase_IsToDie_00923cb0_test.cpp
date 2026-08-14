#include <cstdio>

struct CTCShotDefinitionBase {
    char pad[0x34];
    int m_dieTime; // +0x34
};

static int g_val = 0;
int GetCurrentValue_49d870(void) { return g_val; }

static bool IsToDie(CTCShotDefinitionBase* self)
{
    int t = self->m_dieTime;
    if (t != 0) {
        if (GetCurrentValue_49d870() > self->m_dieTime)
            return true;
    }
    return false;
}

int main()
{
    CTCShotDefinitionBase s;
    // dieTime == 0 -> false regardless
    s.m_dieTime = 0; g_val = 100;
    if (IsToDie(&s)) { printf("FAIL1\n"); return 1; }
    // dieTime set, val <= dieTime -> false
    s.m_dieTime = 50; g_val = 50;
    if (IsToDie(&s)) { printf("FAIL2\n"); return 1; }
    g_val = 49;
    if (IsToDie(&s)) { printf("FAIL3\n"); return 1; }
    // val > dieTime -> true
    g_val = 51;
    if (!IsToDie(&s)) { printf("FAIL4\n"); return 1; }
    printf("BEHAVIOR_OK\n");
    return 0;
}