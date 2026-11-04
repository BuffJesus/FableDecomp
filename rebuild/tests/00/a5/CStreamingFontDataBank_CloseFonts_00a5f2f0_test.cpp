#include <stdio.h>
#include <string.h>
#include "fable_font_data_banks.h"
#include "engine/CStreamingFontDataBankEntry.h"

static char g_events[32];
static unsigned g_eventCount;
static const CCharString* g_expectedFilename;
static bool g_filenameValid;
static void Record(char id, char event)
{
    g_events[g_eventCount++] = id;
    g_events[g_eventCount++] = event;
    g_events[g_eventCount] = 0;
}

const CCharString& CCharString::operator=(const char* value)
{
    g_filenameValid = this == g_expectedFilename && value && value[0] == 0;
    Record('0', 'F');
    return *this;
}

CStreamingFontDataBankEntry::~CStreamingFontDataBankEntry() {}
bool CStreamingFontDataBankEntry::IsLoaded() const { return false; }
bool CStreamingFontDataBankEntry::CanUnload() const { return false; }
void CStreamingFontDataBankEntry::Unload() {}

struct TestEntry : CStreamingFontDataBankEntry
{
    TestEntry(char identifier, bool isLoaded)
        : id(identifier), loaded(isLoaded) {}
    ~TestEntry() { Record(id, 'D'); }
    bool IsLoaded() const { Record(id, 'I'); return loaded; }
    bool CanUnload() const { Record(id, 'C'); return false; }
    void Unload() { Record(id, 'U'); }
    char id;
    bool loaded;
};

struct CStreamingFontDataBankCloseFontsTestAccess
{
    static void Close(CStreamingFontDataBank* bank) { bank->CloseFonts(); }
};

int main()
{
    unsigned char bankStorage[sizeof(CStreamingFontDataBank)];
    memset(bankStorage, 0, sizeof(bankStorage));
    CStreamingFontDataBank* bank =
        reinterpret_cast<CStreamingFontDataBank*>(bankStorage);
    CStreamingFontDataBankEntry* entries[3];
    entries[0] = new TestEntry('1', false);
    entries[1] = 0;
    entries[2] = new TestEntry('2', true);
    CStreamingFontDataBankEntry* first = entries[0];
    CStreamingFontDataBankEntry* last = entries[2];
    bank->Fonts._M_start = entries;
    bank->Fonts._M_finish = entries + 3;
    bank->Fonts._M_end_of_storage._M_data = entries + 3;
    g_expectedFilename = &bank->Filename;
    CStreamingFontDataBankCloseFontsTestAccess::Close(bank);
    if (strcmp(g_events, "1I1D2I2C2U2D0F") != 0 ||
        !g_filenameValid || bank->Fonts._M_finish != entries ||
        entries[0] != first || entries[1] || entries[2] != last)
        return 1;
    g_eventCount = 0;
    g_events[0] = 0;
    CStreamingFontDataBankCloseFontsTestAccess::Close(bank);
    if (strcmp(g_events, "0F") != 0 || !g_filenameValid)
        return 2;
    puts("STREAMING_CLOSE_FONTS_00a5f2f0_PASS");
    return 0;
}