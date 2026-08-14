// CMultiStaticMeshDef::Copy @ 0x004eb6c8
// void __fastcall Copy(CMultiStaticMeshDef* this, CDefClassBase const* other)

struct CDefClassBase;

// container member at offset +0x28 (holds a sub-object with its own Copy)
struct SubContainer {
    char pad[4];
    void Copy(const SubContainer* other);  // __fastcall
};

struct CDefClassBase {
    char pad[0x28];
    SubContainer sub;   // +0x28
    void Copy(const CDefClassBase* other);  // __fastcall base copy
};

struct CMultiStaticMeshDef {
    char pad[0x28];
    SubContainer sub;   // +0x28
};

void __fastcall Copy(CMultiStaticMeshDef* self, void* /*edx*/, const CDefClassBase* other)
{
    ((CDefClassBase*)self)->Copy(other);
    self->sub.Copy((const SubContainer*)((const char*)other + 0x28));
}