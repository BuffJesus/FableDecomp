#pragma optimize("s",on)
// CEngineSubPrimitiveGameMesh::`vector_deleting_destructor' @ 0x00bb1760
// Standard MSVC vector deleting destructor.

extern "C" void __fastcall SubPrim_dtor(void* self);   // 0x99a300 scalar dtor (this in ecx)
extern "C" void __fastcall SubPrim_opdelete(void* self); // 0xbeb550 operator delete path

struct CEngineSubPrimitiveGameMesh
{
    // vector deleting destructor
    void* vector_deleting_destructor(unsigned int flags)
    {
        SubPrim_dtor(this);
        if (flags & 1)
            SubPrim_opdelete(this);
        return this;
    }
};

void* run_vdd(CEngineSubPrimitiveGameMesh* p, unsigned int flags)
{
    return p->vector_deleting_destructor(flags);
}