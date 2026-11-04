// Behaviour fixture for CBankFile's default notification virtuals: each is
// the real compiled provider, called non-virtually on bank storage, and must
// leave the bank (and its arguments) untouched.
#include "fable_game_text_bank.h"
#include <stdio.h>
#include <string.h>

// Exposes the protected notifications to the fixture.
class CBankFileProbe : public CBankFile
{
public:
    void Run(unsigned long a, unsigned long b, const void* array)
    {
        CBankFile::NotifyBankSize(a, *static_cast<const CArray<CBankFileDataTypeCount>*>(array));
        CBankFile::NotifyBankOpened();
        CBankFile::NotifyEntryAdded(a, b, *static_cast<const CArray<char>*>(array));
        CBankFile::NotifyEntryUpdated(a, b, *static_cast<const CArray<char>*>(array));
        CBankFile::NotifyEntryRemoved(a);
        CBankFile::NotifyPostEntriesLoaded();
    }
};

int main()
{
    static union { double align; unsigned char bytes[sizeof(CBankFile) + 16]; } bank, before;
    static unsigned char array[64], arrayBefore[64];
    int cases = 0;
    for (unsigned long a = 0; a < 4; ++a) for (unsigned long b = 0; b < 3; ++b)
    {
        for (unsigned i = 0; i < sizeof(bank.bytes); ++i) bank.bytes[i] = (unsigned char)(i * 7 + a + b);
        for (unsigned i = 0; i < sizeof(array); ++i) array[i] = (unsigned char)(i ^ a);
        before = bank;
        memcpy(arrayBefore, array, sizeof(array));
        reinterpret_cast<CBankFileProbe*>(&bank)->Run(a * 0x10001, b ? 0xffffffffu : 0, array);
        if (memcmp(&bank, &before, sizeof(bank)) != 0 || memcmp(array, arrayBefore, sizeof(array)) != 0)
        {
            printf("BANK_NOTIFY_FAIL a=%lu b=%lu\n", a, b);
            return 1;
        }
        ++cases;
    }
    printf("BANK_NOTIFY_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
