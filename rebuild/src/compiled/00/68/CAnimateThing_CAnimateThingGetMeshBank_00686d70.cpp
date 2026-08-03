struct CMeshDataBank;

struct GlobalMeshBankInner
{
    char pad2[0x10];
    CMeshDataBank* pBank;
};

struct GlobalMeshBankHolder
{
    char pad[0x1c];
    GlobalMeshBankInner* pInner;
};

extern GlobalMeshBankHolder* g_meshBankHolder;

struct CAnimateThing
{
    CMeshDataBank* GetMeshBank();
};

CMeshDataBank* CAnimateThing::GetMeshBank()
{
    return g_meshBankHolder->pInner->pBank;
}