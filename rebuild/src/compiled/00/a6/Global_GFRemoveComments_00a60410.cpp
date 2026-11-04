// GFRemoveComments @ 0x00A60410. Strip delimited text while preserving its
// newline count, so source locations after a comment retain their line number.
#include "fable_comments.h"
#include <string.h>

void __fastcall GFRemoveComments(char* source, char* opening, char* closing)
{
    char* end = source + strlen(source) + 1;
    char* cursor = source;
    for (;;)
    {
        char* begin = strstr(cursor, opening);
        if (!begin)
            return;

        if (!*closing)
        {
            unsigned int lineCount = 0;
            for (char* p = begin; p < end; ++p)
                if (*p == '\n')
                    ++lineCount;
            char* next = begin + lineCount;
            for (char* p = begin; p < next; ++p)
                *p = '\n';
            *next = 0;
            return;
        }

        char* after = strstr(begin, closing);
        if (!after)
            return;
        after += strlen(closing);
        unsigned int lineCount = 0;
        for (char* p = begin; p <= after; ++p)
            if (*p == '\n')
                ++lineCount;
        char* next = begin + lineCount;
        for (char* p = begin; p < next; ++p)
            *p = '\n';
        memmove(next, after, end - after);
        end += next - after;
        cursor = next;
    }
}
