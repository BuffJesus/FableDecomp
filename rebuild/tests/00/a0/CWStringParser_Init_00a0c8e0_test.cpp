#include <stdio.h>
struct CWStringParser {
 int _padding_; const wchar_t* String; unsigned long StringPos; unsigned long Line;
 void Init(const wchar_t*);
};
void CWStringParser::Init(const wchar_t* value) { String=value; StringPos=0; Line=0; }
int main() { const wchar_t text[]=L"oakvale"; CWStringParser parser;
 parser.String=0; parser.StringPos=12; parser.Line=4; parser.Init(text);
 if(parser.String!=text || parser.StringPos!=0 || parser.Line!=0) return 1;
 printf("WSTRING_PARSER_INIT PASS\n"); return 0; }