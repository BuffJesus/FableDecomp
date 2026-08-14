#include <stdio.h>

enum ENavigatorType { ENAV_0 };

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}

int main()
{
    ENavigatorType buf[5];
    int i;
    for (i = 0; i < 5; ++i) buf[i] = (ENavigatorType)999;
    ENavigatorType v = (ENavigatorType)7;

    /* empty range: no writes */
    fill_ENavigatorType(buf, buf, &v);
    if (buf[0] != (ENavigatorType)999) { printf("BAD\n"); return 1; }

    /* fill 5 */
    fill_ENavigatorType(buf, buf + 5, &v);
    for (i = 0; i < 5; ++i)
        if (buf[i] != (ENavigatorType)7) { printf("BAD\n"); return 1; }

    printf("FILL_OK\n");
    return 0;
}