#include <stdio.h>
struct Pair { int x; int y; };
struct RHSHolder { Pair a; Pair b; };
RHSHolder* __fastcall CRightHandedSet(RHSHolder* self, int edx_dummy, const Pair* p, const Pair* q) {
    (void)edx_dummy;
    self->a = *p;
    self->b = *q;
    return self;
}
int main() {
    RHSHolder h;
    h.a.x = -1; h.a.y = -1; h.b.x = -1; h.b.y = -1;
    Pair p; p.x = 11; p.y = 22;
    Pair q; q.x = 33; q.y = 44;
    RHSHolder* r = CRightHandedSet(&h, 0, &p, &q);
    if (r != &h) { printf("BAD_RET\n"); return 1; }
    if (h.a.x==11 && h.a.y==22 && h.b.x==33 && h.b.y==44) {
        printf("OK_0x004371a0\n");
        return 0;
    }
    printf("BAD_FIELDS %d %d %d %d\n", h.a.x, h.a.y, h.b.x, h.b.y);
    return 1;
}