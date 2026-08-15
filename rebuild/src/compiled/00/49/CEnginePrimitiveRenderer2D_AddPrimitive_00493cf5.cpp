// Byte-exact reconstruction of CEnginePrimitiveRenderer2D::AddPrimitive @ 0x00493cf5
struct CEnginePrimitiveRenderer2D;

// slot init @ 0x43e3d0 : __fastcall(void* slot)  -- no args
extern void __fastcall InitPrimitiveSlot(void* slot);

// grow-and-add @ 0x493c25 : __fastcall(self) -- no args
extern void __fastcall GrowAndAddPrimitive(CEnginePrimitiveRenderer2D* self);

struct CEnginePrimitiveRenderer2D {
    char* m_cur;   // +0
    char* m_pad;   // +4
    char* m_end;   // +8
};

void __fastcall AddPrimitive(CEnginePrimitiveRenderer2D* self)
{
    if (self->m_cur != self->m_end - 0x1c) {
        InitPrimitiveSlot(self->m_cur);
        self->m_cur += 0x1c;
        return;
    }
    GrowAndAddPrimitive(self);
}