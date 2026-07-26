
struct CScriptTarget {
    unsigned char _pad[0x91];
    unsigned char flags; // +0x91
    unsigned char _pad2[0x100];
};

struct CScriptObj;

struct CScriptObjVtbl {
    void* slots0to10[11]; // 0..0x28
    CScriptTarget* (__fastcall* GetTarget)(CScriptObj*); // +0x2c
};

struct CScriptObj {
    CScriptObjVtbl* vt;
};

struct CSubObj {
    void AttachToScript(int arg);
};

void __stdcall CGameScriptInterface_EntityAttachToScript(CScriptObj* self, int arg)
{
    CScriptTarget* t = self->vt->GetTarget(self);
    if (t == 0)
        return;
    if (t->flags & 1)
        return;
    ((CSubObj*)((char*)t + 0x58))->AttachToScript(arg);
}