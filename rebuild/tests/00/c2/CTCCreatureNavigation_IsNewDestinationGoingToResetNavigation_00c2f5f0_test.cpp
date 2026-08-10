#include <cstdio>

struct C3DVector { float x, y, z; };
struct Nav;

struct Delegate {
    virtual bool decide(Nav* n, C3DVector* d) = 0;
};

struct Nav {
    Delegate* del;
    bool IsNewDestinationGoingToResetNavigation(C3DVector* dest) {
        return del->decide(this, dest);
    }
};

struct YesDel : Delegate {
    Nav* sawNav; C3DVector* sawDest;
    bool decide(Nav* n, C3DVector* d) { sawNav = n; sawDest = d; return true; }
};
struct NoDel : Delegate {
    bool decide(Nav* n, C3DVector* d) { (void)n;(void)d; return false; }
};

int main() {
    C3DVector dest = {1.0f, 2.0f, 3.0f};
    YesDel yd; yd.sawNav = 0; yd.sawDest = 0;
    NoDel nd;

    Nav a; a.del = &yd;
    bool r1 = a.IsNewDestinationGoingToResetNavigation(&dest);
    if (!r1) { printf("FAIL r1\n"); return 1; }
    if (yd.sawNav != &a) { printf("FAIL nav-arg\n"); return 1; }
    if (yd.sawDest != &dest) { printf("FAIL dest-arg\n"); return 1; }

    Nav b; b.del = &nd;
    bool r2 = b.IsNewDestinationGoingToResetNavigation(&dest);
    if (r2) { printf("FAIL r2\n"); return 1; }

    printf("OK_00c2f5f0\n");
    return 0;
}