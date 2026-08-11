#include <stdio.h>
#include <string.h>

struct CCharStringData
{
    char* text;
    unsigned int unknown04;
    unsigned int unknown08;
    unsigned char flags0C;
    int owners;
};

class CCharString
{
public:
    CCharString(const char* text, long length);
    CCharStringData* storage_;
};

static char g_lastText[64];
static long g_lastLen = 0;

// Out-of-line ctor so GetActionName emits a real 'call' (matches retail).
CCharString::CCharString(const char* text, long length)
{
    strncpy(g_lastText, text ? text : "", sizeof(g_lastText) - 1);
    g_lastText[sizeof(g_lastText) - 1] = 0;
    g_lastLen = length;
    storage_ = 0;
}

CCharString __fastcall GetActionName();

int main()
{
    CCharString s = GetActionName();
    (void)s;
    if (strcmp(g_lastText, "DoCreatureAction") == 0 && g_lastLen == -1)
        printf("008fd7d9_TEST PASS\n");
    else
        printf("FAIL text=%s len=%ld\n", g_lastText, g_lastLen);
    return 0;
}