#include <stdio.h>

typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}

int main()
{
    ENavigatorType buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = -1;
    ENavigatorType v = 7;
    fill_ENavigatorType(buf, buf + 5, &v);
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (buf[i] != 7) ok = 0;
    for (int i = 5; i < 8; ++i) if (buf[i] != -1) ok = 0;
    /* empty range no-op */
    fill_ENavigatorType(buf + 5, buf + 5, &v);
    if (buf[5] != -1) ok = 0;
    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}