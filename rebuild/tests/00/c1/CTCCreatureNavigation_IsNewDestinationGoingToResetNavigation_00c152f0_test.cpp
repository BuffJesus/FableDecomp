#include <stdio.h>

struct C3DVector { float x, y, z; };

struct INavTarget {
    virtual bool Query(void* self, C3DVector* v) = 0;
};

struct FakeTarget : INavTarget {
    bool ret;
    void* seenSelf;
    C3DVector* seenV;
    bool Query(void* self, C3DVector* v) { seenSelf = self; seenV = v; return ret; }
};

struct CTCCreatureNavigation {
    void* field_0;
    INavTarget* field_4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
        return field_4->Query(this, dest);
    }
};

int main() {
    FakeTarget t;
    t.ret = true;
    CTCCreatureNavigation nav;
    nav.field_0 = 0;
    nav.field_4 = &t;
    C3DVector v = {1.f, 2.f, 3.f};

    bool r = nav.IsNewDestinationGoingToResetNavigation(&v);
    if (r != true) { printf("FAIL ret\n"); return 1; }
    if (t.seenSelf != (void*)&nav) { printf("FAIL self\n"); return 1; }
    if (t.seenV != &v) { printf("FAIL vec\n"); return 1; }

    t.ret = false;
    r = nav.IsNewDestinationGoingToResetNavigation(&v);
    if (r != false) { printf("FAIL ret2\n"); return 1; }

    printf("OK_00c152f0\n");
    return 0;
}