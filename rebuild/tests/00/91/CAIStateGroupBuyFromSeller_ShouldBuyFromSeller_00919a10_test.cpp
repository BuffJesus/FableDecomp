#include <cstdio>

int g_div = 10;
int g_cmp = 5;
int g_now = 0;

int GetTick() { return g_now; }

bool ShouldBuy(int field14)
{
    int elapsed = GetTick() - field14;
    int quot = elapsed / g_div;
    if ((unsigned int)quot > (unsigned int)g_cmp)
        return true;
    return false;
}

int main()
{
    // quot = (now - field14)/div compared > cmp (unsigned)
    // case 1: now=100 field14=0 div=10 -> quot=10 > 5 -> true
    g_now = 100; g_div = 10; g_cmp = 5;
    if (ShouldBuy(0) != true) { printf("FAIL1\n"); return 1; }
    // case 2: quot=3 > 5 -> false
    g_now = 30;
    if (ShouldBuy(0) != false) { printf("FAIL2\n"); return 1; }
    // case 3: exactly equal quot=5 > 5 -> false (seta strict)
    g_now = 50;
    if (ShouldBuy(0) != false) { printf("FAIL3\n"); return 1; }
    // case 4: quot=6 > 5 -> true
    g_now = 60;
    if (ShouldBuy(0) != true) { printf("FAIL4\n"); return 1; }
    printf("BEHAVIOR_OK\n");
    return 0;
}