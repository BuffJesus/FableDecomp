#pragma optimize("s",on)
// Dispatch one event kind to a virtual (vtable slot 4). The `dec eax; dec eax` is the
// switch lowering VC7.1 uses for a single-case switch. __fastcall this=ecx (ret 4).
struct Event { int type; };
struct T {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void OnQuit();      // slot 4 -> call [eax+0x10]
    void ProcessEvent(Event* e);
};
void T::ProcessEvent(Event* e) {
    switch (e->type) {
    case 2:
        this->OnQuit();
        break;
    }
}
