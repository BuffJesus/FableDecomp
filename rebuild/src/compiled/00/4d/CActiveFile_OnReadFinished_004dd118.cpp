#pragma optimize("s",on)
struct CActiveFile;
void __fastcall Helper_4db748(CActiveFile* self);
void __cdecl Free_bfea14(void* p);
struct CActiveFile { void* field0; };
void __fastcall OnReadFinished(CActiveFile* self)
{
    Helper_4db748(self);
    void* p = self->field0;
    if (p)
        Free_bfea14(p);
}