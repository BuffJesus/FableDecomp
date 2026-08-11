// CActiveFile::OnReadFinished @ 0x0070b9e0
// 56 8bf1 e8b8feffff 8b36 85f6 7409 56 e820304f00 83c404 5e c3
// push esi; mov esi,ecx; call base(0x70b8a0); mov esi,[esi]; test esi,esi;
// jz end; push esi; call free(0xbfea14); add esp,4; end: pop esi; ret

struct CActiveFile {
    void* buffer; // +0x00
};

// base @ 0x70b8a0, thiscall (this in ecx)
extern "C" void __fastcall CActiveFile_base_Cleanup(CActiveFile* self);

// free @ 0xbfea14, cdecl, one arg, add esp,4
extern "C" void __cdecl fable_free(void* p);

void __fastcall OnReadFinished(CActiveFile* self)
{
    CActiveFile_base_Cleanup(self);
    void* p = self->buffer;
    if (p)
        fable_free(p);
}