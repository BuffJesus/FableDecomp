#include <stdio.h>
#include <string.h>

struct CBasicStringChar
{
    char* data;
    unsigned int len;

    void Reserve(unsigned int newLen);

    void __fastcall AppendData(void* edx_unused, const char* src, unsigned int addLen);
};

static char g_buf[256];

void CBasicStringChar::Reserve(unsigned int newLen)
{
    // buffer already large enough for the test; keep data pointer stable.
    (void)newLen;
}

void __fastcall CBasicStringChar::AppendData(void* edx_unused, const char* src, unsigned int addLen)
{
    (void)edx_unused;
    Reserve(len + addLen);

    char* dest = data + len;
    memcpy(dest, src, addLen);
    dest[addLen] = 0;

    unsigned int newLen = len + addLen;
    len = newLen;
    data[newLen] = 0;

    unsigned int start = len;
    char* padStart = data;
    start = start + 1;
    unsigned int alignedEnd = (start + 3) & ~3u;
    unsigned int pad = alignedEnd - start;
    padStart = padStart + start;
    memset(padStart, 0, pad);
}

int main()
{
    memset(g_buf, 0xCD, sizeof(g_buf));

    CBasicStringChar str;
    str.data = g_buf;
    str.len = 5;
    memcpy(g_buf, "Hello", 5);

    str.AppendData(0, " World!", 7);

    int ok = 1;
    if (str.len != 12) ok = 0;
    if (memcmp(g_buf, "Hello World!", 12) != 0) ok = 0;
    if (g_buf[12] != 0) ok = 0;

    unsigned int start = str.len + 1;
    unsigned int alignedEnd = (start + 3) & ~3u;
    for (unsigned int i = start; i < alignedEnd; i++)
    {
        if (g_buf[i] != 0) ok = 0;
    }

    if (ok)
        printf("APPENDDATA_TEST_OK\n");
    else
        printf("APPENDDATA_TEST_FAIL\n");

    return 0;
}