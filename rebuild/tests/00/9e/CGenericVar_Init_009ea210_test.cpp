#include <cstdio>

class CCharString {
public:
    CCharString& operator=(const CCharString& other) {
        m_data = other.m_data;
        return *this;
    }
    char* m_data;
};

class CGenericVar {
public:
    void Init(const CCharString& value);
    int   m_pad0;
    int   m_type;
    CCharString m_value;
};

void CGenericVar::Init(const CCharString& value)
{
    m_value = value;
    m_type = 5;
}

int main()
{
    CCharString s;
    s.m_data = (char*)0xABCDEF;

    CGenericVar v;
    v.m_pad0 = 111;
    v.m_type = 0;
    v.m_value.m_data = 0;

    v.Init(s);

    bool ok = (v.m_type == 5)
           && (v.m_value.m_data == (char*)0xABCDEF)
           && (v.m_pad0 == 111);

    if (ok) {
        printf("GENERICVAR_INIT_OK\n");
        return 0;
    }
    printf("GENERICVAR_INIT_FAIL type=%d\n", v.m_type);
    return 1;
}