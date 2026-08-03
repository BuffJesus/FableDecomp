struct CDefEntry
{
    void* pad0;
    void* pad1;
    long classIndex;
};

class CDefinitionManager
{
public:
    char pad[0x438];
    CDefEntry** m_globalEntries;

    long __fastcall GetDefClassIndexFromGlobalIndex(void* /*edx*/, long globalIndex);
};

long __fastcall CDefinitionManager::GetDefClassIndexFromGlobalIndex(void* /*edx*/, long globalIndex)
{
    return m_globalEntries[globalIndex]->classIndex;
}