// Non-virtual member whose entire body calls one of its own virtuals; VC7.1
// tail-jumps through the vtable (slot 12). __fastcall this=ecx.
struct T {
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
    virtual void Target();
    void Run();
};
void T::Run() { this->Target(); }