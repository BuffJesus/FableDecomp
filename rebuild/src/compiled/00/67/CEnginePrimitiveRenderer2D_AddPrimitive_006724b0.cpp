// CEnginePrimitiveRenderer2D::AddPrimitive @ 0x006724b0
struct CEngineInternalPrimitiveBase;

struct CEnginePrimitiveRenderer2D {
    char pad00[0x18];
    int  mode;      // +0x18
    char pad1c[0x24 - 0x1c];
    void* sub24;    // +0x24
    void* sub28;    // +0x28
};

// assert/log helper at 0x404500 (stdcall, one string arg)
extern "C" void __stdcall EngineAssertLog(const char* msg);

// forwarding targets (fastcall: ecx=self, edx=prim)
extern "C" void __fastcall AddPrimitive_Path24(void* self, CEngineInternalPrimitiveBase* prim); // 0x672530
extern "C" void __fastcall AddPrimitive_Path28(void* self, CEngineInternalPrimitiveBase* prim); // 0x6724f0

void __fastcall AddPrimitive(CEnginePrimitiveRenderer2D* self, void* edx, CEngineInternalPrimitiveBase* prim)
{
    EngineAssertLog((const char*)0x122d70e);
    if (self->mode - 2 != 0) {
        AddPrimitive_Path28(self->sub28, prim);
    } else {
        AddPrimitive_Path24(self->sub24, prim);
    }
}