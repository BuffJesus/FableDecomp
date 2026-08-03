struct Pair { int x; int y; };
struct RHSHolder { Pair a; Pair b; };
RHSHolder* __fastcall CRightHandedSet(RHSHolder* self, int edx_dummy, const Pair* p, const Pair* q) {
    (void)edx_dummy;
    self->a = *p;
    self->b = *q;
    return self;
}