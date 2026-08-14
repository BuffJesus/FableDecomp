#include <cstdio>

struct Inner {
    bool rv;
    virtual bool IsHitResponse() { return rv; }
};

struct CThing {
    unsigned char flags;
    Inner* inner;
};

static bool model(CThing* self)
{
    if ((self->flags & 8) == 0)
        return true;
    Inner* obj = self->inner;
    if (obj == 0)
        return true;
    if (obj->IsHitResponse())
        return true;
    return false;
}

int main()
{
    Inner iTrue; iTrue.rv = true;
    Inner iFalse; iFalse.rv = false;

    CThing a; a.flags = 0; a.inner = &iFalse;          // flag clear -> true
    CThing b; b.flags = 8; b.inner = 0;                // null inner -> true
    CThing c; c.flags = 8; c.inner = &iTrue;           // vcall true -> true
    CThing d; d.flags = 8; d.inner = &iFalse;          // vcall false -> false
    CThing e; e.flags = 0xF7; e.inner = &iFalse;       // bit3 clear (0xF7) -> true

    bool ok = true;
    ok = ok && (model(&a) == true);
    ok = ok && (model(&b) == true);
    ok = ok && (model(&c) == true);
    ok = ok && (model(&d) == false);
    ok = ok && (model(&e) == true);

    if (ok) printf("BEHAVIOUR_OK\n");
    else    printf("BEHAVIOUR_FAIL\n");
    return 0;
}