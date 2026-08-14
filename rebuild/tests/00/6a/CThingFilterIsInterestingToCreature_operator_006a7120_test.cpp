#include <stdio.h>

struct CThing {
    char pad[0x10];
    unsigned char kind;
};

struct Filter {
    CThing* target;
    int call(CThing* t) {
        if (t != this->target) {
            unsigned char k = t->kind;
            if (k == 1 || k == 2) return 1;
        }
        return 0;
    }
};

int main() {
    CThing a; a.kind = 1;   // is the target
    CThing b; b.kind = 1;
    CThing c; c.kind = 2;
    CThing d; d.kind = 0;
    CThing e; e.kind = 5;

    Filter f; f.target = &a;

    // target itself -> excluded -> 0 (even though kind=1)
    if (f.call(&a) != 0) { printf("no1\n"); return 1; }
    // not target, kind 1 -> 1
    if (f.call(&b) != 1) { printf("no2\n"); return 1; }
    // not target, kind 2 -> 1
    if (f.call(&c) != 1) { printf("no3\n"); return 1; }
    // not target, kind 0 -> 0
    if (f.call(&d) != 0) { printf("no4\n"); return 1; }
    // not target, kind 5 -> 0
    if (f.call(&e) != 0) { printf("no5\n"); return 1; }

    printf("PARITY_OK\n");
    return 0;
}