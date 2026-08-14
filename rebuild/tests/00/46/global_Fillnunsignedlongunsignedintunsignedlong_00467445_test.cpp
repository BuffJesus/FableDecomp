#include <stdio.h>
#include <string.h>

struct V12 { unsigned long a, b, c; };

static V12* fill_n_model(V12 *_First, unsigned int _Count, const V12 *_Val)
{
    for (; 0 < _Count; --_Count, ++_First)
        if (_First)
            memcpy(_First, _Val, 12);
    return _First;
}

int main()
{
    V12 buf[8];
    memset(buf, 0xEE, sizeof(buf));
    V12 v = { 0x11111111u, 0x22222222u, 0x33333333u };

    V12 *ret = fill_n_model(buf, 5, &v);

    int ok = 1;
    if (ret != buf + 5) ok = 0;
    for (int i = 0; i < 5; ++i)
        if (buf[i].a != v.a || buf[i].b != v.b || buf[i].c != v.c) ok = 0;
    for (int i = 5; i < 8; ++i)
        if (buf[i].a != 0xEEEEEEEEu) ok = 0;

    V12 buf2[2];
    memset(buf2, 0x55, sizeof(buf2));
    V12 *ret2 = fill_n_model(buf2, 0, &v);
    if (ret2 != buf2) ok = 0;
    if (buf2[0].a != 0x55555555u) ok = 0;

    printf(ok ? "FILLN_OK\n" : "FILLN_BAD\n");
    return 0;
}