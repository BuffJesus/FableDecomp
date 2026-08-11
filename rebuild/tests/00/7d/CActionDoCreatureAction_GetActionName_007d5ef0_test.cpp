#include <cstdio>

class CCharString {
public:
    CCharString(const char* str, int len);
    void* m_data;
};

// Local standalone impl of the ctor for the behaviour test.
CCharString::CCharString(const char* str, int len) {
    m_data = (void*)str;
    (void)len;
}

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x0123d550, -1);
}

int main() {
    CActionDoCreatureAction act;
    CCharString r = act.GetActionName();
    if (r.m_data == (void*)0x0123d550)
        printf("OK name=%p\n", r.m_data);
    else
        printf("FAIL name=%p\n", r.m_data);
    return 0;
}