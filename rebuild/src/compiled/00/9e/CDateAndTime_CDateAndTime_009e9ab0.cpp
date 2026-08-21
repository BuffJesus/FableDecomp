// Zero the first 7 dword members and return `this`. __fastcall this=ecx.
#pragma pack(push,1)
struct T {
    int f0;
    int f1;
    int f2;
    int f3;
    int f4;
    int f5;
    int f6;
    T* Init();
};
#pragma pack(pop)
T* T::Init() {
    this->f0 = 0;
    this->f1 = 0;
    this->f2 = 0;
    this->f3 = 0;
    this->f4 = 0;
    this->f5 = 0;
    this->f6 = 0;
    return this;
}