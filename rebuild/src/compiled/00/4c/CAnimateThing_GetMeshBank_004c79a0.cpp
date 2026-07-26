struct CMeshDataBank;
struct SubObj { char pad[0xd0]; CMeshDataBank* bank; };
struct GlobalObj { char pad[0x20]; SubObj* sub; };
extern GlobalObj* g_animGlobal;
struct CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    return g_animGlobal->sub->bank;
}