#include "rebuild_abi.h"
struct C {
    char a[0x14]; int f14; char b[0x1c-0x18]; int f1c; char c[0x44-0x20]; int f44;
    void InitStateGroup(int p1, int p2);
};
void C::InitStateGroup(int p1, int p2){ this->f14=0; this->f1c=-1; this->f44=-1; }