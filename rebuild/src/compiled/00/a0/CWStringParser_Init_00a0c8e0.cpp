// lib_wstringparser (Ego_r module order 0x00B77400..): CWStringParser::Init,
// retail 0x00A0C8E0 (retail neighbours GetLine 0x00A0C900, SkipWhitespace
// 0x00A0C910, NextItemExists 0x00A0C950, SkipUntilPastChar 0x00A0C9A0 follow in
// the same order). Ego_r.pdb layout: CWStringParser 0x10 bytes { int _padding_;
// wchar_t* String; ulong StringPos; ulong Line; }. No shared engine/ header models
// CWStringParser, so the class is declared here.
#include <stddef.h>

class CWStringParser
{
public:
    void Init(const wchar_t* string);

private:
    int _padding_;            // +0x00
    const wchar_t* String;    // +0x04
    unsigned long StringPos;  // +0x08
    unsigned long Line;       // +0x0C
};

void CWStringParser::Init(const wchar_t* string)
{
    String = string;
    StringPos = 0;
    Line = 0;
}
