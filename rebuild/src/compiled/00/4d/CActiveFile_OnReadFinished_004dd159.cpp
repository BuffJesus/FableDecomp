#pragma optimize("s",on)
struct CActiveFile;
extern "C" void __fastcall helper_4db795(CActiveFile* self);
extern "C" void __cdecl free_bfea14(void* p);
struct CActiveFile { void* p0; };
extern "C" void __fastcall OnReadFinished_2(CActiveFile* self)
{
    helper_4db795(self);
    void* p = self->p0;
    if (p)
        free_bfea14(p);
}