#include <cstdio>
struct CWideString {
    void* p;
    CWideString(const char* s, int len){ p=(void*)s; }
    CWideString(const char* s){ p=(void*)s; }
    ~CWideString(){}
};
struct SaveSub { CWideString Format(CWideString* in){ return CWideString((const char*)0xF07,0); } };
struct SaveMgr { char pad[0x14]; SaveSub* f14; };
SaveMgr* g_saveMgr;
struct CUserProfileManager { CWideString GetEmptySlotName(); };
CWideString CUserProfileManager::GetEmptySlotName(){
    SaveMgr* mgr=g_saveMgr;
    if(mgr){ CWideString temp((const char*)0x122df80,-1); return mgr->f14->Format(&temp); }
    return CWideString((const char*)0x122df5c);
}
int main(){
    CUserProfileManager u;
    g_saveMgr=0;
    CWideString r1=u.GetEmptySlotName();          // null -> literal factory
    static SaveSub ss; static SaveMgr mgr; SaveSub* psub=&ss; mgr.f14=psub; g_saveMgr=&mgr;
    CWideString r2=u.GetEmptySlotName();          // non-null -> Format path
    if(r1.p==(void*)0x122df5c && r2.p==(void*)0xF07){ printf("EMPTYSLOT_OK\n"); return 0; }
    printf("FAIL r1=%p r2=%p\n", r1.p, r2.p); return 1;
}