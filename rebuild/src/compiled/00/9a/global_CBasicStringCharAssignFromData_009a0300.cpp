extern "C" void* malloc(unsigned int size);
extern "C" void free(void* p);
extern "C" void* memcpy(void*, const void*, unsigned int);
extern "C" void* memset(void*, int, unsigned int);

void* operator new[](unsigned int size) { return malloc(size); }
void operator delete[](void* p) { free(p); }

struct CBasicStringChar
{
    char* data;
    unsigned int len;
    unsigned int capField;

    void __fastcall AssignFromData(void* /*edx_unused*/, const char* src, unsigned int newLen);
};

void __fastcall CBasicStringChar::AssignFromData(void* /*edx_unused*/, const char* src, unsigned int newLen)
{
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