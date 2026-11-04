// lib_string_parser.cpp, retail 0x009BA830-0x009BA890: queries over the
// parser's comment boundaries (Ego_r names), in list order.
#include "fable_string_parser.h"

// Retail 009BA830: inside any comment, end exclusive.
bool CStringParser::IsPosInComments(const char* pos)
{
    for (std::list<CStringParserCommentBoundaries>::iterator it = Comments.begin(); it != Comments.end(); ++it)
    {
        if (it->PStart <= pos && it->PEnd > pos)
            return true;
    }
    return false;
}

// Retail 009BA860: the first comment starting after the position, or null.
const char* CStringParser::GetNextCommentStart(const char* pos)
{
    for (std::list<CStringParserCommentBoundaries>::iterator it = Comments.begin(); it != Comments.end(); ++it)
    {
        if (it->PStart > pos)
            return it->PStart;
    }
    return 0;
}

// Retail 009BA890: the first comment ending after the position, or null.
const char* CStringParser::GetNextCommentEnd(const char* pos)
{
    for (std::list<CStringParserCommentBoundaries>::iterator it = Comments.begin(); it != Comments.end(); ++it)
    {
        if (it->PEnd > pos)
            return it->PEnd;
    }
    return 0;
}
