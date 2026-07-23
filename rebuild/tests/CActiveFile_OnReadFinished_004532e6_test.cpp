#include <cstdio>

struct CInner { int calls; int edxseen; };
static CInner* g_deleted = 0;
static int g_processCalls = 0;
static int g_edxSeen = 0;

void __fastcall Inner_Process(CInner* self, int edxparam, char* p) {
    g_processCalls++;
    g_edxSeen = edxparam;
    if (self) self->calls++;
    *p = 1;
}
void __cdecl op_delete(void* p) { g_deleted = (CInner*)p; }

struct CActiveFile {
    CInner* p;   /* +0x0 */
    int     f4;  /* +0x4 */
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Inner_Process(self->p, self->f4, &local);
    CInner* q = self->p;
    if (q)
        op_delete(q);
}

int main() {
    CInner inner; inner.calls = 0; inner.edxseen = 0;
    CActiveFile a; a.p = &inner; a.f4 = 0x1234;
    CActiveFile_OnReadFinished(&a);
    if (g_processCalls != 1) { std::printf("FAIL processCalls\n"); return 1; }
    if (g_edxSeen != 0x1234) { std::printf("FAIL edx\n"); return 1; }
    if (inner.calls != 1) { std::printf("FAIL innercall\n"); return 1; }
    if (g_deleted != &inner) { std::printf("FAIL delete\n"); return 1; }

    /* null branch: no delete */
    g_deleted = 0; g_processCalls = 0;
    CActiveFile b; b.p = 0; b.f4 = 7;
    CActiveFile_OnReadFinished(&b);
    if (g_processCalls != 1) { std::printf("FAIL processCalls2\n"); return 1; }
    if (g_deleted != 0) { std::printf("FAIL nulldelete\n"); return 1; }

    std::printf("CActiveFile_004532e6_TEST PASS\n");
    return 0;
}