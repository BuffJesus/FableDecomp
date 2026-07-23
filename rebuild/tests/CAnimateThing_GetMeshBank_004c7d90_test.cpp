#include <cstdio>
#include <cstring>
struct CMeshDataBank { int tag; };
char g_subbuf[0x161c8 + 16];
char g_rootbuf[0x1c + 16];
char* g_root;
struct CAnimateThing;
CMeshDataBank* __fastcall CAnimateThing_GetMeshBank(CAnimateThing* self)
{
    char* r = *(char**)(g_root + 0x1c);
    return *(CMeshDataBank**)(r + 0x161c8);
}
int main()
{
    static CMeshDataBank bank; bank.tag = 0x1234;
    CMeshDataBank* pb = &bank;
    std::memcpy(g_subbuf + 0x161c8, &pb, sizeof(pb));
    char* psub = g_subbuf;
    std::memcpy(g_rootbuf + 0x1c, &psub, sizeof(psub));
    g_root = g_rootbuf;
    CMeshDataBank* r = CAnimateThing_GetMeshBank((CAnimateThing*)0);
    if (r == &bank && r->tag == 0x1234) { std::printf("CAnimateThing_004c7d90_TEST PASS\n"); return 0; }
    std::printf("FAIL\n"); return 1;
}