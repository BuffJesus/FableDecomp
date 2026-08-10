// CEnginePrimitiveRenderer::ClearAllChildPrimitives @ 0x00a0b6c0

struct Child;
struct ChildVtbl { void (__fastcall *slot0)(Child* self); };
struct Child { ChildVtbl* vtbl; };

struct CEnginePrimitiveHandle;

struct CEnginePrimitiveRenderer {
    char pad[0x10];
    Child* child;   // +0x10
};

// this in ECX, handle in EDX (both register args, no stack cleanup -> "ret")
void __fastcall ClearAllChildPrimitives(CEnginePrimitiveRenderer* self, CEnginePrimitiveHandle* /*handle*/)
{
    Child* c = self->child;
    if (c) {
        c->vtbl->slot0(c);
        self->child = 0;
    }
}