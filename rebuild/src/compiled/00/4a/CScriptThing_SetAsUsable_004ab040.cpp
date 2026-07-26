#pragma optimize("s",on)
struct CInner {
    virtual void v0();
};

struct CScriptThing {
    void* pad0;
    CInner* inner;
};

typedef void (__fastcall *SlotFn)(CInner*, int, int);

void __fastcall CScriptThing_SetAsUsable(CScriptThing* self, int edx, int arg)
{
    CInner* p = self->inner;
    if (p) {
        void** vt = *(void***)p;
        ((SlotFn)vt[66])(p, edx, arg);
    }
}