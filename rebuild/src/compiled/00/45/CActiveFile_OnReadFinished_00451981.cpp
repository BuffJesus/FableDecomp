#pragma optimize("s",on)
struct CActiveFile;
extern "C" void __fastcall CActiveFile_helper(CActiveFile* self);
extern "C" void __cdecl free_buf(void* p);

struct CActiveFile { void* buf; };

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->buf;
    if (p)
        free_buf(p);
}