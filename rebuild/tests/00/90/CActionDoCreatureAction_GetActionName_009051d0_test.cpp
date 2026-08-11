#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int len);
    const char* c_str() const { return m_data; }
private:
    char* m_data;
};

const char kActionName[] = "DoCreatureAction";

// Simple ctor: len == -1 means strlen.
CCharString::CCharString(const char* s, int len)
{
    if (len < 0) len = (int)std::strlen(s);
    static char buf[256];
    std::memcpy(buf, s, len);
    buf[len] = 0;
    m_data = buf;
}

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString(kActionName, -1);
}

int main()
{
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (std::strcmp(r.c_str(), "DoCreatureAction") == 0) {
        std::printf("GETACTIONNAME_OK\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}