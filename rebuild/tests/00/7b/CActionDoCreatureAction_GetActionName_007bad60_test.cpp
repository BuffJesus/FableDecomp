#include <cstdio>

// Model of CCharString: records the (ptr,len) it was constructed with.
static const char* g_lastPtr = 0;
static int g_lastLen = 999;

class CCharString
{
public:
    CCharString(const char* pText, int length)
    {
        g_lastPtr = pText;
        g_lastLen = length;
    }
};

// The literal the retail function feeds to the ctor.
static const char* const kActionNameLiteral =
    reinterpret_cast<const char*>(0x0123cb08);

class CActionDoCreatureAction
{
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionNameLiteral, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    obj.GetActionName();

    bool ok = (g_lastPtr == reinterpret_cast<const char*>(0x0123cb08)) &&
              (g_lastLen == -1);

    if (ok)
        std::printf("007bad60_TEST PASS\n");
    else
        std::printf("007bad60_TEST FAIL ptr=%p len=%d\n", (void*)g_lastPtr, g_lastLen);
    return 0;
}