#include <stdio.h>

typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType *dest, ENavigatorType *dest_end, ENavigatorType *val)
{
    for (; dest != dest_end; ++dest)
        *dest = *val;
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

    /* empty-range: dest==dest_end must copy nothing */
    ENavigatorType b2[2]; b2[0] = 42; b2[1] = 99;
    fill_ENavigatorType(b2, b2, &v);
    if (b2[0] != 42 || b2[1] != 99) ok = 0;

    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}