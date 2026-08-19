// OnReadFinished variant that frees this->m0 through the alternate heap helper (0xbfea14)
// with `add esp,4` cleanup. __fastcall this=ecx, no args.
struct CActiveFile { void* m0; void OnReadFinished(); void Base(); };
extern "C" void __cdecl Free2(void* p);
void CActiveFile::OnReadFinished() {
    this->Base();
    void* p = this->m0;
    if (p) Free2(p);
}
