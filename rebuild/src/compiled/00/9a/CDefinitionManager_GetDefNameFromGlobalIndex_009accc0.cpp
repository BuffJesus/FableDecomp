
struct CDefEntry
{
    void* unused0;
    const char* name;
};

struct CDefString
{
    const char* name;

    CDefString() {}
    CDefString(const CDefString& other) : name(other.name) {}
    CDefString& operator=(const CDefString& other) { name = other.name; return *this; }
};

struct CDefinitionManager
{
    char pad[0xA4];
    CDefEntry** globalIndexTable;

    CDefString GetDefNameFromGlobalIndex(long index);
};

CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index)
{
    CDefString result;
    result.name = globalIndexTable[index]->name;
    return result;
}