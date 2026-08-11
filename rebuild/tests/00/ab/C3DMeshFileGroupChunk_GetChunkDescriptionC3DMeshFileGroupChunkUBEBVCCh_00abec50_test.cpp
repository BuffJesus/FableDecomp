#include <stdio.h>
#include <string.h>

class CCharString {
public:
    CCharString(const char* str, long len);
    const char* c_str() const { return m_data; }
private:
    char* m_data;
    static char s_buf[64];
};
char CCharString::s_buf[64];

// Local definition of the ctor so the test links standalone.
CCharString::CCharString(const char* str, long len) {
    (void)len;
    strncpy(s_buf, str, 63);
    s_buf[63] = 0;
    m_data = s_buf;
}

class C3DMeshFileGroupChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileGroupChunk::GetChunkDescription() const {
    return CCharString("group", -1);
}

int main() {
    C3DMeshFileGroupChunk chunk;
    CCharString d = chunk.GetChunkDescription();
    if (strcmp(d.c_str(), "group") == 0) {
        printf("CHUNKDESC_OK\n");
        return 0;
    }
    printf("CHUNKDESC_FAIL\n");
    return 1;
}