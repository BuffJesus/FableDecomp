#include <cstdio>

struct CActiveFile {
    void* p; // +0x0
};

static int g_helper_called = 0;
static void* g_released = (void*)-1;

extern "C" void __fastcall CActiveFile_Helper(CActiveFile* self) { g_helper_called++; (void)self; }
extern "C" void __cdecl CActiveFile_Release(void* obj) { g_released = obj; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* q = self->p;
    if (q != 0)
        CActiveFile_Release(q);
}

int main()
{
    CActiveFile a;
    int dummy = 0;
    a.p = &dummy;
    g_helper_called = 0;
    g_released = (void*)-1;
    CActiveFile_OnReadFinished(&a);
    if (g_helper_called != 1) { std::printf("FAIL helper\n"); return 1; }
    if (g_released != &dummy) { std::printf("FAIL release\n"); return 1; }

    CActiveFile b;
    b.p = 0;
    g_helper_called = 0;
    g_released = (void*)-1;
    CActiveFile_OnReadFinished(&b);
    if (g_helper_called != 1) { std::printf("FAIL helper2\n"); return 1; }
    if (g_released != (void*)-1) { std::printf("FAIL release2\n"); return 1; }

    std::printf("CActiveFile_004543af_TEST PASS\n");
    return 0;
}