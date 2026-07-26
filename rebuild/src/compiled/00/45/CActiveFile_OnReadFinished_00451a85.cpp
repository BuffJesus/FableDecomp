#pragma optimize("s",on)
struct CActiveFile { void* payload; };
void __fastcall CActiveFile_Prep(CActiveFile* self);
void CActiveFile_FreePayload(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_Prep(self);
    void* p = self->payload;
    if (p)
        CActiveFile_FreePayload(p);
}