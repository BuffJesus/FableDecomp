#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

#include "fable_definition_manager.h"
#include "fable_disk_file.h"
#include "fable_file_information.h"

namespace {
    wchar_t*& WSlot(const CWideString& s) { return *reinterpret_cast<wchar_t**>(const_cast<CWideString*>(&s)); }
    char*& CSlot(const CCharString& s) { return *reinterpret_cast<char**>(const_cast<CCharString*>(&s)); }
    wchar_t* WDup(const wchar_t* s) { size_t n = wcslen(s) + 1; wchar_t* p = (wchar_t*)malloc(n * sizeof(wchar_t)); memcpy(p, s, n * sizeof(wchar_t)); return p; }
    char* CDup(const char* s) { size_t n = strlen(s) + 1; char* p = (char*)malloc(n); memcpy(p, s, n); return p; }
    const wchar_t* WText(const CWideString& s) { return WSlot(s) ? WSlot(s) : L""; }
    const char* CText(const CCharString& s) { return CSlot(s) ? CSlot(s) : ""; }
    bool g_good = true;
    int g_scans, g_sorts, g_reads, g_parses, g_compileParses, g_updates, g_closes;
    const CWideString* g_patterns;
    const CWideString* g_pattern;
    CAFile::CFileScanInfo* g_scanInfo;
    CWideString* g_path;
    unsigned int g_scanIndex;
    const wchar_t* g_openName;
    CDefinitionManager* g_manager;
    CSymbolMap* g_symbols;
    CSymbolMap* g_definitionSymbols;
    const wchar_t* const kFiles[3][3] = {
        { L"Z.DEF", L"Alpha.def", 0 }, { 0 }, { L"Mid.Def", 0 }
    };
    const char* Content(const wchar_t* name) {
        if (!wcscmp(name, L"alpha.def")) return "Alpha //x\nsymbol";
        if (!wcscmp(name, L"z.def")) return "Z/*x*/ed";
        if (!wcscmp(name, L"mid.def")) return "M//x\rid";
        g_good = false; return "";
    }
}

struct ManagerView : CDefinitionManager {
    static void Run(CDefinitionManager* m, const _STL::vector<CWideString>& paths, bool flag) {
        static_cast<ManagerView*>(m)->CreateSymbolsFromPathList(paths, flag);
    }
    static void Compile(CDefinitionManager* m, const _STL::vector<CWideString>& paths,
                        const CCharString& prefix) {
        static_cast<ManagerView*>(m)->DoCompilePathList(paths, prefix);
    }
    static const CCharString& Filename(CDefinitionManager* m) {
        return static_cast<ManagerView*>(m)->CurrentFilename;
    }
    static void SetMaps(CDefinitionManager* m, CSymbolMap* a, CSymbolMap* b) {
        static_cast<ManagerView*>(m)->PSymbolMap.Data = a;
        static_cast<ManagerView*>(m)->PDefinitionSymbolMap.Data = b;
    }
};

CWideString::CWideString() : storage_(0) {}
CWideString::CWideString(const wchar_t* s) : storage_(0) { WSlot(*this) = WDup(s); }
CWideString::CWideString(const CWideString& s) : storage_(0) { WSlot(*this) = WDup(WText(s)); }
CWideString::~CWideString() { free(WSlot(*this)); WSlot(*this) = 0; }
CCharString::CCharString(const char* s, long) : storage_(0) { CSlot(*this) = CDup(s); }
CCharString::CCharString(const CCharString& s) : storage_(0) { CSlot(*this) = CDup(CText(s)); }
CCharString::~CCharString() { free(CSlot(*this)); CSlot(*this) = 0; }
CCharString& CCharString::operator=(const CCharString& s) {
    char* copy = CDup(CText(s)); free(CSlot(*this)); CSlot(*this) = copy; return *this;
}
CCharString CWideString::ToCharString() const {
    const wchar_t* w = WText(*this); char text[256]; size_t i = 0;
    for (; w[i] && i < 255; ++i) text[i] = (char)w[i]; text[i] = 0;
    return CCharString(text);
}
CCharString CCharString::GetAsLowerCase() const {
    CCharString result(*this);
    for (char* p = CSlot(result); *p; ++p) *p = (char)tolower(*p);
    return result;
}
CWideString CCharString::ToWideString() const {
    const char* c = CText(*this); wchar_t text[256]; size_t i = 0;
    for (; c[i] && i < 255; ++i) text[i] = (unsigned char)c[i]; text[i] = 0;
    return CWideString(text);
}

static bool NextFile(CWideString& pathname) {
    const wchar_t* next = kFiles[g_pattern - g_patterns][g_scanIndex];
    if (!next) return false;
    ++g_scanIndex; free(WSlot(pathname)); WSlot(pathname) = WDup(next); return true;
}
bool FABLE_FASTCALL CAFile::StartFileScan(const CWideString& pattern, CFileScanInfo& info, CWideString& pathname) {
    ++g_scans; g_good &= &pattern >= g_patterns && &pattern < g_patterns + 3;
    g_pattern = &pattern; g_scanInfo = &info; g_path = &pathname; g_scanIndex = 0;
    return NextFile(pathname);
}
bool FABLE_FASTCALL CAFile::ScanNextFile(const CWideString& pattern, CFileScanInfo& info, CWideString& pathname) {
    g_good &= &pattern == g_pattern && &info == g_scanInfo && &pathname == g_path;
    return NextFile(pathname);
}

namespace _STL {
    template <> void FABLE_FASTCALL sort<CWideString*, less<CWideString> >(
        CWideString* first, CWideString* last, less<CWideString>) {
        ++g_sorts;
        for (CWideString* a = first; a != last; ++a)
            for (CWideString* b = a + 1; b != last; ++b)
                if (wcscmp(WText(*a), WText(*b)) > 0) {
                    wchar_t* temp = WSlot(*a); WSlot(*a) = WSlot(*b); WSlot(*b) = temp;
                }
    }
}

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CAFile::CAFile() {}
CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags) {
    g_good &= access == FILE_ACCESS_READ && flags == 2;
    g_openName = WText(pathname);
    *reinterpret_cast<long*>(reinterpret_cast<char*>(this) + 0x0C) = (long)strlen(Content(g_openName));
}
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() { ++g_closes; }
void CDiskFileWin32::Read(void* data, long size, CASuspendableProcess* process) {
    ++g_reads; const char* content = Content(g_openName);
    g_good &= process == 0 && size == (long)strlen(content);
    memcpy(data, content, size);
}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation result; return result; }
CDateAndTime::CDateAndTime() { memset(this, 0, sizeof(*this)); }

void CDefinitionManager::ParseStringForSymbols(const char* source, bool flag) {
    static const wchar_t* names[] = { L"alpha.def", L"z.def", L"mid.def" };
    static const char* contents[] = { "Alpha \nsymbol", "Zed", "Mid" };
    g_good &= g_parses < 3 && flag && !strcmp(source, contents[g_parses]);
    g_good &= !strcmp(CText(ManagerView::Filename(this)),
                      g_parses == 0 ? "alpha.def" : g_parses == 1 ? "z.def" : "mid.def");
    g_good &= !wcscmp(g_openName, names[g_parses]);
    ++g_parses;
}
void CDefinitionManager::CompileString(const char* source, const CCharString& prefix) {
    static const wchar_t* names[] = { L"alpha.def", L"z.def", L"mid.def" };
    static const char* contents[] = { "Alpha \nsymbol", "Zed", "Mid" };
    g_good &= g_compileParses < 3 && !strcmp(CText(prefix), "prefix");
    g_good &= !strcmp(source, contents[g_compileParses]);
    g_good &= !wcscmp(g_openName, names[g_compileParses]);
    g_good &= !strcmp(CText(ManagerView::Filename(this)),
                      g_compileParses == 0 ? "alpha.def" : g_compileParses == 1 ? "z.def" : "mid.def");
    ++g_compileParses;
}
void CSymbolMap::UpdateDirty() const {
    g_good &= g_parses == 3;
    g_good &= this == (g_updates % 2 == 0 ? g_symbols : g_definitionSymbols);
    ++g_updates;
}

int main() {
    static unsigned char managerBytes[sizeof(CDefinitionManager)];
    g_manager = reinterpret_cast<CDefinitionManager*>(managerBytes);
    static unsigned char symbolBytes[2][sizeof(CSymbolMap)];
    g_symbols = reinterpret_cast<CSymbolMap*>(symbolBytes[0]);
    g_definitionSymbols = reinterpret_cast<CSymbolMap*>(symbolBytes[1]);
    ManagerView::SetMaps(g_manager, g_symbols, g_definitionSymbols);
    CWideString patterns[3] = { CWideString(L"one"), CWideString(L"empty"), CWideString(L"three") };
    g_patterns = patterns;
    struct RawVector { CWideString* first; CWideString* last; CWideString* end; } list = {
        patterns, patterns + 3, patterns + 3
    };
    ManagerView::Run(g_manager, *reinterpret_cast<_STL::vector<CWideString>*>(&list), true);
    RawVector empty = { patterns, patterns, patterns };
    ManagerView::Run(g_manager, *reinterpret_cast<_STL::vector<CWideString>*>(&empty), false);
    CCharString prefix("prefix");
    ManagerView::Compile(g_manager, *reinterpret_cast<_STL::vector<CWideString>*>(&list), prefix);
    ManagerView::Compile(g_manager, *reinterpret_cast<_STL::vector<CWideString>*>(&empty), prefix);
    if (!g_good || g_scans != 6 || g_sorts != 6 || g_reads != 6 ||
        g_parses != 3 || g_compileParses != 3 || g_updates != 4 || g_closes != 6) return 1;
    puts("CREATE_SYMBOLS_PATH_LIST_OK");
    return 0;
}
