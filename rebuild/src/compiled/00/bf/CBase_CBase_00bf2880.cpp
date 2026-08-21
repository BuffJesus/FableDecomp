// Zero the first 1 dword members and return `this`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    int f0;
    T* Init();
};
#pragma pack(pop)
T* T::Init() {
    this->f0 = 0;
    return this;
}