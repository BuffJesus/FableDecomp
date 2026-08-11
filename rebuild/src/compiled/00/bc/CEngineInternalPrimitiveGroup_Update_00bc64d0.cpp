// CEngineInternalPrimitiveGroup::Update  @ 00bc64d0
// void __fastcall Update(CEngineInternalPrimitiveGroup*, CEnginePrimitive const&, unsigned long)

struct CEnginePrimitive {
    int pad0;   // +0x0
    int pad4;   // +0x4
    int pad8;   // +0x8
    int fieldC; // +0xc
};

// sub-object living at CEngineInternalPrimitiveGroup+0x50; the callee 0xbc5e90 is
// a __fastcall member taking one int arg.
struct CInner {
    void Add(int v);  // 0xbc5e90 — real logic elsewhere
};

struct CEngineInternalPrimitiveGroup {
    char pad_0[0x50];   // +0x00 .. +0x4f
    CInner inner;       // +0x50
    char pad_54[0x5c-0x54];
    unsigned long m_5c; // +0x5c

    void Update(const CEnginePrimitive& prim, unsigned long a3);
};

void CEngineInternalPrimitiveGroup::Update(const CEnginePrimitive& prim, unsigned long a3)
{
    m_5c = a3;
    inner.Add(prim.fieldC);
}