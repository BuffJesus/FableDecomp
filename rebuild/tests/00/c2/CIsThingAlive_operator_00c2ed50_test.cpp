#include <stdio.h>

struct IPolyA {
    virtual int va0();
    virtual int va1();
    virtual int va2();
    virtual int va3();
    virtual int va4();
    virtual bool va5();
};
struct IPolyB {
    virtual int vb0();
    virtual int vb1();
    virtual int vb2();
    virtual int vb3();
    virtual int vb4();
    virtual bool vb5();
};
struct IPolyC {
    virtual int vc0();
    virtual int vc1();
    virtual int vc2();
    virtual int vc3();
    virtual int vc4();
    virtual bool vc5();
};

struct CIsThingAlive : public IPolyA {
    char pad[12];
    IPolyB objB;
    char padB[12];
    IPolyC objC;
    bool operatorCall();
};

bool CIsThingAlive::operatorCall()
{
    this->va5();
    this->objB.vb5();
    return this->objC.vc5();
}

// Base virtual definitions (needed so CIsThingAlive's IPolyA vtable links).
int IPolyA::va0(){return 0;} int IPolyA::va1(){return 0;} int IPolyA::va2(){return 0;}
int IPolyA::va3(){return 0;} int IPolyA::va4(){return 0;} bool IPolyA::va5(){return false;}
int IPolyB::vb0(){return 0;} int IPolyB::vb1(){return 0;} int IPolyB::vb2(){return 0;}
int IPolyB::vb3(){return 0;} int IPolyB::vb4(){return 0;} bool IPolyB::vb5(){return false;}
int IPolyC::vc0(){return 0;} int IPolyC::vc1(){return 0;} int IPolyC::vc2(){return 0;}
int IPolyC::vc3(){return 0;} int IPolyC::vc4(){return 0;} bool IPolyC::vc5(){return false;}

// ---- test harness with concrete override classes ----
static int g_aCalled = 0;
static int g_bCalled = 0;
static int g_cCalled = 0;
static bool g_cResult = true;

struct AImpl : public IPolyA {
    int va0() { return 0; } int va1() { return 0; } int va2() { return 0; }
    int va3() { return 0; } int va4() { return 0; }
    bool va5() { g_aCalled = 1; return true; }
};
struct BImpl : public IPolyB {
    int vb0() { return 0; } int vb1() { return 0; } int vb2() { return 0; }
    int vb3() { return 0; } int vb4() { return 0; }
    bool vb5() { g_bCalled = 1; return true; }
};
struct CImpl : public IPolyC {
    int vc0() { return 0; } int vc1() { return 0; } int vc2() { return 0; }
    int vc3() { return 0; } int vc4() { return 0; }
    bool vc5() { g_cCalled = 1; return g_cResult; }
};

int main()
{
    // Build a CIsThingAlive-shaped object and overwrite the three
    // sub-object vtable pointers with our concrete implementations.
    CIsThingAlive obj;

    AImpl a; BImpl b; CImpl c;
    // Copy the vtable pointer (first machine word) of each impl into
    // the corresponding sub-object of obj.
    *(void**)((char*)&obj + 0x00) = *(void**)&a;
    *(void**)((char*)&obj + 0x10) = *(void**)&b;
    *(void**)((char*)&obj + 0x20) = *(void**)&c;

    g_cResult = true;
    bool r1 = obj.operatorCall();

    g_cResult = false;
    bool r2 = obj.operatorCall();

    if (g_aCalled && g_bCalled && g_cCalled && r1 == true && r2 == false) {
        printf("CIsThingAlive_PASS\n");
        return 0;
    }
    printf("FAIL a=%d b=%d c=%d r1=%d r2=%d\n", g_aCalled, g_bCalled, g_cCalled, (int)r1, (int)r2);
    return 1;
}