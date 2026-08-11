#include <cstdio>

struct CThreadedFile;

struct CThreadedFileManager
{
    void RemovePending(CThreadedFile* f);
};

CThreadedFileManager g_ThreadedFileManager;

static int g_removeCalls = 0;

struct CThreadedFile
{
    int   field_0;
    int   field_4;
    void* handle;

    void* FinishAndGetFileHandle();
};

void CThreadedFileManager::RemovePending(CThreadedFile* /*f*/)
{
    g_removeCalls++;
}

void* CThreadedFile::FinishAndGetFileHandle()
{
    if (this->handle != (void*)-1)
    {
        g_ThreadedFileManager.RemovePending(this);
    }
    return this->handle;
}

int main()
{
    // Case 1: handle == -1  -> no RemovePending call, returns -1
    CThreadedFile a;
    a.field_0 = 1; a.field_4 = 2; a.handle = (void*)-1;
    void* r1 = a.FinishAndGetFileHandle();
    if (r1 != (void*)-1) { printf("FAIL r1\n"); return 1; }
    if (g_removeCalls != 0) { printf("FAIL rc1\n"); return 1; }

    // Case 2: handle != -1 -> RemovePending called once, returns handle
    CThreadedFile b;
    b.field_0 = 3; b.field_4 = 4; b.handle = (void*)0x1234;
    void* r2 = b.FinishAndGetFileHandle();
    if (r2 != (void*)0x1234) { printf("FAIL r2\n"); return 1; }
    if (g_removeCalls != 1) { printf("FAIL rc2\n"); return 1; }

    printf("VERIFY_OK_0098ec70\n");
    return 0;
}