struct CManager;

struct CGSI {
    void GSI_PreMsg();
    void GSI_PostMsg();
};
struct CMgr {
    void* Find(int* pKey);
    void Handle(void* pObj);
};

bool __fastcall CGameScriptInterface_MsgOnChestOpeningCancelled(CGSI* self)
{
    self->GSI_PreMsg();
    self->GSI_PostMsg();
    CMgr* mgr = *(CMgr**)((char*)(*(void**)((char*)self + 4)) + 0x60);
    int key = 0x48;
    void* obj = mgr->Find(&key);
    if (obj) {
        mgr->Handle(obj);
        return true;
    }
    return false;
}