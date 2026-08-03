
#include <stdio.h>
#include <string.h>

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

int main()
{
    CDefEntry e0;
    e0.unused0 = (void*)0x11111111;
    e0.name = "ZeroEntry";

    CDefEntry e1;
    e1.unused0 = (void*)0x22222222;
    e1.name = "FirstEntry_Marker";

    CDefEntry e2;
    e2.unused0 = (void*)0x33333333;
    e2.name = "SecondEntry";

    CDefEntry* table[3];
    table[0] = &e0;
    table[1] = &e1;
    table[2] = &e2;

    CDefinitionManager mgr;
    memset(mgr.pad, 0, sizeof(mgr.pad));
    mgr.globalIndexTable = table;

    CDefString got = mgr.GetDefNameFromGlobalIndex(1);

    if (got.name != 0 && strcmp(got.name, "FirstEntry_Marker") == 0)
    {
        printf("TEST_PASS_GETDEFNAME_9ACCC0\n");
    }
    else
    {
        printf("TEST_FAIL got=%s\n", got.name ? got.name : "(null)");
    }

    return 0;
}