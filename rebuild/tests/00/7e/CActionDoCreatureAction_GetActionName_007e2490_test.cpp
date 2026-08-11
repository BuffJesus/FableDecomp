#include <stdio.h>
#include <string.h>

class CWideString
{
public:
    CWideString(const char* s, int len);
    void* m_data;
};

class CActionDoCreatureAction
{
public:
    CWideString GetActionName();
    void* vtbl;
};

// Local stand-in for the engine ctor at 0x99ebf0.
static char g_captured[64];
static int  g_capturedLen = 0;

CWideString::CWideString(const char* s, int len)
{
    g_capturedLen = len;
    strncpy(g_captured, s, sizeof(g_captured) - 1);
    g_captured[sizeof(g_captured) - 1] = 0;
    m_data = g_captured;
}

int main()
{
    CActionDoCreatureAction act;
    act.vtbl = 0;

    CWideString r = act.GetActionName();

    if (g_capturedLen == -1 &&
        strcmp(g_captured, "DoCreatureAction") == 0 &&
        r.m_data == g_captured)
    {
        printf("GETACTIONNAME_OK\n");
        return 0;
    }
    printf("GETACTIONNAME_FAIL len=%d s=%s\n", g_capturedLen, g_captured);
    return 1;
}