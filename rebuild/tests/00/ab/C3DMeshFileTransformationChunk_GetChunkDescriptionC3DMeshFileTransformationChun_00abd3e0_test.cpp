#include <cstdio>
#include <cstring>

// Behaviour test: standalone CCharString + chunk, verifying
// GetChunkDescription returns the expected description string by value.

class CCharString {
public:
    CCharString(const char* s, int len) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_data = new char[n + 1];
        memcpy(m_data, s, n);
        m_data[n] = 0;
    }
    CCharString(const CCharString& o) {
        int n = (int)strlen(o.m_data);
        m_data = new char[n + 1];
        memcpy(m_data, o.m_data, n + 1);
    }
    ~CCharString() { delete[] m_data; }
    const char* c_str() const { return m_data; }
private:
    char* m_data;
};

class C3DMeshFileTransformationChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileTransformationChunk::GetChunkDescription() const {
    return CCharString("Transformation", -1);
}

int main() {
    C3DMeshFileTransformationChunk chunk;
    CCharString d = chunk.GetChunkDescription();
    if (strcmp(d.c_str(), "Transformation") == 0) {
        printf("CHUNKDESC_OK\n");
        return 0;
    }
    printf("CHUNKDESC_FAIL\n");
    return 1;
}