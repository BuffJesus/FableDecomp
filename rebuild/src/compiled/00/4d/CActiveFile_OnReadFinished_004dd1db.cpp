#pragma optimize("s",on)
struct CActiveFile { void* p0; };

void __fastcall helper_4db853(CActiveFile* self);
void __cdecl free_bfea14(void* p);

void __fastcall OnReadFinished_4(CActiveFile* self)
{
    helper_4db853(self);
    void* p = self->p0;
    if (p)
        free_bfea14(p);
}