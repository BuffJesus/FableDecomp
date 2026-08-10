struct CPersistContext;

struct SubObj {
    void Method(CPersistContext* ctx);
};

struct CPersistTraitsLong {
    int flags;   // +0
    char pad[4]; // +4
    SubObj sub;  // +8
};

void __fastcall TransferOut(CPersistTraitsLong* self, int /*edx*/, CPersistContext* ctx)
{
    self->sub.Method(ctx);
    self->flags |= 1;
}