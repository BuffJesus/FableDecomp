// Behaviour of CStartupDefinitionManager::InitAndCompilePlatformSpecific 0x004336BC:
// the symbol path list (bank header directory + "pc\" for the textures, front-end
// bank and particle headers, the def directory for the rest) and the compile path
// list, with the PC-only entries or frontend_test.def on the other platform; the
// five startup def classes in order with their allocators; then the two lists are
// handed to the manager and Compile runs. Every temporary string is released.
#include "fable_definition_manager.h"
#include "fable_bank_file.h"
#include "fable_resource_directories.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

// A wide string is a pointer to a heap copy of its text.
static int g_live;
static wchar_t* Dup(const wchar_t* a, const wchar_t* b)
{
    size_t n = wcslen(a) + (b ? wcslen(b) : 0);
    wchar_t* s = (wchar_t*)malloc((n + 1) * sizeof(wchar_t));
    wcscpy(s, a);
    if (b) wcscat(s, b);
    ++g_live;
    return s;
}
static wchar_t*& Text(const CWideString& s) { return *reinterpret_cast<wchar_t**>(const_cast<CWideString*>(&s)); }
CWideString::CWideString(const wchar_t* text) { Text(*this) = Dup(text, 0); }
CWideString::CWideString(const CWideString& other) { Text(*this) = Dup(Text(other), 0); }
CWideString::~CWideString() { free(Text(*this)); --g_live; }
CWideString FABLE_FASTCALL operator+(const CWideString& left, const CWideString& right)
{
    CWideString r(L"");
    free(Text(r)); --g_live;
    Text(r) = Dup(Text(left), Text(right));
    return r;
}
CWideString FABLE_FASTCALL operator+(const CWideString& left, const wchar_t* right)
{
    CWideString r(L"");
    free(Text(r)); --g_live;
    Text(r) = Dup(Text(left), right);
    return r;
}
CWideString FABLE_FASTCALL NBankFileManager::GetHeaderDir() { return CWideString(L"H\\"); }
CWideString FABLE_FASTCALL NResourceDirectoryNames::GetDefDir() { return CWideString(L"D\\"); }

// The two lists: pushes are recorded per vector.
static wchar_t g_pushed[2][24][64];
static int g_pushCount[2];
static const void* g_vectors[2];
static int Which(const void* v)
{
    for (int i = 0; i < 2; ++i) if (g_vectors[i] == v) return i;
    for (int i = 0; i < 2; ++i) if (!g_vectors[i]) { g_vectors[i] = v; return i; }
    return 0;
}
template <> void _STL::vector<CWideString>::push_back(const CWideString& x)
{
    int w = Which(this);
    wcscpy(g_pushed[w][g_pushCount[w]++], Text(x));
}
static char g_log[512];
static char g_buf[128];
static void Log(const char* t) { strcat(g_log, t); }
static const void* g_manager;
template <> _STL::vector<CWideString>& _STL::vector<CWideString>::operator=(const _STL::vector<CWideString>& x)
{
    sprintf(g_buf, "assign%d@%d;", Which(&x), (int)((const char*)this - (const char*)g_manager)); Log(g_buf);
    return *this;
}

// Def classes: names and allocators.
CCharString::CCharString(const char* text, long length) { *reinterpret_cast<const char**>(this) = text; if (length != -1) Log("badlen;"); }
CCharString::CCharString(const CCharString& other) { *reinterpret_cast<const char**>(this) = *reinterpret_cast<const char* const*>(&other); }
CCharString::~CCharString() {}
CDefClassBase* FABLE_FASTCALL CEngineVideoOptionsDef::Alloc() { return 0; }
CDefClassBase* FABLE_FASTCALL CEngineDef::Alloc() { return 0; }
CDefClassBase* FABLE_FASTCALL NUISystem::CUIDef::Alloc() { return 0; }
CDefClassBase* FABLE_FASTCALL CControlsDef::Alloc() { return 0; }
CDefClassBase* FABLE_FASTCALL CConfigOptionsDefaultsDef::Alloc() { return 0; }
void CDefinitionManager::AddDefClass(const CDefClassInit& init)
{
    const char* name = *reinterpret_cast<const char* const*>(&init.Name);
    const char* alloc = init.AllocFunc == CEngineVideoOptionsDef::Alloc ? "evo" : init.AllocFunc == CEngineDef::Alloc ? "eng" :
                        init.AllocFunc == NUISystem::CUIDef::Alloc ? "ui" : init.AllocFunc == CControlsDef::Alloc ? "ctl" :
                        init.AllocFunc == CConfigOptionsDefaultsDef::Alloc ? "cfg" : "?";
    sprintf(g_buf, "%s%s;", this == g_manager ? "" : "badthis:", name); Log(g_buf);
    sprintf(g_buf, "=%s;", alloc); Log(g_buf);
}
void CDefinitionManager::Compile() { Log(this == g_manager ? "compile;" : "badcompile;"); }

static int Check(int w, const wchar_t* const* want, int n, const char* what)
{
    if (g_pushCount[w] != n) { printf("%s: %d entries\n", what, g_pushCount[w]); return 1; }
    for (int i = 0; i < n; ++i)
        if (wcscmp(g_pushed[w][i], want[i]) != 0) { printf("%s %d: %ls\n", what, i, g_pushed[w][i]); return 1; }
    return 0;
}

static int Run(bool xbox)
{
    static unsigned char store[sizeof(CStartupDefinitionManager)];
    memset(g_pushCount, 0, sizeof(g_pushCount));
    g_vectors[0] = g_vectors[1] = 0;
    g_log[0] = 0;
    g_live = 0;
    g_manager = store;
    reinterpret_cast<CStartupDefinitionManager*>(store)->InitAndCompilePlatformSpecific(xbox);
    static const wchar_t* symPc[] = {
        L"H\\pc\\textures.h", L"H\\fonts.h", L"H\\pc\\front_end_bank.h", L"H\\pc\\particles_frontend.h",
        L"D\\FrontEndDefs\\engine.def", L"D\\FrontEndDefs\\engine_video_options.def", L"D\\FrontEndDefs\\front_end.def",
        L"D\\ui_types.h", L"D\\ui_dialogs.def", L"D\\FrontEndDefs\\pc_frontend.def", L"D\\config_options_defaults.def",
        L"D\\controller_def.h", L"D\\keyboard_keys.h", L"D\\pc_controls.def" };
    static const wchar_t* symX[] = {
        L"H\\pc\\textures.h", L"H\\fonts.h", L"H\\pc\\front_end_bank.h", L"H\\pc\\particles_frontend.h",
        L"D\\FrontEndDefs\\engine.def", L"D\\FrontEndDefs\\engine_video_options.def", L"D\\FrontEndDefs\\front_end.def",
        L"D\\ui_types.h", L"D\\ui_dialogs.def", L"D\\FrontEndDefs\\frontend_test.def" };
    static const wchar_t* cmpPc[] = {
        L"D\\FrontEndDefs\\engine.def", L"D\\FrontEndDefs\\engine_video_options.def", L"D\\FrontEndDefs\\front_end.def",
        L"D\\ui_dialogs.def", L"D\\FrontEndDefs\\pc_frontend.def", L"D\\pc_controls.def", L"D\\config_options_defaults.def" };
    static const wchar_t* cmpX[] = {
        L"D\\FrontEndDefs\\engine.def", L"D\\FrontEndDefs\\engine_video_options.def", L"D\\FrontEndDefs\\front_end.def",
        L"D\\ui_dialogs.def", L"D\\FrontEndDefs\\frontend_test.def" };
    if (xbox ? Check(0, symX, 10, "symbol") || Check(1, cmpX, 5, "compile")
             : Check(0, symPc, 14, "symbol") || Check(1, cmpPc, 7, "compile"))
        return 1;
    const char* want = "ENGINE_VIDEO_OPTIONS;=evo;ENGINE;=eng;UI;=ui;CONTROL_SCHEME;=ctl;CONFIG_OPTIONS_DEFAULTS_DEF;=cfg;"
                       "assign0@64;assign1@76;compile;";
    if (strcmp(g_log, want) != 0) { printf("log: %s\n", g_log); return 1; }
    if (g_live != 0) { printf("%d strings leaked\n", g_live); return 1; }
    return 0;
}

int main()
{
    if (Run(false) || Run(true)) return 1;
    puts("FABLETLC_CSTARTUPDEFINITIONMANAGER_INITANDCOMPILEPLATFORMSPECIFIC_004336BC_PASS");
    return 0;
}