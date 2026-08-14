#include <cstdio>

struct CComp { int tag; };

struct CountedPtr {
    CComp* p;
    void assign(CComp* src) { p = src; }
};

struct CSpellContainerList {
    CountedPtr pad0;
    CountedPtr sub1;
    CountedPtr pad2;
    CountedPtr pad3;
    CountedPtr sub2;
};

void AddChild(CSpellContainerList* self, CComp* arg)
{
    self->sub1.assign(arg);
    self->sub2.assign(arg);
}

int main()
{
    CSpellContainerList o;
    o.sub1.p = 0; o.sub2.p = 0; o.pad0.p = (CComp*)1;
    CComp c; c.tag = 7;
    AddChild(&o, &c);
    if (o.sub1.p == &c && o.sub2.p == &c && o.pad0.p == (CComp*)1) {
        printf("PARITY_OK\n");
    } else {
        printf("FAIL\n");
    }
    return 0;
}