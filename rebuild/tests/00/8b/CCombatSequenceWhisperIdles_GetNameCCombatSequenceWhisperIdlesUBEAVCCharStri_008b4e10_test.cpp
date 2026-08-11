#include <stdio.h>
#include <string.h>

struct CCharString {
    char* m_data;
};

static const char* g_last_str = 0;
static int g_last_len = 0;

// Local stand-in for the real ctor at 0x0099ebf0.
CCharString* __fastcall CCharString_ctor(CCharString* self, int /*edx*/, const char* str, int len)
{
    g_last_str = str;
    g_last_len = len;
    self->m_data = (char*)str;
    return self;
}

struct CCombatSequenceWhisperIdles {
    virtual CCharString GetName();
};

CCharString CCombatSequenceWhisperIdles::GetName()
{
    CCharString ret;
    CCharString_ctor(&ret, 0, (const char*)0x1278e38, -1);
    return ret;
}

int main()
{
    CCombatSequenceWhisperIdles obj;
    CCharString s = obj.GetName();

    int ok = 1;
    if (s.m_data != (char*)0x1278e38) ok = 0;
    if (g_last_str != (const char*)0x1278e38) ok = 0;
    if (g_last_len != -1) ok = 0;

    if (ok) printf("WHISPERIDLES_GETNAME_OK\n");
    else    printf("FAIL str=%p len=%d\n", (void*)s.m_data, g_last_len);
    return ok ? 0 : 1;
}