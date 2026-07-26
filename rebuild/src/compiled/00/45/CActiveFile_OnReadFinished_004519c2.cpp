#pragma optimize("s",on)
struct CActiveFile { void* field0; };
extern void __fastcall CActiveFile_step(CActiveFile* self);
extern void FreeThing(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_step(self);
    void* p = self->field0;
    if (p) {
        FreeThing(p);
    }
}