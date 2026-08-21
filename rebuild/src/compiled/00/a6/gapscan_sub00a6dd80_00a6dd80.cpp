// Null-guarded virtual call: `if (p) p->Slot3(1);` __fastcall p=ecx.
struct Obj {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void Call(int flags);
};
extern "C" void __fastcall CallIfSet(Obj* p) { if (p) p->Call(1); }