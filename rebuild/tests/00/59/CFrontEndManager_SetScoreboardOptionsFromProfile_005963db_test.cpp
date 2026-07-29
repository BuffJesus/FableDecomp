// Behavior fixture for SetScoreboardOptionsFromProfile @ 0x005963DB.

#include <cstdio>
#include <cstring>

extern "C" void __fastcall
CFrontEndManager_SetScoreboardOptionsFromProfile_005963db(void*, void*);

extern "C" const char FableFrontEndScoreboardLogin[] =
    "UI_SCOREBOARD_LOGIN_EDITBOX_FE";
extern "C" const char FableFrontEndScoreboardPassword[] =
    "UI_SCOREBOARD_PASSWORD_EDITBOX_FE";
extern "C" const char FableFrontEndScoreboardProxyIp[] =
    "UI_SCOREBOARD_PROXY_IP_EDITBOX_FE";
extern "C" const char FableFrontEndScoreboardProxyPort[] =
    "UI_SCOREBOARD_PROXY_PORT_EDITBOX_FE";
extern "C" const char FableFrontEndScoreboardServerPort[] =
    "UI_SCOREBOARD_SERVER_PORT_EDITBOX_FE";

struct Probe { const void* value; };
struct Applied { const char* name; const void* value; };
static unsigned char g_service[0x124];
static void* g_manager;
static Applied g_applied[5];
static int g_applyCount, g_nameConstructs, g_nameDestroys, g_wideDestroys;
static long g_formatted[2];
static int g_formatCount, g_convertCount, g_serviceCalls;
static unsigned long g_portMarkers[2];

extern "C" void __fastcall
FableFrontEndScoreboardOptionsConstructName(
    void* name, void*, const char* text, long length)
{
    if (length == -1) ++g_nameConstructs;
    ((Probe*)name)->value = text;
}
extern "C" void* __cdecl FableFrontEndScoreboardOptionsGetProfileService()
{
    ++g_serviceCalls; return g_service;
}
extern "C" void __fastcall FableFrontEndScoreboardOptionsCopyWide(
    void* destination, void*, const void* source)
{
    ((Probe*)destination)->value = source;
}
extern "C" void __fastcall FableFrontEndScoreboardOptionsSetEdit(
    void* manager, void*, void* name, void* value)
{
    if (manager == g_manager) {
        g_applied[g_applyCount].name = (const char*)((Probe*)name)->value;
        g_applied[g_applyCount].value = ((Probe*)value)->value;
        ++g_applyCount;
    }
}
extern "C" void __fastcall
FableFrontEndScoreboardOptionsDestroyWide(void*, void*) { ++g_wideDestroys; }
extern "C" void __fastcall
FableFrontEndScoreboardOptionsDestroyName(void*, void*) { ++g_nameDestroys; }
extern "C" void* __fastcall FableFrontEndScoreboardOptionsFormatInteger(
    void* temporary, long value, void* output)
{
    g_formatted[g_formatCount] = value;
    ((Probe*)output)->value = &g_portMarkers[g_formatCount];
    ((Probe*)temporary)->value = &g_portMarkers[g_formatCount];
    ++g_formatCount;
    return output;
}
extern "C" void* __fastcall
FableFrontEndScoreboardOptionsConvertInteger(void* formatted, void*)
{
    ++g_convertCount; return formatted;
}

int main()
{
    unsigned char manager[0x20];
    std::memset(manager, 0, sizeof(manager));
    std::memset(g_service, 0, sizeof(g_service));
    g_manager = manager;
    g_applyCount = g_nameConstructs = g_nameDestroys = g_wideDestroys = 0;
    g_formatCount = g_convertCount = g_serviceCalls = 0;
    *(unsigned long*)(g_service + 0x11c) = 8080;
    *(unsigned long*)(g_service + 0x120) = 6500;
    CFrontEndManager_SetScoreboardOptionsFromProfile_005963db(manager, 0);

    static const char* names[] = {
        FableFrontEndScoreboardLogin, FableFrontEndScoreboardPassword,
        FableFrontEndScoreboardProxyIp, FableFrontEndScoreboardProxyPort,
        FableFrontEndScoreboardServerPort };
    bool mapping = true;
    for (int i=0;i<5;++i)
        mapping = mapping && std::strcmp(g_applied[i].name,names[i])==0;
    mapping = mapping &&
        g_applied[0].value == g_service + 0x110 &&
        g_applied[1].value == g_service + 0x114 &&
        g_applied[2].value == g_service + 0x118 &&
        g_applied[3].value == &g_portMarkers[0] &&
        g_applied[4].value == &g_portMarkers[1];
    const bool pass = mapping && g_applyCount==5 && g_serviceCalls==5 &&
        g_nameConstructs==5 && g_nameDestroys==7 && g_wideDestroys==5 &&
        g_formatCount==2 && g_convertCount==2 &&
        g_formatted[0]==8080 && g_formatted[1]==6500;
    if (!pass) {
        std::printf("FSE2_005963db_TEST FAIL map=%d apply=%d svc=%d "
                    "names=%d/%d wide=%d fmt=%d conv=%d vals=%ld/%ld\n",
            mapping?1:0,g_applyCount,g_serviceCalls,g_nameConstructs,
            g_nameDestroys,g_wideDestroys,g_formatCount,g_convertCount,
            g_formatted[0],g_formatted[1]);
        return 1;
    }
    std::puts("FSE2_005963db_TEST PASS");
    return 0;
}
