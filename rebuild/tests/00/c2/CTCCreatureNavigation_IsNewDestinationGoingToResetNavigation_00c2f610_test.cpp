#include <cstdio>

struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual bool check(CTCCreatureNavigation* nav, C3DVector* v) = 0;
};

struct CTCCreatureNavigation {
    void* field_0;
    Helper* helper;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* v) {
        return this->helper->check(this, v);
    }
};

struct HelperTrue : Helper {
    CTCCreatureNavigation* got_nav; C3DVector* got_v;
    bool check(CTCCreatureNavigation* nav, C3DVector* v) { got_nav=nav; got_v=v; return true; }
};
struct HelperFalse : Helper {
    bool check(CTCCreatureNavigation* nav, C3DVector* v) { (void)nav;(void)v; return false; }
};

int main() {
    C3DVector v = {1,2,3};
    HelperTrue ht;
    CTCCreatureNavigation a; a.field_0=0; a.helper=&ht;
    bool r1 = a.IsNewDestinationGoingToResetNavigation(&v);
    if (!r1) { printf("FAIL r1\n"); return 1; }
    if (ht.got_nav != &a || ht.got_v != &v) { printf("FAIL args\n"); return 1; }

    HelperFalse hf;
    CTCCreatureNavigation b; b.field_0=0; b.helper=&hf;
    bool r2 = b.IsNewDestinationGoingToResetNavigation(&v);
    if (r2) { printf("FAIL r2\n"); return 1; }

    printf("OK_00c2f610\n");
    return 0;
}