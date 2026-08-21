// Forward to vtable slot 3 of the sub-object pointer at this+0x4, passing
// `this` as the argument. __fastcall this=ecx.
#pragma pack(push,1)
struct T;
struct Sub {
    virtual void slot0();
    virtual void slot1();
    virtual void slot2();
    virtual void Do(T* p);
};
struct T {
    char pad_0[0x4];
    Sub* sub;
    void Run();
};
#pragma pack(pop)
void T::Run() { this->sub->Do(this); }