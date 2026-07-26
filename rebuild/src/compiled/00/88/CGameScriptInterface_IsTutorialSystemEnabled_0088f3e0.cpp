struct TutMgr { char pad0[0xc]; char flagC; char pad1[0x17-0xd]; char flag17; char pad2[0xd7-0x18]; char flagD7; };
extern "C" TutMgr* __cdecl GetTutMgr(void);

char __fastcall CGameScriptInterface_IsTutorialSystemEnabled(void* self)
{
    TutMgr* p = GetTutMgr();
    if (p->flagC)
        return p->flagD7;
    return p->flag17;
}