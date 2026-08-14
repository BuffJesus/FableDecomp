#include <stdio.h>

typedef int ENavigatorType;

void __fastcall fill_ENavigatorType(ENavigatorType *_First, ENavigatorType *_Last, const ENavigatorType *_Val)
{
    for (; _First != _Last; ++_First)
        *_First = *_Val;
}

int main()
{
    ENavigatorType buf[5];
    for (int i = 0; i < 5; ++i) buf[i] = -1;
    ENavigatorType v = 7;
    fill_ENavigatorType(buf, buf + 5, &v);
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (buf[i] != 7) ok = 0;
    ENavigatorType b2[2] = { 3, 4 };
    ENavigatorType v2 = 99;
    fill_ENavigatorType(b2, b2, &v2);
    if (b2[0] != 3 || b2[1] != 4) ok = 0;
    fill_ENavigatorType(b2, b2 + 1, &v2);
    if (b2[0] != 99 || b2[1] != 4) ok = 0;
    if (ok) printf("FILL_OK\n"); else printf("FILL_BAD\n");
    return 0;
}