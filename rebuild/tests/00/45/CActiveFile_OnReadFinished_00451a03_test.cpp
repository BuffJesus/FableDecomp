#include <cstdio>

struct CActiveFile {
    void* field0;
};

static int g_helper_called = 0;
static void* g_released = (void*)0;

extern "C" void __fastcall CActiveFile_helper(CActiveFile* self) { (void)self; g_helper_called++; }
extern "C" void __cdecl CActiveFile_release(void* p) { g_released = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->field0;
    if (p) {
        CActiveFile_release(p);
    }
}

int main()
{
    CActiveFile a;
    int dummy = 0;
    a.field0 = &dummy;
    g_helper_called = 0; g_released = (void*)0;
    CActiveFile_OnReadFinished(&a);
    if (g_helper_called != 1 || g_released != &dummy) { std::printf("FAIL nonnull\n"); return 1; }

    CActiveFile b;
    b.field0 = (void*)0;
    g_helper_called = 0; g_released = (void*)0;
    CActiveFile_OnReadFinished(&b);
    if (g_helper_called != 1 || g_released != (void*)0) { std::printf("FAIL null\n"); return 1; }

    std::printf("CActiveFile_00451a03_TEST PASS\n");
    return 0;
}