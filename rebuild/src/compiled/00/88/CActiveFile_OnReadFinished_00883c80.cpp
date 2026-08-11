// CActiveFile::OnReadFinished @ 0x00883c80  __fastcall void(CActiveFile*)

// cdecl free (0xbfea14)
void ExternFree(void* p);

struct CActiveFileBase {
    void BaseOnReadFinished();   // direct __fastcall target (0x882c00)
};

struct CActiveFile : public CActiveFileBase {
    void* m_ptr;   // +0x00
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    BaseOnReadFinished();
    void* p = m_ptr;
    if (p)
        ExternFree(p);
}