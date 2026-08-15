// CEnginePrimitiveRenderer2D::AddPrimitive @ 0x00b4a670
// void __fastcall AddPrimitive(CEnginePrimitiveRenderer2D* this, CEngineInternalPrimitiveBase* prim)

struct CEnginePrimitiveRenderer2D {
    char pad[0x100];
};

// helper at 0xb8fdf0 is a thiscall member on the primitive taking one pointer arg.
struct CEngineInternalPrimitiveBase {
    char pad0[0x0c];
    unsigned int flags;   // +0x0c
    void Add(void* p);    // thiscall member -> this in ecx, p on stack
};

void __fastcall AddPrimitive(CEnginePrimitiveRenderer2D* self, int edx_dummy, CEngineInternalPrimitiveBase* prim)
{
    if (prim->flags & 0x400)
        prim->Add((char*)self + 0x44);
    else
        prim->Add((char*)self + 0x40);
}