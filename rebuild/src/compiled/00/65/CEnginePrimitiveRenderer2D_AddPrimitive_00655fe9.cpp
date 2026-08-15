// CEnginePrimitiveRenderer2D::AddPrimitive @ 0x00655fe9
struct CEngineInternalPrimitiveBase;

struct CEnginePrimitiveRenderer2D {
    char *cur;    // +0
    char *pad4;   // +4
    char *end;    // +8
};

// helper at 0x99eae0: thiscall-ish, ecx = current slot pointer, void, no stack args
extern void __fastcall PrimSlotAssign(char *slot);
// grow path at 0x655ca7: thiscall, ecx = self
extern void __fastcall PrimGrow(CEnginePrimitiveRenderer2D *self);

void __fastcall AddPrimitive(CEnginePrimitiveRenderer2D *self, CEngineInternalPrimitiveBase *prim)
{
    char *e = self->end;
    char *c = self->cur;
    if (c != e - 8) {
        PrimSlotAssign(c);
        self->cur += 8;
        return;
    }
    PrimGrow(self);
}