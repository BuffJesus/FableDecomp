struct CScriptThing;

struct CInner {
    // 19 dummy virtuals so the real one lands at vtable offset 0x4c (0x13*4)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18();
    virtual void OnKilled(CScriptThing* msg); // vtable +0x4c
};

struct CScriptThing {
    unsigned char _pad_0x0[0x4];
    CInner* inner; // +0x4
    CScriptThing* MsgWhoKilledMe(CScriptThing* msg);
    void HandleNull(CScriptThing* killer);
};

CScriptThing* CScriptThing::MsgWhoKilledMe(CScriptThing* msg)
{
    CInner* p = this->inner;
    if (p == 0) {
        msg->HandleNull(this);
        return msg;
    }
    p->OnKilled(msg);
    return msg;
}