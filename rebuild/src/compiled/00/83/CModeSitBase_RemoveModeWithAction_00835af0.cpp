// CModeSitBase::RemoveModeWithAction  (retail 0x00835af0)

struct CCreatureActionBase {
    virtual void v0();
    virtual int v1();   // slot 1: [eax+4], returns int
};

struct CSubObject {
    void RemoveByKey(int key);   // thiscall at this+0xd0 -> 0x836580
};

struct CModeSitBase {
    char pad[0xd0];
    CSubObject sub;   // at +0xd0
    void RemoveModeWithAction(CCreatureActionBase* action);
};

void CModeSitBase::RemoveModeWithAction(CCreatureActionBase* action)
{
    this->sub.RemoveByKey(action->v1());
}