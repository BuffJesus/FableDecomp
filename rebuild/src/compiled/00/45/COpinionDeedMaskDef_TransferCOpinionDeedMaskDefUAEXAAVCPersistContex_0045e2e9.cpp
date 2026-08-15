#pragma optimize("s",on)
// Byte-exact reconstruction of COpinionDeedMaskDef::Transfer @ 0x0045e2e9

struct SubA;   // object at self+0x39
struct SubB;   // object at self+0x3c

// CPersistContext exposes two transfer helpers; each is a default __fastcall
// member (ctx in ecx, object pushed on the stack).
struct CPersistContext {
    void XferA(SubA* o);   // 0x43314a
    void XferB(SubB* o);   // 0x45e30a
};

struct COpinionDeedMaskDef {
    void Transfer(CPersistContext* ctx);
};

void COpinionDeedMaskDef::Transfer(CPersistContext* ctx)
{
    char* base = reinterpret_cast<char*>(this);
    ctx->XferA(reinterpret_cast<SubA*>(base + 0x39));
    ctx->XferB(reinterpret_cast<SubB*>(base + 0x3c));
}