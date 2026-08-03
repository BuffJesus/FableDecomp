#include <cstdio>

struct C3DVector { float x; float y; float z; };

static C3DVector g_seenPos = {0,0,0};
static float g_seenMag = 0.0f, g_seenDur = 0.0f;
static bool g_seenOneShot = true;
static int g_called = 0;

struct CCamObj {
    char body[4];
    void SetEarthquakeAt(C3DVector* pos, float magnitude, float duration, bool oneShot);
};

void CCamObj::SetEarthquakeAt(C3DVector* pos, float magnitude, float duration, bool oneShot)
{
    g_called++;
    g_seenPos = *pos;
    g_seenMag = magnitude;
    g_seenDur = duration;
    g_seenOneShot = oneShot;
}

static CCamObj g_obj;

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
    virtual bool GetObj(CCamObj** out){ *out = &g_obj; return true; }
};

struct COuter { char pad[0x38]; };
struct CGameScriptInterface { char pad4[4]; COuter* outer; };

void __fastcall CGameScriptInterface_CameraShake(const CGameScriptInterface* self, float magnitude, float duration)
{
    CCamObj* local;
    COuter* a = self->outer;
    CCamMgr* b = *(CCamMgr**)((char*)a + 0x34);
    if (b->GetObj(&local)) {
        local->SetEarthquakeAt((C3DVector*)((char*)local + 4), magnitude, duration, false);
    }
}

int main(){
    static char blob[0x40];
    CCamMgr mgr;
    *(CCamMgr**)(blob + 0x34) = &mgr;
    CGameScriptInterface gsi; gsi.outer = (COuter*)blob;

    CGameScriptInterface_CameraShake(&gsi, 2.5f, 4.0f);

    if (g_called == 1 && g_seenMag == 2.5f && g_seenDur == 4.0f && g_seenOneShot == false) {
        std::printf("CGameScriptInterface_0088ece0_TEST PASS\n");
        return 0;
    }
    std::printf("FAIL called=%d mag=%f dur=%f one=%d\n", g_called, g_seenMag, g_seenDur, (int)g_seenOneShot);
    return 1;
}