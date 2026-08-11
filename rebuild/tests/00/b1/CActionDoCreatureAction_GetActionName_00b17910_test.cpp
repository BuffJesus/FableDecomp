#include <cstdio>
#include <cstring>

// Behaviour test: standalone CCharString + action, verifying GetActionName
// returns the expected action name string by value.

class CCharString {
public:
    CCharString(const char* s, int len) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_data = new char[n + 1];
        memcpy(m_data, s, n);
        m_data[n] = 0;
    }
    CCharString(const CCharString& o) {
        int n = (int)strlen(o.m_data);
        m_data = new char[n + 1];
        memcpy(m_data, o.m_data, n + 1);
    }
    ~CCharString() { delete[] m_data; }
    const char* c_str() const { return m_data; }
private:
    char* m_data;
};

class CActionDoCreatureAction {
public:
    CCharString GetActionName() const;
};

CCharString CActionDoCreatureAction::GetActionName() const {
    return CCharString("DoCreatureAction", -1);
}

int main() {
    CActionDoCreatureAction action;
    CCharString d = action.GetActionName();
    if (strcmp(d.c_str(), "DoCreatureAction") == 0) {
        printf("ACTIONNAME_OK\n");
        return 0;
    }
    printf("ACTIONNAME_FAIL\n");
    return 1;
}