extern "C" long g_CCharString_liveCount;

struct CCharString
{
    void* data;

    void ReleaseData();

    CCharString(const CCharString& rhs);
};

long g_CCharString_liveCount = 0;

CCharString::CCharString(const CCharString& rhs)
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