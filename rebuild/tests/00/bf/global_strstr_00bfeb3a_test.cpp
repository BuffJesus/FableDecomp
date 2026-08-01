#include <stdio.h>
#include <string.h>

int main() {
    char haystack[] = "the quick brown fox jumps over the lazy dog";
    char* found = strstr(haystack, "brown");
    char* notfound = strstr(haystack, "zzz");
    char* atstart = strstr(haystack, "the");
    char* empty = strstr(haystack, "");

    int ok = 1;
    if (found == 0 || found != haystack + 10) ok = 0;
    if (notfound != 0) ok = 0;
    if (atstart == 0 || atstart != haystack) ok = 0;
    if (empty == 0 || empty != haystack) ok = 0;

    if (ok) {
        printf("STRSTR_TEST_PASS\n");
    } else {
        printf("STRSTR_TEST_FAIL\n");
    }
    return 0;
}