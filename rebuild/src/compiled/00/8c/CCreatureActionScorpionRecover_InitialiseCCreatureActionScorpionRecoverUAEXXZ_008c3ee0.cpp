// Byte-exact reconstruction of CCreatureAction_ScorpionRecover::Initialise 0x008c3ee0

struct Target {
    virtual void v00();
    virtual void v04();
    virtual void v08();
    virtual void v0c();
    virtual void v10();
    virtual void v14();
    virtual void v18();
    virtual void v1c();
    virtual void v20();
    virtual void v24();
    virtual void v28();
    virtual void v2c();
    virtual void v30();
    virtual void v34();
    virtual void v38();
    virtual void v3c();
    virtual void v40();
    virtual void v44();
    virtual void v48();
    virtual void v4c();
    virtual void v50();
    virtual void v54();
    virtual void v58();
    virtual void v5c();
    virtual void v60();
    virtual void v64();
    virtual void v68();
    virtual void v6c();
    virtual void slot70(int flag); // 0x70/4 = 28
};

struct Member8 {
    Target* GetTarget();   // non-virtual, 0xa01b50
};

struct CCreatureAction_ScorpionRecover {
    void BaseInit();       // non-virtual base Initialise, 0x857520
    // vtable ptr at +0x00 (4 bytes), pad to +0x08
    char pad[4];
    Member8 m8;            // +0x08
    virtual void Initialise();
};

void CCreatureAction_ScorpionRecover::Initialise()
{
    BaseInit();
    Target* t = m8.GetTarget();
    t->slot70(1);
}