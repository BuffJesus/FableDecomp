// Behaviour fixture for the CMemoryFile family (Open/Read/Write/seek/length,
// Close, Copy, Clone, OpenFromFile, DumpToFile, the vector copy helper).
// The CMemoryFile members are the real compiled providers; strings, the CAFile
// statics and CDiskFileWin32 are recording stand-ins over an in-memory disk.
#include <new>
class CFileInformation { public: unsigned long Words[8]; };
// The string stand-ins reach the storage pointer; no private member functions are used.
#define private public
#include "fable_string.h"
#undef private
#include "fable_memory_file.h"
#include "fable_file_exception.h"
#include "fable_stlport_vector_impl.h"
#define private public
#include "fable_disk_file.h"
#undef private
// Retail's vector<unsigned char>::resize lives in another unit; instantiate it.
template void _STL::vector<unsigned char>::resize(size_t, const unsigned char&);
// Copy is private: an explicit instantiation may name it (access is not
// checked there), which hands the fixture a pointer to the real member.
typedef void (CMemoryFile::*CopyMember)(const CMemoryFile&);
static CopyMember g_copyMember;
template <CopyMember P> struct CopyGrab { CopyGrab() { g_copyMember = P; } static CopyGrab instance; };
template <CopyMember P> CopyGrab<P> CopyGrab<P>::instance;
template struct CopyGrab<&CMemoryFile::Copy>;
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>

// ---- strings: heap copies behind the storage pointer -----------------------
static char* NarrowCopy(const char* text) { size_t n = strlen(text); char* p = (char*)malloc(n + 1); memcpy(p, text, n + 1); return p; }
static wchar_t* WideCopy(const wchar_t* text) { size_t n = wcslen(text); wchar_t* p = (wchar_t*)malloc((n + 1) * 2); memcpy(p, text, (n + 1) * 2); return p; }
#define NARROW(s) reinterpret_cast<char*>((s).storage_)
#define WIDE(s) reinterpret_cast<wchar_t*>((s).storage_)
static char g_lastText[256];
CCharString::CCharString(const char* text, long length) { storage_ = reinterpret_cast<CCharStringData*>(NarrowCopy(length < 0 ? text : "")); }
CCharString::CCharString(const CCharString& other) { storage_ = reinterpret_cast<CCharStringData*>(NarrowCopy(NARROW(other))); }
CCharString::~CCharString() { free(NARROW(*this)); }
CCharString::operator const char*() const { strncpy(g_lastText, NARROW(*this), 255); return NARROW(*this); }
CCharString FABLE_FASTCALL operator+(const CCharString& left, const char* right)
{
    char buffer[512]; strcpy(buffer, NARROW(left)); strcat(buffer, right); return CCharString(buffer);
}
CCharString FABLE_FASTCALL operator+(const CCharString& left, const CCharString& right) { return left + NARROW(right); }
CWideString::CWideString() { storage_ = reinterpret_cast<CWideStringData*>(WideCopy(L"")); }
CWideString::CWideString(const wchar_t* text) { storage_ = reinterpret_cast<CWideStringData*>(WideCopy(text)); }
CWideString::CWideString(const CWideString& other) { storage_ = reinterpret_cast<CWideStringData*>(WideCopy(WIDE(other))); }
CWideString::~CWideString() { free(WIDE(*this)); }
CWideString& CWideString::operator=(const CWideString& other) { if (this != &other) { free(WIDE(*this)); storage_ = reinterpret_cast<CWideStringData*>(WideCopy(WIDE(other))); } return *this; }
const CWideString& CWideString::operator=(const wchar_t* text) { free(WIDE(*this)); storage_ = reinterpret_cast<CWideStringData*>(WideCopy(text)); return *this; }
CCharString CWideString::ToCharString() const { char buffer[256]; wcstombs(buffer, WIDE(*this), 255); buffer[255] = 0; return CCharString(buffer); }
static bool Is(const CWideString& s, const wchar_t* text) { return wcscmp(WIDE(s), text) == 0; }

// ---- base classes and CAFile statics ---------------------------------------
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CAFile::CAFile() {}
static int g_fileInfo;
CFileInformation CAFile::GetFileInformation() { ++g_fileInfo; return *(CFileInformation*)0; }
CWideString FABLE_FASTCALL CAFile::ConvertPathToFullPath(const CWideString& path)
{
    wchar_t buffer[128]; wcscpy(buffer, L"full:"); wcscat(buffer, WIDE(path)); return CWideString(buffer);
}

// ---- the in-memory disk ----------------------------------------------------
struct Disk { wchar_t name[32]; unsigned char bytes[256]; long length; bool exists; };
static Disk g_disk[4];
static int g_diskOpens, g_diskCloses, g_diskReads, g_diskWrites; static unsigned long g_diskFlags; static FileAccessType g_diskAccess;
static Disk* Find(const wchar_t* name) { for (int i = 0; i < 4; ++i) if (g_disk[i].exists && !wcscmp(g_disk[i].name, name)) return &g_disk[i]; return 0; }
bool FABLE_FASTCALL CAFile::PathExists(const CWideString& full)
{
    const wchar_t* text = WIDE(full);
    return wcsncmp(text, L"full:", 5) == 0 && Find(text + 5) != 0;
}
CDiskFileWin32::CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags) : Opened(false)
{
    Open(pathname, access, flags);
}
void CDiskFileWin32::Open(const CWideString& pathname, FileAccessType access, unsigned long flags)
{
    ++g_diskOpens; g_diskFlags = flags; g_diskAccess = access; Pathname = pathname;
    Disk* d = Find(WIDE(pathname));
    if (!d) for (int i = 0; i < 4; ++i) if (!g_disk[i].exists) { d = &g_disk[i]; d->exists = true; wcscpy(d->name, WIDE(pathname)); d->length = 0; break; }
    WinFileHandle = d; AccessType = access; Position = 0; Length = d->length; Opened = true;
}
void CDiskFileWin32::Close() { ++g_diskCloses; Opened = false; }
void CDiskFileWin32::Read(void* data, long size, CASuspendableProcess*) { ++g_diskReads; memcpy(data, ((Disk*)WinFileHandle)->bytes + Position, size); Position += size; }
void CDiskFileWin32::Write(const void* data, long size, CASuspendableProcess*)
{
    ++g_diskWrites; Disk* d = (Disk*)WinFileHandle; memcpy(d->bytes + Position, data, size); Position += size; if (Position > d->length) d->length = Position; Length = d->length;
}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return Position; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
CFileInformation CDiskFileWin32::GetFileInformation() { return *(CFileInformation*)0; }

// An aligned source for OpenFromFile.
struct AlignedSource : public CMemoryFile
{
    long alignment;
    virtual long GetAlignment() const { return alignment; }
};

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("MEMORY_FILE_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)
template <class F> static int Throws(F f, CBBBFileException::EFileExceptionType type, const char* text)
{
    g_lastText[0] = 0;
    try { f(); } catch (CBBBFileException& e) { return e.Type == type && strcmp(g_lastText, text) == 0; }
    return 0;
}
static CMemoryFile* g_file; static long g_arg; static void* g_ptr;
static void DoSet() { g_file->SetPosition(g_arg); }
static void DoMove() { g_file->MovePosition(g_arg); }
static void DoRead() { g_file->Read(g_ptr, g_arg, 0); }
static void DoWrite() { g_file->Write(g_ptr, g_arg, 0); }
static CMemoryFile* g_source; static CMemoryFile* g_target;
static void DoOpenFrom() { g_target->OpenFromFile(*g_source, 0, 0); }
static void DoOpenMissing() { g_file->Open(CWideString(L"missing"), FILE_ACCESS_READ, 2); }

static void Fill(CMemoryFile& f, long n, int seed)
{
    unsigned char bytes[256]; for (long i = 0; i < n; ++i) bytes[i] = (unsigned char)(i * 13 + seed);
    f.Open(bytes, n);
}
static bool Same(const CMemoryFile& f, const unsigned char* bytes, long n)
{
    return f.Length == n && (long)f.DataBuffer.size() == n && (n == 0 || memcmp(&f.DataBuffer[0], bytes, n) == 0);
}

int main()
{
    const long lengths[] = { 0, 1, 5, 17, 64 };
    int cases = 0;
    for (int li = 0; li < 5; ++li) for (int seed = 0; seed < 3; ++seed) {
        const long n = lengths[li];
        unsigned char expect[512]; for (long i = 0; i < n; ++i) expect[i] = (unsigned char)(i * 13 + seed);
        CMemoryFile f; g_file = &f;
        // Open(): empty, open, nameless.
        f.Open(); CHECK(f.Opened && f.Position == 0 && f.Length == 0 && Is(f.Pathname, L""), 1);
        // Open(void*, long): an exact copy.
        Fill(f, n, seed); CHECK(Same(f, expect, n) && f.Position == 0 && f.Opened && f.IsOpen() && f.GetLength() == n, 2);
        // Seeks: every position in and just past range.
        for (long p = -2; p <= n + 2; ++p) {
            f.Position = 0; g_arg = p;
            if (p > n) CHECK(Throws(DoSet, CBBBFileException::BAD_SEEK, "Cannot seek past end of file") && f.Position == 0, 3);
            else { f.SetPosition(p); CHECK(f.GetPosition() == p, 4); }
            for (long q = -3; q <= 3; ++q) {
                f.Position = p < 0 ? 0 : p > n ? n : p; long before = f.Position; g_arg = q;
                if (before + q < 0) CHECK(Throws(DoMove, CBBBFileException::BAD_SEEK, "Cannot move before beginning of file") && f.Position == before, 5);
                else if (before + q > n) CHECK(Throws(DoMove, CBBBFileException::BAD_SEEK, "Cannot seek past end of file") && f.Position == before, 6);
                else { f.MovePosition(q); CHECK(f.Position == before + q, 7); }
                ++cases;
            }
        }
        // Reads from every position, every size.
        for (long p = 0; p <= n; ++p) for (long z = 0; z <= n - p + 1; ++z) {
            unsigned char out[512]; memset(out, 0xEE, sizeof(out)); f.Position = p; g_arg = z; g_ptr = out;
            if (p + z > n) CHECK(Throws(DoRead, CBBBFileException::END_OF_FILE, "Cannot read past end of file") && f.Position == p, 8);
            else { f.Read(out, z, 0); CHECK(f.Position == p + z && memcmp(out, expect + p, z) == 0 && out[z] == 0xEE, 9); }
            ++cases;
        }
        g_ptr = 0; g_arg = 1; CHECK(Throws(DoRead, CBBBFileException::BAD_PARAMETERS, "Null dst parameter"), 10);
        f.Position = n; f.Read(0 + (char*)expect, 0, 0); CHECK(f.Position == n, 11);
        // Writes: overwrite and extend from every position.
        for (long p = 0; p <= n; ++p) for (long z = 0; z <= 9; z += 3) {
            Fill(f, n, seed); f.AccessType = FILE_ACCESS_READ_WRITE; f.Position = p;
            unsigned char src[16]; for (long i = 0; i < z; ++i) src[i] = (unsigned char)(0xA0 + i);
            f.Write(src, z, 0);
            unsigned char want[512]; memcpy(want, expect, n); long end = p + z > n ? p + z : n; memset(want + n, 0, end > n ? end - n : 0); memcpy(want + p, src, z);
            CHECK(Same(f, want, end) && f.Position == p + z, 12);
            ++cases;
        }
        // Read-only buffers refuse writes; null sources are rejected; overflowing ends are ignored.
        Fill(f, n, seed); f.AccessType = FILE_ACCESS_READ; g_ptr = expect; g_arg = 1;
        CHECK(Throws(DoWrite, CBBBFileException::NOT_OPEN_FOR_WRITING, "File is not open for writing") && Same(f, expect, n), 13);
        f.AccessType = FILE_ACCESS_READ; f.Write(expect, 0, 0); CHECK(Same(f, expect, n), 14);
        g_ptr = 0; CHECK(Throws(DoWrite, CBBBFileException::BAD_PARAMETERS, "Null src parameter"), 15);
        f.AccessType = FILE_ACCESS_READ; f.Position = 0x7fffff00; f.Write(expect, 0x200, 0); CHECK(f.Position == 0x7fffff00 && Same(f, expect, n), 16);
        // SetLength only grows (zero-filled).
        for (long z = 0; z <= n + 4; z += 2) {
            Fill(f, n, seed); f.SetLength(z); long end = z > n ? z : n; unsigned char want[512]; memcpy(want, expect, n); memset(want + n, 0, end - n);
            CHECK(Same(f, want, end), 17); ++cases;
        }
        // Copy constructor through Clone; Copy appends to the existing buffer.
        Fill(f, n, seed); f.Pathname = L"clone"; f.Position = n / 2; f.AccessType = FILE_ACCESS_READ_WRITE;
        CMemoryFile* c = static_cast<CMemoryFile*>(f.Clone());
        CHECK(Same(*c, expect, n) && c->Position == n / 2 && c->AccessType == FILE_ACCESS_READ_WRITE && c->Opened && Is(c->Pathname, L"clone"), 18);
        (c->*g_copyMember)(f); unsigned char twice[512]; memcpy(twice, expect, n); memcpy(twice + n, expect, n);
        CHECK((long)c->DataBuffer.size() == 2 * n && (n == 0 || memcmp(&c->DataBuffer[0], twice, 2 * n) == 0) && c->Length == n, 19);
        delete c;
        // OpenFromFile: name from the source, aligned buffer, exact read.
        for (long a = 1; a <= 16; a *= 4) {
            AlignedSource s; Fill(s, n, seed); s.Pathname = L"source"; s.alignment = a; s.Position = 0;
            CMemoryFile g; g_source = &s; g_target = &g;
            long rounded = (n + a - 1) / a * a;
            if (n == 0)  // an empty buffer has no storage: the source's Read rejects the null destination
                CHECK(Throws(DoOpenFrom, CBBBFileException::BAD_PARAMETERS, "Null dst parameter") && Is(g.Pathname, L"source") && g.Opened && g.DataBuffer.size() == 0, 26);
            else {
                g.OpenFromFile(s, n, 0);
                CHECK(Is(g.Pathname, L"source") && g.Length == n && g.Position == 0 && g.Opened && (long)g.DataBuffer.size() == rounded && memcmp(&g.DataBuffer[0], expect, n) == 0 && s.Position == n, 20);
            }
            ++cases;
        }
        // Disk round trip: DumpToFile writes, Open(3) reads; a missing read-only path throws.
        memset(g_disk, 0, sizeof(g_disk)); g_diskOpens = g_diskCloses = g_diskReads = g_diskWrites = 0;
        Fill(f, n, seed); f.Pathname = L"disk"; f.AccessType = FILE_ACCESS_READ_WRITE; f.DumpToFile();
        CHECK(g_diskOpens == 1 && g_diskWrites == 1 && g_diskCloses == 1 && g_diskFlags == 2 && g_diskAccess == FILE_ACCESS_READ_WRITE && Find(L"disk")->length == n, 21);
        CMemoryFile h(CWideString(L"disk"), FILE_ACCESS_READ, 7);
        CHECK(Same(h, expect, n) && h.Position == 0 && h.Opened && h.AccessType == FILE_ACCESS_READ && Is(h.Pathname, L"disk") && g_diskFlags == 7 && g_diskReads == (n > 0) && g_diskCloses == 2, 22);
        g_file = &h; CHECK(Throws(DoOpenMissing, CBBBFileException::BAD_PATH, "File does not exist: full:missing"), 23);
        h.Open(CWideString(L"fresh"), FILE_ACCESS_READ_WRITE_CREATE, 1);
        // A path that does not exist yet keeps the current buffer (nothing is read).
        CHECK(Same(h, expect, n) && h.Position == 0 && h.AccessType == FILE_ACCESS_READ_WRITE_CREATE && Is(h.Pathname, L"fresh") && h.Opened && g_diskOpens == 2, 24);
        // Close empties and closes; the destructor's Close is harmless again.
        Fill(f, n, seed); f.Close(); CHECK(!f.Opened && f.DataBuffer.size() == 0 && f.Length == n, 25);
        cases += 4;
    }
    if (g_fail) return 1;
    printf("MEMORY_FILE_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
