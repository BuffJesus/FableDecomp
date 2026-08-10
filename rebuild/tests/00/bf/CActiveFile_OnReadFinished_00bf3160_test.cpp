#include <cstdio>

struct CActiveFile {
    void* buffer;
};

static int g_freed = 0;
static void* g_freed_ptr = 0;

void __stdcall ReleaseBuffer(void* p) { g_freed++; g_freed_ptr = p; }

void __fastcall OnReadFinished(CActiveFile* self)
{
    if (self->buffer)
    {
        ReleaseBuffer(self->buffer);
        self->buffer = 0;
    }
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.buffer = &dummy;
    OnReadFinished(&a);
    bool ok1 = (g_freed == 1) && (g_freed_ptr == &dummy) && (a.buffer == 0);

    CActiveFile b;
    b.buffer = 0;
    OnReadFinished(&b);
    bool ok2 = (g_freed == 1) && (b.buffer == 0);

    if (ok1 && ok2)
        printf("00bf3160_TEST PASS\n");
    else
        printf("FAIL g_freed=%d\n", g_freed);
    return 0;
}