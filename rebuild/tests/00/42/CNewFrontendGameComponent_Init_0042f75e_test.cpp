#include "fable_game.h"
#include "fable_console.h"
#include "test_support/console_member_stubs.h"
#include <stdio.h>
#include <string.h>

CNewFrontendGameComponent* g_FableFrontendGameComponent_013B871C;
CCountedPointer<CConsole> g_FableConsolePClass_013CAA40;
static int sequence, defsAt, loadAt, consoleAt, constructions, resets;
static bool ok;
static CNewFrontendGameComponent* expectedFrontend;
static const CFontBank* expectedFont = reinterpret_cast<const CFontBank*>(0x12345000);
static const void* consoleThis;
void CNewFrontendGameComponent::InitialiseDefs() {
    defsAt = ++sequence;
    ok = ok && this == expectedFrontend && g_FableFrontendGameComponent_013B871C == this;
    *reinterpret_cast<const CFontBank**>(reinterpret_cast<char*>(this) + 0x10) = expectedFont;
}
void FABLE_FASTCALL StaticLoadXMVCode() {
    loadAt = ++sequence;
    ok = ok && defsAt == 1 && reinterpret_cast<unsigned char*>(expectedFrontend)[0xB1] == 0;
}
CConsole::CConsole() { ++constructions; }
template<> void CCountedPointer<CConsole>::Reset(CConsole* console) { Data = console; ++resets; }
void CConsole::Initialise(char activate, EInputKey key, const CFontBank* font) {
    consoleAt = ++sequence;
    consoleThis = this;
    ok = ok && loadAt == 2 && activate == '`' && key == EInputKey(0x29) &&
        font == expectedFont && reinterpret_cast<unsigned char*>(expectedFrontend)[0xB1] == 1;
}
static bool Check(bool existing) {
    unsigned long storage[sizeof(CNewFrontendGameComponent)/4];
    unsigned char expected[sizeof(storage)];
    memset(storage, 0xA5, sizeof(storage));
    reinterpret_cast<unsigned char*>(storage)[0xB1] = 0;
    memcpy(expected, storage, sizeof(storage));
    expected[0xB1] = 1;
    memcpy(expected + 0x10, &expectedFont, sizeof(expectedFont));
    expectedFrontend = reinterpret_cast<CNewFrontendGameComponent*>(storage);
    g_FableFrontendGameComponent_013B871C = 0;
    sequence = defsAt = loadAt = consoleAt = constructions = resets = 0;
    ok = true;
    if (existing) g_FableConsolePClass_013CAA40.Data = new CConsole;
    constructions = 0;
    expectedFrontend->CNewFrontendGameComponent::Init();
    ok = ok && defsAt == 1 && loadAt == 2 && consoleAt == 3 &&
        constructions == (existing ? 0 : 1) && resets == (existing ? 0 : 1) &&
        consoleThis == g_FableConsolePClass_013CAA40.Data &&
        memcmp(expected, storage, sizeof(storage)) == 0;
    delete g_FableConsolePClass_013CAA40.Data;
    g_FableConsolePClass_013CAA40.Data = 0;
    return ok;
}
int main() {
    if (!Check(false) || !Check(true)) return 1;
    puts("FABLETLC_NEW_FRONTEND_INIT PASS");
    return 0;
}