#include <cstdio>

struct CInner {
    virtual bool m0(){return false;} virtual bool m1(){return false;}
    virtual bool m2(){return false;} virtual bool m3(){return false;}
    virtual bool m4(){return false;} virtual bool m5(){return false;}
    virtual bool m6(){return false;} virtual bool m7(){return false;}
    virtual bool m8(){return false;} virtual bool m9(){return false;}
    virtual bool m10(){return false;} virtual bool m11(){return false;}
    virtual bool m12(){return false;} virtual bool m13(){return false;}
    virtual bool m14(){return false;} virtual bool m15(){return false;}
    virtual bool m16(){return false;} virtual bool m17(){return false;}
    virtual bool m18(){return false;} virtual bool m19(){return false;}
    virtual bool m20(){return false;} virtual bool m21(){return false;}
    virtual bool m22(){return false;} virtual bool m23(){return false;}
    virtual bool m24(){return false;} virtual bool m25(){return false;}
    virtual bool m26(){return false;} virtual bool m27(){return false;}
    virtual bool m28(){return false;} virtual bool m29(){return false;}
    virtual bool m30(){return false;} virtual bool m31(){return false;}
    virtual bool m32(){return false;} virtual bool m33(){return false;}
    virtual bool m34(){return false;} virtual bool m35(){return false;}
    virtual bool m36(){return false;} virtual bool m37(){return false;}
    virtual bool m38(){return false;} virtual bool m39(){return false;}
    virtual bool m40(){return false;} virtual bool m41(){return false;}
    virtual bool m42(){return false;} virtual bool m43(){return false;}
    virtual bool m44(){return false;} virtual bool m45(){return true;}
};

struct CScriptThing {
    void* pad0;
    CInner* m_inner;
};

bool __fastcall CScriptThing_MsgOpenedChest(CScriptThing* self)
{
    CInner* p = self->m_inner;
    if (!p)
        return false;
    return p->m45();
}

int main()
{
    /* null inner -> false */
    CScriptThing a;
    a.pad0 = 0;
    a.m_inner = 0;
    if (CScriptThing_MsgOpenedChest(&a) != false) { std::printf("FAIL null\n"); return 1; }

    /* non-null inner -> forwards to m45 returning true */
    CInner inner;
    CScriptThing b;
    b.pad0 = 0;
    b.m_inner = &inner;
    if (CScriptThing_MsgOpenedChest(&b) != true) { std::printf("FAIL forward\n"); return 1; }

    std::printf("CScriptThing_004aada0_TEST PASS\n");
    return 0;
}