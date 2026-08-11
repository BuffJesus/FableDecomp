#include <cstdio>
#include <cstring>

struct CCharString {
    char* m_ptr;
    CCharString(const char* s, int len);
};

struct CCombatSequence_BanditKingAdvance {
    void* vtbl;
    CCharString GetName() const;
};

// Local definition of the CCharString ctor so the test links standalone.
static char g_buf[256];
CCharString::CCharString(const char* s, int len) {
    if (len < 0) len = (int)strlen(s);
    memcpy(g_buf, s, len);
    g_buf[len] = 0;
    m_ptr = g_buf;
}

CCharString CCombatSequence_BanditKingAdvance::GetName() const {
    return CCharString("BanditKingAdvance", -1);
}

int main() {
    CCombatSequence_BanditKingAdvance obj;
    obj.vtbl = 0;
    CCharString r = obj.GetName();
    if (r.m_ptr && strcmp(r.m_ptr, "BanditKingAdvance") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}