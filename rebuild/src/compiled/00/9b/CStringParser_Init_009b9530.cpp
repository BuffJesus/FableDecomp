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

    long m_unk0;             // +0x00
    char* m_buf;             // +0x04
    long m_n1;               // +0x08
    CCharString m_str;       // +0x0c .. +0x17 (embedded CCharString, 12 bytes)
    ParserCallback m_cb;     // +0x18
    unsigned char m_flag;    // +0x1c
    long m_zero;             // +0x20
    long m_n2;               // +0x24
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