#include <cstdio>
#include <cstring>

extern "C" long g_charStringRefTrace = 0;
extern "C" unsigned short g_emptyWideStringData = 0;

struct CWideStringData
{
    unsigned short* data;
    long unused1;
    long unused2;
    long refCount;
};

struct CCharString
{
    CWideStringData* storage;

    CWideStringData* AllocStringData(unsigned short* s, long n);
    void UnassignString();
    CCharString* SetSubstring(CCharString* retVal, unsigned long start, unsigned long len);
};

static unsigned short g_buf[64];
static long g_freeCount = 0;
static long g_allocCount = 0;

CWideStringData* CCharString::AllocStringData(unsigned short* s, long n)
{
    ++g_allocCount;
    CWideStringData* d = new CWideStringData();
    d->data = new unsigned short[n + 1];
    for (long i = 0; i < n; ++i) d->data[i] = s[i];
    d->data[n] = 0;
    d->unused1 = 0;
    d->unused2 = 0;
    d->refCount = 0;
    return d;
}

void CCharString::UnassignString()
{
    if (storage != 0)
    {
        ++g_freeCount;
    }
    storage = 0;
}

static int g_freeDataCalls = 0;
static int g_opDeleteCalls = 0;

extern "C" void __cdecl free_00bfea14(void* p)
{
    (void)p;
    ++g_freeDataCalls;
}

extern "C" void __cdecl operator_delete_00bfe9bc(void* p)
{
    (void)p;
    ++g_opDeleteCalls;
}

CCharString* CCharString::SetSubstring(CCharString* retVal, unsigned long start, unsigned long len)
{
    ++g_charStringRefTrace;

    CWideStringData* newStorage = 0;
    CCharString localTmp;
    localTmp.storage = 0;

    CWideStringData* storage_ = this->storage;
    unsigned short* base = storage_ ? storage_->data : &g_emptyWideStringData;

    unsigned short* subPtr = base + start;
    if (subPtr != 0 && *subPtr != 0)
    {
        long subLen = (long)len - (long)start;
        newStorage = AllocStringData(subPtr, subLen);
    }

    retVal->storage = 0;
    ++g_charStringRefTrace;

    if ((void*)&localTmp != (void*)retVal)
    {
        CWideStringData* oldRetStorage = retVal->storage;
        if (newStorage != oldRetStorage)
        {
            if (oldRetStorage != 0)
            {
                retVal->UnassignString();
            }
            if (newStorage != 0)
            {
                retVal->storage = newStorage;
                ++newStorage->refCount;
            }
        }
    }

    if (newStorage != 0)
    {
        if (--newStorage->refCount <= 0)
        {
            if (newStorage->data != 0)
                free_00bfea14(newStorage->data);
            operator_delete_00bfe9bc(newStorage);
        }
    }
    --g_charStringRefTrace;

    return retVal;
}

static void setw(unsigned short* dst, const char* s)
{
    while (*s) *dst++ = (unsigned char)*s++;
    *dst = 0;
}

static int wlen(const unsigned short* s)
{
    int n = 0;
    while (s[n]) ++n;
    return n;
}

static int wcmp(const unsigned short* a, const char* b, int n)
{
    for (int i = 0; i < n; ++i)
    {
        if (a[i] != (unsigned char)b[i]) return 1;
    }
    return 0;
}

int main()
{
    setw(g_buf, "HelloWorld");
    CWideStringData sd;
    sd.data = g_buf;
    sd.unused1 = 0;
    sd.unused2 = 0;
    sd.refCount = 0;

    CCharString src;
    src.storage = &sd;

    CCharString result;
    CCharString* r = src.SetSubstring(&result, 2, 7);

    bool ok = (r == &result);
    ok = ok && (result.storage != 0);
    ok = ok && (wcmp(result.storage->data, "lloWo", 5) == 0);

    // empty-substring case: start beyond terminator
    CCharString src2;
    src2.storage = &sd;
    CCharString result2;
    result2.storage = 0;
    src2.SetSubstring(&result2, 10, 10);
    bool ok2 = (result2.storage == 0);

    // null storage case
    CCharString src3;
    src3.storage = 0;
    CCharString result3;
    result3.storage = 0;
    src3.SetSubstring(&result3, 0, 0);
    bool ok3 = (result3.storage == 0);

    if (ok && ok2 && ok3)
        std::printf("SETSUBSTRING_OK\n");
    else
        std::printf("SETSUBSTRING_FAIL ok=%d ok2=%d ok3=%d\n", ok, ok2, ok3);

    return 0;
}