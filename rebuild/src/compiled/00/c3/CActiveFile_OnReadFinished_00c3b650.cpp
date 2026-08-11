// CActiveFile::OnReadFinished @ 0x00c3b650
// void __fastcall CActiveFile::OnReadFinished(CActiveFile* this)

void operator_delete_thunk(void* p);

struct CActiveFile {
    void* m_buffer; // +0x00
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (this->m_buffer != 0) {
        operator_delete_thunk(this->m_buffer);
        this->m_buffer = 0;
    }
}