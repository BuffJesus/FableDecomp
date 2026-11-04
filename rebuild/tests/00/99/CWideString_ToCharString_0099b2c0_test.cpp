#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_string.h"
wchar_t g_FableEmptyWideString_0129A8E0[1] = {0};
static const unsigned char* expected;
static unsigned expectedBytes, constructors, destructors;
CCharString::CCharString(const char* text, long length) : storage_(0) {
    if (!text || length != -1 || memcmp(text, expected, expectedBytes)) abort();
    ++constructors;
}
CCharString::~CCharString() { ++destructors; }
int main() {
    const unsigned char empty[] = {0};
    const unsigned char narrowed[] = {'A',0,0xA9,0xFF,0};
    wchar_t wide[] = {L'A',0x0100,0x03A9,0x00FF,0};
    unsigned long data[4] = {reinterpret_cast<unsigned long>(wide), reinterpret_cast<unsigned long>(wide + 4), reinterpret_cast<unsigned long>(wide + 5), 1};
    for (unsigned mode = 0; mode < 3; ++mode) {
        CWideStringData* storage = mode ? reinterpret_cast<CWideStringData*>(data) : 0;
        if (mode == 1) data[1] = data[0]; else data[1] = reinterpret_cast<unsigned long>(wide + 4);
        unsigned long original[4]; memcpy(original, data, sizeof(data));
        const CWideString& input = *reinterpret_cast<const CWideString*>(&storage);
        expected = mode == 2 ? narrowed : empty; expectedBytes = mode == 2 ? 5 : 1;
        { CCharString output = input.ToCharString(); }
        if (constructors != mode + 1 || destructors != mode + 1 || memcmp(original, data, sizeof(data))) return 1;
    }
    puts("TO_CHAR_STRING_0099b2c0_PASS"); return 0;
}
