#pragma optimize("s",on)
// CFactionDef::Transfer  0x004717a1
struct SubA { int x; };
struct SubB { int y; };
struct CPersistContext { void Xfer(void* p); };
struct CFactionDef {
    char pad[0x3c];
    SubA a;
    char pad2[0x48 - 0x3c - sizeof(SubA)];
    SubB b;
    void Transfer(CPersistContext* ctx);
};
void CFactionDef::Transfer(CPersistContext* ctx) {
    ctx->Xfer(&this->a);
    ctx->Xfer(&this->b);
}