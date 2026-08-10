#include <cstdio>

struct C3DVector { float x, y, z; };

struct IHelper {
    virtual bool CheckReset(void* nav, C3DVector* dest) = 0;
    virtual ~IHelper() {}
};

struct CTCCreatureNavigation {
    void* field0;
    IHelper* field4;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
        return this->field4->CheckReset(this, dest);
    }
};

struct FakeHelper : IHelper {
    void* got_nav;
    C3DVector* got_dest;
    bool ret;
    FakeHelper() : got_nav(0), got_dest(0), ret(false) {}
    bool CheckReset(void* nav, C3DVector* dest) {
        got_nav = nav; got_dest = dest; return ret;
    }
};

int main() {
    FakeHelper h;
    CTCCreatureNavigation nav;
    nav.field0 = 0;
    nav.field4 = &h;
    C3DVector v = {1.0f, 2.0f, 3.0f};

    h.ret = true;
    bool r1 = nav.IsNewDestinationGoingToResetNavigation(&v);
    if (r1 != true) { printf("FAIL r1\n"); return 1; }
    if (h.got_nav != (void*)&nav) { printf("FAIL nav\n"); return 1; }
    if (h.got_dest != &v) { printf("FAIL dest\n"); return 1; }

    h.ret = false;
    bool r2 = nav.IsNewDestinationGoingToResetNavigation(&v);
    if (r2 != false) { printf("FAIL r2\n"); return 1; }

    printf("OK_00c2dd10\n");
    return 0;
}