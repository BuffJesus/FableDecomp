struct TypeList { char data[20]; }; // sizeof 20

struct TypeArrays {
    TypeList* base;  // +0x0
    TypeList* lists; // +0x4
};

struct CThingSearchTools {
    char pad[8];
    TypeArrays* arrays; // +0x8
};

// __fastcall: this in ecx, type param on stack (ret 0x4); sizeof(list)=20
TypeList* __fastcall CThingSearchTools_PeekTypeList(CThingSearchTools* self, int /*edx*/, int type)
{
    return self->arrays->lists + (unsigned char)type;
}