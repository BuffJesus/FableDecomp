#include <cstdio>
#include <cstring>

// Mirror the source's declared types.
struct CCharString
{
    void* buf;
    CCharString(const char* s, int len);
};

// Local stand-in for the retail ctor 0x0099ebf0: record the args so the test
// can prove GetName built the string from the fixed literal with len -1.
static const char* g_lastPtr = 0;
static int g_lastLen = 999;

CCharString::CCharString(const char* s, int len)
{
    g_lastPtr = s;
    g_lastLen = len;
    this->buf = (void*)s;
}

struct CCombatSequenceWhisperSortYourselfOut
{
    void* vfptr;
    CCharString GetName() const;
};

CCharString CCombatSequenceWhisperSortYourselfOut::GetName() const
{
    return CCharString((const char*)0x1278de4, -1);
}

int main()
{
    CCombatSequenceWhisperSortYourselfOut obj;
    obj.vfptr = 0;
    CCharString r = obj.GetName();

    bool ok = (g_lastPtr == (const char*)0x1278de4)
           && (g_lastLen == -1)
           && (r.buf == (void*)0x1278de4);

    if (ok)
        printf("WHISPER_GETNAME_OK\n");
    else
        printf("FAIL ptr=%p len=%d\n", (void*)g_lastPtr, g_lastLen);
    return ok ? 0 : 1;
}