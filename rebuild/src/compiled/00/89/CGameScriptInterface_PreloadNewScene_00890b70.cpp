// CGameScriptInterface::PreloadNewScene  @ 0x00890b70
// Reads a global manager, hops two pointers to reach a scene object,
// then makes a virtual call (vtable slot 0xdc/4 = 55) with args (4, param).
// this = the scene object (passed in ecx).

struct SceneTarget
{
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void PreloadScene(int mode, float param);  // slot 55 (offset 0xdc)
};

struct SceneMidHolder
{
    char pad[0x2c];
    SceneTarget* target;   // +0x2c
};

struct SceneRootHolder
{
    char pad[0x28];
    SceneMidHolder* mid;   // +0x28
};

// The global manager pointer at 0x013b86a0.
extern SceneRootHolder* g_sceneRoot;

struct CGameScriptInterface
{
    void PreloadNewScene(float param);
};

void CGameScriptInterface::PreloadNewScene(float param)
{
    SceneTarget* t = g_sceneRoot->mid->target;
    t->PreloadScene(4, param);
}