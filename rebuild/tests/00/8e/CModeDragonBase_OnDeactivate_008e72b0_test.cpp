#include <cstdio>

struct Inner
{
    virtual void s00(){} virtual void s01(){} virtual void s02(){} virtual void s03(){}
    virtual void s04(){} virtual void s05(){} virtual void s06(){} virtual void s07(){}
    virtual void s08(){} virtual void s09(){} virtual void s10(){} virtual void s11(){}
    virtual void s12(){} virtual void s13(){} virtual void s14(){} virtual void s15(){}
    virtual void s16(){} virtual void s17(){} virtual void s18(){} virtual void s19(){}
    virtual void s20(){} virtual void s21(){} virtual void s22(){} virtual void s23(){}
    virtual void s24(){} virtual void s25(){} virtual void s26(){} virtual void s27(){}
    virtual void s28(){} virtual void s29(){} virtual void s30(){} virtual void s31(){}
    virtual void s32(){} virtual void s33(){} virtual void s34(){} virtual void s35(){}
    virtual void s36(){} virtual void s37(){}
    int gotFlag;
    Inner():gotFlag(-999){}
    virtual void Deactivate(int flag){ gotFlag = flag; }
};

struct Holder { char pad0[0x60]; Inner* inner; };

struct CModeDragonBase
{
    void* pad0;
    Holder* holder;
    char pad1[0x10];
    char active;
};

void __fastcall CModeDragonBase_OnDeactivate(CModeDragonBase* self)
{
    self->holder->inner->Deactivate(1);
    self->active = 0;
}

int main()
{
    Inner inner;
    Holder holder; holder.inner = &inner;
    CModeDragonBase m;
    m.holder = &holder;
    m.active = 7;

    CModeDragonBase_OnDeactivate(&m);

    if (inner.gotFlag == 1 && m.active == 0)
        printf("DRAGON_DEACTIVATE_OK\n");
    else
        printf("FAIL flag=%d active=%d\n", inner.gotFlag, (int)m.active);
    return 0;
}