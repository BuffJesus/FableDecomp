#include <cstdio>

struct Helper18
{
    int v;
    int GetValue();
};

struct Helper24
{
    int expectA;
    long expectB;
    bool Check(long b, int a);
};

struct CInputProcessGameBase
{
    char pad0[0x18];
    Helper18* m18;
    char pad1[0x24 - 0x1C];
    Helper24* m24;

    bool IsOtherQuickAccessItemButtonHeld(long param);
};

int Helper18::GetValue() { return v; }
bool Helper24::Check(long b, int a) { return a == expectA && b == expectB; }

int main()
{
    Helper18 h18; h18.v = 7;
    Helper24 h24; h24.expectA = 7; h24.expectB = 42;

    CInputProcessGameBase obj;
    obj.m18 = &h18;
    obj.m24 = &h24;

    bool r1 = obj.IsOtherQuickAccessItemButtonHeld(42);
    bool r2 = obj.IsOtherQuickAccessItemButtonHeld(99);

    if (r1 && !r2) {
        printf("QUICKACCESS_OK\n");
        return 0;
    }
    printf("QUICKACCESS_FAIL r1=%d r2=%d\n", r1, r2);
    return 1;
}