#include <cstdio>

static int g_slot = 0;
static int g_arg = -1;
static int g_base = 0;

struct VObj {
    virtual void v00(){} virtual void v01(){} virtual void v02(){} virtual void v03(){}
    virtual void v04(){} virtual void v05(){} virtual void v06(){} virtual void v07(){}
    virtual void v08(){} virtual void v09(){} virtual void v10(){} virtual void v11(){}
    virtual void v12(){} virtual void v13(){} virtual void v14(){} virtual void v15(){}
    virtual void v16(){} virtual void v17(){} virtual void v18(){} virtual void v19(){}
    virtual void v20(){} virtual void v21(){} virtual void v22(){} virtual void v23(){}
    virtual void v24(){} virtual void v25(){} virtual void v26(){} virtual void v27(){}
    virtual void v28(){} virtual void v29(){} virtual void v30(){} virtual void v31(){}
    virtual void v32(){} virtual void v33(){} virtual void v34(){} virtual void v35(){}
    virtual void v36(){} virtual void v37(){}
    virtual void slot38(int a){ g_slot = 1; g_arg = a; }
};

struct Mid {
    char pad[0x60];
    VObj* obj;
};

struct CModeBase {
    void* vfp;
    void OnActivate();
};

struct CModeDragonBase : CModeBase {
    Mid* f4;
    void OnActivate();
};

void CModeBase::OnActivate(){ g_base = 1; }

void CModeDragonBase::OnActivate()
{
    this->f4->obj->slot38(0);
    CModeBase::OnActivate();
}

int main()
{
    VObj o;
    Mid m; m.obj = &o;
    CModeDragonBase c; c.vfp = 0; c.f4 = &m;
    c.OnActivate();
    if (g_slot==1 && g_arg==0 && g_base==1)
        printf("DRAGON_ONACTIVATE_OK\n");
    else
        printf("FAIL\n");
    return (g_slot==1 && g_arg==0 && g_base==1) ? 0 : 1;
}