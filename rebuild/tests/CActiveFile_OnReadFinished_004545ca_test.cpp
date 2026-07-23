#include <cstdio>
struct CActiveFile { void* p; };
static int g_freed = 0;
static void* g_freed_ptr = 0;
void __cdecl eng_free(void* q) { g_freed++; g_freed_ptr = q; }
// Plain-C++ replica of the two behaviours the retail 36-byte span encodes:
//   part1: OnReadFinished(this) -> if (this->p) free(this->p)
//   part2: factory -> void* m = new(0x3c); return m ? ctor(m) : 0
void __fastcall CActiveFile_OnReadFinished_beh(CActiveFile* self) {
    void* p = self->p;
    if (p) eng_free(p);
}
static char g_pool[0x3c];
static int g_ctored = 0;
void* factory_replica() {
    void* m = (void*)g_pool; // stands in for operator new(0x3c)
    if (m) { g_ctored++; return m; }
    return 0;
}
int main() {
    int dummy = 0;
    CActiveFile a; a.p = &dummy;
    CActiveFile_OnReadFinished_beh(&a);
    if (g_freed != 1 || g_freed_ptr != &dummy) { std::printf("FAIL nonnull free path\n"); return 1; }
    CActiveFile b; b.p = 0;
    CActiveFile_OnReadFinished_beh(&b);
    if (g_freed != 1) { std::printf("FAIL null path should not free\n"); return 1; }
    void* r = factory_replica();
    if (r == 0 || g_ctored != 1) { std::printf("FAIL factory path\n"); return 1; }
    std::printf("CActiveFile_004545ca_TEST PASS\n");
    return 0;
}