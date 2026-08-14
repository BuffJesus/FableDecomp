#include <stdio.h>

enum ENavigatorType { ENT_A, ENT_B };

void __fastcall fill_ENavigatorType(ENavigatorType* first, ENavigatorType* last, const ENavigatorType* val)
{
    for (; first != last; ++first)
        *first = *val;
}

int main()
{
    ENavigatorType buf[5];
    for (int i = 0; i < 5; ++i) buf[i] = ENT_A;
    ENavigatorType v = ENT_B;
    fill_ENavigatorType(buf, buf + 5, &v);
    int ok = 1;
    for (int i = 0; i < 5; ++i) if (buf[i] != ENT_B) ok = 0;

    // empty range: no writes
    ENavigatorType buf2[2];
    buf2[0] = ENT_A; buf2[1] = ENT_A;
    fill_ENavigatorType(buf2, buf2, &v);
    if (buf2[0] != ENT_A) ok = 0;

    if (ok) printf("FILL_OK\n");
    else printf("FILL_BAD\n");
    return 0;
}