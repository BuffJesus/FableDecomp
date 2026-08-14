// CParticleAttacherDef::Copy  0x004e299b  (VC7.1 /O2 /Oy)
// void __fastcall CParticleAttacherDef::Copy(CParticleAttacherDef*, CDefClassBase const*)
// Real member function => genuine __fastcall (this in ecx, src on stack, ret 4).

struct CDefClassBase { };

// base class Copy at 0x431f10: __fastcall base::Copy(self, const CDefClassBase* src)
struct BaseDef {
    char base_storage[0x28];   // base class occupies [0..0x28)
    void Copy(const CDefClassBase* src);
};

// sub-object at +0x28 with its own Copy at 0x4e29ba: __fastcall SubDef::Copy(self, const void* src)
struct SubDef {
    void Copy(const void* src);
};

struct CParticleAttacherDef : public BaseDef {
    SubDef sub;   // at offset 0x28 (BaseDef occupies [0..0x28))
    char pad[0x100];
    void Copy(const CDefClassBase* src);
};

void CParticleAttacherDef::Copy(const CDefClassBase* src)
{
    // base class copy (this in ecx, src pushed)
    ((BaseDef*)this)->Copy(src);
    // sub-object copy: src advanced by 0x28
    this->sub.Copy((const char*)src + 0x28);
}