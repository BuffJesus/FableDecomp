#include <stdlib.h>

struct CActiveFile
{
    void* m_buffer;      // +0x00
    void Cleanup();      // 0x9aae50 (non-virtual thiscall)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    this->Cleanup();
    void* p = this->m_buffer;
    if (p)
    {
        free(p);
    }
}