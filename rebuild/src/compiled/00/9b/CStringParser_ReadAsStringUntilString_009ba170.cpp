// CStringParser::ReadAsStringUntilString  (0x009ba110)
//
// Scans the parser's buffer (starting at m_pos within m_buffer) for a
// matching terminator/substring using FindMatchingSubstring (updates m_pos
// on success). If found, builds a temporary CCharString slice spanning the
// consumed bytes [oldPos, newPos), assigns it into item->value, marks
// item->type = 2 (string), and returns true. If not found, returns false.

class CCharString
{
public:
    CCharString() : data(0) {}
    CCharString(const char* text, int length);
    ~CCharString();
    CCharString& operator=(const CCharString& rhs);

    void* data;
};

struct CParsedItem
{
    long type;         // +0x00
    long m_pad0;        // +0x04
    CCharString value; // +0x08 (embedded)
};

class CStringParser
{
public:
    bool FindMatchingSubstring(CCharString* terminator);
    bool ReadAsStringUntilString(CCharString* terminator, CParsedItem* item);

    long m_unk0;    // +0x00
    long m_pos;     // +0x04
    // ... unrelated fields between +0x08 and +0x20 not needed here.
    long m_pad[6];  // +0x08 .. +0x1f
    char* m_buffer; // +0x20
};

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