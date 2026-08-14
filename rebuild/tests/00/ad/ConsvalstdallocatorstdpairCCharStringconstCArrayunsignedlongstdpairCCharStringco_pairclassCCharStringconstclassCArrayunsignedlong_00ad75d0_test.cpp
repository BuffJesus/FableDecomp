#include <stdio.h>
#include <string.h>

struct SrcPair { char first[0xc]; char second[8]; };
struct DstPair { char first[0xc]; char second[8]; };

static int g_first_calls = 0;
static int g_second_calls = 0;

static void ctor_first(char* dst, const char* src) { g_first_calls++; memcpy(dst, src, 0xc); }
static void ctor_second(char* dst, const char* src) { g_second_calls++; memcpy(dst, src, 8); }

static void cons_val(char* self, char* src)
{
    if (self != 0)
    {
        ctor_first(self, src);
        ctor_second(self + 0xc, src + 0xc);
    }
}

int main()
{
    SrcPair s;
    for (int i = 0; i < 0xc; ++i) s.first[i] = (char)(i + 1);
    for (int i = 0; i < 8; ++i) s.second[i] = (char)(0x40 + i);

    DstPair d;
    memset(&d, 0, sizeof(d));
    cons_val((char*)&d, (char*)&s);

    int ok = (g_first_calls == 1) && (g_second_calls == 1)
             && memcmp(d.first, s.first, 0xc) == 0
             && memcmp(d.second, s.second, 8) == 0;

    g_first_calls = g_second_calls = 0;
    cons_val(0, (char*)&s);
    ok = ok && (g_first_calls == 0) && (g_second_calls == 0);

    if (ok) printf("CONSVAL_OK\n");
    else printf("FAIL\n");
    return 0;
}