// Fill a one-byte local through a two-out-param member call, then return this.
// __fastcall this=ecx, no args (frame-pointer form: the local forces ebp).
struct T {
    void Fill(bool* a, bool* b);
    T* CopyBackBufferToTexture();
};
T* T::CopyBackBufferToTexture() {
    bool flag;
    this->Fill(&flag, &flag);
    return this;
}
