#include <cstdio>

struct CThing;

struct CRenderManager {
    void Add(CThing* t);
};

struct CThing {
    virtual void v00(){} virtual void v01(){} virtual void v02(){}
    virtual void v03(){} virtual void v04(){} virtual void v05(){}
    virtual void v06(){} virtual void v07(){} virtual void v08(){}
    virtual void v09(){} virtual void v10(){} virtual void v11(){}
    virtual void v12(){} virtual void v13(){} virtual void v14(){}
    virtual void v15(){} virtual void v16(){} virtual void v17(){}
    virtual void v18(){} virtual void v19(){} virtual void v20(){}
    virtual void v21(){} virtual void v22(){} virtual void v23(){}
    virtual void v24(){} virtual void v25(){} virtual void v26(){}
    bool renderable;
    virtual bool IsRenderable(){ return renderable; }
    void AddToRenderList();
};

// global manager pointer + capture of what was added
static CRenderManager* g_mgrObj = 0;
static CThing* g_lastAdded = 0;
static CRenderManager** g_renderMgr = &g_mgrObj;

void CRenderManager::Add(CThing* t){ g_lastAdded = t; }

void CThing::AddToRenderList()
{
    if (this->IsRenderable())
        (*g_renderMgr)->Add(this);
}

int main()
{
    CRenderManager mgr;
    g_mgrObj = &mgr;

    CThing a; a.renderable = true;  g_lastAdded = 0;
    a.AddToRenderList();
    bool ok1 = (g_lastAdded == &a);

    CThing b; b.renderable = false; g_lastAdded = 0;
    b.AddToRenderList();
    bool ok2 = (g_lastAdded == 0);

    if (ok1 && ok2) { printf("RENDERLIST_OK\n"); return 0; }
    printf("RENDERLIST_FAIL\n"); return 1;
}