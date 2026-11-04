// Behaviour fixture for the input core: CInputManager's scans, checksum,
// exclusive mode, device-number query, ClearEvents (with the real
// vector<CInputEvent>::erase), recording/replay entry points, and the device
// members (CJoystick scans and defaults, the three ClearAllMaintainedEvents,
// CMouse/CKeyboard defaults). The members are the real compiled providers.
// Device objects are raw storage of the retail size with a recording vtable;
// strings, the event dtor/type, the list clear, the disk file and package
// loading are recording stand-ins.
#include <new>
#include "fable_input_manager.h"
#include <stdio.h>
#include <string.h>

// ---- stand-ins -------------------------------------------------------------
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CWideString::CWideString() {}
CWideString::CWideString(const CWideString&) {}
CWideString::~CWideString() {}
CInputEvent::CInputEvent() {}
static int g_eventDtors;
CInputEvent::~CInputEvent() { ++g_eventDtors; }
EInputEventType CInputEvent::GetType() const { return *reinterpret_cast<const EInputEventType*>(reinterpret_cast<const char*>(this) + 0x28); }
static const void* g_listCleared[8]; static int g_listClears;
template <> void _STL::list<CMaintainedEvent>::clear() { g_listCleared[g_listClears++] = this; }
static int g_loads; static const CVersion* g_loadVersion; static CAFile* g_loadFile; static FileAccessType g_diskAccess; static unsigned long g_diskFlags; static int g_diskCloses;
void CInputManager::LoadAllEventPackages(const CVersion& version, CAFile& file) { ++g_loads; g_loadVersion = &version; g_loadFile = &file; }
CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType access, unsigned long flags) : Opened(false) { g_diskAccess = access; g_diskFlags = flags; }
void CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) {}
void CDiskFileWin32::Close() { ++g_diskCloses; }
void CDiskFileWin32::Read(void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::Write(const void*, long, CASuspendableProcess*) {}
void CDiskFileWin32::SetPosition(long) {}
void CDiskFileWin32::MovePosition(long) {}
long CDiskFileWin32::GetPosition() const { return 0; }
void CDiskFileWin32::SetLength(long) {}
CAFile* CDiskFileWin32::Clone() const { return 0; }
class CFileInformation { public: long Words[4]; };
CFileInformation CDiskFileWin32::GetFileInformation() { CFileInformation f = {0}; return f; }
CFileInformation CAFile::GetFileInformation() { CFileInformation f = {0}; return f; }
CAFile::CAFile() {}

struct CInputManagerTestAccess
{
    static CInputManager* Make(void* storage) { memset(storage, 0xa5, sizeof(CInputManager)); return static_cast<CInputManager*>(storage); }
    static void SetMouse(CInputManager* m, void* p) { *reinterpret_cast<void**>(&m->PMouse) = p; }
    static void SetKeyboard(CInputManager* m, void* p) { *reinterpret_cast<void**>(&m->PKeyboard) = p; }
    static void SetJoysticks(CInputManager* m, CCountedPointer<CJoystick>* begin, long count) { m->PJoysticks._M_start = begin; m->PJoysticks._M_finish = begin + count; m->PJoysticks._M_end_of_storage._M_data = begin + count; }
    static void SetEvents(_STL::vector<CInputEvent>& v, CInputEvent* begin, long count) { v._M_start = begin; v._M_finish = begin + count; v._M_end_of_storage._M_data = begin + count; }
    static _STL::vector<CInputEvent>& Processed(CInputManager* m) { return m->ProcessedEvents; }
    static _STL::vector<CInputEvent>& Store(CInputManager* m) { return m->EventStore; }
    static long& NoStored(CInputManager* m) { return m->NoStoredEvents; }
    static long& NoScans(CInputManager* m) { return m->NoEventScansRunning; }
    static CChecksum& Checksum(CInputManager* m) { return m->Checksum; }
    static long& TimeStamp(CInputManager* m) { return m->TimeStamp; }
    static long& PackageIndex(CInputManager* m) { return m->CurrentFilePackageIndex; }
    static bool& Loading(CInputManager* m) { return m->Loading; }
    static bool& Saving(CInputManager* m) { return m->Saving; }
    static void* PackageFile(CInputManager* m) { return &m->PackageFile; }
};
typedef CInputManagerTestAccess Access;
struct JoystickAccess : CJoystick
{
    static bool Start(const CJoystick* j, CInputEventScanInfo* i, CInputEvent* e) { return static_cast<const JoystickAccess*>(j)->CJoystick::EventScanStart(i, e); }
    static bool Next(const CJoystick* j, CInputEventScanInfo* i, CInputEvent* e) { return static_cast<const JoystickAccess*>(j)->CJoystick::EventScanNext(i, e); }
    static void End(const CJoystick* j, CInputEventScanInfo* i) { static_cast<const JoystickAccess*>(j)->CJoystick::EventScanEnd(i); }
    static long& Stored(CJoystick* j) { return static_cast<JoystickAccess*>(j)->NoStoredEvents; }
    static long& Scans(CJoystick* j) { return static_cast<JoystickAccess*>(j)->NoEventScansRunning; }
    static CInputEvent* Events(CJoystick* j) { return static_cast<JoystickAccess*>(j)->EventStore; }
};
struct MouseAccess : CMouse { static long& Stored(CMouse* m) { return static_cast<MouseAccess*>(m)->NoStoredEvents; } static void* List(CMouse* m) { return &static_cast<MouseAccess*>(m)->MaintainedEventsList; } };
struct KeyboardAccess : CKeyboard { static long& Stored(CKeyboard* k) { return static_cast<KeyboardAccess*>(k)->NoStoredEvents; } static void* List(CKeyboard* k) { return &static_cast<KeyboardAccess*>(k)->MaintainedEventsList; } };

// ---- recording device vtables ------------------------------------------------
static char g_trace[64]; static int g_traceSize; static bool g_lastBool; static long g_deviceNumbers[4];
static void Record(char c) { g_trace[g_traceSize++] = c; g_trace[g_traceSize] = 0; }
static char g_joystickMemory[4][sizeof(CJoystick)]; static char g_mouseMemory[sizeof(CMouse)], g_keyboardMemory[sizeof(CKeyboard)];
static int JoystickIndex(void* self) { for (int i = 0; i < 4; ++i) if (self == g_joystickMemory[i]) return i; return 9; }
static bool FABLE_FASTCALL MouseExclusive(void* self, void*, bool on) { Record(self == g_mouseMemory ? 'M' : '?'); g_lastBool = on; return false; }
static bool FABLE_FASTCALL KeyboardExclusive(void* self, void*, bool on) { Record(self == g_keyboardMemory ? 'K' : '?'); g_lastBool = on; return false; }
static bool FABLE_FASTCALL JoystickExclusive(void* self, void*, bool on) { Record((char)('0' + JoystickIndex(self))); g_lastBool = on; return false; }
static void FABLE_FASTCALL JoystickClear(void* self) { Record((char)('a' + JoystickIndex(self))); }
static long FABLE_FASTCALL JoystickNumber(void* self) { Record((char)('n')); return g_deviceNumbers[JoystickIndex(self)]; }
static void* g_mouseTable[18]; static void* g_keyboardTable[5]; static void* g_joystickTable[19];
static int g_fileOpens, g_fileWrites; static FileAccessType g_openAccess; static unsigned long g_openFlags; static const void* g_writeData; static long g_writeSize;
static void FABLE_FASTCALL FileOpen(void*, void*, const CWideString&, FileAccessType access, unsigned long flags) { ++g_fileOpens; g_openAccess = access; g_openFlags = flags; }
static void FABLE_FASTCALL FileWrite(void*, void*, const void* data, long size, CASuspendableProcess*) { ++g_fileWrites; g_writeData = data; g_writeSize = size; }
static void* g_fileTable[16];

static int g_fail;
#define CHECK(c, code) do { if (!(c)) { printf("INPUT_CORE_FAIL %d line %d\n", code, __LINE__); g_fail = 1; } } while (0)
static void SetEvent(CInputEvent& e, int seed, EInputEventType type) { memset(&e, seed, sizeof(e)); *reinterpret_cast<EInputEventType*>(reinterpret_cast<char*>(&e) + 0x28) = type; }

int main()
{
    int cases = 0;
    g_mouseTable[6] = (void*)MouseExclusive; g_keyboardTable[2] = (void*)KeyboardExclusive;
    g_joystickTable[8] = (void*)JoystickNumber; g_joystickTable[12] = (void*)JoystickExclusive; g_joystickTable[15] = (void*)JoystickClear;
    g_fileTable[1] = (void*)FileOpen; g_fileTable[4] = (void*)FileWrite;
    for (int j = 0; j < 4; ++j) *reinterpret_cast<void**>(g_joystickMemory[j]) = g_joystickTable;
    *reinterpret_cast<void**>(g_mouseMemory) = g_mouseTable; *reinterpret_cast<void**>(g_keyboardMemory) = g_keyboardTable;
    static union { double a; char b[sizeof(CInputManager)]; } storage;
    // Scans over the manager's store and a joystick's fixed store.
    CInputEvent events[8];
    for (int stored = 0; stored <= 6; ++stored) for (int nulls = 0; nulls < 4; ++nulls) {
        CInputManager* m = Access::Make(&storage);
        for (int i = 0; i < 8; ++i) SetEvent(events[i], 0x10 + i, ((nulls >> (i & 1)) & 1) && i ? IE_NULL : (EInputEventType)3);
        Access::SetEvents(Access::Store(m), events, 8); Access::NoStored(m) = stored; Access::NoScans(m) = 5;
        CInputEventScanInfo info; info.Started = false; info.Index = 77; CInputEvent out; memset(&out, 0xee, sizeof(out));
        bool found = m->EventScanStart(info, out);
        CHECK(info.Started && Access::NoScans(m) == 6 && found == (stored > 0), 1);
        if (stored) CHECK(info.Index == 0 && memcmp(&out, &events[0], sizeof(out)) == 0, 2); else CHECK(info.Index == 77, 3);
        long expected = 0;
        while (found) {
            long previous = info.Index; found = m->EventScanNext(info, out);
            long next = previous + 1; while (next < stored && events[next].GetType() == IE_NULL) ++next;
            if (next < stored) { CHECK(found && info.Index == next && memcmp(&out, &events[next], sizeof(out)) == 0, 4); ++expected; }
            else CHECK(!found && info.Index == previous, 5);
        }
        m->EventScanEnd(info); CHECK(!info.Started && Access::NoScans(m) == 5, 6);
        // The same walk over a joystick (no null skipping).
        CJoystick* j = reinterpret_cast<CJoystick*>(g_joystickMemory[0]);
        memcpy(JoystickAccess::Events(j), events, sizeof(events)); JoystickAccess::Stored(j) = stored; JoystickAccess::Scans(j) = 2;
        info.Index = 55; found = JoystickAccess::Start(j, &info, &out);
        CHECK(found == (stored > 0) && JoystickAccess::Scans(j) == 3 && info.Started, 7);
        long walked = found ? 1 : 0;
        while (found) { found = JoystickAccess::Next(j, &info, &out); if (found) { CHECK(info.Index == walked && memcmp(&out, &events[walked], sizeof(out)) == 0, 8); ++walked; } }
        CHECK(walked == stored, 9); JoystickAccess::End(j, &info); CHECK(!info.Started && JoystickAccess::Scans(j) == 2, 10);
        cases += 2;
    }
    // Checksum and time stamp.
    for (int k = 0; k < 4; ++k) {
        CInputManager* m = Access::Make(&storage); CChecksum c(k, k * 3, -k); c.Checksum4 = 0x1234 + k;
        unsigned char before[sizeof(CInputManager)]; memcpy(before, m, sizeof(before));
        m->UpdateChecksum(1000 + k, c);
        CHECK(memcmp(&Access::Checksum(m), &c, sizeof(c)) == 0 && Access::TimeStamp(m) == 1000 + k, 11);
        CHECK(memcmp(before, m, 0x3c) == 0 && memcmp(before + 0x60, reinterpret_cast<char*>(m) + 0x60, sizeof(before) - 0x60) == 0, 12);
        ++cases;
    }
    // Exclusive mode, device numbers and ClearEvents over every device mix.
    for (int haveMouse = 0; haveMouse < 2; ++haveMouse) for (int haveKeyboard = 0; haveKeyboard < 2; ++haveKeyboard)
    for (int count = 0; count <= 4; ++count) for (int nullMask = 0; nullMask < 4; ++nullMask) for (int on = 0; on < 2; ++on) {
        CInputManager* m = Access::Make(&storage);
        Access::SetMouse(m, haveMouse ? g_mouseMemory : 0); Access::SetKeyboard(m, haveKeyboard ? g_keyboardMemory : 0);
        unsigned long slots[8]; for (int i = 0; i < 4; ++i) { slots[i * 2] = (nullMask >> (i & 1) & 1) && i == 1 ? 0 : (unsigned long)g_joystickMemory[i]; slots[i * 2 + 1] = 0xc0de0000 + i; }
        Access::SetJoysticks(m, reinterpret_cast<CCountedPointer<CJoystick>*>(slots), count);
        g_traceSize = 0; g_trace[0] = 0;
        CHECK(m->CInputManager::SetExclusiveMode(on != 0), 13);
        char want[32] = ""; if (haveMouse) strcat(want, "M"); if (haveKeyboard) strcat(want, "K");
        for (int i = 0; i < count; ++i) if (slots[i * 2]) { size_t n = strlen(want); want[n] = (char)('0' + i); want[n + 1] = 0; }
        CHECK(strcmp(g_trace, want) == 0 && (!*want || g_lastBool == (on != 0)), 14);
        // Device numbers (null slots excluded from this query's setup: retail does not test them).
        unsigned long full[8]; for (int i = 0; i < 4; ++i) { full[i * 2] = (unsigned long)g_joystickMemory[i]; full[i * 2 + 1] = 0; g_deviceNumbers[i] = 10 + i * (nullMask + 1); }
        Access::SetJoysticks(m, reinterpret_cast<CCountedPointer<CJoystick>*>(full), count);
        for (long query = 9; query < 20; ++query) {
            bool want2 = false; for (int i = 0; i < count; ++i) if (g_deviceNumbers[i] == query) want2 = true;
            CHECK(m->IsJoystickDeviceNumberActive(query) == want2, 15);
        }
        // ClearEvents: both vectors emptied through the real erase, the store counter reset, devices cleared.
        Access::SetJoysticks(m, reinterpret_cast<CCountedPointer<CJoystick>*>(slots), count);
        CInputEvent processed[5], store[5]; for (int i = 0; i < 5; ++i) { SetEvent(processed[i], i, (EInputEventType)1); SetEvent(store[i], i + 5, (EInputEventType)2); }
        long pn = (count + on) % 6, sn = (count * 2 + nullMask) % 6;
        Access::SetEvents(Access::Processed(m), processed, pn); Access::SetEvents(Access::Store(m), store, sn); Access::NoStored(m) = 4;
        for (int i = 0; i < 4; ++i) JoystickAccess::Stored(reinterpret_cast<CJoystick*>(g_joystickMemory[i])) = 9;
        MouseAccess::Stored(reinterpret_cast<CMouse*>(g_mouseMemory)) = 9; KeyboardAccess::Stored(reinterpret_cast<CKeyboard*>(g_keyboardMemory)) = 9;
        g_traceSize = 0; g_trace[0] = 0; g_listClears = 0; g_eventDtors = 0;
        m->ClearEvents();
        CHECK(Access::Processed(m).size() == 0 && Access::Store(m).size() == 0 && Access::Processed(m)._M_start == processed && Access::Store(m)._M_start == store, 16);
        CHECK(g_eventDtors == pn + sn && Access::NoStored(m) == 0, 17);
        char want3[32] = ""; int expectLists = 0;
        for (int i = 0; i < count; ++i) if (slots[i * 2]) { size_t n = strlen(want3); want3[n] = (char)('a' + i); want3[n + 1] = 0; CHECK(g_listCleared[expectLists++] == (char*)g_joystickMemory[i] + 0xd0c, 18); CHECK(JoystickAccess::Stored(reinterpret_cast<CJoystick*>(g_joystickMemory[i])) == 0, 19); }
        CHECK(strcmp(g_trace, want3) == 0, 20);
        if (haveMouse) { CHECK(g_listCleared[expectLists++] == MouseAccess::List(reinterpret_cast<CMouse*>(g_mouseMemory)) && MouseAccess::Stored(reinterpret_cast<CMouse*>(g_mouseMemory)) == 0, 21); }
        if (haveKeyboard) { CHECK(g_listCleared[expectLists++] == KeyboardAccess::List(reinterpret_cast<CKeyboard*>(g_keyboardMemory)) && KeyboardAccess::Stored(reinterpret_cast<CKeyboard*>(g_keyboardMemory)) == 0, 22); }
        CHECK(g_listClears == expectLists, 23);
        cases += 3;
    }
    // Input focus reaches every device once (gate_input_focus.py covers the mutation cases).
    g_mouseTable[14] = (void*)MouseExclusive; g_keyboardTable[3] = (void*)KeyboardExclusive; g_joystickTable[16] = (void*)JoystickExclusive;
    for (int on = 0; on < 2; ++on) for (int count = 0; count <= 4; ++count) {
        CInputManager* m = Access::Make(&storage);
        Access::SetMouse(m, g_mouseMemory); Access::SetKeyboard(m, g_keyboardMemory);
        unsigned long slots[8]; for (int i = 0; i < 4; ++i) { slots[i * 2] = (unsigned long)g_joystickMemory[i]; slots[i * 2 + 1] = 0; }
        Access::SetJoysticks(m, reinterpret_cast<CCountedPointer<CJoystick>*>(slots), count);
        g_traceSize = 0; g_trace[0] = 0;
        m->SetHasInputFocus(on != 0);
        char want[16] = "MK"; for (int i = 0; i < count; ++i) { want[2 + i] = (char)('0' + i); want[3 + i] = 0; }
        CHECK(strcmp(g_trace, want) == 0 && g_lastBool == (on != 0), 30);
        ++cases;
    }
    // Recording and replay entry points.
    for (int k = 0; k < 3; ++k) {
        CInputManager* m = Access::Make(&storage); *reinterpret_cast<void**>(Access::PackageFile(m)) = g_fileTable;
        Access::Saving(m) = false; Access::Loading(m) = false; Access::PackageIndex(m) = 99;
        CVersion version; version.Month = (unsigned char)k; version.Day = 2; version.Year = 2004; version.Build = 1000 + k;
        CWideString* name = reinterpret_cast<CWideString*>(&version);  // only passed through
        g_fileOpens = g_fileWrites = 0;
        m->StreamInputsToFile(version, *name);
        CHECK(g_fileOpens == 1 && g_openAccess == FILE_ACCESS_CREATE && g_openFlags == 2 && g_fileWrites == 1 && g_writeData == &version && g_writeSize == 12, 24);
        CHECK(Access::PackageIndex(m) == 0 && Access::Saving(m) && !Access::Loading(m), 25);
        g_loads = 0; g_diskCloses = 0;
        m->StreamInputsFromFile(version, *name);
        CHECK(g_loads == 1 && g_loadVersion == &version && g_diskAccess == FILE_ACCESS_READ && g_diskFlags == 2 && g_diskCloses == 1 && Access::Loading(m), 26);
        cases += 2;
    }
    // Device defaults.
    CJoystick* pad = reinterpret_cast<CJoystick*>(g_joystickMemory[1]);
    unsigned char before[sizeof(CJoystick)]; memcpy(before, pad, sizeof(before));
    CHECK(pad->CJoystick::SetExclusiveMode(false) && pad->CJoystick::SetExclusiveMode(true), 27);
    pad->CJoystick::Update(); pad->CJoystick::UpdateInsertionsAndRemovals(1, 2); pad->CJoystick::Clear(); pad->CJoystick::SetHasInputFocus(true);
    reinterpret_cast<CMouse*>(g_mouseMemory)->CMouse::SetHasInputFocus(true);
    reinterpret_cast<CKeyboard*>(g_keyboardMemory)->CKeyboard::SetHasInputFocus(false);
    CHECK(!reinterpret_cast<CKeyboard*>(g_keyboardMemory)->CKeyboard::GetKeyName(EInputKey(5), *reinterpret_cast<CWideString*>(before)), 28);
    CHECK(memcmp(before, pad, sizeof(before)) == 0, 29);
    cases += 4;
    if (g_fail) return 1;
    printf("INPUT_CORE_BEHAVIOR_PASS scenarios=%d\n", cases);
    return 0;
}
