#include <stdio.h>

typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}

int main()
{
    ENavigatorType buf[5];
    for (int i = 0; i < 5; ++i) buf[i] = -1;
    ENavigatorType v = 7;
    fill_ENavigatorType(buf, buf + 5, &v);
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (buf[i] != 7) ok = 0;

    // empty range: no writes
    ENavigatorType buf2[3];
    for (int i = 0; i < 3; ++i) buf2[i] = 42;
    ENavigatorType v2 = 99;
    fill_ENavigatorType(buf2, buf2, &v2);
    for (int i = 0; i < 3; ++i) if (buf2[i] != 42) ok = 0;

    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}