#include "fable_system_manager_state.h"
#include <stdio.h>
#include <string.h>
#include <wchar.h>

static const char* expectedInput;
static const wchar_t* convertedText;
static const CCharString* narrow;
static const CWideString* wide;
static int failures, narrowAlive, wideAlive, dialogs;
static int dialogResult;
static char trace[64];
static int traceSize;
#define CHECK(x) do { if (!(x)) { ++failures; printf("FAIL line=%d\n", __LINE__); } } while (0)
static void Record(char c) { trace[traceSize++] = c; trace[traceSize] = 0; }

CCharString::CCharString(const char* text, long length)
{
    CHECK(text == expectedInput && length == -1);
    CHECK(!narrowAlive);
    storage_ = 0;
    narrow = this; narrowAlive = 1;
    Record('N');
}
CCharString::~CCharString()
{
    CHECK(this == narrow && narrowAlive && !wideAlive);
    narrowAlive = 0; Record('n');
}
CWideString::CWideString() { storage_ = 0; ++wideAlive; }
CWideString::CWideString(const CWideString&) { storage_ = 0; ++wideAlive; }
CWideString CCharString::ToWideString() const
{
    CHECK(this == narrow && narrowAlive);
    Record('T');
    return CWideString();
}
const wchar_t* CWideString::GetAsCharArray() const
{
    CHECK(narrowAlive && wideAlive == 1);
    wide = this;
    Record('A');
    return convertedText;
}
CWideString::~CWideString()
{
    CHECK(narrowAlive && wideAlive > 0);
    --wideAlive;
    // Ignore conversion-stub return copies, if the fixture compiler makes any.
    if (this == wide) Record('w');
}
static int FABLE_STDCALL Dialog(HWND__* window, const wchar_t* text,
                                const wchar_t* caption, unsigned int type)
{
    CHECK(window == 0 && type == 0);
    CHECK(narrowAlive && wideAlive == 1);
    CHECK(text == convertedText);
    CHECK(wcscmp(caption, L"Application Error") == 0);
    ++dialogs; Record('D');
    return dialogResult;
}
extern "C" int (FABLE_STDCALL *TestMessageBoxW)(HWND__*, const wchar_t*, const wchar_t*, unsigned int) = Dialog;
#pragma comment(linker, "/alternatename:__imp__MessageBoxW@16=_TestMessageBoxW")

int main()
{
    const char* inputs[] = {0, "", "Disk error", "value %s %n", "\x80\xff"};
    const wchar_t* outputs[] = {L"", L"converted text", L"\x4e2d\x6587"};
    const int results[] = {0, 1, -1};
    int scenarios = 0;
    for (int i = 0; i != 5; ++i)
    for (int j = 0; j != 3; ++j)
    for (int k = 0; k != 3; ++k)
    {
        unsigned long storage[sizeof(CSystemManager)/sizeof(unsigned long)];
        memset(storage, 0xa5, sizeof(storage));
        unsigned char before[sizeof(storage)];
        memcpy(before, storage, sizeof(storage));
        expectedInput = inputs[i]; convertedText = outputs[j]; dialogResult = results[k];
        traceSize = narrowAlive = wideAlive = dialogs = 0;
        narrow = 0; wide = 0; trace[0] = 0;
        reinterpret_cast<CSystemManager*>(storage)->DisplayCriticalMessage(expectedInput);
        CHECK(strcmp(trace, "NTADwn") == 0);
        CHECK(!narrowAlive && !wideAlive && dialogs == 1);
        CHECK(memcmp(before, storage, sizeof(storage)) == 0);
        ++scenarios;
    }
    if (failures) return 1;
    printf("CRITICAL_MESSAGE_BEHAVIOR_PASS scenarios=%d\n", scenarios);
    return 0;
}
