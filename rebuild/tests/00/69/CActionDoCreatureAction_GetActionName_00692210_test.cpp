#include <stdio.h>
#include <string.h>

class CCharString {
public:
    CCharString(const char* s, int n) {
        m_data = (char*)s;
        m_len = (n < 0) ? (int)strlen(s) : n;
    }
    char* m_data;
    int m_len;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName() {
        return CCharString("DO_CREATURE_ACTION", -1);
    }
    int m_dummy;
};

int main() {
    CActionDoCreatureAction a;
    a.m_dummy = 0;
    CCharString r = a.GetActionName();
    int ok = (r.m_data != 0) && (strcmp(r.m_data, "DO_CREATURE_ACTION") == 0) && (r.m_len == 18);
    printf("GETNAME ok=%d len=%d str=%s\n", ok, r.m_len, r.m_data);
    if (!ok) { printf("FAIL\n"); return 1; }
    printf("PASS_GETACTIONNAME\n");
    return 0;
}