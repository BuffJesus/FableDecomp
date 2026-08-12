#include <cstdio>

struct SubObjectAt360 {
    int base;
    int Compute(int a) { return base + a; }
};

struct CAnimationEntry {
    char pad[0x360];
    SubObjectAt360 sub;
};

struct CAnimFilterOfGroup {
    int m0;
    int m4;
    int m8;
    bool Call(CAnimationEntry* entry)
    {
        return (bool)(entry->sub.Compute(this->m4) + this->m8);
    }
};

int main()
{
    CAnimationEntry e;
    e.sub.base = 10;

    CAnimFilterOfGroup f;
    f.m0 = 0;
    f.m4 = 5;   // Compute -> 10+5 = 15
    f.m8 = -15; // 15 + (-15) = 0 -> false

    bool r1 = f.Call(&e);

    f.m8 = -14; // 15 + (-14) = 1 -> true
    bool r2 = f.Call(&e);

    if (r1 == false && r2 == true) {
        printf("FILTEROFGROUP_OP_OK\n");
        return 0;
    }
    printf("FAIL r1=%d r2=%d\n", (int)r1, (int)r2);
    return 1;
}