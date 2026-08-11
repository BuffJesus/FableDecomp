#include <cstdio>

struct CCharString {
    char* m_data;
    CCharString(const char* s, int len);
};

__declspec(noinline) CCharString::CCharString(const char* s, int len) {
    m_data = (char*)s;
    if (len) m_data = (char*)s + 1;
}

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x40, -1);
}

int main() {
    CActionDoCreatureAction c;
    CCharString r = c.GetActionName();
    // len=-1 (nonzero) -> s+1 == 0x41
    if (r.m_data == (char*)0x41) {
        printf("GETACTIONNAME_OK\n");
        return 0;
    }
    printf("FAIL %p\n", r.m_data);
    return 1;
}