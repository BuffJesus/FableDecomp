#include <cstdio>
#include <cstring>

// Independent behaviour mirror of CEngineVSConstantLayoutParticles::PeekName.
// PeekName returns a CCharString constructed from a fixed literal with len -1.

class CCharString {
public:
    CCharString(const char* s, int len) {
        int n = (len < 0) ? (int)strlen(s) : len;
        m_len = n;
        for (int i = 0; i < n && i < 63; ++i) m_buf[i] = s[i];
        m_buf[(n < 63) ? n : 63] = '\0';
    }
    const char* c_str() const { return m_buf; }
    int length() const { return m_len; }
private:
    int  m_len;
    char m_buf[64];
};

class CEngineVSConstantLayoutParticles {
public:
    CCharString PeekName();
};

CCharString CEngineVSConstantLayoutParticles::PeekName()
{
    return CCharString("engineParticleVSConstantLayout", -1);
}

int main()
{
    CEngineVSConstantLayoutParticles obj;
    CCharString s = obj.PeekName();
    const char* expected = "engineParticleVSConstantLayout";

    bool ok = (s.length() == (int)strlen(expected))
              && (strcmp(s.c_str(), expected) == 0);

    if (ok) {
        printf("PEEKNAME_OK len=%d val=%s\n", s.length(), s.c_str());
        return 0;
    }
    printf("PEEKNAME_FAIL\n");
    return 1;
}