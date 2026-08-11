// CModeDragonBase::OnDeactivate  @ 008e72b0
// void __fastcall (modeled as __fastcall self in ecx)

struct Target
{
    virtual void v0();
    virtual void v1();
    // slot 0x98/4 = 38 -> pad to 38 then place OnSomething(int)
};

// A class whose vtable has the called method at slot 0x98 (byte offset 0x98 => index 38).
struct Inner
{
    virtual void s00();  virtual void s01();  virtual void s02();  virtual void s03();
    virtual void s04();  virtual void s05();  virtual void s06();  virtual void s07();
    virtual void s08();  virtual void s09();  virtual void s10();  virtual void s11();
    virtual void s12();  virtual void s13();  virtual void s14();  virtual void s15();
    virtual void s16();  virtual void s17();  virtual void s18();  virtual void s19();
    virtual void s20();  virtual void s21();  virtual void s22();  virtual void s23();
    virtual void s24();  virtual void s25();  virtual void s26();  virtual void s27();
    virtual void s28();  virtual void s29();  virtual void s30();  virtual void s31();
    virtual void s32();  virtual void s33();  virtual void s34();  virtual void s35();
    virtual void s36();  virtual void s37();
    virtual void Deactivate(int flag);   // slot 38 -> byte 0x98
};

struct Holder
{
    char pad0[0x60];  // +0x00 .. +0x5F
    Inner* inner;     // +0x60
};

struct CModeDragonBase
{
    void* pad0;      // +0x00
    Holder* holder;  // +0x04
    char pad1[0x10]; // +0x08 .. +0x17
    char active;     // +0x18
};

void __fastcall CModeDragonBase_OnDeactivate(CModeDragonBase* self)
{
    self->holder->inner->Deactivate(1);
    self->active = 0;
}