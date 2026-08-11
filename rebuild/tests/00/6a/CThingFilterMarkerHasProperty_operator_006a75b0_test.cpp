#include <cstdio>

struct CThingMarker {
    virtual int v00()=0;  virtual int v01()=0;  virtual int v02()=0;  virtual int v03()=0;
    virtual int v04()=0;  virtual int v05()=0;  virtual int v06()=0;  virtual int v07()=0;
    virtual int v08()=0;  virtual int v09()=0;  virtual int v10()=0;  virtual int v11()=0;
    virtual int v12()=0;  virtual int v13()=0;  virtual int v14()=0;  virtual int v15()=0;
    virtual int v16()=0;  virtual int v17()=0;  virtual int v18()=0;  virtual int v19()=0;
    virtual int v20()=0;  virtual int v21()=0;  virtual int v22()=0;  virtual int v23()=0;
    virtual int v24()=0;  virtual int v25()=0;  virtual int v26()=0;  virtual int v27()=0;
    virtual int v28()=0;  virtual int v29()=0;  virtual int v30()=0;  virtual int v31()=0;
    virtual int v32()=0;  virtual int v33()=0;  virtual int v34()=0;  virtual int v35()=0;
    virtual int v36()=0;  virtual int v37()=0;  virtual int v38()=0;  virtual int v39()=0;
    virtual int v40()=0;  virtual int v41()=0;  virtual int v42()=0;  virtual int v43()=0;
    virtual int v44()=0;  virtual int v45()=0;  virtual int v46()=0;  virtual int v47()=0;
    virtual int GetProperties()=0;
};

struct CThingFilter_MarkerHasProperty {
    int m_mask;
    bool operator()(CThingMarker* marker);
};

bool CThingFilter_MarkerHasProperty::operator()(CThingMarker* marker)
{
    int mask = this->m_mask;
    return (marker->GetProperties() & mask) != 0;
}

static int g_props = 0;
struct FakeMarker : CThingMarker {
    int v00(){return 0;} int v01(){return 0;} int v02(){return 0;} int v03(){return 0;}
    int v04(){return 0;} int v05(){return 0;} int v06(){return 0;} int v07(){return 0;}
    int v08(){return 0;} int v09(){return 0;} int v10(){return 0;} int v11(){return 0;}
    int v12(){return 0;} int v13(){return 0;} int v14(){return 0;} int v15(){return 0;}
    int v16(){return 0;} int v17(){return 0;} int v18(){return 0;} int v19(){return 0;}
    int v20(){return 0;} int v21(){return 0;} int v22(){return 0;} int v23(){return 0;}
    int v24(){return 0;} int v25(){return 0;} int v26(){return 0;} int v27(){return 0;}
    int v28(){return 0;} int v29(){return 0;} int v30(){return 0;} int v31(){return 0;}
    int v32(){return 0;} int v33(){return 0;} int v34(){return 0;} int v35(){return 0;}
    int v36(){return 0;} int v37(){return 0;} int v38(){return 0;} int v39(){return 0;}
    int v40(){return 0;} int v41(){return 0;} int v42(){return 0;} int v43(){return 0;}
    int v44(){return 0;} int v45(){return 0;} int v46(){return 0;} int v47(){return 0;}
    int GetProperties(){ return g_props; }
};

int main()
{
    FakeMarker m;
    CThingFilter_MarkerHasProperty f;
    int fails = 0;

    f.m_mask = 0x04;
    g_props = 0x04; if (f(&m) != true)  fails++;   // overlap -> true
    g_props = 0x08; if (f(&m) != false) fails++;   // no overlap -> false
    g_props = 0x0C; if (f(&m) != true)  fails++;   // partial overlap -> true
    g_props = 0x00; if (f(&m) != false) fails++;   // zero -> false

    f.m_mask = 0;
    g_props = 0xFFFFFFFF; if (f(&m) != false) fails++; // mask zero -> false

    if (fails == 0) printf("MARKERHASPROPERTY_OK\n");
    else            printf("MARKERHASPROPERTY_FAIL %d\n", fails);
    return fails == 0 ? 0 : 1;
}