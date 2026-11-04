#include "fable_comments.h"
#include <stdio.h>
#include <string.h>

static bool check(const char* input, char* opening, char* closing, const char* expected)
{
    char text[256];
    strcpy(text, input);
    GFRemoveComments(text, opening, closing);
    return strcmp(text, expected) == 0;
}

int main()
{
    char blockOpen[] = "/*", blockClose[] = "*/";
    char lineOpen[] = "//", lineClose[] = "\n";
    char tailOpen[] = "#", noClose[] = "";
    if (!check("alpha", blockOpen, blockClose, "alpha")) return 1;
    if (!check("a/* x */b", blockOpen, blockClose, "ab")) return 2;
    if (!check("a/* x\ny */b", blockOpen, blockClose, "a\nb")) return 3;
    if (!check("a/* x */b/* y */c", blockOpen, blockClose, "abc")) return 4;
    if (!check("a/* unfinished", blockOpen, blockClose, "a/* unfinished")) return 5;
    // The retail scan counts the first byte after the closer. When it is a
    // newline, the copied tail keeps that byte and the count inserts one.
    if (!check("a// x\nb", lineOpen, lineClose, "a\nb")) return 6;
    if (!check("a/* x */\nb", blockOpen, blockClose, "a\n\nb")) return 7;
    if (!check("a#x\ny\n", tailOpen, noClose, "a\n\n")) return 8;
    puts("GF_REMOVE_COMMENTS_OK");
    return 0;
}
