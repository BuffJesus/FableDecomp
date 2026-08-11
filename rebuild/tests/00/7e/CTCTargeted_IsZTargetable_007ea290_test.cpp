#include <cstdio>

struct CTCTargeted {
    char pad0[0x0c];
    unsigned int f0c;   // +0x0c
    unsigned char f10;  // +0x10
};

bool __fastcall IsZTargetable(CTCTargeted *self);

static void set(CTCTargeted &o, unsigned int f0c, unsigned char f10) {
    for (int i=0;i<0x0c;i++) o.pad0[i]=(char)0xEE;
    o.f0c=f0c; o.f10=f10;
}

int main()
{
    CTCTargeted o;
    bool ok = true;

    set(o, 0, 1);            ok &= (IsZTargetable(&o) == false);      // f0c==0
    set(o, 0x20, 0);         ok &= (IsZTargetable(&o) == false);      // f10==0
    set(o, 0x01, 1);         ok &= (IsZTargetable(&o) == false);      // bit0x20 clear
    set(o, 0x20, 1);         ok &= (IsZTargetable(&o) == true);       // all set
    set(o, 0x21, 5);         ok &= (IsZTargetable(&o) == true);       // bit set among others
    set(o, 0xFFFFFFDF, 1);   ok &= (IsZTargetable(&o) == false);      // nonzero but bit0x20 clear
    set(o, 0x120, 1);        ok &= (IsZTargetable(&o) == true);       // low byte 0x20 set

    if (ok) printf("007ea290_TEST PASS\n");
    else    printf("007ea290_TEST FAIL\n");
    return 0;
}