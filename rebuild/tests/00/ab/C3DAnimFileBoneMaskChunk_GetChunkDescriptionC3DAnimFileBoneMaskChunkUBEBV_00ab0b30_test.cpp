#include <cstdio>
#include <cstring>

static const char* g_lastPtr = 0;
static int g_lastLen = 0;

class CCharString {
public:
    CCharString(const char* s, int n) { g_lastPtr = s; g_lastLen = n; m_data = (char*)s; }
    const char* data() const { return m_data; }
private:
    char* m_data;
};

static const char* const kDesc = "BoneMask";

class C3DAnimFileBoneMaskChunk {
public:
    const CCharString GetChunkDescription() const { return CCharString(kDesc, -1); }
};

int main() {
    C3DAnimFileBoneMaskChunk c;
    CCharString s = c.GetChunkDescription();
    bool ok = (g_lastPtr == kDesc) && (g_lastLen == -1) && (strcmp(s.data(), "BoneMask") == 0);
    printf(ok ? "CHUNKDESC_OK\n" : "CHUNKDESC_FAIL\n");
    return ok ? 0 : 1;
}