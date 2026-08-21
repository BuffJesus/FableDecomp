// Zero the first 3 dword members and return `this`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    int f0;
    int f1;
    int f2;
    T* Init(int a0);
};
#pragma pack(pop)
T* T::Init(int a0) {
    this->f0 = 0;
    this->f1 = 0;
    this->f2 = 0;
    return this;
}