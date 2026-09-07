#include <stdio.h>
static int g_result;
static int g_calls;
static bool g_argument;
struct CTCStealth { int GetAttackAvailability(bool includePending); bool IsAvailableForAttack(); };
int CTCStealth::GetAttackAvailability(bool value) { ++g_calls; g_argument = value; return g_result; }
bool CTCStealth::IsAvailableForAttack() { return GetAttackAvailability(false) == -1; }
int main() {
    CTCStealth stealth;
    g_result = -1; if (!stealth.IsAvailableForAttack()) return 1;
    g_result = 0; if (stealth.IsAvailableForAttack()) return 1;
    g_result = -2; if (stealth.IsAvailableForAttack()) return 1;
    if (g_calls != 3 || g_argument) return 1;
    printf("STEALTH_ATTACK_AVAILABILITY PASS\n"); return 0;
}