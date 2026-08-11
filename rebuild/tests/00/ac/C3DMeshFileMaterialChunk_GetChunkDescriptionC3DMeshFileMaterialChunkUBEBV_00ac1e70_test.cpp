#include <stdio.h>
#include <string.h>

class CCharString {
public:
    CCharString(const char* s, int n) {
        int len = (n < 0) ? (int)strlen(s) : n;
        m_p = new char[len + 1];
        memcpy(m_p, s, len);
        m_p[len] = 0;
    }
    CCharString(const CCharString& o) {
        int len = (int)strlen(o.m_p);
        m_p = new char[len + 1];
        memcpy(m_p, o.m_p, len + 1);
    }
    ~CCharString() { delete[] m_p; }
    const char* c_str() const { return m_p; }
private:
    char* m_p;
};

class C3DMeshFileMaterialChunk {
public:
    virtual CCharString GetChunkDescription() const;
};

CCharString C3DMeshFileMaterialChunk::GetChunkDescription() const
{
    return CCharString("MaterialChunk", -1);
}

int main() {
    C3DMeshFileMaterialChunk c;
    CCharString s = c.GetChunkDescription();
    if (strcmp(s.c_str(), "MaterialChunk") == 0) {
        printf("GETCHUNKDESC_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}