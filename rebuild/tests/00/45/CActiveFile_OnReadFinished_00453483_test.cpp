#include <cstdio>

struct CActiveFile {
    void* p;     // +0x0
    int field4;  // +0x4
};

static int g_cb_called = 0;
static int g_free_called = 0;
static void* g_last_free = 0;
static void* g_cb_ecx = 0;
static int g_cb_edx = 0;

int __fastcall CActiveFile_cb(void* ecx, int edx, char* arg)
{
    g_cb_called++;
    g_cb_ecx = ecx;
    g_cb_edx = edx;
    if (arg) *arg = 1;
    return 0;
}

void __cdecl CActiveFile_free(void* q)
{
    g_free_called++;
    g_last_free = q;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    char buf;
    CActiveFile_cb(self->p, self->field4, &buf);
    void* q = self->p;
    if (q)
        CActiveFile_free(q);
}

int main()
{
    // Case 1: p nonzero -> cb called, free called with p
    int dummy;
    CActiveFile a;
    a.p = &dummy;
    a.field4 = 0x1234;
    CActiveFile_OnReadFinished(&a);
    if (g_cb_called != 1) { std::printf("FAIL cb not called\n"); return 1; }
    if (g_cb_ecx != &dummy) { std::printf("FAIL cb ecx\n"); return 1; }
    if (g_cb_edx != 0x1234) { std::printf("FAIL cb edx\n"); return 1; }
    if (g_free_called != 1) { std::printf("FAIL free not called\n"); return 1; }
    if (g_last_free != &dummy) { std::printf("FAIL free arg\n"); return 1; }

    // Case 2: p zero -> cb called, free NOT called
    CActiveFile b;
    b.p = 0;
    b.field4 = 7;
    CActiveFile_OnReadFinished(&b);
    if (g_cb_called != 2) { std::printf("FAIL cb2\n"); return 1; }
    if (g_free_called != 1) { std::printf("FAIL free2 should not fire\n"); return 1; }

    std::printf("CActiveFile_00453483_TEST PASS\n");
    return 0;
}