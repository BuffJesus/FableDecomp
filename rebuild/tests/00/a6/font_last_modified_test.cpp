#include <stdio.h>
#include <string.h>
#include "fable_font_data_banks.h"
#include "fable_date_and_time.h"

static unsigned g_calls;
CDateAndTime::CDateAndTime(long year, unsigned long month, unsigned long day,
                           unsigned long hour, unsigned long minute,
                           unsigned long second, unsigned long millisecond)
    : Year(year), Month(month), Day(day), Hour(hour), Minute(minute),
      Second(second), Millisecond(millisecond) { ++g_calls; }

struct FontLastModifiedAccess : CFontDataBank {
    static CDateAndTime Read(CFontDataBank* bank, const CCharString& name) {
        return reinterpret_cast<FontLastModifiedAccess*>(bank)->CFontDataBank::GetLastModifiedTime(name);
    }
};
struct StreamingFontLastModifiedAccess : CStreamingFontDataBank {
    static CDateAndTime Read(CStreamingFontDataBank* bank, const CCharString& name) {
        return reinterpret_cast<StreamingFontLastModifiedAccess*>(bank)->CStreamingFontDataBank::GetLastModifiedTime(name);
    }
};

template<class Bank, class Access>
static int Check(unsigned seed) {
    unsigned char storage[sizeof(Bank)];
    memset(storage, 0xa5, sizeof(storage));
    Bank* bank = reinterpret_cast<Bank*>(storage);
    unsigned char nameBytes[sizeof(CCharString)];
    memset(nameBytes, seed, sizeof(nameBytes));
    const CCharString& name = *reinterpret_cast<const CCharString*>(nameBytes);
    g_calls = 0;
    CDateAndTime stamp = Access::Read(bank, name);
    if (g_calls != 1 || stamp.Year != 1 || stamp.Month != 1 || stamp.Day != 1 ||
        stamp.Hour != 1 || stamp.Minute != 1 || stamp.Second != 0 || stamp.Millisecond != 0)
        return 1;
    for (unsigned i = 0; i < sizeof(storage); ++i) if (storage[i] != 0xa5) return 2;
    for (unsigned i = 0; i < sizeof(nameBytes); ++i) if (nameBytes[i] != seed) return 3;
    return 0;
}

int main() {
    for (unsigned i = 0; i < 16; ++i) {
#ifndef FONT_LAST_MODIFIED_STREAMING_ONLY
        int a = Check<CFontDataBank, FontLastModifiedAccess>(i);
        if (a) return 10 + a;
#endif
#ifndef FONT_LAST_MODIFIED_STATIC_ONLY
        int b = Check<CStreamingFontDataBank, StreamingFontLastModifiedAccess>(i);
        if (b) return 20 + b;
#endif
    }
    puts("FONT_LAST_MODIFIED_PASS cases=16");
    return 0;
}
