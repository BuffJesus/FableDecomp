extern "C" void* malloc(unsigned int size);
extern "C" void free(void* p);
extern "C" void* memset(void*, int, unsigned int);

void* operator new[](unsigned int size) { return malloc(size); }
void operator delete[](void* p) { free(p); }

struct CBasicStringChar
{
    char* data;
    unsigned int len;
    unsigned int capField;

    void __fastcall SetString(void* /*edx_unused*/, unsigned int count, int ch);
};

void __fastcall CBasicStringChar::SetString(void* /*edx_unused*/, unsigned int count, int ch)
{
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