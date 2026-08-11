#include <cstdio>
#include <cstdlib>

struct CActiveFile {
    void* stream;
    int   status;
};

static void* g_notified_stream = 0;
static int   g_notified_status = -1;
static void* g_deleted = 0;

void __fastcall CActiveFile_Notify(void* stream, int status)
{
    g_notified_stream = stream;
    g_notified_status = status;
}

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self);

int main()
{
    // Case 1: non-null stream
    void* buf = ::operator new(16);
    CActiveFile a;
    a.stream = buf;
    a.status = 42;
    CActiveFile_OnReadFinished(&a);
    if (g_notified_stream != buf) { printf("FAIL stream\n"); return 1; }
    if (g_notified_status != 42) { printf("FAIL status\n"); return 1; }

    // Case 2: null stream -> notify called, no delete
    CActiveFile b;
    b.stream = 0;
    b.status = 7;
    g_notified_status = -1;
    CActiveFile_OnReadFinished(&b);
    if (g_notified_stream != 0) { printf("FAIL stream2\n"); return 1; }
    if (g_notified_status != 7) { printf("FAIL status2\n"); return 1; }

    printf("ONREADFINISHED_OK\n");
    return 0;
}