#include <cstdio>
#include <cstring>

// Local standalone model mirroring s.cpp (independent behaviour test).

struct CCharString {
    char* m_buf;
    CCharString(const char* s, int len) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_buf = new char[n + 1];
        memcpy(m_buf, s, n);
        m_buf[n] = 0;
    }
};

static const char kName[] = "FodderSecondaryAttack";

struct CCombatSequence_FodderSecondaryAttack {
    void* vtbl;
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_FodderSecondaryAttack::GetName() const
{
    return CCharString(kName, -1);
}

int main() {
    CCombatSequence_FodderSecondaryAttack obj;
    obj.vtbl = 0;
    CCharString s = obj.GetName();
    if (strcmp(s.m_buf, "FodderSecondaryAttack") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_BAD\n");
    return 1;
}