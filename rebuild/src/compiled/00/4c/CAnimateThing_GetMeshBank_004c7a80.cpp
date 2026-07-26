struct CMeshDataBank;
struct SubObj { char pad0[0x10]; CMeshDataBank* bank; };
struct RootObj { char pad0[0x1c]; SubObj* sub; };
extern RootObj* g_animRoot;
struct CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    (void)self;
    return g_animRoot->sub->bank;
}