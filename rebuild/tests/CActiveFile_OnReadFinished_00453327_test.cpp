#include <cstdio>

struct CActiveFile {
    void* p;    // +0x0
    void* f4;   // +0x4
};

static int g_reader_calls = 0;
static int g_free_calls = 0;
static void* g_last_free = 0;
static void* g_last_self = 0;
static void* g_last_edx = 0;
static char* g_last_buf = 0;

void __fastcall Reader_Op(void* self, void* edx, char* buf)
{
    g_reader_calls++;
    g_last_self = self;
    g_last_edx = edx;
    g_last_buf = buf;
}

void FreeThing(void* p)
{
    g_free_calls++;
    g_last_free = p;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char local;
    Reader_Op(self->p, self->f4, &local);
    if (self->p) {
        FreeThing(self->p);
    }
}

int main()
{
    // Case 1: p non-null -> reader called and free called
    int dummyReader;
    int dummyF4;
    CActiveFile a;
    a.p = &dummyReader;
    a.f4 = &dummyF4;
    g_reader_calls = g_free_calls = 0;
    CActiveFile_OnReadFinished(&a);
    if (g_reader_calls != 1) { std::printf("FAIL reader not called\n"); return 1; }
    if (g_last_self != &dummyReader) { std::printf("FAIL self mismatch\n"); return 1; }
    if (g_last_edx != &dummyF4) { std::printf("FAIL edx mismatch\n"); return 1; }
    if (g_free_calls != 1) { std::printf("FAIL free not called\n"); return 1; }
    if (g_last_free != &dummyReader) { std::printf("FAIL free arg mismatch\n"); return 1; }

    // Case 2: p null -> reader called but free NOT called
    CActiveFile b;
    b.p = 0;
    b.f4 = 0;
    g_reader_calls = g_free_calls = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_reader_calls != 1) { std::printf("FAIL reader not called (null)\n"); return 1; }
    if (g_free_calls != 0) { std::printf("FAIL free called on null\n"); return 1; }

    std::printf("CActiveFile_00453327_TEST PASS\n");
    return 0;
}