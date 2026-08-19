// FreeBuffer — hand the buffer back to the owner, then release it through the engine
// free helper (0xbfea14). __fastcall this=ecx, no args. The second `this->m20` read is
// faithful: the member call may clear it.
struct T {
    char pad00[0x0c];
    void* mC;
    char pad10[0x0c];
    char* m1C;
    void* m20;
    void FreeBuffer();
    void Release(void* a, void* b);
};
extern "C" void __cdecl Free2(void* p);
void T::FreeBuffer() {
    if (this->m20) {
        this->Release(this->mC, this->m1C + 4);
        if (this->m20) Free2(this->m20);
    }
}