struct CInner {
    virtual bool m0();
    virtual bool m1();
    virtual bool m2();
    virtual bool m3();
    virtual bool m4();
    virtual bool m5();
    virtual bool m6();
    virtual bool m7();
    virtual bool m8();
    virtual bool m9();
    virtual bool m10();
    virtual bool m11();
    virtual bool m12();
    virtual bool m13();
    virtual bool m14();
    virtual bool m15();
    virtual bool m16();
    virtual bool m17();
    virtual bool m18();
    virtual bool m19();
    virtual bool m20();
    virtual bool m21();
    virtual bool m22();
    virtual bool m23();
    virtual bool m24();
    virtual bool m25();
    virtual bool m26();
    virtual bool m27();
    virtual bool m28();
    virtual bool m29();
    virtual bool m30();
    virtual bool m31();
    virtual bool m32();
    virtual bool m33();
    virtual bool m34();
    virtual bool m35();
    virtual bool m36();
    virtual bool m37();
    virtual bool m38();
    virtual bool m39();
    virtual bool m40();
    virtual bool m41();
    virtual bool m42();
    virtual bool m43();
    virtual bool m44();
    virtual bool m45(); /* slot 45 = 0xb4 */
};

struct CScriptThing {
    void* pad0;
    CInner* m_inner; /* +0x4 */
};

bool __fastcall CScriptThing_MsgOpenedChest(CScriptThing* self)
{
    CInner* p = self->m_inner;
    if (!p)
        return false;
    return p->m45();
}