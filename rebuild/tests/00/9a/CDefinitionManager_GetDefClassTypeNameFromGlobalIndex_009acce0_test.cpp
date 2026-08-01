#include <cstdio>
#include <cstring>

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
    CDefClassInfoRec** m_classInfoTable;

    CDefString GetDefClassTypeNameFromGlobalIndex(long index)
    {
        return CDefString(*(CDefString*)&m_classInfoTable[index]->m_typeName);
    }
};

int main()
{
    CDefClassInfoRec recA;
    recA.m_typeName = "CHERO";
    CDefClassInfoRec recB;
    recB.m_typeName = "CDOOR";

    CDefClassInfoRec* table[2];
    table[0] = &recA;
    table[1] = &recB;

    CDefinitionManager mgr;
    memset(&mgr, 0, sizeof(mgr));
    mgr.m_classInfoTable = table;

    CDefString r0 = mgr.GetDefClassTypeNameFromGlobalIndex(0);
    CDefString r1 = mgr.GetDefClassTypeNameFromGlobalIndex(1);

    if (strcmp(r0.m_str, "CHERO") == 0 && strcmp(r1.m_str, "CDOOR") == 0)
    {
        printf("TESTOK_009ACCE0\n");
        return 0;
    }

    printf("FAIL: %s %s\n", r0.m_str, r1.m_str);
    return 1;
}