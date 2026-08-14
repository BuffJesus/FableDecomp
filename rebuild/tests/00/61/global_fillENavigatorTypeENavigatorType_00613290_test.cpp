#include <stdio.h>
typedef int ENavigatorType;

static void myfill(ENavigatorType* first, ENavigatorType* last, const ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}

int main()
{
    ENavigatorType buf[8];
    for (int i = 0; i < 8; ++i) buf[i] = -1;
    ENavigatorType v = 42;
    myfill(buf, buf + 5, &v);
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (buf[i] != 42) ok = 0;
    for (int i = 5; i < 8; ++i) if (buf[i] != -1) ok = 0;
    // empty-range case
    ENavigatorType b2[2] = {7, 7};
    myfill(b2, b2, &v);
    if (b2[0] != 7 || b2[1] != 7) ok = 0;
    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}