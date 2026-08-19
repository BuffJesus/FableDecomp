// CCreatureAction_Die::FinishAction  (retail 0x008ed830, __fastcall, void)

struct RetObj
{
    void Kill(bool destroy);      // call 0x4c9b80 (CThing::Kill, bool stack arg)
};

struct SubObj
{
    RetObj* GetPItem();           // call 0xa01b50 (thiscall on this+8)
};

struct Base
{
    virtual void FinishAction();  // call 0x694430 (base FinishAction on this)
};

struct CCreatureAction_Die : public Base
{
    int    filler;   // +4
    SubObj sub;      // +8

    virtual void FinishAction();
};

void CCreatureAction_Die::FinishAction()
{
    Base::FinishAction();
    this->sub.GetPItem()->Kill(true);
}