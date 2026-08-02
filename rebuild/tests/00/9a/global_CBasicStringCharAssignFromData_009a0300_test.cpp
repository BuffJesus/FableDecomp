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

    void __fastcall AssignFromData(void* edx_unused, const char* src, unsigned int newLen);
};

void __fastcall CBasicStringChar::AssignFromData(void* edx_unused, const char* src, unsigned int newLen)
{
    (void)edx_unused;
    const char* srcKeep = src;

    if (data != 0)
    {
        operator delete[](data);
        data = 0;
    }

    unsigned int allocSize = (newLen + 4) & ~3u;
    char* newData = (char*)operator new[](allocSize);

    unsigned int mergedCap = capField ^ ((capField ^ allocSize) & 0x7fffffff);

    data = newData;
    capField = mergedCap;
    len = newLen;
    newData[newLen] = 0;

    unsigned int padStartOff = len;
    char* padPtr = data;
    padStartOff = padStartOff + 1;
    unsigned int alignedPadEnd = (padStartOff + 3) & ~3u;
    unsigned int padCount = alignedPadEnd - padStartOff;
    padPtr = padPtr + padStartOff;

    memset(padPtr, 0, padCount);

    memcpy(data, srcKeep, newLen);
}

int main()
{
    CBasicStringChar str;
    str.data = 0;
    str.len = 0;
    str.capField = 0x80000005u;

    str.AssignFromData(0, "Hello World!", 12);

    int ok = 1;
    if (str.len != 12) ok = 0;
    if (str.data == 0) ok = 0;
    if (ok && memcmp(str.data, "Hello World!", 12) != 0) ok = 0;
    if (ok && str.data[12] != 0) ok = 0;

    if ((str.capField & 0x80000000u) == 0) ok = 0;
    if ((str.capField & 0x7fffffffu) != 16) ok = 0;

    unsigned int start = str.len + 1;
    unsigned int alignedEnd = (start + 3) & ~3u;
    for (unsigned int i = start; i < alignedEnd; i++)
    {
        if (str.data[i] != 0) ok = 0;
    }

    str.AssignFromData(0, "Hi", 2);
    if (str.len != 2) ok = 0;
    if (ok && memcmp(str.data, "Hi", 2) != 0) ok = 0;
    if (ok && str.data[2] != 0) ok = 0;

    if (ok)
        printf("ASSIGNFROMDATA_TEST_OK\n");
    else
        printf("ASSIGNFROMDATA_TEST_FAIL\n");

    operator delete[](str.data);

    return 0;
}