#include <stdio.h>
#include <string.h>

// Standalone re-implementation matching the source's model for behaviour test.
class CCharString {
public:
    CCharString(const char* s, int len) {
        if (len < 0) len = (int)strlen(s);
        m_nLength = len;
        m_nAlloc = len + 1;
        m_pData = new char[m_nAlloc];
        memcpy(m_pData, s, len);
        m_pData[len] = 0;
    }
    CCharString(const CCharString& o) {
        m_nLength = o.m_nLength;
        m_nAlloc = o.m_nAlloc;
        m_pData = new char[m_nAlloc];
        memcpy(m_pData, o.m_pData, m_nAlloc);
    }
    ~CCharString() { delete[] m_pData; }
    const char* c_str() const { return m_pData; }
    int length() const { return m_nLength; }
private:
    char* m_pData;
    int   m_nLength;
    int   m_nAlloc;
};

class C3DMeshFileMaterialExtChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileMaterialExtChunk::GetChunkDescription() const
{
    return CCharString("MaterialExt", -1);
}

int main() {
    C3DMeshFileMaterialExtChunk chunk;
    CCharString d = chunk.GetChunkDescription();
    if (strcmp(d.c_str(), "MaterialExt") == 0 && d.length() == 11) {
        printf("GETCHUNKDESC_PASS\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}