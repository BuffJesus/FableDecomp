#include <cstdio>

struct CScriptBase;

struct CScriptBaseVtbl {
    void (__fastcall *slot0)(CScriptBase*);
    void (__fastcall *slot1)(CScriptBase*);
    void (__fastcall *slot2)(CScriptBase*);
    void (__fastcall *slot3)(CScriptBase*);
};

struct CScriptBase {
    CScriptBaseVtbl* vtbl;
    int log;
};

void __fastcall Activate(CScriptBase* self)
{
    self->vtbl->slot3(self);
    self->vtbl->slot1(self);
}

static int g_order = 0;
static int g_s3 = 0;
static int g_s1 = 0;

void __fastcall f0(CScriptBase* s) {}
void __fastcall f1(CScriptBase* s) { g_s1 = ++g_order; }
void __fastcall f2(CScriptBase* s) {}
void __fastcall f3(CScriptBase* s) { g_s3 = ++g_order; }

int main()
{
    CScriptBaseVtbl vt = { f0, f1, f2, f3 };
    CScriptBase obj;
    obj.vtbl = &vt;
    obj.log = 0;
    Activate(&obj);
    if (g_s3 == 1 && g_s1 == 2)
        printf("00cb7900_TEST PASS\n");
    else
        printf("FAIL s3=%d s1=%d\n", g_s3, g_s1);
    return 0;
}