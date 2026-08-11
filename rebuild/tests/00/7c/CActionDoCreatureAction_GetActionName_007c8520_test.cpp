#include "rebuild_abi.h"
#include <cstdio>

// Behaviour fixture for CActionDoCreatureAction::GetActionName @ 0x007c8520.
// The function constructs its CCharString result from the relocated string
// constant 0x0123d1d8 with the -1 length sentinel and returns it by value
// (hidden return pointer). We supply a stand-in CCharString ctor that records
// the forwarded arguments to prove the sentinel and the string constant.

static const char* g_seen_text;
static int g_seen_len;

struct CCharString
{
    void* m_data;
    CCharString(const char* text, int length);
};

CCharString::CCharString(const char* text, int length)
{
    g_seen_text = text;
    g_seen_len = length;
    m_data = (void*)text;
}

struct CActionDoCreatureAction
{
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123d1d8, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    if (g_seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (g_seen_text != (const char*)0x0123d1d8) { std::printf("BAD_TEXT\n"); return 1; }
    if (r.m_data != (void*)0x0123d1d8) { std::printf("BAD_DATA\n"); return 1; }
    std::printf("007c8520_TEST PASS\n");
    return 0;
}