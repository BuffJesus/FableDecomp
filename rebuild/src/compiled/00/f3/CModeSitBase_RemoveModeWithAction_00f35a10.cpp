// CModeSitBase::RemoveModeWithAction  @ 0x00f35a10
// __fastcall void(CModeSitBase* this, CCreatureActionBase* action)
// this->field2c.RemoveMode( action->GetActionId() )
// virtual slot 1 (vtable+4) returns a value; passed to a thiscall on this+0x2c.

struct CCreatureActionBase {
    virtual void slot0();
    virtual int  slot1();   // vtable+4
};

struct ModeContainer2C {
    // subobject at this+0x2c; RemoveMode is thiscall @ 0x00f35aa0
    void RemoveMode(int actionId);
};

struct CModeSitBase {
    char pad[0x2c];
    ModeContainer2C container;   // at +0x2c
    void RemoveModeWithAction(CCreatureActionBase* action);
};

void CModeSitBase::RemoveModeWithAction(CCreatureActionBase* action)
{
    this->container.RemoveMode( action->slot1() );
}