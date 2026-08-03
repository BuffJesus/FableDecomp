#include <cstdio>

class CCharString;
class CIDrawEnvironment;

struct CWorldMap {
    CIDrawEnvironment* GetRegionNumberFromName(const CCharString& regionName);
};
struct CWorld {
    CIDrawEnvironment* DrawGetEnvironment();
};
struct WorldProvider { void** vtable; };
struct CGameScriptInterface {
    WorldProvider* worldProvider;
    bool IsRegionLoaded(const CCharString& regionName) const;
};
typedef void* (__fastcall* GetWorldObjectFn)(WorldProvider* self);

static void* g_env_region;
static void* g_env_current;
static int g_getworld_calls;

static void* __fastcall FakeGetWorld(WorldProvider* self) { g_getworld_calls++; return self; }

CIDrawEnvironment* CWorldMap::GetRegionNumberFromName(const CCharString& rn) {
    return (CIDrawEnvironment*)g_env_region;
}
CIDrawEnvironment* CWorld::DrawGetEnvironment() {
    return (CIDrawEnvironment*)g_env_current;
}

bool CGameScriptInterface::IsRegionLoaded(const CCharString& regionName) const
{
    WorldProvider* wp = this->worldProvider;
    CIDrawEnvironment* regionEnv =
        ((CWorldMap*)((GetWorldObjectFn)wp->vtable[0x34 / 4])(wp))->GetRegionNumberFromName(regionName);

    WorldProvider* wp2 = this->worldProvider;
    CIDrawEnvironment* currentEnv =
        ((CWorld*)((GetWorldObjectFn)wp2->vtable[0x34 / 4])(wp2))->DrawGetEnvironment();

    return (bool)(regionEnv == currentEnv);
}

int main() {
    void* vt[16];
    for (int i = 0; i < 16; i++) vt[i] = 0;
    vt[0x34/4] = (void*)(&(FakeGetWorld));
    WorldProvider wp;
    WorldProvider* pwp = &(wp);
    pwp->vtable = vt;
    CGameScriptInterface gsi;
    gsi.worldProvider = pwp;
    CGameScriptInterface* pgsi = &(gsi);
    CCharString* dummy = 0;

    g_env_region = (void*)0x1000; g_env_current = (void*)0x1000; g_getworld_calls = 0;
    bool loaded = pgsi->IsRegionLoaded(*dummy);
    bool ok = loaded && (g_getworld_calls == 2);

    g_env_region = (void*)0x1000; g_env_current = (void*)0x2000;
    bool notLoaded = pgsi->IsRegionLoaded(*dummy);
    ok = ok && !notLoaded;

    if (ok) printf("REGION_LOADED_OK\n");
    else printf("FAIL loaded=%d notLoaded=%d calls=%d\n", (int)loaded, (int)notLoaded, g_getworld_calls);
    return 0;
}