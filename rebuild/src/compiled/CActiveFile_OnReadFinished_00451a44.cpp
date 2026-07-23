#pragma optimize("s",on)
struct CActiveFile { void* p; };
extern void __fastcall CActiveFile_Notify(CActiveFile* self);
extern void __cdecl FreeBuffer(void* p);
void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Notify(self);
    void* q = self->p;
    if (q)
        FreeBuffer(q);
}