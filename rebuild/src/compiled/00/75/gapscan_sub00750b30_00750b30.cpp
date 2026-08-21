// Zero the first 1 dword members and return `this`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    int f0;
    T* Init(int a0);
};
#pragma pack(pop)
T* T::Init(int a0) {
    this->f0 = 0;
    return this;
}