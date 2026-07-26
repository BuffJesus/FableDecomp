struct CComponent {
    /* vtable layout must match: slots at byte offsets 0x2c(f11), 0x34(f13),
       0x3c(f15), 0xc8(f50). Declare virtuals in that index order. */
    virtual void f00();  virtual void f01();  virtual void f02();  virtual void f03();
    virtual void f04();  virtual void f05();  virtual void f06();  virtual void f07();
    virtual void f08();  virtual void f09();  virtual void f10();
    virtual void f11(void* p);                 /* 0x2c */
    virtual void f12();
    virtual void f13(void* p);                 /* 0x34 */
    virtual void f14();
    virtual void f15(void* p);                 /* 0x3c */
    virtual void f16(); virtual void f17(); virtual void f18(); virtual void f19();
    virtual void f20(); virtual void f21(); virtual void f22(); virtual void f23();
    virtual void f24(); virtual void f25(); virtual void f26(); virtual void f27();
    virtual void f28(); virtual void f29(); virtual void f30v(); virtual void f31();
    virtual void f32(); virtual void f33(); virtual void f34v(); virtual void f35();
    virtual void f36(); virtual void f37(); virtual void f38(); virtual void f39();
    virtual void f40(); virtual void f41(); virtual void f42(); virtual void f43();
    virtual void f44(); virtual void f45(); virtual void f46(); virtual void f47();
    virtual void f48(); virtual void f49();
    virtual char* f50();                        /* 0xc8 */

    char pad[0x30 - 4];
    int f30;                /* 0x30 */
    char pad2[0x98 - 0x34];
    int f98;                /* 0x98 */
    int f9c;
    int fa0;                /* 0xa0 */
    int fa4;
    int fa8;                /* 0xa8 */
};

void __fastcall CComponent_ResetState(CComponent* self)
{
    char* p = self->f50();
    self->f15(p + 0x8);
    self->f11(p + 0x10);
    self->f13(p + 0x18);
    self->fa0 = 0;
    self->f98 = 0;
    self->fa8 = 0;
    self->f30 = 0;
}