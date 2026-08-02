#include <cstdio>

class CMeshDataBank;

struct CEngineSingleton
{
    char pad[0x24];
    struct CInner* pInner;
};

struct CInner
{
    char pad[0x3c];
    CMeshDataBank* pMeshBank;
};

extern CEngineSingleton* g_pEngineSingleton;

class CAnimateThing
{
public:
    CMeshDataBank* GetMeshBank_2();
};

CMeshDataBank* CAnimateThing::GetMeshBank_2()
{
    return g_pEngineSingleton->pInner->pMeshBank;
}

CEngineSingleton* g_pEngineSingleton = 0;

int main()
{
    CInner inner;
    char innerBuf[0x40];
    CInner* pInnerObj = (CInner*)innerBuf;
    int marker = 0x1234abcd;
    *(int*)((char*)pInnerObj + 0x3c) = marker;

    CEngineSingleton outer;
    char outerBuf[0x28];
    CEngineSingleton* pOuterObj = (CEngineSingleton*)outerBuf;
    *(CInner**)((char*)pOuterObj + 0x24) = pInnerObj;

    g_pEngineSingleton = pOuterObj;

    CAnimateThing thing;
    CMeshDataBank* result = thing.GetMeshBank_2();

    if ((int)result == marker) {
        printf("TEST_PASS_GETMESHBANK2\n");
    } else {
        printf("TEST_FAIL\n");
    }

    return 0;
}