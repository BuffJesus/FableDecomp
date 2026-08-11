#include <stdio.h>
#include <string.h>

// Local stand-in matching the source's CCharString layout/ctor.
class CCharString {
public:
    CCharString(const char* s, int len);
    void* buf;
};

static const char* g_lastStr = 0;
static int g_lastLen = 999;

CCharString::CCharString(const char* s, int len) {
    g_lastStr = s;
    g_lastLen = len;
    buf = (void*)s;
}

class CCombatSequence_VillagerStepIn {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_VillagerStepIn::GetName() const {
    return CCharString("Villager Step In", -1);
}

int main() {
    CCombatSequence_VillagerStepIn obj;
    CCharString r = obj.GetName();
    bool ok = (g_lastLen == -1)
           && (g_lastStr != 0)
           && (strcmp(g_lastStr, "Villager Step In") == 0)
           && (r.buf == (void*)g_lastStr);
    if (ok) {
        printf("GETNAME_VSI_OK\n");
        return 0;
    }
    printf("GETNAME_VSI_FAIL len=%d\n", g_lastLen);
    return 1;
}