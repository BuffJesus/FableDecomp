#include "fable_string_parser.h"
#include <stdio.h>
#include <string.h>
CCharString::operator const char*() const
{
    CCharStringData* data = *reinterpret_cast<CCharStringData* const*>(this);
    return data ? data->Data.PData : "";
}
int main()
{
    struct Case { char* source; char* needle; unsigned long expected; bool found; };
    Case cases[] = {
        {"enum E", "enum", 4, true},
        {"preenum enumX enum_ enum2 enum E", "enum", 30, true},
        {"enumX", "enum", 0, false},
        {"#definition_template T X", "#definition", 0, false},
        {"x #definition C N", "#definition", 13, true},
        {"\"quoted\" enum E", "enum", 13, true}
    };
    for (unsigned int c = 0; c < sizeof(cases)/sizeof(cases[0]); ++c)
    {
        unsigned long raw[10] = {0};
        CStringParser* parser = reinterpret_cast<CStringParser*>(raw);
        parser->String = cases[c].source;
        CCharStringData data;
        data.Data.PData = cases[c].needle;
        data.Data.StringLength = strlen(cases[c].needle);
        CCharStringData* dataPointer = &data;
        CCharString& needle = *reinterpret_cast<CCharString*>(&dataPointer);
        bool found = parser->SkipPastWholeString(needle);
        if (found != cases[c].found || parser->StringPos != cases[c].expected)
        { printf("case %u pos %lu expected %lu\n", c, parser->StringPos, cases[c].expected); return 1; }
    }
    puts("WHOLE_STRING_OK cases=6");
    return 0;
}
