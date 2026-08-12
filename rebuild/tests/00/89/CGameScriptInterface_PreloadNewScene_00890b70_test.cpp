#include <cstdio>

static int g_gotMode = -1;
static float g_gotParam = 0.0f;
static void* g_gotSelf = 0;

struct SceneTarget
{
    virtual void v00() {} virtual void v01() {} virtual void v02() {}
    virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {}
    virtual void v15() {} virtual void v16() {} virtual void v17() {}
    virtual void v18() {} virtual void v19() {} virtual void v20() {}
    virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {}
    virtual void v27() {} virtual void v28() {} virtual void v29() {}
    virtual void v30() {} virtual void v31() {} virtual void v32() {}
    virtual void v33() {} virtual void v34() {} virtual void v35() {}
    virtual void v36() {} virtual void v37() {} virtual void v38() {}
    virtual void v39() {} virtual void v40() {} virtual void v41() {}
    virtual void v42() {} virtual void v43() {} virtual void v44() {}
    virtual void v45() {} virtual void v46() {} virtual void v47() {}
    virtual void v48() {} virtual void v49() {} virtual void v50() {}
    virtual void v51() {} virtual void v52() {} virtual void v53() {}
    virtual void v54() {}
    virtual void PreloadScene(int mode, float param)
    {
        g_gotSelf = this;
        g_gotMode = mode;
        g_gotParam = param;
    }
};

struct SceneMidHolder
{
    char pad[0x2c];
    SceneTarget* target;
};

struct SceneRootHolder
{
    char pad[0x28];
    SceneMidHolder* mid;
};

SceneRootHolder* g_sceneRoot;

struct CGameScriptInterface
{
    void PreloadNewScene(float param);
};

void CGameScriptInterface::PreloadNewScene(float param)
{
    SceneTarget* t = g_sceneRoot->mid->target;
    t->PreloadScene(4, param);
}

int main()
{
    static SceneTarget tgt;
    SceneTarget* ptgt = &tgt;
    static SceneMidHolder midh;
    midh.target = ptgt;
    SceneMidHolder* pmidh = &midh;
    static SceneRootHolder rooth;
    rooth.mid = pmidh;
    g_sceneRoot = &rooth;

    CGameScriptInterface* gsi = (CGameScriptInterface*)0x12345678;
    gsi->PreloadNewScene(2.5f);

    bool ok = (g_gotSelf == (void*)ptgt) && (g_gotMode == 4) && (g_gotParam == 2.5f);
    if (ok)
        printf("PRELOAD_SCENE_OK\n");
    else
        printf("PRELOAD_SCENE_FAIL self=%p mode=%d param=%f\n", g_gotSelf, g_gotMode, g_gotParam);
    return ok ? 0 : 1;
}