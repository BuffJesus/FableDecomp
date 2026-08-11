#include <cstdio>

struct Helped {
    void* seen_a;
    void* seen_b;
    void method(void* a, void* b) { seen_a = a; seen_b = b; }
};

struct Self { char pad[0x24]; Helped* f24; };

void* __fastcall GFPredicateAnd_and_predicate(Self* self, int /*edx*/, void* a, void* b)
{
    self->f24->method(a, b);
    return a;
}

int main()
{
    Helped h; h.seen_a = 0; h.seen_b = 0;
    Self s; s.f24 = &h;
    int aval = 11, bval = 22;
    void* r = GFPredicateAnd_and_predicate(&s, 0, &aval, &bval);
    if (r == &aval && h.seen_a == &aval && h.seen_b == &bval)
        printf("00687d20_TEST PASS\n");
    else
        printf("FAIL r=%p sa=%p sb=%p\n", r, h.seen_a, h.seen_b);
    return 0;
}