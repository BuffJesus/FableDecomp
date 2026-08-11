#include <cstdio>

static int g_base = 0;
static int g_getp = 0;
static int g_killflag = -99;

struct RetObj
{
    void Kill(bool destroy);
};

struct SubObj
{
    RetObj* GetPItem();
};

struct Base { virtual void FinishAction(); };

static RetObj g_ret;

void RetObj::Kill(bool destroy)
{
    if (this == &g_ret) g_killflag = destroy ? 1 : 0;
}

RetObj* SubObj::GetPItem()
{
    g_getp = 1;
    return &g_ret;
}

void Base::FinishAction()
{
    g_base = 1;
}

struct CCreatureAction_Die : public Base
{
    int filler;
    SubObj sub;
    virtual void FinishAction();
};

void CCreatureAction_Die::FinishAction()
{
    Base::FinishAction();
    this->sub.GetPItem()->Kill(true);
}

int main()
{
    CCreatureAction_Die d;
    d.filler = 0;
    d.FinishAction();
    if (g_base == 1 && g_getp == 1 && g_killflag == 1)
        printf("DIE_FINISH_OK\n");
    else
        printf("DIE_FINISH_FAIL b=%d g=%d k=%d\n", g_base, g_getp, g_killflag);
    return (g_base == 1 && g_getp == 1 && g_killflag == 1) ? 0 : 1;
}