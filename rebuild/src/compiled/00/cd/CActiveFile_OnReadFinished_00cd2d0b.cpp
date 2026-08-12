// CActiveFile::OnReadFinished  0x00cd2d0b
// mov eax,[ecx]; test eax,eax; je end; push eax; call helper; pop ecx; ret

void __cdecl CActiveFile_helper(void* p);

struct CActiveFile {
    void* m_pReader;   // +0x00
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    if (m_pReader != 0)
        CActiveFile_helper(m_pReader);
}