// CCreatureActionBase::IsPlayingAnimation  (retail 0x00692850)
// bool __fastcall f(CCreatureActionBase* this)
// return this->m_1c >= (this->vslot6() + this->m_48)

class CCreatureActionBase
{
public:
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual int  v6();               // vtable slot 6 = [vptr+0x18]
    // members
    char pad_04[0x1c - 0x04];        // +0x04 .. +0x1b
    int  m_1c;                       // +0x1c
    char pad_20[0x48 - 0x20];        // +0x20 .. +0x47
    int  m_48;                       // +0x48
public:
    virtual bool IsPlayingAnimation() const;
};

bool CCreatureActionBase::IsPlayingAnimation() const
{
    int end = ((CCreatureActionBase*)this)->v6() + m_48;
    return m_1c >= end;
}