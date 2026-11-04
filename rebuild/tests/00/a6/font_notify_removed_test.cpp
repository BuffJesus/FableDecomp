#include <stdio.h>
#include <string.h>
#include "fable_font_data_banks.h"
#include "engine/CFontDataBankEntry.h"
#include "engine/CStreamingFontDataBankEntry.h"

struct FontAccess : CFontDataBank {
    static void Invoke(CFontDataBank* bank, unsigned long index) {
        reinterpret_cast<FontAccess*>(bank)->CFontDataBank::NotifyEntryRemoved(index);
    }
};
struct StreamingAccess : CStreamingFontDataBank {
    static void Invoke(CStreamingFontDataBank* bank, unsigned long index) {
        reinterpret_cast<StreamingAccess*>(bank)->CStreamingFontDataBank::NotifyEntryRemoved(index);
    }
};

static void** g_newSlot;
static void* g_oldSlot;
static unsigned g_calls;
static unsigned g_flag;
struct FakeEntry {
    virtual void Destroy(unsigned long flag) {
        ++g_calls;
        g_flag = flag;
        if (g_newSlot) {
            *g_newSlot = g_oldSlot;
        }
    }
};

template <class Bank, class Entry, class Access>
static int Check(unsigned index, bool present, bool swapStorage) {
    unsigned char bytes[sizeof(Bank)];
    memset(bytes, 0, sizeof(bytes));
    Bank* bank = reinterpret_cast<Bank*>(bytes);
    FakeEntry entry;
    Entry* first[4] = { 0, 0, 0, 0 };
    Entry* second[4] = { 0, 0, 0, 0 };
    if (present) first[index] = reinterpret_cast<Entry*>(&entry);
    bank->Fonts._M_start = first;
    bank->Fonts._M_finish = first + 4;
    bank->Fonts._M_end_of_storage._M_data = first + 4;
    g_calls = g_flag = 0;
    g_newSlot = (present && swapStorage) ? reinterpret_cast<void**>(&bank->Fonts._M_start) : 0;
    g_oldSlot = second;
    Access::Invoke(bank, index);
    if (g_calls != (present ? 1u : 0u)) return 1;
    if (present && g_flag != 1) return 2;
    if (swapStorage && present) {
        if (bank->Fonts._M_start != second || second[index] != 0 ||
            first[index] != reinterpret_cast<Entry*>(&entry)) return 3;
    } else if (first[index] != 0 || bank->Fonts._M_start != first) return 4;
    return 0;
}

int main() {
    unsigned count = 0;
    for (unsigned index = 0; index < 4; ++index) {
        for (unsigned present = 0; present < 2; ++present) {
            for (unsigned swap = 0; swap < 2; ++swap) {
#ifndef FONT_NOTIFY_STREAMING_ONLY
                int a = Check<CFontDataBank, CFontDataBankEntry, FontAccess>(index, present != 0, swap != 0);
                if (a) return 10 + a;
#endif
#ifndef FONT_NOTIFY_STATIC_ONLY
                int b = Check<CStreamingFontDataBank, CStreamingFontDataBankEntry, StreamingAccess>(index, present != 0, swap != 0);
                if (b) return 20 + b;
#endif
                ++count;
            }
        }
    }
    printf("FONT_NOTIFY_REMOVED_PASS cases=%u\n", count);
    return 0;
}
