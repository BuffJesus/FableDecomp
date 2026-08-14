struct CMatrix3x4 { char pad[64]; };

struct CEngineInternalPrimitiveAnimatedMesh {
    char pad[0x58];
    int  m58;        // +0x58
    char pad2[0x88 - 0x5c];
    char m88[0x1c];  // +0x88
    char mA4[0x1c];  // +0xa4
};

// callee at 0xbebaa0: ecx = matrix arg, edx = self+0x88, stack: self+0xa4, m58
extern "C" int __fastcall Callee_bebaa0(CMatrix3x4* mtx, void* edxArg, void* pA4, int val58);

bool __fastcall GetTransformFrameStart(CEngineInternalPrimitiveAnimatedMesh* self, void* edxDummy, CMatrix3x4& mtx)
{
    Callee_bebaa0(&mtx, self->m88, self->mA4, self->m58);
    return true;
}