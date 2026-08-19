#pragma optimize("s",on)
// SetStaticCast<T> — unpack the source counted pointer and forward both words to
// ShareData (a this-in-ecx member that cleans its own two args). __fastcall this=ecx,
// src=stack (ret 4).
struct CP { void* p; void* info; };
struct T {
    void SetStaticCast(const CP* src);
    void ShareData(void* p, void* info);
};
void T::SetStaticCast(const CP* src) { this->ShareData(src->p, src->info); }
