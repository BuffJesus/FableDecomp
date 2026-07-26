#include <cstdio>
struct CCamObj { int v; };
bool __fastcall GetCameraScriptedMode(CCamObj* self){ return self && self->v==7; }

struct CCamMgr {
    virtual void v0(){}
    virtual void v1(){}
    virtual void v2(){}
    virtual void v3(){}
    virtual void v4(){}
    virtual void v5(){}
    virtual void v6(){}
    virtual void v7(){}
    virtual void v8(){}
    virtual void GetObj(CCamObj** out);
};
static CCamObj g_obj = {7};
void CCamMgr::GetObj(CCamObj** out){ *out = &g_obj; }

struct COuter { char pad[0x38]; };
struct CGameScriptInterface { char pad4[4]; COuter* outer; };

bool __fastcall CGameScriptInterface_IsCameraInScriptedMode(CGameScriptInterface* self)
{
    CCamObj* local;
    COuter* a = self->outer;
    CCamMgr* b = *(CCamMgr**)((char*)a + 0x34);
    b->GetObj(&local);
    return GetCameraScriptedMode(local);
}

int main(){
    static char blob[0x40];
    CCamMgr mgr;
    *(CCamMgr**)(blob + 0x34) = &mgr;
    CGameScriptInterface gsi; gsi.outer = (COuter*)blob;
    bool r = CGameScriptInterface_IsCameraInScriptedMode(&gsi);
    if (r) { std::printf("CGameScriptInterface_0088ecc0_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}