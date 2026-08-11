// CActiveFile::OnReadFinished @ 0066a1c0
extern "C" void free(void*);

void __fastcall base_read(void* self);

struct CActiveFile {
    void* buf;
};

void __fastcall OnReadFinished(CActiveFile* self)
{
    base_read(self);
    void* p = self->buf;
    if (p) {
        free(p);
    }
}