#include <stdio.h>
#include <string.h>

// Independent behaviour test for
// C3DMeshFileHelperPointChunk::GetChunkDescription.
// We reimplement a matching CCharString + class locally so this TU
// links standalone, and assert the returned string content.

class CCharString {
public:
    CCharString(const char* s, int len) {
        if (len < 0) len = (int)strlen(s);
        m_len = len;
        for (int i = 0; i < len && i < 63; ++i) m_buf[i] = s[i];
        m_buf[(len < 63) ? len : 63] = 0;
    }
    const char* c_str() const { return m_buf; }
    int length() const { return m_len; }
private:
    char m_buf[64];
    int  m_len;
};

class C3DMeshFileHelperPointChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileHelperPointChunk::GetChunkDescription() const {
    return CCharString("PointChunk", -1);
}

int main() {
    C3DMeshFileHelperPointChunk chunk;
    CCharString d = chunk.GetChunkDescription();
    int ok = 1;
    if (strcmp(d.c_str(), "PointChunk") != 0) ok = 0;
    if (d.length() != 10) ok = 0;
    if (ok) {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("GETCHUNKDESC_FAIL\n");
    return 1;
}