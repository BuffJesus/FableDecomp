// Behaviour fixture for the CWADFile unit 0x00A39BA0..0x00A39E10, linked against the provider object (it defines no
// CWADFile member). A real CWADFile is built by the retail constructor over a raw WAD bank and driven through the
// CAFile interface, so the provider's vtable slot order is exercised too. The bank, string and base-class calls the
// unit makes are test doubles below.
#include "fable_wad_file.h"
#include "fable_file_information.h"
#include "fable_bank_entry_update.h"
#include <stdio.h>
#include <string.h>

static int g_fail;
#define CHECK(x) do { if (!(x)) { printf("FAIL line %d: %s\n", __LINE__, #x); ++g_fail; } } while (0)

static unsigned char g_bankStorage[sizeof(CWADBank)];
static CBankFile* const g_bank = reinterpret_cast<CBankFile*>(g_bankStorage);
static int g_baseCtors, g_baseDtors, g_dateCtors, g_charDtors, g_crcCalls;
static unsigned long g_readIndex, g_readOffset, g_readSize;
static void* g_readData;
static const CBankFileEntryUpdateData* g_infoEntry;
static unsigned long g_infoOffset, g_infoSize;

char g_FableEmptyNarrowString_0129AAF4[1] = "";
unsigned int FABLE_FASTCALL FableCRC_Calc_004014A0(unsigned int seed, const void* data, unsigned int size)
{
    ++g_crcCalls;
    CHECK(seed == 0 && data == g_FableEmptyNarrowString_0129AAF4 && size == 0);
    return 0x1234;
}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() { ++g_baseCtors; }
CBaseClassNonCopyable::~CBaseClassNonCopyable() { ++g_baseDtors; }
CDateAndTime::CDateAndTime() { ++g_dateCtors; }
CCharString::CCharString() { memset(this, 0, sizeof(*this)); }
CCharString::~CCharString() { ++g_charDtors; }
CWideString::CWideString() { memset(this, 0, sizeof(*this)); }
CWideString::~CWideString() {}
CCharString CWideString::ToCharString() const { return CCharString(); }
CWideString CCharString::ToWideString() const { return CWideString(); }
unsigned long CBankFile::FindIndexBySymbolCRC(unsigned long crc) const
{
    CHECK(this == g_bank && crc == 0x1234);
    return 7;
}
CCharString CBankFile::GetEntrySymbol(unsigned long index) const
{
    CHECK(this == g_bank && index == 7);
    return CCharString();
}
unsigned long CBankFile::GetEntryDataSize(unsigned long index) const { return this == g_bank ? 100 + index : 0; }
unsigned long CBankFile::GetAlignment() const { return this == g_bank ? 0x800 : 0; }
void CBankFile::ReadEntryData(unsigned long index, unsigned long offset, void* data, unsigned long size)
{
    CHECK(this == g_bank);
    g_readIndex = index; g_readOffset = offset; g_readData = data; g_readSize = size;
}
void CBankFileEntryUpdateData::ReadInfo(unsigned long offset, void* data, unsigned long size) const
{
    g_infoEntry = this; g_infoOffset = offset; g_infoSize = size;
    static_cast<CFileInformation*>(data)->ReadOnly = true;
}

int main()
{
    // UpdateData (CArrayVector, start pointer first) points at a table whose entry 7 is ours.
    static CBankFileEntryUpdateData entry;
    static CBankFileEntryUpdateData* table[8];
    table[7] = &entry;
    CBankFileEntryUpdateData** start = table;
    memcpy(g_bankStorage + offsetof(CBankFile, UpdateData), &start, sizeof(start));

    CWideString path;
    CAFile* file = new CWADFile(*reinterpret_cast<CWADBank*>(g_bankStorage), path, FILE_ACCESS_READ, 0);
    CHECK(g_baseCtors == 1 && g_crcCalls == 1 && g_charDtors == 1);   // InternalOpen hashed one narrow copy
    CHECK(file->IsOpen() && file->GetPosition() == 0);
    CHECK(file->GetLength() == 107 && file->GetAlignment() == 0x800 && file->IsIOFinished());

    char buffer[16];
    file->SetPosition(10);
    file->Read(buffer, 6, 0);
    CHECK(g_readIndex == 7 && g_readOffset == 10 && g_readData == buffer && g_readSize == 6);
    CHECK(file->GetPosition() == 16);
    file->MovePosition(-4);
    CHECK(file->GetPosition() == 12);
    file->Write(buffer, 6, 0);
    file->SetLength(1);
    CHECK(file->GetPosition() == 12 && file->GetLength() == 107);

    g_charDtors = 0;
    CWideString name = file->GetPathname();
    CHECK(g_charDtors == 1);

    g_dateCtors = 0;
    CFileInformation info = file->GetFileInformation();
    CHECK(g_dateCtors == 3 && g_infoEntry == &entry && g_infoOffset == 0 && g_infoSize == sizeof(CFileInformation));
    CHECK(info.ReadOnly);

    CAFile* copy = file->Clone();
    CHECK(copy && copy != file && g_baseCtors == 2 && copy->GetPosition() == 12 && copy->IsOpen());

    file->Close();
    CHECK(!file->IsOpen() && file->GetPosition() == 0 && copy->IsOpen());
    g_crcCalls = 0;
    file->Open(path, FILE_ACCESS_READ, 0);
    CHECK(g_crcCalls == 1 && file->IsOpen() && file->GetPosition() == 0);

    delete copy;
    delete file;
    CHECK(g_baseDtors == 2);

    if (!g_fail)
    {
        printf("CWADFILE_MODULE_OK\n");
        return 0;
    }
    return 1;
}
