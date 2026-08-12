#include <stdio.h>

struct ISized {
    virtual ~ISized() {}
    virtual long Sizeof() { return 0; }
};

struct CPhysicsFacesSegmentBranch {
    char pad[0x14];
    ISized* m14;
    ISized* m18;
    long Sizeof();
};

long CPhysicsFacesSegmentBranch::Sizeof() {
    return this->m14->Sizeof() + this->m18->Sizeof() + 0x1c;
}

struct A : ISized { long Sizeof() { return 100; } };
struct B : ISized { long Sizeof() { return 7; } };

int main() {
    A a; B b;
    CPhysicsFacesSegmentBranch obj;
    obj.m14 = &a;
    obj.m18 = &b;
    long r = obj.Sizeof();
    // expect 100 + 7 + 28 = 135
    if (r == 135) printf("SIZEOF_PASS_135\n");
    else printf("FAIL %ld\n", r);
    return r == 135 ? 0 : 1;
}