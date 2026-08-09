#include <cstdio>
struct Inner { char pad[0xb]; unsigned char flags; };
struct Held { Inner* p; };
struct C3DAnimation2 { char pad[0x40]; Held* held; };

bool HasRotation(C3DAnimation2* self)
{
    Held* h = self->held;
    if (h != 0) {
        if (h->p->flags & 3)
            return true;
    }
    return false;
}

int main()
{
    C3DAnimation2 a; a.held = 0;
    if (HasRotation(&a)) { printf("FAIL null\n"); return 1; }

    Inner in0; in0.flags = 0;
    Held h0; h0.p = &in0;
    a.held = &h0;
    if (HasRotation(&a)) { printf("FAIL zeroflags\n"); return 1; }

    Inner in1; in1.flags = 1;
    Held h1; h1.p = &in1;
    a.held = &h1;
    if (!HasRotation(&a)) { printf("FAIL bit1\n"); return 1; }

    Inner in2; in2.flags = 2;
    Held h2; h2.p = &in2;
    a.held = &h2;
    if (!HasRotation(&a)) { printf("FAIL bit2\n"); return 1; }

    Inner in4; in4.flags = 4;
    Held h4; h4.p = &in4;
    a.held = &h4;
    if (HasRotation(&a)) { printf("FAIL bit4\n"); return 1; }

    printf("OK_00a4c1a0\n");
    return 0;
}