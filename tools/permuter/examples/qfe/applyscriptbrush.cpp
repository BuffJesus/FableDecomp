struct CCharString;
struct Result { void Finalize(); };
struct Sub {
    virtual void v0();  virtual void v1();  virtual void v2();  virtual void v3();
    virtual void v4();  virtual void v5();  virtual void v6();  virtual void v7();
    virtual void v8();  virtual void v9();  virtual void v10(); virtual void v11();
    virtual Result* Apply(CCharString const& name, int flag);   // vtbl+0x30 (slot 12)
};
struct CGameScriptInterface {
    void* pad0;    // +0x00
    Sub*  sub;     // +0x04
    void ApplyScriptBrush(CCharString const& name);
};
void CGameScriptInterface::ApplyScriptBrush(CCharString const& name)
{
    Result* r = this->sub->Apply(name, 1);
    r->Finalize();
}
