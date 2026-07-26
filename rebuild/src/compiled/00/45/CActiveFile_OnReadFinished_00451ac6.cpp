#pragma optimize("s",on)
struct CActiveFile { void* buf; };
void __cdecl engine_free(void* p);
void __fastcall CActiveFile_helper(CActiveFile* self);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* p = self->buf;
    if (p)
        engine_free(p);
}