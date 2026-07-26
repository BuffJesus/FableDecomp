#include <cstdio>

struct CActiveFile { void* field0; };

static int g_step_calls = 0;
static void* g_freed = 0;

void __fastcall CActiveFile_step(CActiveFile* self) { g_step_calls++; (void)self; }
void FreeThing(void* p) { g_freed = p; }

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_step(self);
    void* p = self->field0;
    if (p) {
        FreeThing(p);
    }
}

int main()
{
    int dummy = 0;
    CActiveFile a;
    a.field0 = &dummy;
    CActiveFile_OnReadFinished(&a);
    if (g_step_calls != 1) { std::printf("FAIL step not called\n"); return 1; }
    if (g_freed != &dummy) { std::printf("FAIL free not called\n"); return 1; }

    CActiveFile b;
    b.field0 = 0;
    g_step_calls = 0; g_freed = 0;
    CActiveFile_OnReadFinished(&b);
    if (g_step_calls != 1) { std::printf("FAIL step2\n"); return 1; }
    if (g_freed != 0) { std::printf("FAIL freed on null\n"); return 1; }

    std::printf("CActiveFile_004519c2_TEST PASS\n");
    return 0;
}