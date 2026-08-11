#include <cstdio>
#include <cstring>

class CCharString {
public:
    char buf[64];
    int  len;
    CCharString(const char* s, int n) {
        std::strncpy(buf, s, 63);
        buf[63] = 0;
        len = (n < 0) ? (int)std::strlen(buf) : n;
    }
};

CCharString __fastcall GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}

int main()
{
    CCharString r = GetActionName();
    if (std::strcmp(r.buf, "DoCreatureAction") == 0 && r.len == 16) {
        std::printf("GETACTIONNAME_OK\n");
        return 0;
    }
    std::printf("FAIL\n");
    return 1;
}