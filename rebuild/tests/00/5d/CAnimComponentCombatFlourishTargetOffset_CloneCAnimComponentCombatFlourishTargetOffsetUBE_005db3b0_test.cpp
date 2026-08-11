#include <stdio.h>

#pragma pack(push,1)
struct CAnimComponentBase
{
    virtual void v0() {}
    virtual void v1() {}
    virtual void v2() {}
    virtual void v3() {}
    virtual CAnimComponentBase* v4();
    int    f4;
    char   f8;
    int    f9;
    int    fd;
};
#pragma pack(pop)

struct CAnimComponentCombatFlourishTargetOffset : public CAnimComponentBase
{
    CAnimComponentBase* Clone() const;
};

static CAnimComponentBase g_dst;

CAnimComponentBase* CAnimComponentBase::v4()
{
    // return a fresh (blank) base to be filled by Clone
    g_dst.f8 = 0;
    g_dst.f9 = 0;
    g_dst.fd = 0;
    return &g_dst;
}

CAnimComponentBase* CAnimComponentCombatFlourishTargetOffset::Clone() const
{
    CAnimComponentCombatFlourishTargetOffset* self =
        (CAnimComponentCombatFlourishTargetOffset*)this;
    CAnimComponentBase* dst = self->v4();
    dst->f8 = self->f8;
    dst->f9 = self->f9;
    dst->fd = self->fd;
    return dst;
}

int main()
{
    CAnimComponentCombatFlourishTargetOffset src;
    src.f8 = (char)0x5A;
    src.f9 = 0x11223344;
    src.fd = 0x55667788;

    CAnimComponentBase* c = src.Clone();

    if (c->f8 == (char)0x5A && c->f9 == 0x11223344 && c->fd == 0x55667788)
        printf("CLONE_OFFSET_OK\n");
    else
        printf("CLONE_OFFSET_FAIL\n");
    return 0;
}