#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* operator new[](unsigned int size) { return malloc(size); }
void operator delete[](void* p) { free(p); }

struct CBasicStringChar
{
    char* data;
    unsigned int len;
    unsigned int capField;

    void __fastcall SetString(void* edx_unused, unsigned int count, int ch);
};

void __fastcall CBasicStringChar::SetString(void* edx_unused, unsigned int count, int ch)
{
    (void)edx_unused;

    if (data != 0)
    {
        operator delete[](data);
        data = 0;
    }

    unsigned int allocSize = (count + 4) & ~3u;
    char* newData = (char*)operator new[](allocSize);

    unsigned int mergedCap = capField ^ ((capField ^ allocSize) & 0x7fffffff);

    data = newData;
    capField = mergedCap;

    if (count > 0)
    {
        memset(newData, ch, count);
    }

    data[count] = 0;
    len = count;
    data[count] = 0;

    unsigned int padStart = len + 1;
    unsigned int alignedPadEnd = (padStart + 3) & ~3u;
    unsigned int padCount = alignedPadEnd - padStart;
    char* padPtr = data + padStart;

    memset(padPtr, 0, padCount);
}

int main()
{
    CBasicStringChar str;
    str.data = 0;
    str.len = 0;
    str.capField = 0x80000005u;

    str.SetString(0, 12, 'A');

    int ok = 1;
    if (str.len != 12) ok = 0;
    if (str.data == 0) ok = 0;
    if (ok)
    {
        for (int i = 0; i < 12; i++)
            if (str.data[i] != 'A') ok = 0;
    }
    if (ok && str.data[12] != 0) ok = 0;

    if ((str.capField & 0x80000000u) == 0) ok = 0;
    if ((str.capField & 0x7fffffffu) != 16) ok = 0;

    unsigned int start = str.len + 1;
    unsigned int alignedEnd = (start + 3) & ~3u;
    for (unsigned int i = start; i < alignedEnd; i++)
    {
        if (str.data[i] != 0) ok = 0;
    }

    str.SetString(0, 2, 'z');
    if (str.len != 2) ok = 0;
    if (ok && str.data[0] != 'z') ok = 0;
    if (ok && str.data[1] != 'z') ok = 0;
    if (ok && str.data[2] != 0) ok = 0;

    str.SetString(0, 0, 'q');
    if (str.len != 0) ok = 0;
    if (ok && str.data[0] != 0) ok = 0;

    if (ok)
        printf("SETSTRING_TEST_OK\n");
    else
        printf("SETSTRING_TEST_FAIL\n");

    operator delete[](str.data);

    return 0;
}