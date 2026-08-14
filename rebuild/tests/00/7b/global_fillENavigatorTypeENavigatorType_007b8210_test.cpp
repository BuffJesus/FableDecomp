#include <stdio.h>

typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
    {
        *first = *val;
    }
}

int main()
{
    ENavigatorType buf[5] = {0,0,0,0,0};
    ENavigatorType v = 7;
    fill_ENavigatorType(buf, buf + 5, &v);
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (buf[i] != 7) ok = 0;

    // empty range no-op
    ENavigatorType b2[1] = {42};
    fill_ENavigatorType(b2, b2, &v);
    if (b2[0] != 42) ok = 0;

    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}