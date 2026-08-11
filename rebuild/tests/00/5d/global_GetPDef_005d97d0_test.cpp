#include <cstdio>

struct RefTarget {
    void* vtbl_or_field0;
    int   refCount;
};
struct CDefPointer {
    RefTarget* p;
};

extern RefTarget* g_currentDef;
long g_lastId;
void __fastcall PrepareDef(long id) { g_lastId = id; }

CDefPointer* __fastcall GetPDef(CDefPointer* ret, long id);

int main()
{
    RefTarget obj;
    obj.vtbl_or_field0 = 0;
    obj.refCount = 5;
    g_currentDef = &obj;
    g_lastId = -1;

    CDefPointer out;
    out.p = 0;
    CDefPointer* r = GetPDef(&out, 42);

    bool ok = (r == &out) && (out.p == &obj) && (obj.refCount == 6) && (g_lastId == 42);

    // null-global path
    g_currentDef = 0;
    CDefPointer out2;
    out2.p = (RefTarget*)0xDEAD;
    GetPDef(&out2, 7);
    ok = ok && (out2.p == 0) && (g_lastId == 7);

    if (ok) printf("GETPDEF_OK_XYZ\n");
    else    printf("GETPDEF_FAIL\n");
    return ok ? 0 : 1;
}