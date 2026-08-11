#include <cstdio>
#include <cstring>

class CCharString {
public:
    CCharString(const char* s, int count) {
        int n = (count < 0) ? (int)strlen(s) : count;
        m_data = new char[n + 1];
        memcpy(m_data, s, n);
        m_data[n] = 0;
    }
    const char* c_str() const { return m_data; }
private:
    char* m_data;
};

class CVertexShaderConstantLayout {
public:
    CCharString PeekName();
};

CCharString CVertexShaderConstantLayout::PeekName()
{
    return CCharString("PeekName", -1);
}

int main()
{
    CVertexShaderConstantLayout layout;
    CCharString r = layout.PeekName();
    if (strcmp(r.c_str(), "PeekName") == 0) {
        printf("PEEKNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}