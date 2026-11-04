#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <ctype.h>

#include "fable_definition_manager.h"
#include "fable_checksum_cache_file.h"
#include "fable_filesystem.h"
#define FABLE_FIXTURE_OWNS_CHECKSUM_DTOR
#include "test_support/bank_file_cleanup_stubs.h"

// Strings in this fixture own a heap copy of their text in the storage slot.
namespace
{
    wchar_t*& WSlot(const CWideString& s) { return *reinterpret_cast<wchar_t**>(const_cast<CWideString*>(&s)); }
    char*& CSlot(const CCharString& s) { return *reinterpret_cast<char**>(const_cast<CCharString*>(&s)); }
    wchar_t* WDup(const wchar_t* t) { if (!t) return 0; size_t n = wcslen(t) + 1; wchar_t* p = (wchar_t*)malloc(n * sizeof(wchar_t)); memcpy(p, t, n * sizeof(wchar_t)); return p; }
    char* CDup(const char* t) { if (!t) return 0; size_t n = strlen(t) + 1; char* p = (char*)malloc(n); memcpy(p, t, n); return p; }
    const wchar_t* WText(const CWideString& s) { return WSlot(s) ? WSlot(s) : L""; }

    bool g_valid;
    int g_cacheCtor, g_cacheDtor, g_opened, g_scans, g_checksums, g_sorts;
    CDefinitionManager* g_manager;
    CChecksumCacheFile* g_cache;
    const CWideString* g_patterns;
    const CWideString* g_scanPattern;
    CAFile::CFileScanInfo* g_scanInfo;
    CWideString* g_scanPath;
    unsigned int g_scanIndex;

    // Files each pattern matches, and their checksums.
    const wchar_t* const kFiles[3][3] = { { L"Zeta.def", L"alpha.DEF", 0 }, { 0 }, { L"Mid.def", 0 } };
    struct FileSum { const wchar_t* name; unsigned long sum; };
    const FileSum kSums[] = { { L"Zeta.def", 0x11111111 }, { L"alpha.DEF", 0x22222222 }, { L"Mid.def", 0x33333333 } };

    void Check(bool c) { if (!c) g_valid = false; }
}

fable_u32 g_FableCRCTable_0129A168[256];

struct ManagerView : CDefinitionManager
{
    static const CWideString& FileName(CDefinitionManager* m) { return static_cast<ManagerView*>(m)->CompiledFileName; }
    static unsigned long& CRC(CDefinitionManager* m) { return static_cast<ManagerView*>(m)->DependencyCheckCRC; }
    static void SetChecksums(bool on) { CreateChecksums = on; }
    static void Check(CDefinitionManager* m, const _STL::vector<CWideString>& list) { static_cast<ManagerView*>(m)->CheckDependencies(list); }
};
bool CDefinitionManager::CreateChecksums;

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const CWideString& other) : storage_(0) { WSlot(*this) = WDup(WSlot(other)); }
CWideString::~CWideString() { free(WSlot(*this)); WSlot(*this) = 0; }

static CWideString Concat(const wchar_t* a, const wchar_t* b)
{
    CWideString r;
    size_t n = wcslen(a) + wcslen(b) + 1;
    wchar_t* p = (wchar_t*)malloc(n * sizeof(wchar_t));
    wcscpy(p, a); wcscat(p, b);
    WSlot(r) = p;
    return r;
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right) { return Concat(WText(left), WText(right)); }
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right) { return Concat(WText(left), right); }

CCharString::CCharString(const CCharString& other) : storage_(0) { CSlot(*this) = CDup(CSlot(other)); }
CCharString::~CCharString() { free(CSlot(*this)); CSlot(*this) = 0; }

CCharString CWideString::ToCharString() const
{
    CCharString r(*reinterpret_cast<const CCharString*>(&g_patterns[1]));  // empty
    const wchar_t* w = WText(*this);
    char* p = (char*)malloc(wcslen(w) + 1);
    size_t i = 0;
    for (; w[i]; ++i) p[i] = (char)w[i];
    p[i] = 0;
    CSlot(r) = p;
    return r;
}

CCharString CCharString::GetAsLowerCase() const
{
    CCharString r(*this);
    for (char* p = CSlot(r); p && *p; ++p) *p = (char)tolower(*p);
    return r;
}

void FABLE_FASTCALL CAFile::SplitPath(const CWideString& path, CWideString* drive, CWideString* directory,
                                      CWideString* filename, CWideString* extension)
{
    Check(&path == &ManagerView::FileName(g_manager) && extension == 0);
    WSlot(*drive) = WDup(L"C:");
    WSlot(*directory) = WDup(L"\\defs\\");
    WSlot(*filename) = WDup(L"frontend");
}

CChecksumCacheFile::CChecksumCacheFile(long bufferSize) { ++g_cacheCtor; g_cache = this; Check(bufferSize == 0x10000 && g_opened == 0); }
CChecksumCacheFile::~CChecksumCacheFile() { ++g_cacheDtor; Check(this == g_cache); }
void CChecksumCacheFile::Open(const CWideString& filename)
{
    ++g_opened;
    Check(this == g_cache && wcscmp(WText(filename), L"C:\\defs\\frontend.cks") == 0);
}
unsigned long CChecksumCacheFile::GetChecksum(const CWideString& filename)
{
    ++g_checksums;
    Check(this == g_cache && &filename == g_scanPath);
    for (unsigned int i = 0; i < sizeof(kSums) / sizeof(kSums[0]); ++i)
        if (wcscmp(kSums[i].name, WText(filename)) == 0)
            return kSums[i].sum;
    g_valid = false;
    return 0;
}

static bool NextFile(CWideString& pathname)
{
    const wchar_t* next = kFiles[g_scanPattern - g_patterns][g_scanIndex];
    if (!next)
        return false;
    ++g_scanIndex;
    free(WSlot(pathname));
    WSlot(pathname) = WDup(next);
    return true;
}

bool FABLE_FASTCALL CAFile::StartFileScan(const CWideString& pattern, CFileScanInfo& info, CWideString& pathname)
{
    ++g_scans;
    Check(&pattern >= g_patterns && &pattern < g_patterns + 3);
    Check(WSlot(pathname) == 0);
    g_scanPattern = &pattern;
    g_scanInfo = &info;
    g_scanPath = &pathname;
    g_scanIndex = 0;
    return NextFile(pathname);
}

bool FABLE_FASTCALL CAFile::ScanNextFile(const CWideString& pattern, CFileScanInfo& info, CWideString& pathname)
{
    Check(&pattern == g_scanPattern && &info == g_scanInfo && &pathname == g_scanPath);
    return NextFile(pathname);
}

namespace _STL
{
    template <>
    void FABLE_FASTCALL sort<pair<CCharString, unsigned long>*, CKeyPairCompareLess<CCharString, unsigned long> >(
        pair<CCharString, unsigned long>* first, pair<CCharString, unsigned long>* last,
        CKeyPairCompareLess<CCharString, unsigned long>)
    {
        ++g_sorts;
        for (pair<CCharString, unsigned long>* i = first; i != last; ++i)
            for (pair<CCharString, unsigned long>* j = i + 1; j != last; ++j)
                if (strcmp(CSlot(j->first), CSlot(i->first)) < 0)
                {
                    char tmp[sizeof(*i)];
                    memcpy(tmp, i, sizeof(tmp)); memcpy(i, j, sizeof(tmp)); memcpy(j, tmp, sizeof(tmp));
                }
    }
}

namespace
{
    struct RawVector { CWideString* first; CWideString* last; CWideString* end; };

    unsigned long Fold(unsigned long crc, unsigned long value)
    {
        const fable_u8* b = reinterpret_cast<const fable_u8*>(&value);
        for (int i = 0; i < 4; ++i)
            crc = g_FableCRCTable_0129A168[b[i] ^ (crc & 0xFF)] ^ (crc >> 8);
        return crc;
    }

    bool Run(bool checksums, unsigned long expectedCRC, int expectedScans, int expectedSums)
    {
        static fable_u8 managerBytes[sizeof(CDefinitionManager)];
        memset(managerBytes, 0, sizeof(managerBytes));
        g_manager = reinterpret_cast<CDefinitionManager*>(managerBytes);
        ManagerView::CRC(g_manager) = 0xA5A5A5A5;
        ManagerView::SetChecksums(checksums);
        g_valid = true;
        g_cacheCtor = g_cacheDtor = g_opened = g_scans = g_checksums = g_sorts = 0;

        static CWideString patterns[3];
        g_patterns = patterns;
        RawVector list = { patterns, patterns + 3, patterns + 3 };
        ManagerView::Check(g_manager, *reinterpret_cast<_STL::vector<CWideString>*>(&list));

        const int opened = checksums ? 1 : 0;
        return g_valid && ManagerView::CRC(g_manager) == expectedCRC &&
               g_cacheCtor == opened && g_cacheDtor == opened && g_opened == opened &&
               g_scans == expectedScans && g_checksums == expectedSums && g_sorts == opened;
    }
}

int main()
{
    for (unsigned long i = 0; i < 256; ++i)
    {
        unsigned long c = i;
        for (int k = 0; k < 8; ++k)
            c = (c & 1) ? 0xEDB88320UL ^ (c >> 1) : c >> 1;
        g_FableCRCTable_0129A168[i] = c;
    }
    // Sorted by lower-case name: alpha.def, mid.def, zeta.def.
    unsigned long expected = Fold(Fold(Fold(0, 0x22222222), 0x33333333), 0x11111111);

    const bool passed = Run(false, 0, 0, 0) && Run(true, expected, 3, 3);
    if (!passed)
    {
        printf("FABLETLC_CDEFINITIONMANAGER_CHECKDEPENDENCIES_009B05F0 FAIL\n");
        return 1;
    }
    printf("FABLETLC_CDEFINITIONMANAGER_CHECKDEPENDENCIES_009B05F0_PASS\n");
    return 0;
}
