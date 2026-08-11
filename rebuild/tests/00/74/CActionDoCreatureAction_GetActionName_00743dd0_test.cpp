#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* text, long length);
    long storage_;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

static const char* g_lastText = 0;
static long g_lastLen = 0;

// Provide the out-of-line ctor the source calls so the test links.
CCharString::CCharString(const char* text, long length)
    : storage_(0)
{
    g_lastText = text;
    g_lastLen = length;
    if (text != 0 && text[0] != '\0')
        storage_ = reinterpret_cast<long>(text);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();

    bool ok = true;
    if (r.storage_ == 0) ok = false;
    if (g_lastLen != -1) ok = false;
    if (g_lastText == 0 || std::strcmp(g_lastText, "DoCreatureAction") != 0) ok = false;

    if (ok) std::printf("00743dd0_TEST PASS\n");
    else std::printf("FAIL text=%s len=%ld storage=%ld\n",
                     g_lastText ? g_lastText : "(null)", g_lastLen,
                     r.storage_);
    return 0;
}