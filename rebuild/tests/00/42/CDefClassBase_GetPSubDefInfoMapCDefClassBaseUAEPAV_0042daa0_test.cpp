// Standalone behaviour test for CDefClassBase::GetPSubDefInfoMap (0x0042daa0).
// Confirms the accessor returns a null map pointer regardless of object state.
#include <cstdio>

struct CVectorMap_ulong_CSubDefInfo;

struct CDefClassBase {
    void **vptr;
    int filler[8];
};

CVectorMap_ulong_CSubDefInfo * __fastcall
CDefClassBase_GetPSubDefInfoMap(CDefClassBase *thisptr);

int main()
{
    CDefClassBase obj;
    obj.vptr = 0;
    for (int i = 0; i < 8; ++i) obj.filler[i] = 0x11223344; // dirty state

    CVectorMap_ulong_CSubDefInfo *r = CDefClassBase_GetPSubDefInfoMap(&obj);

    if (r == 0) {
        std::printf("CDEFCLASSBASE_0042daa0_TEST PASS\n");
        return 0;
    }
    std::printf("CDEFCLASSBASE_0042daa0_TEST FAIL\n");
    return 1;
}
