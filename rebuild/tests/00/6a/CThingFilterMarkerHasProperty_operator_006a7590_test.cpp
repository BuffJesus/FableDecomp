#include <cstdio>

static int g_flags = 0;

// Base with 49 virtuals, all inline-defined so the test links standalone.
struct CThingMarker {
    virtual int vf00(){return 0;} virtual int vf01(){return 0;} virtual int vf02(){return 0;}
    virtual int vf03(){return 0;} virtual int vf04(){return 0;} virtual int vf05(){return 0;}
    virtual int vf06(){return 0;} virtual int vf07(){return 0;} virtual int vf08(){return 0;}
    virtual int vf09(){return 0;} virtual int vf10(){return 0;} virtual int vf11(){return 0;}
    virtual int vf12(){return 0;} virtual int vf13(){return 0;} virtual int vf14(){return 0;}
    virtual int vf15(){return 0;} virtual int vf16(){return 0;} virtual int vf17(){return 0;}
    virtual int vf18(){return 0;} virtual int vf19(){return 0;} virtual int vf20(){return 0;}
    virtual int vf21(){return 0;} virtual int vf22(){return 0;} virtual int vf23(){return 0;}
    virtual int vf24(){return 0;} virtual int vf25(){return 0;} virtual int vf26(){return 0;}
    virtual int vf27(){return 0;} virtual int vf28(){return 0;} virtual int vf29(){return 0;}
    virtual int vf30(){return 0;} virtual int vf31(){return 0;} virtual int vf32(){return 0;}
    virtual int vf33(){return 0;} virtual int vf34(){return 0;} virtual int vf35(){return 0;}
    virtual int vf36(){return 0;} virtual int vf37(){return 0;} virtual int vf38(){return 0;}
    virtual int vf39(){return 0;} virtual int vf40(){return 0;} virtual int vf41(){return 0;}
    virtual int vf42(){return 0;} virtual int vf43(){return 0;} virtual int vf44(){return 0;}
    virtual int vf45(){return 0;} virtual int vf46(){return 0;} virtual int vf47(){return 0;}
    virtual int GetPropertyFlags(){ return g_flags; }   // slot 48 -> [eax+0xC0]
};

struct CThingFilter_MarkerHasProperty {
    int mask;
    bool operator()(CThingMarker* marker);
};

bool CThingFilter_MarkerHasProperty::operator()(CThingMarker* marker)
{
    int m = this->mask;
    return (m & marker->GetPropertyFlags()) != 0;
}

int main()
{
    CThingMarker m;
    CThingFilter_MarkerHasProperty f;

    f.mask = 0x0F;
    g_flags = 0x02;   // overlaps mask -> true
    bool r1 = f(&m);

    g_flags = 0x10;   // no overlap -> false
    bool r2 = f(&m);

    g_flags = 0x00;   // zero -> false
    bool r3 = f(&m);

    f.mask = 0;
    g_flags = 0xFFFF; // mask 0 -> false
    bool r4 = f(&m);

    if (r1 && !r2 && !r3 && !r4) {
        printf("PARITY_OK_MARKERHASPROP\n");
        return 0;
    }
    printf("FAIL %d %d %d %d\n", r1, r2, r3, r4);
    return 1;
}