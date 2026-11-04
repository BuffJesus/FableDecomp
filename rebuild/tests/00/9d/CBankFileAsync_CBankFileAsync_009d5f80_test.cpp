#include "test_support/bank_extended_virtual_boundaries.h"
#include "test_support/async_bank_lifetime_stubs.h"
#include "fable_game_text_bank.h"
#include "test_support/bank_file_member_stubs.h"
#include <new>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" __declspec(dllimport) void __stdcall DeleteCriticalSection(void*);

static int baseConstructCalls;
CBankFile::CBankFile() { ++baseConstructCalls; }
CBankFile::~CBankFile() {}
template<> _STL::list<CCountedPointer<CBankFileAsyncData> >::~list() {}
CBankFileAsync::~CBankFileAsync() {}
bool CBankFile::OpenReadOnly(const CCharString&, unsigned long) { return false; }

static unsigned long ReadWord(const unsigned char* object, unsigned offset)
{
    return *reinterpret_cast<const unsigned long*>(object + offset);
}

class FixtureConcreteAsyncBank : public CBankFileAsync { protected: virtual unsigned long GetDataFormatVersion() { return 0; } };
int main()
{
    unsigned char storage[sizeof(CBankFileAsync)];
    memset(storage, 0xA5, sizeof(storage));
    CBankFileAsync* bank = new (storage) FixtureConcreteAsyncBank;
    if (bank != reinterpret_cast<CBankFileAsync*>(storage) ||
        baseConstructCalls != 1 || sizeof(CBankFileAsync) != 0x164)
        return 1;
    if (!ReadWord(storage, 0) || !ReadWord(storage, 0x128) ||
        !ReadWord(storage, 0x134) || !ReadWord(storage, 0x144))
        return 2;
    for (unsigned offset = 0x110; offset <= 0x124; offset += 4)
        if (ReadWord(storage, offset) != 0) return 3;
    if (ReadWord(storage, 0x12c) || ReadWord(storage, 0x138) ||
        ReadWord(storage, 0x140) || storage[0x148] || storage[0x149])
        return 4;
    for (unsigned offset = 0x128; offset <= 0x134; offset += 0x0c)
    {
        unsigned char* node = reinterpret_cast<unsigned char*>(ReadWord(storage, offset));
        if (node[0] || ReadWord(node, 4) ||
            ReadWord(node, 8) != reinterpret_cast<unsigned long>(node) ||
            ReadWord(node, 12) != reinterpret_cast<unsigned long>(node))
            return 5;
        free(node);
    }
    unsigned char* list = reinterpret_cast<unsigned char*>(ReadWord(storage, 0x144));
    if (ReadWord(list, 0) != reinterpret_cast<unsigned long>(list) ||
        ReadWord(list, 4) != reinterpret_cast<unsigned long>(list))
        return 6;
    free(list);
    if (ReadWord(storage, 0x14c) != 0xffffffffUL) return 7;
    DeleteCriticalSection(storage + 0x14c);
    puts("ASYNC_BANK_CTOR_PASS");
    return 0;
}
