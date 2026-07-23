struct CCamObj;
extern bool __fastcall GetCameraScriptedMode(CCamObj* self);

struct CCamMgr {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void GetObj(CCamObj** out); // slot 9 -> vtbl offset 0x24
};

struct COuter { char pad[0x38]; }; // CCamMgr* at +0x34
struct CGameScriptInterface { char pad4[4]; COuter* outer; }; // outer at +0x4

bool __fastcall CGameScriptInterface_IsCameraInScriptedMode(CGameScriptInterface* self)
{
    CCamObj* local;
    COuter* a = self->outer;
    CCamMgr* b = *(CCamMgr**)((char*)a + 0x34);
    b->GetObj(&local);
    return GetCameraScriptedMode(local);
}