struct CCharString;
struct CRegions;

extern int __fastcall CRegions_GetCount(CRegions* r);
extern void* __fastcall CRegions_GetEntry(CRegions* r);

// The accessor lives at vtable slot 13 (byte offset 0x34). It is used in two
// overloaded forms sharing that slot: a no-arg form returning the collection and
// an int form returning an element. Two view-classes expose the slot each way so
// the compiler emits a direct virtual dispatch (call dword ptr [vtbl+0x34]).
struct CMidNoArg {
    virtual void* v0(); virtual void* v1(); virtual void* v2(); virtual void* v3();
    virtual void* v4(); virtual void* v5(); virtual void* v6(); virtual void* v7();
    virtual void* v8(); virtual void* v9(); virtual void* v10(); virtual void* v11();
    virtual void* v12();
    virtual CRegions* GetRegions();          // slot 13
};
struct CMidArg {
    virtual void* v0(); virtual void* v1(); virtual void* v2(); virtual void* v3();
    virtual void* v4(); virtual void* v5(); virtual void* v6(); virtual void* v7();
    virtual void* v8(); virtual void* v9(); virtual void* v10(); virtual void* v11();
    virtual void* v12();
    virtual CRegions* GetRegions(int n);     // slot 13
};

struct CGameScriptInterface {
    void* vt;      // +0x0
    void* mid;     // +0x4  (CMid*)
};

const CCharString& __fastcall CGameScriptInterface_GetRegionName(CGameScriptInterface* self)
{
    CMidNoArg* m = (CMidNoArg*)self->mid;
    CRegions* col = m->GetRegions();
    int n = CRegions_GetCount(col);
    if (n > 0) {
        CMidArg* m2 = (CMidArg*)self->mid;
        CRegions* e = m2->GetRegions(n);
        return *(const CCharString*)((char*)CRegions_GetEntry(e) + 0x18);
    }
    return *(const CCharString*)0x13bd804;
}