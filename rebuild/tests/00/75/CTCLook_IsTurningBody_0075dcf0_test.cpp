#include <cstdio>

struct Loc { int x; int y; };
struct Arg { int v; };

struct SubObj {
    int marker;
    bool Compute(Loc* out, Arg* in) {
        out->x = in->v;
        return (in->v & 1) != 0;
    }
};

struct CTCLook {
    char pad[0xc0];
    SubObj sub;
    bool IsTurningBody(Arg arg) {
        Loc local;
        return this->sub.Compute(&local, &arg);
    }
};

int main() {
    CTCLook o;
    o.sub.marker = 0;
    Arg a1; a1.v = 3;
    Arg a2; a2.v = 4;
    bool r1 = o.IsTurningBody(a1);
    bool r2 = o.IsTurningBody(a2);
    if (r1 == true && r2 == false) {
        printf("BEHAVIOR_OK\n");
    } else {
        printf("BEHAVIOR_FAIL %d %d\n", (int)r1, (int)r2);
    }
    return 0;
}