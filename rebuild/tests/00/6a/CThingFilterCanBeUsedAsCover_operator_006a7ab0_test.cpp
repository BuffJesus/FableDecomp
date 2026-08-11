#include <cstdio>

struct CThing
{
    virtual bool vf00(); virtual bool vf01(); virtual bool vf02(); virtual bool vf03();
    virtual bool vf04(); virtual bool vf05(); virtual bool vf06(); virtual bool vf07();
    virtual bool vf08(); virtual bool vf09(); virtual bool vf10(); virtual bool vf11();
    virtual bool vf12(); virtual bool vf13(); virtual bool vf14(); virtual bool vf15();
    virtual bool vf16(); virtual bool vf17(); virtual bool vf18(); virtual bool vf19();
    virtual bool vf20(); virtual bool vf21(); virtual bool vf22(); virtual bool vf23();
    virtual bool vf24(); virtual bool vf25(); virtual bool vf26(); virtual bool vf27();
    virtual bool vf28(); virtual bool vf29(); virtual bool vf30(); virtual bool vf31();
    virtual bool vf32(); virtual bool vf33(); virtual bool vf34(); virtual bool vf35();
    virtual bool vf36(); virtual bool vf37(); virtual bool vf38(); virtual bool vf39();
    virtual bool vf40(); virtual bool vf41(); virtual bool vf42(); virtual bool vf43();
    virtual bool vf44(); virtual bool vf45(); virtual bool vf46(); virtual bool vf47();
    virtual bool vf48(); virtual bool vf49(); virtual bool vf50(); virtual bool vf51();
    virtual bool vf52(); virtual bool vf53(); virtual bool vf54(); virtual bool vf55();
    virtual bool vf56(); virtual bool vf57(); virtual bool vf58(); virtual bool vf59();
    virtual bool vf60(); virtual bool vf61(); virtual bool vf62(); virtual bool vf63();
    virtual bool CanBeUsedAsCoverImpl();
    char pad[0x6c - 4];
    unsigned char flags;
};

struct CThingFilter_CanBeUsedAsCover
{
    bool operator()(CThing *t);
};

// The real function under test.
bool CThingFilter_CanBeUsedAsCover::operator()(CThing *t)
{
    if (t->flags & 4)
        return t->CanBeUsedAsCoverImpl();
    return false;
}

// Concrete CThing whose slot-64 virtual returns true.
struct MyThing : CThing
{
    bool CanBeUsedAsCoverImpl() { return true; }
    // provide bodies via a base default; only slot 64 matters here
};

// Provide out-of-line defaults for the pure-ish base virtuals so it links.
bool CThing::vf00(){return false;} bool CThing::vf01(){return false;} bool CThing::vf02(){return false;} bool CThing::vf03(){return false;}
bool CThing::vf04(){return false;} bool CThing::vf05(){return false;} bool CThing::vf06(){return false;} bool CThing::vf07(){return false;}
bool CThing::vf08(){return false;} bool CThing::vf09(){return false;} bool CThing::vf10(){return false;} bool CThing::vf11(){return false;}
bool CThing::vf12(){return false;} bool CThing::vf13(){return false;} bool CThing::vf14(){return false;} bool CThing::vf15(){return false;}
bool CThing::vf16(){return false;} bool CThing::vf17(){return false;} bool CThing::vf18(){return false;} bool CThing::vf19(){return false;}
bool CThing::vf20(){return false;} bool CThing::vf21(){return false;} bool CThing::vf22(){return false;} bool CThing::vf23(){return false;}
bool CThing::vf24(){return false;} bool CThing::vf25(){return false;} bool CThing::vf26(){return false;} bool CThing::vf27(){return false;}
bool CThing::vf28(){return false;} bool CThing::vf29(){return false;} bool CThing::vf30(){return false;} bool CThing::vf31(){return false;}
bool CThing::vf32(){return false;} bool CThing::vf33(){return false;} bool CThing::vf34(){return false;} bool CThing::vf35(){return false;}
bool CThing::vf36(){return false;} bool CThing::vf37(){return false;} bool CThing::vf38(){return false;} bool CThing::vf39(){return false;}
bool CThing::vf40(){return false;} bool CThing::vf41(){return false;} bool CThing::vf42(){return false;} bool CThing::vf43(){return false;}
bool CThing::vf44(){return false;} bool CThing::vf45(){return false;} bool CThing::vf46(){return false;} bool CThing::vf47(){return false;}
bool CThing::vf48(){return false;} bool CThing::vf49(){return false;} bool CThing::vf50(){return false;} bool CThing::vf51(){return false;}
bool CThing::vf52(){return false;} bool CThing::vf53(){return false;} bool CThing::vf54(){return false;} bool CThing::vf55(){return false;}
bool CThing::vf56(){return false;} bool CThing::vf57(){return false;} bool CThing::vf58(){return false;} bool CThing::vf59(){return false;}
bool CThing::vf60(){return false;} bool CThing::vf61(){return false;} bool CThing::vf62(){return false;} bool CThing::vf63(){return false;}
bool CThing::CanBeUsedAsCoverImpl(){return false;}

int main()
{
    CThingFilter_CanBeUsedAsCover filter;

    MyThing a; a.flags = 4;   // flag set, virtual returns true -> expect true
    MyThing b; b.flags = 0;   // flag clear -> expect false
    MyThing c; c.flags = 2;   // bit 4 not set -> expect false
    MyThing d; d.flags = 6;   // bit 4 set (4|2) -> expect true

    bool ra = filter(&a);
    bool rb = filter(&b);
    bool rc = filter(&c);
    bool rd = filter(&d);

    if (ra == true && rb == false && rc == false && rd == true)
    {
        printf("COVER_FILTER_OK\n");
        return 0;
    }
    printf("COVER_FILTER_FAIL %d %d %d %d\n", ra, rb, rc, rd);
    return 1;
}