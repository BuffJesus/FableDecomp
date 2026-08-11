// CActiveFile::OnReadFinished @ 009d7fa0

extern "C" void __cdecl EngineFree_bfea14(void* p);

struct CActiveFile {
    void* field0;
    void Helper_9d7db0();      // thiscall member @ 0x9d7db0
    void OnReadFinished();
};

void CActiveFile::OnReadFinished()
{
    Helper_9d7db0();
    void* p = field0;
    if (p != 0) {
        EngineFree_bfea14(p);
    }
}