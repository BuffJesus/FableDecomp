struct CEnginePrimitiveHandle {};
struct CEnginePrimitive {};

struct CGlobalTarget {
    void RealSet(CEnginePrimitiveHandle* h, CEnginePrimitive* p, unsigned long x, int flag);
};

extern CGlobalTarget* g_target;

void __stdcall SetPersistentPrimitive(CEnginePrimitiveHandle& h,
                                      CEnginePrimitive const& p,
                                      unsigned long x)
{
    g_target->RealSet(&h, (CEnginePrimitive*)&p, x, 0);
}