struct C3DVector { float x; float y; float z; };

struct CCamObj {
    char body[4]; // C3DVector at +4
    void SetEarthquakeAt(C3DVector* pos, float magnitude, float duration, bool oneShot);
};

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
    virtual bool GetObj(CCamObj** out); // slot 9 -> vtbl offset 0x24
};

struct COuter { char pad[0x38]; }; // CCamMgr* at +0x34
struct CGameScriptInterface { char pad4[4]; COuter* outer; }; // outer at +0x4

void __fastcall CGameScriptInterface_CameraShake(const CGameScriptInterface* self, float magnitude, float duration)
{
    CCamObj* local;
    COuter* a = self->outer;
    CCamMgr* b = *(CCamMgr**)((char*)a + 0x34);
    if (b->GetObj(&local)) {
        local->SetEarthquakeAt((C3DVector*)((char*)local + 4), magnitude, duration, false);
    }
}