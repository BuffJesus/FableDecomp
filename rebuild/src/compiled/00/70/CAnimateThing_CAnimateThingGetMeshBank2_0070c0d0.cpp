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