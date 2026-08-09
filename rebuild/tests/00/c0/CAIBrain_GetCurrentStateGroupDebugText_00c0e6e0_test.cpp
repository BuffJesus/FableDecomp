#include <cstdio>

struct CCharString {};
static CCharString g_dbg;

struct IState {
    virtual CCharString* GetName0() { return 0; }
    virtual CCharString* GetName1() { return 0; }
    virtual CCharString* GetDebugText() { return &g_dbg; }
};

struct CAIBrain {
    void*   field0;
    IState* state;
};

CCharString* __fastcall GetCurrentStateGroupDebugText(CAIBrain* self)
{
    IState* s = self->state;
    if (s == 0)
        return (CCharString*)1;
    return s->GetDebugText();
}

int main()
{
    // null state -> returns 1
    CAIBrain a; a.field0 = 0; a.state = 0;
    CCharString* r1 = GetCurrentStateGroupDebugText(&a);
    if (r1 != (CCharString*)1) { printf("FAIL null\n"); return 1; }

    // non-null state -> forwards to slot 2
    IState st;
    CAIBrain b; b.field0 = 0; b.state = &st;
    CCharString* r2 = GetCurrentStateGroupDebugText(&b);
    if (r2 != &g_dbg) { printf("FAIL fwd\n"); return 2; }

    printf("OK_00c0e6e0\n");
    return 0;
}