// CThreadedFile::FinishAndGetFileHandle  @ 0x0098ec70
// __fastcall, returns void* (the file handle stored at this+8)

struct CThreadedFile;

struct CThreadedFileManager
{
    void RemovePending(CThreadedFile* f);
};

// Global manager instance at 0x13bc870
extern CThreadedFileManager g_ThreadedFileManager;

struct CThreadedFile
{
    int   field_0;   // +0
    int   field_4;   // +4
    void* handle;    // +8

    void* FinishAndGetFileHandle();
};

void* CThreadedFile::FinishAndGetFileHandle()
{
    if (this->handle != (void*)-1)
    {
        g_ThreadedFileManager.RemovePending(this);
    }
    return this->handle;
}