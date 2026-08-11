#include <cstdio>
#include <cstring>

struct CCharString {
    char* m_pData;
    CCharString(const char* s, int n);
};

struct CActionDoCreatureAction {
    int dummy;
    CCharString GetActionName();
};

// Local stand-in for the CCharString ctor: strdup semantics, n=-1 => full string.
CCharString::CCharString(const char* s, int n) {
    int len = (n < 0) ? (int)strlen(s) : n;
    m_pData = new char[len + 1];
    memcpy(m_pData, s, len);
    m_pData[len] = 0;
}

int main() {
    CActionDoCreatureAction a; a.dummy = 123;
    CCharString r = a.GetActionName();
    if (r.m_pData && strcmp(r.m_pData, "DoCreatureAction") == 0) {
        printf("PASS_GETACTIONNAME_DoCreatureAction\n");
        return 0;
    }
    printf("FAIL got=%s\n", r.m_pData ? r.m_pData : "(null)");
    return 1;
}