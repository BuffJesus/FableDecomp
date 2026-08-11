// CAnimComponentCombatFlourishTargetOffset::Clone  0x005db3b0
// virtual CAnimComponentBase* Clone() const;

#pragma pack(push,1)
struct CAnimComponentBase
{
    // vtable slot 4 (offset 0x10) = a virtual returning a fresh base object ptr
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual CAnimComponentBase* v4();   // slot 4 -> [vptr+0x10]
    // fields
    int    f4;      // +0x04 (base-class members preceding this component's data)
    char   f8;      // +0x08
    int    f9;      // +0x09  (unaligned dword copy)
    int    fd;      // +0x0d  (unaligned dword copy)
};
#pragma pack(pop)

struct CAnimComponentCombatFlourishTargetOffset : public CAnimComponentBase
{
    CAnimComponentBase* Clone() const;
};

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