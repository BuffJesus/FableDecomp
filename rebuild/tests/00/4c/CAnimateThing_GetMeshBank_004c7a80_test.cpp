#include <cstdio>
struct CMeshDataBank { int marker; };
struct SubObj { char pad0[0x10]; CMeshDataBank* bank; };
struct RootObj { char pad0[0x1c]; SubObj* sub; };
RootObj* g_animRoot;
struct CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    (void)self;
    return g_animRoot->sub->bank;
}
int main(){
    static CMeshDataBank mdb; mdb.marker=0x1234;
    static SubObj s; s.bank=&mdb;
    static RootObj r; r.sub=&s;
    g_animRoot=&r;
    CMeshDataBank* got = CAnimateThing_GetMeshBank((CAnimateThing*)0);
    if(got==&mdb && got->marker==0x1234){ std::printf("CAnimateThing_004c7a80_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}