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

static const char kName[] = "TrollBurrowPursue";

struct CCombatSequence_TrollBurrowPursue {
    void* vtbl;
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollBurrowPursue::GetName() const
{
    return CCharString(kName, -1);
}

int main() {
    CCombatSequence_TrollBurrowPursue obj;
    obj.vtbl = 0;
    CCharString s = obj.GetName();
    if (strcmp(s.m_buf, "TrollBurrowPursue") == 0) {
        printf("GETNAME_OK\n");
        return 0;
    }
    printf("GETNAME_BAD\n");
    return 1;
}