#include <cstdio>
#include <cstring>

class CCharString
{
public:
    CCharString() : data(0) {}
    CCharString(const char* text, int length);
    ~CCharString();
    CCharString& operator=(const CCharString& rhs);

    void* data;
};

static int g_ctorCalls = 0;
static const char* g_lastCtorText = 0;
static int g_lastCtorLen = -1;
static int g_dtorCalls = 0;
static int g_assignCalls = 0;
static void* g_lastAssignRhsData = 0;

CCharString::CCharString(const char* text, int length)
{
    g_ctorCalls++;
    g_lastCtorText = text;
    g_lastCtorLen = length;
    data = (void*)text;
}

CCharString::~CCharString()
{
    g_dtorCalls++;
}

CCharString& CCharString::operator=(const CCharString& rhs)
{
    g_assignCalls++;
    g_lastAssignRhsData = rhs.data;
    data = rhs.data;
    return *this;
}

struct CParsedItem
{
    long type;
    long m_pad0;
    CCharString value;
};

static bool g_findResult = false;
static long g_findNewPos = 0;
static void* g_lastTerminator = 0;

class CStringParser
{
public:
    bool FindMatchingSubstring(CCharString* terminator);
    bool ReadAsStringUntilString(CCharString* terminator, CParsedItem* item);

    long m_unk0;
    long m_pos;
    long m_pad[6];
    char* m_buffer;
};

bool CStringParser::FindMatchingSubstring(CCharString* terminator)
{
    g_lastTerminator = terminator;
    if (g_findResult)
        m_pos = g_findNewPos;
    return g_findResult;
}

bool CStringParser::ReadAsStringUntilString(CCharString* terminator, CParsedItem* item)
{
    char* scanStart = m_buffer + m_pos;

    if (FindMatchingSubstring(terminator))
    {
        long matchLen = (m_pos + (long)m_buffer) - (long)scanStart;

        CCharString slice(scanStart, matchLen);
        item->value = slice;
        item->type = 2;
        return true;
    }
    return false;
}

int main()
{
    bool ok = true;

    char buf[64];
    strcpy(buf, "hello WORLD done");

    // Case 1: match found - pos advances from 6 to 11 ("WORLD" is 5 bytes).
    {
        CStringParser p;
        p.m_unk0 = 0;
        p.m_pos = 6;
        p.m_buffer = buf;
        for (int i = 0; i < 6; i++) p.m_pad[i] = 0;

        CParsedItem item;
        item.type = 0;
        item.value.data = 0;

        CCharString terminator;
        g_findResult = true;
        g_findNewPos = 11;
        g_ctorCalls = 0; g_dtorCalls = 0; g_assignCalls = 0;

        bool r = p.ReadAsStringUntilString(&terminator, &item);

        if (!r) ok = false;
        if (item.type != 2) ok = false;
        if (g_ctorCalls != 1) ok = false;
        if (g_lastCtorLen != 5) ok = false;
        if (g_lastCtorText != buf + 6) ok = false;
        if (g_assignCalls != 1) ok = false;
        if (g_lastAssignRhsData != (void*)(buf + 6)) ok = false;
        if (g_dtorCalls != 1) ok = false;
        if (g_lastTerminator != &terminator) ok = false;
    }

    // Case 2: no match - returns false, item untouched, no CCharString built.
    {
        CStringParser p;
        p.m_unk0 = 0;
        p.m_pos = 6;
        p.m_buffer = buf;
        for (int i = 0; i < 6; i++) p.m_pad[i] = 0;

        CParsedItem item;
        item.type = 0;
        item.value.data = 0;

        CCharString terminator;
        g_findResult = false;
        g_ctorCalls = 0; g_dtorCalls = 0; g_assignCalls = 0;

        bool r = p.ReadAsStringUntilString(&terminator, &item);

        if (r) ok = false;
        if (item.type != 0) ok = false;
        if (g_ctorCalls != 0) ok = false;
        if (g_assignCalls != 0) ok = false;
    }

    if (ok)
        std::printf("TEST_OK_READASSTRINGUNTILSTRING\n");
    else
        std::printf("TEST_FAIL\n");
    return 0;
}