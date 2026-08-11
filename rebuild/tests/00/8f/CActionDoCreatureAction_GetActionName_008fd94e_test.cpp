#include <cstdio>

struct CCharString {
    int dummy;
    void ctor(const char* lit, int len);
};

static const char* g_lit = 0;
static int g_len = 0;
static CCharString* g_self = 0;

void CCharString::ctor(const char* lit, int len)
{
    g_self = this; g_lit = lit; g_len = len;
    this->dummy = 0x1234;
}

CCharString* __fastcall GetActionName(CCharString* thisret)
{
    thisret->ctor((const char*)0x1289748, -1);
    return thisret;
}

int main()
{
    CCharString obj;
    CCharString* r = GetActionName(&obj);
    if (r == &obj && g_self == &obj && g_lit == (const char*)0x1289748 && g_len == -1)
        printf("008fd94e_TEST PASS\n");
    else
        printf("FAIL self=%p lit=%p len=%d\n", g_self, g_lit, g_len);
    return 0;
}