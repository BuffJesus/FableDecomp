#include <stdio.h>
struct V12 { unsigned long a, b, c; };

V12* Fill_n(V12* dest, unsigned int count, const V12* val)
{
    for (; count > 0; --count, ++dest)
        if (dest) *dest = *val;
    return dest;
}

int main()
{
    V12 buf[4];
    for (int i = 0; i < 4; ++i) { buf[i].a = 0; buf[i].b = 0; buf[i].c = 0; }
    V12 v = { 0x11, 0x22, 0x33 };
    V12* end = Fill_n(buf, 3, &v);
    int ok = 1;
    if (end != buf + 3) ok = 0;
    for (int i = 0; i < 3; ++i)
        if (buf[i].a != 0x11 || buf[i].b != 0x22 || buf[i].c != 0x33) ok = 0;
    if (buf[3].a != 0 || buf[3].b != 0 || buf[3].c != 0) ok = 0;
    // count 0 does nothing and returns dest unchanged
    V12 buf2[1]; buf2[0].a = 5; buf2[0].b = 6; buf2[0].c = 7;
    V12* e2 = Fill_n(buf2, 0, &v);
    if (e2 != buf2) ok = 0;
    if (buf2[0].a != 5) ok = 0;
    if (ok) printf("FILLN12_OK\n");
    else printf("FAIL\n");
    return 0;
}