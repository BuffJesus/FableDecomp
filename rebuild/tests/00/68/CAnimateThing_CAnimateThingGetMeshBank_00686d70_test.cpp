#include <cstdio>

struct CMeshDataBank { int tag; };

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

GlobalMeshBankHolder* g_meshBankHolder;

struct CAnimateThing
{
    CMeshDataBank* GetMeshBank();
};

CMeshDataBank* CAnimateThing::GetMeshBank()
{
    return g_meshBankHolder->pInner->pBank;
}

int main()
{
    CMeshDataBank bank;
    bank.tag = 12345;

    GlobalMeshBankInner inner;
    inner.pBank = &bank;

    GlobalMeshBankHolder holder;
    holder.pInner = &inner;

    g_meshBankHolder = &holder;

    CAnimateThing thing;

    CMeshDataBank* result = thing.GetMeshBank();

    if (result == &bank && result->tag == 12345)
    {
        printf("PASS_GETMESHBANK_00686d70\n");
    }
    else
    {
        printf("FAIL result=%p expected=%p\n", (void*)result, (void*)&bank);
    }

    return 0;
}