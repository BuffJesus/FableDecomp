#include <cstdio>
struct CMeshDataBank { int tag; };
struct SubObj { char pad[208]; CMeshDataBank* bank; };
struct GlobalObj { char pad[32]; SubObj* sub; };
GlobalObj* g_animGlobal;
struct CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    return g_animGlobal->sub->bank;
}
CMeshDataBank g_bank;
SubObj g_sub;
GlobalObj g_obj;
int main()
{
    g_bank.tag = 4660;
    g_sub.bank = &g_bank;
    g_obj.sub = &g_sub;
    g_animGlobal = &g_obj;
    CMeshDataBank* r = CAnimateThing_GetMeshBank((CAnimateThing*)0);
    if (r == &g_bank && r->tag == 4660) { std::printf("CAnimateThing_004c79a0_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}