#pragma optimize("s",on)
// Byte-exact reconstruction of
// CPersistContext_TransferableComponent<NUISystem::ETextAlignement>::TransferOut @ 0x0057f95e

struct CPersistContext;
struct CCharString;
typedef int ETextAlignement;

struct Self {
    void* vtbl;
    ETextAlignement* pValue; // field at +4: pointer to the transferable value
};

// callee @ 0x9e2560 : __fastcall(CPersistContext* ctx, CCharString* name, ETextAlignement* pval)
extern "C" void __fastcall Transfer_9e2560(CPersistContext* ctx, CCharString* name, ETextAlignement* pval);

// this in ecx, ctx+name on stack, ret 8. Model with __fastcall + dummy edx so the
// two real args land on the stack and get cleaned by the callee (retN).
void __fastcall TransferOut(Self* self, int /*edx*/, CPersistContext* ctx, CCharString* name)
{
    ETextAlignement v = *(self->pValue);
    Transfer_9e2560(ctx, name, &v);
}