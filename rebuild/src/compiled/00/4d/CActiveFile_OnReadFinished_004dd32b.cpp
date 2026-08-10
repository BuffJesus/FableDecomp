#pragma optimize("s",on)
struct CActiveFile { void* p0; };

// 0x4db99f : base OnReadFinished, __fastcall (ecx=this)
extern "C" void __fastcall base_ORF(CActiveFile* self);
// 0xbfea14 : operator delete(void*), __cdecl
extern "C" void free_thing(void* p);

void __fastcall OnReadFinished_6(CActiveFile* self)
{
    base_ORF(self);
    void* p = self->p0;
    if (p != 0)
        free_thing(p);
}