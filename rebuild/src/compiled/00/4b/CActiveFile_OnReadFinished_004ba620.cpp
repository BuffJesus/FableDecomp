// CActiveFile::OnReadFinished  @ 004ba620
extern "C" void __cdecl free(void *);

struct CActiveFile;
// base OnReadFinished (0x4b8c50 shape): this-in-ecx, no other args.
extern void __fastcall BaseOnReadFinished(CActiveFile *self);

struct CActiveFile {
    void *m_buffer;   // +0x00
    void __fastcall OnReadFinished();
};

void __fastcall CActiveFile::OnReadFinished() {
    BaseOnReadFinished(this);
    void *p = this->m_buffer;
    if (p) {
        free(p);
    }
}