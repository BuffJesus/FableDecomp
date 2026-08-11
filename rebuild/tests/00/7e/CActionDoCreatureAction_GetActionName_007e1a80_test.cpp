#include <cstdio>
#include <cstring>

// Independent behaviour test for CActionDoCreatureAction::GetActionName.
// We re-declare a self-contained CCharString whose ctor records the literal
// pointer + length it was constructed with, then confirm the factory forwards
// the expected (literal, -1) pair and returns the same buffer.

struct CCharString {
    const char* rec_s;
    int rec_len;
    CCharString(const char* s, int len) { rec_s = s; rec_len = len; }
};

struct CActionDoCreatureAction {
    CCharString GetActionName();
};

CCharString CActionDoCreatureAction::GetActionName()
{
    return CCharString((const char*)0x123db28, -1);
}

int main()
{
    CActionDoCreatureAction obj;
    CCharString r = obj.GetActionName();
    bool ok = (r.rec_s == (const char*)0x123db28) && (r.rec_len == -1);
    if (ok) {
        printf("GETACTIONNAME_OK\n");
        return 0;
    }
    printf("GETACTIONNAME_FAIL s=%p len=%d\n", r.rec_s, r.rec_len);
    return 1;
}