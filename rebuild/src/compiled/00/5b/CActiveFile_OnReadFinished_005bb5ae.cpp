#pragma optimize("s",on)
extern "C" void __fastcall CActiveFile_OnReadHelper(void* self);
extern "C" void __cdecl my_free(void* p);
struct CActiveFile { void* buf; };
void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_OnReadHelper(self);
    void* p = self->buf;
    if (p)
        my_free(p);
}