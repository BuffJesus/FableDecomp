// CActiveFile::OnReadFinished @ 0x004b99c0
// push esi; mov esi,ecx; call base(this); esi=*(this); if(esi) free(esi); ret

struct CActiveFile {
    void* field0;
};

// base-class handler (thiscall, this in ecx) @ 0x4b8b70
void __fastcall CActiveFile_baseHandler(CActiveFile* self);

// cdecl free @ 0xbfea14
extern "C" void free_impl(void* p);

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    CActiveFile_baseHandler(self);
    void* p = *(void**)self;
    if (p)
        free_impl(p);
}