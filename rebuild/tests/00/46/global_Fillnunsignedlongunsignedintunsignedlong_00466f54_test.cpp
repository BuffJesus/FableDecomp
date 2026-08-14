#include <stdio.h>
#include <string.h>

struct V12 { unsigned long a, b, c; };

void __fastcall fill_n_model(V12* _First, unsigned int _Count, const V12* _Val)
{
    for (; 0 < _Count; --_Count, ++_First)
        *_First = *_Val;
}

int main()
{
    V12 buf[8];
    memset(buf, 0xAB, sizeof(buf));
    V12 val;
    val.a = 0x11111111u; val.b = 0x22222222u; val.c = 0x33333333u;

    // fill first 5 elements
    fill_n_model(buf, 5u, &val);

    int ok = 1;
    for (int i = 0; i < 5; ++i)
        if (buf[i].a != 0x11111111u || buf[i].b != 0x22222222u || buf[i].c != 0x33333333u) ok = 0;
    // element 5 must remain untouched (0xABABABAB)
    if (buf[5].a != 0xABABABABu) ok = 0;

    // count == 0 => no writes
    V12 buf2[2];
    memset(buf2, 0xCD, sizeof(buf2));
    fill_n_model(buf2, 0u, &val);
    if (buf2[0].a != 0xCDCDCDCDu) ok = 0;

    if (ok) printf("FILLN_OK\n");
    else printf("FILLN_BAD\n");
    return 0;
}