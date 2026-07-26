
struct CCharString {
    char* p;
    CCharString(const char* s);
};

struct CInner {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual void v8();
    virtual CCharString GetName();
};

struct CScriptThing {
    unsigned char _pad_0x0[0x4];
    CInner* inner;
    CCharString GetHomeMapName();
};

extern const char kHomeMapName[];

CCharString CScriptThing::GetHomeMapName()
{
    CInner* inner = this->inner;
    if (inner == 0)
        return CCharString(kHomeMapName);
    return inner->GetName();
}