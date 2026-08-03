extern "C" void* memcpy(void*, const void*, unsigned int);
extern "C" void* memset(void*, int, unsigned int);

struct CBasicStringChar
{
    char* data;
    unsigned int len;

    void Reserve(unsigned int newLen);

    void __fastcall AppendData(void* /*edx_unused*/, const char* src, unsigned int addLen);
};

void __fastcall CBasicStringChar::AppendData(void* /*edx_unused*/, const char* src, unsigned int addLen)
{
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