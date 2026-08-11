// CIsThingAlive::operator() @ 00c26580
//   this->f74 = 2;
//   sub = (Inner*)((char*)arg + 0x2c);   // embedded polymorphic subobject
//   tail: sub->vfn8(this)                // vtable slot 8 (offset 0x20), thiscall

struct Inner {
    // 9 virtuals; slot 8 lives at vtable byte offset 0x20
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual bool v8(void* payload);   // slot 8, offset 0x20
};

struct CIsThingAlive {
    char pad[0x74];
    unsigned short f74;   // +0x74
    bool operator()(void* arg);
};

bool CIsThingAlive::operator()(void* arg)
{
    this->f74 = 2;
    Inner* sub = (Inner*)((char*)arg + 0x2c);
    return sub->v8(this);
}

// force emission of the out-of-line member
typedef bool (CIsThingAlive::*OpPtr)(void*);
OpPtr g_force = &CIsThingAlive::operator();