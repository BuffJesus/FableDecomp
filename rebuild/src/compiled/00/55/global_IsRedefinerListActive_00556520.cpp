struct RedefinerMgr { char pad[0x17c]; char active; };
extern RedefinerMgr* g_redefinerMgr;
char IsRedefinerListActive() {
    RedefinerMgr* m = g_redefinerMgr;
    if (m) return m->active;
    return 0;
}