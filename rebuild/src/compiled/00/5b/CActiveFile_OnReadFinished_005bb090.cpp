#pragma optimize("s",on)
extern "C" void __fastcall CActiveFile_Helper(void* self);
extern "C" void __cdecl my_free(void* p);
struct CActiveFile { void* buf; };
void __fastcall OnReadFinished_4(CActiveFile* self)
{
    CActiveFile_Helper(self);
    void* p = self->buf;
    if (p)
        my_free(p);
}