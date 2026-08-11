// CActiveFile::OnReadFinished  @ 0x00a0a840  __fastcall void(CActiveFile*)
// push esi; mov esi,ecx; call Notify(); mov esi,[esi]; if(esi) free(esi);

extern "C" void __cdecl freX(void*);   // -> 0xbfea14 (operator delete / free)

struct CActiveFile {
    void* m_buf;            // +0x00
    void Notify();          // -> 0xa0a720 (defined elsewhere -> real call)
    void OnReadFinished();
};

void CActiveFile::OnReadFinished() {
    Notify();
    void* p = m_buf;
    if (p) {
        freX(p);
    }
}