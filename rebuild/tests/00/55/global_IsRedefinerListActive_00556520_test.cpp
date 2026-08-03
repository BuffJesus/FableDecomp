#include <stdio.h>

struct RedefinerMgr { char pad[0x17c]; char active; };
RedefinerMgr* g_redefinerMgr = 0;
char IsRedefinerListActive() {
    RedefinerMgr* m = g_redefinerMgr;
    if (m) return m->active;
    return 0;
}

int main() {
    g_redefinerMgr = 0;
    if (IsRedefinerListActive() != 0) { printf("FAIL null\n"); return 1; }

    RedefinerMgr mgr;
    mgr.active = 0;
    g_redefinerMgr = &mgr;
    if (IsRedefinerListActive() != 0) { printf("FAIL inactive\n"); return 1; }

    mgr.active = 1;
    if (IsRedefinerListActive() != 1) { printf("FAIL active\n"); return 1; }

    mgr.active = (char)0x7f;
    if (IsRedefinerListActive() != (char)0x7f) { printf("FAIL field\n"); return 1; }

    printf("OK_0x00556520\n");
    return 0;
}