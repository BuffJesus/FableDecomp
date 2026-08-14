// CReplaceableMeshDef::Copy  0x004e5f51
// void __fastcall CReplaceableMeshDef::Copy(CReplaceableMeshDef* this, CDefClassBase const* src)

struct CDefClassBase;

// base class (its Copy is 0x431f10), src passed on stack (native __fastcall)
struct CDefClassBaseImpl {
    void Copy(CDefClassBase const* src);
};

// member at +0x28 (its Copy is 0x4e5f70), src passed on stack (native __fastcall)
struct Member28 {
    void Copy(Member28 const* src);
};

struct CReplaceableMeshDef : public CDefClassBaseImpl {
    // base occupies [0,0x28); member at +0x28
    char basePad[0x28];
    Member28 member; // at +0x28
};

void __fastcall CReplaceableMeshDef_Copy(CReplaceableMeshDef* self, void* /*edx*/, CDefClassBase const* src)
{
    self->CDefClassBaseImpl::Copy(src);
    self->member.Copy((Member28 const*)((char const*)src + 0x28));
}