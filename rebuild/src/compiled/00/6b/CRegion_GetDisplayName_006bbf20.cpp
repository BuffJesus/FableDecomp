extern const char kEmptyDisplayNameTag[];

struct CCharString {
    char* data;
    bool Compare(const char* s) const;
};

struct CRegion {
    char pad[0x18];
    CCharString nameA;
    CCharString nameB;
};

CCharString* __fastcall CRegion_GetDisplayName(CRegion* self)
{
    CCharString* pB = &self->nameB;
    if (pB->Compare(kEmptyDisplayNameTag))
        return pB;
    return &self->nameA;
}