#include <cstdio>

struct CActiveFile {
    void* p;   // +0x0
    void* q;   // +0x4
};

static int g_consumed = 0;
static int g_released = 0;
static void* g_freed = 0;

void __fastcall Consume(void* a, void* b, char* out) {
    g_consumed++;
    *out = (char)0xAB;
    (void)a; (void)b;
}
void __cdecl Release(void* p) {
    g_released++;
    g_freed = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char status;
    Consume(self->p, self->q, &status);
    if (self->p)
        Release(self->p);
}

int main() {
    int dummy;
    CActiveFile a;
    a.p = &dummy;
    a.q = (void*)0x1234;
    CActiveFile_OnReadFinished(&a);
    if (g_consumed == 1 && g_released == 1 && g_freed == &dummy) {
        // null-pointer path: no release
        CActiveFile b;
        b.p = 0;
        b.q = 0;
        g_consumed = 0; g_released = 0;
        CActiveFile_OnReadFinished(&b);
        if (g_consumed == 1 && g_released == 0) {
            std::printf("CActiveFile_0045322c_TEST PASS\n");
            return 0;
        }
    }
    std::printf("FAIL\n");
    return 1;
}