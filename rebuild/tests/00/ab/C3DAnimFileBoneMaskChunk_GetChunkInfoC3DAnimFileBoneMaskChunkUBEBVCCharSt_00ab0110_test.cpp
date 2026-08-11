#include <stdio.h>
#include <string.h>

// Local stand-in for CCharString: stores a heap copy of the C string.
class CCharString {
public:
    char* m_p;
    CCharString(const char* s, int len = -1) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_p = new char[n + 1];
        memcpy(m_p, s, n);
        m_p[n] = 0;
    }
};

// Same body shape as the target: return a fixed-literal CCharString.
class C3DAnimFileBoneMaskChunk {
public:
    CCharString GetChunkInfo() const {
        return CCharString("BoneMask");
    }
};

int main() {
    C3DAnimFileBoneMaskChunk c;
    CCharString r = c.GetChunkInfo();
    if (r.m_p && strcmp(r.m_p, "BoneMask") == 0) {
        printf("CHUNKINFO_OK\n");
        return 0;
    }
    printf("CHUNKINFO_FAIL\n");
    return 1;
}