#include <cstdio>

struct CActiveFile {
    void* p; // +0x0
    void* q; // +0x4
};

static int g_helper_called = 0;
static void* g_freed = (void*)0;

void __fastcall CActiveFile_Helper(CActiveFile* self)
{
    (void)self;
    g_helper_called++;
}

void __cdecl CActiveFile_FreeBuf(void* buf)
{
    g_freed = buf;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* b = self->p;
    if (b)
        CActiveFile_FreeBuf(b);
}

int main()
{
    // Case 1: p non-null -> helper called, buf freed
    CActiveFile a;
    int dummy = 7;
    a.p = &dummy;
    a.q = (void*)0;
    g_helper_called = 0;
    g_freed = (void*)0;
    CActiveFile_OnReadFinished(&a);
    if (g_helper_called != 1 || g_freed != &dummy) {
        std::printf("CActiveFile_00450dab_TEST FAIL case1\n");
        return 1;
    }

    // Case 2: p null -> helper called, no free
    CActiveFile b;
    b.p = (void*)0;
    b.q = (void*)0;
    g_helper_called = 0;
    g_freed = (void*)0;
    CActiveFile_OnReadFinished(&b);
    if (g_helper_called != 1 || g_freed != (void*)0) {
        std::printf("CActiveFile_00450dab_TEST FAIL case2\n");
        return 1;
    }

    std::printf("CActiveFile_00450dab_TEST PASS\n");
    return 0;
}