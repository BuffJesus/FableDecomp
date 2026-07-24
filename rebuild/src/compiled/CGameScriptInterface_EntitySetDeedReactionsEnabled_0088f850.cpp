// CGameScriptInterface::EntitySetDeedReactionsEnabled @ 0088f850
struct Local {
    int a;
    Local(int x, int y);
    ~Local();
};

struct CGameScriptInterface;
typedef void (__fastcall *PFN)(CGameScriptInterface*, Local*, int, Local*, int);
struct VT { PFN slots[0x300]; };
struct CGameScriptInterface { VT* vt; };

void __fastcall CGameScriptInterface_EntitySetDeedReactionsEnabled(CGameScriptInterface* self, int, int entity, int enabled)
{
    Local loc(0x1275988, -1);
    self->vt->slots[0x938/4](self, &loc, entity, &loc, enabled);
}