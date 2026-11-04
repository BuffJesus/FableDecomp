#include "fable_string_parser.h"

// std::list<CStringParserCommentBoundaries>::~list() @ 0x004046B0, the
// COMDAT ~CStringParser and GFMain run for CStringParser::Comments.
template std::list<CStringParserCommentBoundaries>::~list();
