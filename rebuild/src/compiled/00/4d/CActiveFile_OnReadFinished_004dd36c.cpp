#pragma optimize("s",on)
struct CActiveFile { void* m0; };

// 0x4db9ec - this-in-ecx cleanup on the CActiveFile
extern void __fastcall CAF_cleanup(CActiveFile* self);
// 0xbfea14 - cdecl free/operator delete (single stack arg, caller pops)
extern void __cdecl CAF_free(void* p);

void __fastcall OnReadFinished_7(CActiveFile* self)
{
    CAF_cleanup(self);
    void* p = self->m0;
    if (p)
        CAF_free(p);
}