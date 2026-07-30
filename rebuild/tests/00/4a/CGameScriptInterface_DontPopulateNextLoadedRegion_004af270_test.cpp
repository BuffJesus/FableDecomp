#include <cstdio>
#include <cstring>
struct RegionMgr { char pad[0xda]; unsigned char dontPopulateNext; };
struct CGameScriptInterface { char pad[0x7c]; RegionMgr* mgr; };
void __fastcall DontPopulateNextLoadedRegion(CGameScriptInterface* self)
{
    self->mgr->dontPopulateNext = 1;
}
int main()
{
    RegionMgr mgr;
    memset(&mgr, 0, sizeof(mgr));
    mgr.dontPopulateNext = 0;
    CGameScriptInterface obj;
    memset(&obj, 0, sizeof(obj));
    obj.mgr = &mgr;
    DontPopulateNextLoadedRegion(&obj);
    if (mgr.dontPopulateNext == 1) {
        printf("OK_0x004af270\n");
        return 0;
    }
    printf("BAD\n");
    return 1;
}