#pragma optimize("s",on)
struct CActiveFile { void* member0; };

// helper at 0x4db806, __fastcall(this) -> void  (single ptr arg -> ecx only)
void __fastcall CActiveFile_helper(CActiveFile* self);
// operator delete at 0xbfea14, cdecl(void*)
void __cdecl fable_delete(void* p);

void __fastcall OnReadFinished_3(CActiveFile* self)
{
    CActiveFile_helper(self);
    void* m = self->member0;
    if (m)
        fable_delete(m);
}