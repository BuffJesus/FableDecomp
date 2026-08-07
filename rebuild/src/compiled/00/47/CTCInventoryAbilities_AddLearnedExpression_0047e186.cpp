struct CPersistContext { void Xfer(void* sub); };
struct S { char pad[0x28]; void AddLearnedExpression(CPersistContext* ctx); };
void S::AddLearnedExpression(CPersistContext* ctx){ ctx->Xfer(reinterpret_cast<char*>(this) + 0x28); }