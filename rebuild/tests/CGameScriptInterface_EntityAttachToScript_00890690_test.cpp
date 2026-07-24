
#include <cstdio>

struct CScriptTarget {
    unsigned char _pad[0x91];
    unsigned char flags; // +0x91
    unsigned char _pad2[0x100];
};

struct CScriptObj;

struct CScriptObjVtbl {
    void* slots0to10[11];
    CScriptTarget* (__fastcall* GetTarget)(CScriptObj*);
};

struct CScriptObj {
    CScriptObjVtbl* vt;
};

static int g_subCalls = 0;
static int g_lastArg = 0;
static void* g_lastThis = 0;

struct CSubObj {
    void AttachToScript(int arg);
};

void CSubObj::AttachToScript(int arg)
{
    g_subCalls++;
    g_lastArg = arg;
    g_lastThis = this;
}

void __stdcall CGameScriptInterface_EntityAttachToScript(CScriptObj* self, int arg)
{
    CScriptTarget* t = self->vt->GetTarget(self);
    if (t == 0)
        return;
    if (t->flags & 1)
        return;
    ((CSubObj*)((char*)t + 0x58))->AttachToScript(arg);
}

static CScriptTarget g_target;
static CScriptTarget* g_ret = 0;

CScriptTarget* __fastcall MyGetTarget(CScriptObj* self)
{
    return g_ret;
}

int main()
{
    CScriptObjVtbl vt;
    for (int i = 0; i < 11; i++) vt.slots0to10[i] = 0;
    vt.GetTarget = MyGetTarget;
    CScriptObj obj;
    obj.vt = &vt;

    // Case 1: GetTarget returns null -> no sub call
    g_ret = 0;
    g_subCalls = 0;
    CGameScriptInterface_EntityAttachToScript(&obj, 111);
    if (g_subCalls != 0) { std::printf("FAIL null\n"); return 1; }

    // Case 2: target with flag bit set -> no sub call
    g_target.flags = 1;
    g_ret = &g_target;
    g_subCalls = 0;
    CGameScriptInterface_EntityAttachToScript(&obj, 222);
    if (g_subCalls != 0) { std::printf("FAIL flagset\n"); return 1; }

    // Case 3: target, flag clear -> sub called with arg and this=target+0x58
    g_target.flags = 0;
    g_ret = &g_target;
    g_subCalls = 0;
    CGameScriptInterface_EntityAttachToScript(&obj, 333);
    if (g_subCalls != 1) { std::printf("FAIL nocall\n"); return 1; }
    if (g_lastArg != 333) { std::printf("FAIL arg\n"); return 1; }
    if (g_lastThis != (void*)((char*)&g_target + 0x58)) { std::printf("FAIL this\n"); return 1; }

    // Case 4: flag bit 1 clear but other bits set (flags=2) -> still calls
    g_target.flags = 2;
    g_ret = &g_target;
    g_subCalls = 0;
    CGameScriptInterface_EntityAttachToScript(&obj, 444);
    if (g_subCalls != 1) { std::printf("FAIL flag2\n"); return 1; }

    std::printf("CGameScriptInterface_00890690_TEST PASS\n");
    return 0;
}