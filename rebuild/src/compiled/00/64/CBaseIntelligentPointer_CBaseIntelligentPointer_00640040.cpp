// Zero the first 2 dword members and return `this`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    int f0;
    int f1;
    T* Init();
};
#pragma pack(pop)
T* T::Init() {
    this->f0 = 0;
    this->f1 = 0;
    return this;
}