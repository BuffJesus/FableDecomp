#include <stdio.h>
static int g_notify = 0;
static int g_release = 0;
static void *g_last = 0;
extern "C" void NotifyBeforeDie(void) { ++g_notify; }
extern "C" void ReleaseMapwho(void *p) { ++g_release; g_last = p; }
extern "C" void OnDie(void *self) { if (self) { NotifyBeforeDie(); ReleaseMapwho(self); } }
int main() {
    int obj = 3;
    OnDie(0);
    if (g_notify != 0 || g_release != 0) { printf("FAIL null path\n"); return 1; }
    OnDie(&obj);
    if (g_notify != 1 || g_release != 1 || g_last != &obj) { printf("FAIL live path\n"); return 1; }
    printf("PASS_00445350\n");
    return 0;
}