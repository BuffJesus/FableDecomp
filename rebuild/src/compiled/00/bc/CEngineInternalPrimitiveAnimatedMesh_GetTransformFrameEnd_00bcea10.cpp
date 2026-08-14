struct CMatrix3x4;

// callee at 0xbebaa0: __fastcall(ecx=matrix, edx=self+0x94, stack: self+0xbc, field58)
extern "C" void __fastcall FrameEndCallee(CMatrix3x4* matrix, void* sub, void* p, int v);

struct Mesh {
    char pad0[0x58];
    int field58;        // +0x58
    char pad1[0x94 - 0x58 - 4];
    char field94[0x100]; // +0x94 region; +0xbc lands inside
};

// self in ecx, matrix ref on stack -> ret 4
bool __fastcall GetTransformFrameEnd(Mesh* self, void* /*edx*/, CMatrix3x4& matrix)
{
    char* base = (char*)self;
    FrameEndCallee(&matrix, base + 0x94, base + 0xbc, self->field58);
    return true;
}