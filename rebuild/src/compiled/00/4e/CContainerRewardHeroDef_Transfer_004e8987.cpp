struct CPersistContext { void Xfer(void* sub); };
struct S { char pad[0x28]; void Transfer(CPersistContext* ctx); };
void S::Transfer(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }