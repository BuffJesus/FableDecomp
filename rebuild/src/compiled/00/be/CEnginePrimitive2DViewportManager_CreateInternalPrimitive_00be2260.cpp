// CEnginePrimitive2DViewportManager::CreateInternalPrimitive @ 0x00be2260
// Factory: operator new(0xA0); if non-null run this-call ctor (ecx=this);
// return the ctor result (ctor returns this in eax). __fastcall modeled as
// __fastcall(self /*ecx*/, edxDummy /*edx*/, type /*stack*/) so the epilogue
// is ret 4 (one stack arg) and no tail-call/regsave is emitted.

typedef unsigned int size_t;
typedef int EEnginePrimitiveType;

struct CEnginePrimitive2DViewportManager;
struct CEngineInternalPrimitiveBase;

// this-call ctor: this in ecx, returns this in eax.
extern "C" CEngineInternalPrimitiveBase* __fastcall InternalPrimitiveCtor(CEngineInternalPrimitiveBase* self);

CEngineInternalPrimitiveBase* __fastcall CreateInternalPrimitive(
    CEnginePrimitive2DViewportManager* self, int edxDummy, EEnginePrimitiveType type)
{
    CEngineInternalPrimitiveBase* p = (CEngineInternalPrimitiveBase*)::operator new(0xA0);
    if (p != 0)
        return InternalPrimitiveCtor(p);
    return 0;
}