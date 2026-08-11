#include <cstdio>

struct CTCTargeted {
    int a;
    int b;
    int c;
    bool IsZTargetable();
};

bool CTCTargeted::IsZTargetable()
{
    if (a == b)
        return true;
    if (b == c)
        return true;
    if (c == a)
        return true;
    return false;
}

int main()
{
    CTCTargeted t;
    int fails = 0;

    // all distinct -> false
    t.a = 1; t.b = 2; t.c = 3;
    if (t.IsZTargetable() != false) fails++;

    // a==b -> true
    t.a = 5; t.b = 5; t.c = 9;
    if (t.IsZTargetable() != true) fails++;

    // b==c -> true
    t.a = 1; t.b = 7; t.c = 7;
    if (t.IsZTargetable() != true) fails++;

    // c==a -> true
    t.a = 4; t.b = 8; t.c = 4;
    if (t.IsZTargetable() != true) fails++;

    // all equal -> true
    t.a = 2; t.b = 2; t.c = 2;
    if (t.IsZTargetable() != true) fails++;

    if (fails == 0)
        printf("ZTARGET_OK\n");
    else
        printf("ZTARGET_FAIL %d\n", fails);
    return fails == 0 ? 0 : 1;
}