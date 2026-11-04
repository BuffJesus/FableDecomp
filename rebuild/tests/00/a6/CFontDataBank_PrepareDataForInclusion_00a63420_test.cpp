#include "fable_font_data_banks.h"
#include "fable_data_output_stream.h"
#include <stdio.h>
#include <string.h>

static unsigned g_deleted;
static void* g_deletedData;
static CCPPointerInfo* g_info;
static void __fastcall OnDelete(void* data) { ++g_deleted; g_deletedData = data; }

template<>
CCountedPointer<CBankStateBlock>::CCountedPointer(CBankStateBlock* data)
    : Data(data), Info(new CCPPointerInfo(&OnDelete, data))
{
    g_info = Info;
}

struct PrepareAccess : CFontDataBank {
    void Call(unsigned long index, unsigned long mode);
};

void PrepareAccess::Call(unsigned long index, unsigned long mode)
{
    unsigned char namesBytes[sizeof(CArray<CCharString>)];
    unsigned char streamBytes[sizeof(CDataOutputStream)];
    memset(namesBytes, 0xa5, sizeof(namesBytes));
    memset(streamBytes, 0x5a, sizeof(streamBytes));
    const CArray<CCharString>& names = *reinterpret_cast<const CArray<CCharString>*>(namesBytes);
    CDataOutputStream& stream = *reinterpret_cast<CDataOutputStream*>(streamBytes);
    if (mode == 0) {
        CFontDataBank::PrepareDataForInclusion(index, names, 17,
            CCountedPointer<CBankStateBlock>(), stream, stream);
    } else {
        CBankStateBlock* data = reinterpret_cast<CBankStateBlock*>(0x12340000u + index);
        CFontDataBank::PrepareDataForInclusion(index, names, 17,
            CCountedPointer<CBankStateBlock>(data), stream, stream);
    }
    for (unsigned i = 0; i < sizeof(namesBytes); ++i) if (namesBytes[i] != 0xa5) g_deleted += 100;
    for (unsigned i = 0; i < sizeof(streamBytes); ++i) if (streamBytes[i] != 0x5a) g_deleted += 100;
}

int main()
{
    unsigned char bankBytes[sizeof(CFontDataBank)];
    memset(bankBytes, 0xa5, sizeof(bankBytes));
    PrepareAccess* bank = reinterpret_cast<PrepareAccess*>(bankBytes);
    for (unsigned i = 0; i < 16; ++i) {
        g_deleted = 0; g_deletedData = 0; g_info = 0;
        bank->Call(i, i & 1);
        if (g_deleted != (i & 1)) return 1;
        if ((i & 1) && g_deletedData != reinterpret_cast<void*>(0x12340000u + i)) return 2;
        for (unsigned j = 0; j < sizeof(bankBytes); ++j) if (bankBytes[j] != 0xa5) return 3;
    }
    puts("FONT_PREPARE_EMPTY_PASS cases=16");
    return 0;
}
