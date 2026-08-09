struct Inner {
    char pad[0xb];
    unsigned char flags;
};
struct Held {
    Inner* p;
};
struct C3DAnimation2 {
    char pad[0x40];
    Held* held;
};

bool __fastcall C3DAnimation2_HasRotation(C3DAnimation2* self)
{
    Held* h = self->held;
    if (h != 0) {
        if (h->p->flags & 3)
            return true;
    }
    return false;
}