// __stdcall free initialiser: zero the first 3 dword members of the argument.
#pragma pack(push,1)
struct T {
    int f0;
    int f1;
    int f2;
};
#pragma pack(pop)
extern "C" void __stdcall Init(T* p) {
    p->f0 = 0;
    p->f1 = 0;
    p->f2 = 0;
}