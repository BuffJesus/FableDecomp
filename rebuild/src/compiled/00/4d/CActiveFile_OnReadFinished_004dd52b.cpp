#pragma optimize("s",on)
struct CActiveFile;
extern "C" void __fastcall AF_helper(CActiveFile* self);
extern "C" void __cdecl AF_free(void* p);
struct CActiveFile { void* field0; };
extern "C" void __fastcall OnReadFinished_12(CActiveFile* self)
{
    AF_helper(self);
    void* p = self->field0;
    if (p)
        AF_free(p);
}