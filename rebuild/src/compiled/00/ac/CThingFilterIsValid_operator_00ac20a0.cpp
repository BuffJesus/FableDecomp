// Non-virtual member whose entire body calls one of its own virtuals; VC7.1
// tail-jumps through the vtable (slot 1). __fastcall this=ecx.
struct T {
    virtual void slot0();
    virtual void Target();
    void Run();
};
void T::Run() { this->Target(); }