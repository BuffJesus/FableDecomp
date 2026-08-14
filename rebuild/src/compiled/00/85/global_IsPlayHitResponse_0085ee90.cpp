// Byte-exact reconstruction of IsPlayHitResponse @ 0x0085ee90

struct Inner {
    // vtable slot at +0xC0 -> +0xC0/4 = index 0x30
    virtual bool m00();  virtual bool m01();  virtual bool m02();  virtual bool m03();
    virtual bool m04();  virtual bool m05();  virtual bool m06();  virtual bool m07();
    virtual bool m08();  virtual bool m09();  virtual bool m0a();  virtual bool m0b();
    virtual bool m0c();  virtual bool m0d();  virtual bool m0e();  virtual bool m0f();
    virtual bool m10();  virtual bool m11();  virtual bool m12();  virtual bool m13();
    virtual bool m14();  virtual bool m15();  virtual bool m16();  virtual bool m17();
    virtual bool m18();  virtual bool m19();  virtual bool m1a();  virtual bool m1b();
    virtual bool m1c();  virtual bool m1d();  virtual bool m1e();  virtual bool m1f();
    virtual bool m20();  virtual bool m21();  virtual bool m22();  virtual bool m23();
    virtual bool m24();  virtual bool m25();  virtual bool m26();  virtual bool m27();
    virtual bool m28();  virtual bool m29();  virtual bool m2a();  virtual bool m2b();
    virtual bool m2c();  virtual bool m2d();  virtual bool m2e();  virtual bool m2f();
    virtual bool IsHitResponse(); // +0xC0
};

struct CThing {
    char pad0[0x6c];
    unsigned char flags;      // +0x6c
    char pad1[0x100 - 0x6d];
    Inner* inner;             // +0x100
};

bool __fastcall IsPlayHitResponse(CThing* self)
{
    if (self->flags & 8)
    {
        Inner* obj = self->inner;
        if (obj != 0)
        {
            if (!obj->IsHitResponse())
                return false;
        }
    }
    return true;
}