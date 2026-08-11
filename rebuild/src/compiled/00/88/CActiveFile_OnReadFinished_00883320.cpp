// CActiveFile::OnReadFinished @ 0x00883320
//   push esi; mov esi,ecx; call 0x882c00; mov esi,[esi];
//   test esi,esi; je +; push esi; call 0xbfea14; add esp,4; pop esi; ret

void __cdecl EngineFree(void* p);   // 0xbfea14 (caller-cleanup: add esp,4)

struct CActiveFile {
    void* pBuffer;          // +0x00
    void Helper();          // 0x882c00 (this-call)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    Helper();
    if (pBuffer)
        EngineFree(pBuffer);
}