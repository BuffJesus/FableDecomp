// CModeSitBase::RemoveModeWithAction @ 008d7250
// void __fastcall(CModeSitBase* self, CCreatureActionBase* action)

struct CCreatureActionBase
{
    virtual int v0();
    virtual int v1();
    virtual int v2();
    virtual int v3();
    virtual int v4();
    virtual int v5();
    virtual int v6();
    virtual int v7();
    virtual int v8();
    virtual int v9();
    virtual int v10();
    virtual int GetActionType();   // slot 11 -> call [eax+0x2c]
};

// sub_668d90 : __fastcall member on the subobject at self+0x2c, takes one int arg.
// Defined out-of-line elsewhere so the compiler emits a direct CALL (not inlined).
struct CModeList
{
    void Remove(int key);
    int junk;
};

struct CModeSitBase
{
    char pad[0x2c];
    CModeList list;   // at +0x2c
    void RemoveModeWithAction(CCreatureActionBase* action);
};

void CModeSitBase::RemoveModeWithAction(CCreatureActionBase* action)
{
    this->list.Remove(action->GetActionType());
}