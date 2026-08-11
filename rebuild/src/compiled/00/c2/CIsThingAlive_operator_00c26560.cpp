// CIsThingAlive::operator() @ 00c26560
//   this->f74 = 1;
//   sub = (Inner*)((char*)arg + 0x2c);   // embedded polymorphic subobject
//   tail: sub->vfn9(this)                // vtable slot 9 (offset 0x24), thiscall

struct Inner {
    // 10 virtuals; slot 9 lives at vtable byte offset 0x24
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual bool v9(void* payload);   // slot 9
};

struct CIsThingAlive {
    char pad[0x74];
    unsigned short f74;   // +0x74
    bool operator()(void* arg);
};

bool CIsThingAlive::operator()(void* arg)
{
    this->f74 = 1;
    Inner* sub = (Inner*)((char*)arg + 0x2c);
    return sub->v9(this);
}

// force emission of the out-of-line member
typedef bool (CIsThingAlive::*OpPtr)(void*);
OpPtr g_force = &CIsThingAlive::operator();