#include <stdio.h>

struct CNearestBuildingWithProperty {
    int m0; int m1; int m2; int m3; int m4; int m5;
    CNearestBuildingWithProperty(const CNearestBuildingWithProperty& o);
};
CNearestBuildingWithProperty::CNearestBuildingWithProperty(const CNearestBuildingWithProperty& o)
{
    m0 = o.m0; m1 = o.m1; m2 = o.m2; m3 = o.m3; m4 = o.m4; m5 = o.m5;
}

int main()
{
    int srcraw[6] = {11,22,33,44,55,66};
    int raw[6]    = {-1,-1,-1,-1,-1,-1};
    void* dst  = (void*)raw;
    void* psrc = (void*)srcraw;

    void* fnaddr;
    __asm {
        lea eax, CNearestBuildingWithProperty::CNearestBuildingWithProperty
        mov fnaddr, eax
    }

    // __fastcall copy ctor: this=ecx, source ptr on stack (callee ret 4).
    __asm {
        mov  ecx, dst
        push psrc
        call fnaddr
    }

    if (raw[0]==11 && raw[1]==22 && raw[2]==33 && raw[3]==44 && raw[4]==55 && raw[5]==66) {
        printf("OK_0x0049cf80\n");
        return 0;
    }
    printf("BAD %d %d %d %d %d %d\n", raw[0],raw[1],raw[2],raw[3],raw[4],raw[5]);
    return 1;
}