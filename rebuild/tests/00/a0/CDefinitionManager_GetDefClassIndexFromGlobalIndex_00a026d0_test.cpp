#include <cstdio>

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

    long __fastcall GetDefClassIndexFromGlobalIndex(void* /*edx*/, long globalIndex)
    {
        return m_globalEntries[globalIndex]->classIndex;
    }
};

int main()
{
    CDefinitionManager mgr;
    CDefEntry e0; e0.classIndex = 111;
    CDefEntry e1; e1.classIndex = 222;
    CDefEntry e2; e2.classIndex = 333;
    CDefEntry* arr[3] = { &e0, &e1, &e2 };
    mgr.m_globalEntries = arr;

    long r0 = mgr.GetDefClassIndexFromGlobalIndex(0, 0);
    long r1 = mgr.GetDefClassIndexFromGlobalIndex(0, 1);
    long r2 = mgr.GetDefClassIndexFromGlobalIndex(0, 2);

    if (r0 == 111 && r1 == 222 && r2 == 333)
    {
        printf("GETDEFCLASSINDEX_OK\n");
    }
    else
    {
        printf("GETDEFCLASSINDEX_FAIL %ld %ld %ld\n", r0, r1, r2);
    }
    return 0;
}