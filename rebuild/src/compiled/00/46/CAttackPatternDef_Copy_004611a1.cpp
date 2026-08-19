// CObjectFamilyDef::Copy @ 0x004611d6
struct CDefClassBase;

struct SubDef {
    void Copy(const void* src);   // member @ 0x4611f5 (thiscall)
};

struct BaseDef {
    void BaseCopy(const CDefClassBase* src);  // member @ 0x431ed0 (thiscall)
};

struct CObjectFamilyDef {
    char pad[0x3c];
    SubDef sub;   // at +0x3c
};

void __fastcall CObjectFamilyDef_Copy(CObjectFamilyDef* self, int /*edx*/, const CDefClassBase* src)
{
    ((BaseDef*)self)->BaseCopy(src);
    ((SubDef*)((char*)self + 0x3c))->Copy((const char*)src + 0x3c);
}