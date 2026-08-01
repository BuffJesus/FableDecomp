// CDefString CDefinitionManager::GetDefClassTypeNameFromGlobalIndex(long index)
// retail 009acce0
// bytes: 8b81a40000008b4c24088b14888b0a8b4424048908c20800
//   mov eax,[ecx+0xA4]      ; this->m_classInfoTable (array of pointers, indexed by global index)
//   mov ecx,[esp+8]         ; index (explicit stack arg, pushed after the hidden return pointer)
//   mov edx,[eax+ecx*4]     ; edx = m_classInfoTable[index]  (pointer to a class-info-like record)
//   mov ecx,[edx]           ; ecx = record->m_typeName (first field, a raw name pointer)
//   mov eax,[esp+4]         ; hidden return-value pointer (struct return, non-trivial CDefString)
//   mov [eax],ecx           ; *ret = record->m_typeName
//   ret 8                   ; thiscall cleanup of the one explicit stack arg + hidden ret ptr

struct CDefString
{
    const char* m_str;

    CDefString() {}
    CDefString(const CDefString& other) : m_str(other.m_str) {}
};

struct CDefClassInfoRec
{
    const char* m_typeName;
};

struct CDefinitionManager
{
    char pad_0000[0xA4];
    CDefClassInfoRec** m_classInfoTable; // +0xA4

    CDefString GetDefClassTypeNameFromGlobalIndex(long index)
    {
        return CDefString(*(CDefString*)&m_classInfoTable[index]->m_typeName);
    }
};

// Reference the member function so it is emitted (it would otherwise have
// no callers in this translation unit and be dropped entirely).
CDefString ForceEmitHelper(CDefinitionManager* self, long index)
{
    return self->GetDefClassTypeNameFromGlobalIndex(index);
}