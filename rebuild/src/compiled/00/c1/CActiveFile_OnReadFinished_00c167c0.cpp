// CActiveFile::OnReadFinished @ 0x00c167c0
#include <new>

struct CActiveFile {
    void* m_buffer;   // +0x00
};

void __fastcall CActiveFile_OnReadFinished(CActiveFile* self)
{
    if (self->m_buffer != 0) {
        operator delete(self->m_buffer);
        self->m_buffer = 0;
    }
}