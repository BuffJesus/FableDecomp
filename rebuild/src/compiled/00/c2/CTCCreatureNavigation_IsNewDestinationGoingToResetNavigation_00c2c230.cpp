// Forward to vtable slot 8 of the sub-object pointer at this+0x4, passing
// `this` and the stack argument. __fastcall this=ecx (ret 4).
#pragma pack(push,1)
struct T;
struct Sub {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void slot3();
    virtual void slot4();
    virtual void slot5();
    virtual void slot6();
    virtual void slot7();
    virtual void Do(T* p, int a);
};
struct T {
    char pad_0[0x4];
    Sub* sub;
    void Run(int a);
};
#pragma pack(pop)
void T::Run(int a) { this->sub->Do(this, a); }