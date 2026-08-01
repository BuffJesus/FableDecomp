#include <cstdio>

class CCharString {
public:
    CCharString() : data(0), len(0), capacity(0) {}
    __declspec(noinline) CCharString& operator=(const CCharString& other) { data = other.data; len = other.len; capacity = other.capacity; return *this; }
    void* data;
    long len;
    long capacity;
};

typedef void (__fastcall *ParserCallback)(void*, void*);

class CStringParser {
public:
    void Init(char* buf, ParserCallback cb, CCharString* s, long n);

    long m_unk0;
    char* m_buf;
    long m_n1;
    CCharString m_str;
    ParserCallback m_cb;
    unsigned char m_flag;
    long m_zero;
    long m_n2;
};

void CStringParser::Init(char* buf, ParserCallback cb, CCharString* s, long n)
{
    m_buf = buf;
    m_zero = 0;
    m_n2 = n;
    m_n1 = n;
    m_str = *s;
    m_cb = cb;
    m_flag = 0;
}

void __fastcall dummy_cb(void*, void*) {}

int main()
{
    CStringParser p;
    char buf[8] = "hi";
    CCharString src;
    int marker = 0x1234;
    void* markerPtr = &(marker);
    src.data = markerPtr;
    src.len = 3;
    src.capacity = 7;

    for (unsigned i = 0; i < sizeof(CStringParser); ++i)
        ((unsigned char*)&p)[i] = 0xCD;

    ParserCallback cbPtr = dummy_cb;
    p.Init(buf, cbPtr, &src, 99);

    bool ok = true;
    ok = ok && (p.m_buf == buf);
    ok = ok && (p.m_n1 == 99);
    ok = ok && (p.m_n2 == 99);
    ok = ok && (p.m_zero == 0);
    ok = ok && (p.m_flag == 0);
    ok = ok && (p.m_cb == cbPtr);
    ok = ok && (p.m_str.data == markerPtr);
    ok = ok && (p.m_str.len == 3);
    ok = ok && (p.m_str.capacity == 7);

    if (ok)
        printf("TEST_PASS_INIT_OK\n");
    else
        printf("TEST_FAIL\n");

    return 0;
}