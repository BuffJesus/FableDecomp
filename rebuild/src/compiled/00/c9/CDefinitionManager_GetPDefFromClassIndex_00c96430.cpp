struct CDefClassBase;

struct CDefinitionManager
{
    void* vfptr;
    char pad0[0x18 - 4];
    const CDefClassBase** m_classArray; // offset 0x18
};

long __stdcall ValidateClassIndex(CDefinitionManager* mgr, long classIndex);

const CDefClassBase* __fastcall GetPDefFromClassIndex(CDefinitionManager* self, long classIndex)
{
    long idx = ValidateClassIndex(self, classIndex);
    if (idx < 0)
        return (const CDefClassBase*)idx;
    return self->m_classArray[idx];
}