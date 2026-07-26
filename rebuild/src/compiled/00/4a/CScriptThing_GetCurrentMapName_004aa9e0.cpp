struct CCharString { char* p; CCharString(const char* s); };
extern const char kDefaultMapName[];
struct CInner {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual void v3();
    virtual void v4();
    virtual void v5();
    virtual void v6();
    virtual void v7();
    virtual CCharString getName();
};
struct CScriptThing {
    unsigned char _pad0[4];
    CInner* inner;
    CCharString GetCurrentMapName();
};

CCharString CScriptThing::GetCurrentMapName() {
    CInner* p = this->inner;
    if (p == 0) {
        return CCharString(kDefaultMapName);
    }
    return p->getName();
}