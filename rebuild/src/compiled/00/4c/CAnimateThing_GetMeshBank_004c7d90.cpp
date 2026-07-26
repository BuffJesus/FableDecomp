struct CMeshDataBank;
struct SubObj { char pad[0x161c8]; CMeshDataBank* bank; };
struct RootObj { char pad[0x1c]; SubObj* sub; };
extern RootObj* g_root;
struct CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    return g_root->sub->bank;
}