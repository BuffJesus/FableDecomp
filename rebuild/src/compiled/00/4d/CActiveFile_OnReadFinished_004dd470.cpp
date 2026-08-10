#pragma optimize("s",on)
struct CActiveFile { CActiveFile* f0; };

// thiscall helper (this in ecx)
extern void __fastcall sub_4dbb20(CActiveFile* self);
// cdecl free function taking pointer on stack
extern void __cdecl sub_bfea14(void* p);

void __fastcall OnReadFinished_11(CActiveFile* self)
{
    sub_4dbb20(self);
    CActiveFile* p = self->f0;
    if (p)
        sub_bfea14(p);
}