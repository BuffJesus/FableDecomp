#include <cstdio>

struct C3DVector { float x, y, z; };
struct CTCCreatureNavigation;

struct Helper {
    virtual bool m07(CTCCreatureNavigation* nav, C3DVector* dest) = 0;
};

struct HelperImpl : Helper {
    bool ret;
    CTCCreatureNavigation* seen_nav;
    C3DVector* seen_dest;
    HelperImpl() : ret(false), seen_nav(0), seen_dest(0) {}
    virtual bool m07(CTCCreatureNavigation* nav, C3DVector* dest) {
        seen_nav = nav; seen_dest = dest; return ret;
    }
};

struct CTCCreatureNavigation {
    void* field_0;
    Helper* field_4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
        Helper* h = this->field_4;
        return h->m07(this, dest);
    }
};

int main() {
    HelperImpl h;
    CTCCreatureNavigation nav;
    nav.field_0 = 0;
    nav.field_4 = &h;
    C3DVector v = {1.0f, 2.0f, 3.0f};

    h.ret = true;
    bool r1 = nav.IsNewDestinationGoingToResetNavigation(&v);
    if (r1 != true) { printf("FAIL r1\n"); return 1; }
    if (h.seen_nav != &nav) { printf("FAIL nav\n"); return 1; }
    if (h.seen_dest != &v) { printf("FAIL dest\n"); return 1; }

    h.ret = false;
    bool r2 = nav.IsNewDestinationGoingToResetNavigation(&v);
    if (r2 != false) { printf("FAIL r2\n"); return 1; }

    printf("OK_00c0f1c0\n");
    return 0;
}