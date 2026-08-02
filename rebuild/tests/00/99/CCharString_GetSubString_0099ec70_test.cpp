#include <cstdio>
#include <cstring>

extern "C" long g_charStringRefTrace = 0;
extern "C" char g_emptyCharStringData = 0;

struct CCharStringData
{
    char* data;
    long length;
    char pad[16]; // real header has more fields; refCount-ish byte lives at +0xd
};

struct CCharString
{
    CCharStringData* storage;

    CCharStringData* AllocStringData(char* data, long len);
    void UnassignString();
    CCharString* GetSubString(CCharString* retVal, unsigned long start, unsigned long len);
};

static char g_buf[64];
static long g_freeCount = 0;
static long g_allocCount = 0;

CCharStringData* CCharString::AllocStringData(char* data, long len)
{
    ++g_allocCount;
    CCharStringData* d = new CCharStringData();
    d->data = new char[len + 1];
    for (long i = 0; i < len; ++i) d->data[i] = data[i];
    d->data[len] = 0;
    d->length = len;
    for (int i = 0; i < 16; ++i) d->pad[i] = 0;
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

CCharString* CCharString::GetSubString(CCharString* retVal, unsigned long start, unsigned long len)
{
    ++g_charStringRefTrace;

    CCharString localTmp;
    localTmp.storage = 0;

    CCharStringData* storage_ = this->storage;
    char* base = storage_ ? storage_->data : &g_emptyCharStringData;

    char* subPtr = base + start;
    if (subPtr != 0 && *subPtr != 0)
    {
        long subLen = (long)len - (long)start;
        CCharStringData* newStorage = localTmp.AllocStringData(subPtr, subLen);
        localTmp.storage = newStorage;
    }

    retVal->storage = 0;
    ++g_charStringRefTrace;

    if ((void*)&localTmp != (void*)retVal)
    {
        if (retVal->storage != 0)
        {
            retVal->UnassignString();
        }
        if (localTmp.storage != 0)
        {
            retVal->storage = localTmp.storage;
            ++*((long*)((char*)localTmp.storage + 0xd));
        }
    }

    localTmp.UnassignString();
    --g_charStringRefTrace;

    return retVal;
}

int main()
{
    strcpy(g_buf, "HelloWorld");
    CCharStringData sd;
    sd.data = g_buf;
    sd.length = (long)strlen(g_buf);
    for (int i = 0; i < 16; ++i) sd.pad[i] = 0;

    CCharString src;
    src.storage = &sd;

    CCharString result;
    CCharString* r = src.GetSubString(&result, 2, 7);

    bool ok = (r == &result);
    ok = ok && (result.storage != 0);
    ok = ok && (result.storage->length == 5);
    ok = ok && (strncmp(result.storage->data, "lloWo", 5) == 0);

    // empty-substring case: start beyond terminator
    CCharString src2;
    src2.storage = &sd;
    CCharString result2;
    result2.storage = 0;
    src2.GetSubString(&result2, 10, 10);
    bool ok2 = (result2.storage == 0);

    // null storage case
    CCharString src3;
    src3.storage = 0;
    CCharString result3;
    result3.storage = 0;
    src3.GetSubString(&result3, 0, 0);
    bool ok3 = (result3.storage == 0);

    if (ok && ok2 && ok3)
        std::printf("GETSUBSTRING_OK\n");
    else
        std::printf("GETSUBSTRING_FAIL ok=%d ok2=%d ok3=%d\n", ok, ok2, ok3);

    return 0;
}