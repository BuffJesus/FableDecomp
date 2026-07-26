struct CamObj {
    virtual void s0();
    virtual void s1();
    virtual void s2();
    virtual bool onScreen(void* pos); // slot index 3 => 0xc
};

struct MidObj {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual void v9();
    virtual bool getCam(CamObj** out); // slot index 10 => 0x28
    unsigned char _pad[0x30]; // vptr at +0, then pad to +0x34
    MidObj* m34;               // +0x34
};

struct CGameScriptInterface {
    void* vt;      // +0x0
    MidObj* mid;   // +0x4
};

bool __fastcall CGameScriptInterface_IsCameraPosOnScreen(CGameScriptInterface* self, int edx_unused, void* pos)
{
    CamObj* cam;
    MidObj* m = self->mid->m34;
    if (m->getCam(&cam)) {
        if (cam->onScreen(pos))
            return true;
    }
    return false;
}