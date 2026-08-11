#include <cstdio>

class CCharString {
public:
    char* m_data;
    CCharString(const char* s, int n) { m_data = (char*)s; (void)n; }
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName() {
    return CCharString((const char*)0x123d584, -1);
}

int main() {
    CActionDoCreatureAction a;
    CCharString r = a.GetActionName();
    if (r.m_data == (char*)0x123d584) {
        printf("OK ptr=%p\n", (void*)r.m_data);
    } else {
        printf("FAIL\n");
        return 1;
    }
    return 0;
}