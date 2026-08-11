// CActiveFile::OnReadFinished @ 0x004ba720
// push esi; mov esi,ecx; call base(this); mov esi,[esi]; test esi,esi; jz end;
// push esi; call free; add esp,4; pop esi; ret

extern "C" void __cdecl free(void*);

struct CActiveFileBase {
    void* buffer;   // +0x00
};

struct CActiveFile : CActiveFileBase {
};

// base class read-finished handler @ 0x4b9da0
void __fastcall BaseReadDone(CActiveFileBase* self);

void __fastcall OnReadFinished(CActiveFile* self)
{
    BaseReadDone(self);
    if (self->buffer)
        free(self->buffer);
}