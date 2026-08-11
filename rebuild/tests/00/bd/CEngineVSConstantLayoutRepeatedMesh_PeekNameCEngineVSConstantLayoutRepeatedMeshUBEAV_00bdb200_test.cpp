#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int n);
    const char* c_str() const { return m_data; }
    int len() const { return m_len; }
    char* m_data;
    int   m_len;
};

// local stand-in for the engine ctor: n==-1 means strlen
CCharString::CCharString(const char* s, int n) {
    m_len = (n < 0) ? (int)strlen(s) : n;
    m_data = new char[m_len + 1];
    memcpy(m_data, s, m_len);
    m_data[m_len] = 0;
}

class CEngineVSConstantLayoutRepeatedMesh {
public:
    virtual CCharString PeekName() const;
    void* vfp;
};

CCharString CEngineVSConstantLayoutRepeatedMesh::PeekName() const {
    return CCharString("VSConstantLayoutRepeatedMesh", -1);
}

int main() {
    CEngineVSConstantLayoutRepeatedMesh obj;
    obj.vfp = 0;
    CCharString r = obj.PeekName();
    if (strcmp(r.c_str(), "VSConstantLayoutRepeatedMesh") == 0 && r.len() == 28) {
        printf("PEEKNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}