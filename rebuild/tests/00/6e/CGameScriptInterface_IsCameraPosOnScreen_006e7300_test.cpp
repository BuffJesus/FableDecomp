#include <cstdio>

static bool g_camHasCam = true;
static bool g_onScreen = true;

struct CamObj {
    virtual void s0() {}
    virtual void s1() {}
    virtual void s2() {}
    virtual bool onScreen(void* pos) { return g_onScreen; }
};

struct MidObj {
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual void v4() {}
    virtual void v5() {}
    virtual void v6() {}
    virtual void v7() {}
    virtual void v8() {}
    virtual void v9() {}
    virtual bool getCam(CamObj** out);
    unsigned char _pad[0x30];
    MidObj* m34;
};

static CamObj g_cam;
bool MidObj::getCam(CamObj** out) { *out = &g_cam; return g_camHasCam; }

struct CGameScriptInterface {
    void* vt;
    MidObj* mid;
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

int main() {
    MidObj inner;
    inner.m34 = &inner;
    CGameScriptInterface gsi; gsi.mid = &inner;

    int dummy;

    g_camHasCam = true; g_onScreen = true;
    if (CGameScriptInterface_IsCameraPosOnScreen(&gsi, 0, &dummy) != true) { std::printf("FAIL both true\n"); return 1; }

    g_camHasCam = true; g_onScreen = false;
    if (CGameScriptInterface_IsCameraPosOnScreen(&gsi, 0, &dummy) != false) { std::printf("FAIL onscreen false\n"); return 1; }

    g_camHasCam = false; g_onScreen = true;
    if (CGameScriptInterface_IsCameraPosOnScreen(&gsi, 0, &dummy) != false) { std::printf("FAIL nocam\n"); return 1; }

    std::printf("CGameScriptInterface_006e7300_TEST PASS\n");
    return 0;
}