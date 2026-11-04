#include "fable_string_parser.h"
#include <stdio.h>
#include <string.h>
int main()
{
    unsigned long storage[10];
    CStringParser* parser = reinterpret_cast<CStringParser*>(storage);
    char text[] = "a\"b'c\"d'e";
    for (unsigned long position = 0; position < sizeof(text); ++position)
    {
        memset(storage, 0x5a, sizeof(storage));
        parser->String = text;
        parser->SetStringPos(position);
        bool expected = false;
        for (unsigned long i = 0; i < position; ++i)
            if (text[i] == 34 || text[i] == 39) expected = !expected;
        if (parser->StringPos != position || parser->InQuotes != expected ||
            parser->BaseLineOffset != 0x5a5a5a5a || parser->Line != 0x5a5a5a5a)
            return 1;
    }
    puts("PARSER_POSITION_OK cases=10");
    return 0;
}
