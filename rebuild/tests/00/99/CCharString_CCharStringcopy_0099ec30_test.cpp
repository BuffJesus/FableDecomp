#include <cstdio>
#include <cstring>
#include <new>

extern "C" long g_CCharString_liveCount;

struct CCharString
{
    void* data;

    void ReleaseData();

    CCharString(const CCharString& rhs)
    {
        data = 0;
        ++g_CCharString_liveCount;
        if (&rhs != this)
        {
            if (data != 0)
            {
                ReleaseData();
            }
            void* rhsData = rhs.data;
            if (rhsData != 0)
            {
                data = rhsData;
                ++*(long*)((char*)rhsData + 0xd);
            }
        }
    }
};

void CCharString::ReleaseData()
{
}

long g_CCharString_liveCount = 0;

int main()
{
    unsigned char sdbuf[32];
    memset(sdbuf, 0, sizeof(sdbuf));
    *(long*)(sdbuf + 0xd) = 5;
    void* sd = sdbuf;

    unsigned char rhsRaw[64];
    memset(rhsRaw, 0xCD, sizeof(rhsRaw));
    CCharString* rhs = (CCharString*)rhsRaw;
    rhs->data = sd;

    long before = g_CCharString_liveCount;

    unsigned char dstRaw[64];
    memset(dstRaw, 0xCD, sizeof(dstRaw));
    CCharString* dst = new (dstRaw) CCharString(*rhs);

    long afterRefcount = *(long*)((unsigned char*)sd + 0xd);

    bool ok = true;
    if (dst->data != sd) ok = false;
    if (afterRefcount != 6) ok = false;
    if (g_CCharString_liveCount != before + 1) ok = false;

    if (ok)
        printf("COPY_OK_MARKER\n");
    else
        printf("COPY_FAIL data=%p refcount=%ld count=%ld\n", dst->data, afterRefcount, g_CCharString_liveCount);

    return 0;
}