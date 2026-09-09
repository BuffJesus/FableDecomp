#include <stdio.h>
#include <string.h>
#include "engine/CTCHeroStats.h"

static bool g_states[5];
static int g_calls;

bool CTCHeroStats::HasMarriageInState(EMarriageState state) const
{
    ++g_calls;
    return g_states[state];
}

int main()
{
    CTCHeroStats stats;
    memset(&stats, 0, sizeof(stats));
    memset(g_states, 0, sizeof(g_states));

    g_calls = 0;
    if (stats.HasCurrentMarriage()) return 1;
    if (g_calls != 4) return 2;

    g_states[3] = true;
    g_calls = 0;
    if (!stats.HasCurrentMarriage()) return 3;
    if (g_calls != 3) return 4;

    printf("CURRENT_MARRIAGE PASS\n");
    return 0;
}