// Null-guarded virtual call: `if (p) p->Slot14(1);` __fastcall p=ecx.
struct Obj {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void slot8();
    virtual void slot9();
    virtual void slot10();
    virtual void slot11();
    virtual void slot12();
    virtual void slot13();
    virtual void Call(int flags);
};
extern "C" void __fastcall CallIfSet(Obj* p) { if (p) p->Call(1); }