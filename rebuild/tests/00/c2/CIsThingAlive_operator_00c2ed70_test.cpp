#include <cstdio>

struct IPolyA {
    virtual void v0(){}
    virtual void v1(){}
    virtual void v2(){}
    virtual void v3(){}
    virtual void v4(){}
    virtual int  v5(){ return 0; }
};
struct IPolyB {
    virtual void v0(){}
    virtual void v1(){}
    virtual void v2(){}
    virtual void v3(){}
    virtual void v4(){}
    virtual bool v5(){ return false; }
};
struct CIsThingAlive {
    virtual void s0(){}
    virtual void s1(){}
    virtual void s2(){}
    virtual void s3(){}
    virtual void s4(){}
    virtual void s5(){}
    int pad0, pad1, pad2;
    IPolyA a;
    int padA1, padA2, padA3;
    IPolyB b;
    bool operator()();
};

static int g_s5=0, g_av5=0, g_bv5=0;

struct DA : IPolyA {
    void v0(){} void v1(){} void v2(){} void v3(){} void v4(){}
    int v5(){ g_av5++; return 7; }
};
struct DB : IPolyB {
    void v0(){} void v1(){} void v2(){} void v3(){} void v4(){}
    bool v5(){ g_bv5++; return true; }
};
struct DThing : CIsThingAlive {
    void s0(){} void s1(){} void s2(){} void s3(){} void s4(){}
    void s5(){ g_s5++; }
};

bool CIsThingAlive::operator()()
{
    this->s5();
    this->a.v5();
    return this->b.v5();
}

int main()
{
    DThing t;
    // overlay the derived vtables into the members
    DA da; DB db;
    *(void**)&t.a = *(void**)&da;
    *(void**)&t.b = *(void**)&db;

    bool r = t();
    if (r && g_s5==1 && g_av5==1 && g_bv5==1)
        printf("ISALIVE_OK\n");
    else
        printf("ISALIVE_FAIL r=%d s5=%d av5=%d bv5=%d\n", (int)r,g_s5,g_av5,g_bv5);
    return (r && g_s5==1 && g_av5==1 && g_bv5==1) ? 0 : 1;
}