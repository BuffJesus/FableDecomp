// Retail 0x00A0C8E0. PDB layout names identify the input string,
// current character position, and one-based line counter fields.
typedef unsigned short wchar_t;
struct CWStringParser {
    int _padding_;
    const wchar_t* String;
    unsigned long StringPos;
    unsigned long Line;
    void Init(const wchar_t* pString);
};
void CWStringParser::Init(const wchar_t* pString) {
    String = pString;
    StringPos = 0;
    Line = 0;
}