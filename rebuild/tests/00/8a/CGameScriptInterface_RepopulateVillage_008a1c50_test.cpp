#include <stdio.h>

struct ScriptThing;
class CThing;
class CWorldMap;

enum ETCInterfaceType
{
};

struct Pair
{
    ETCInterfaceType m_Key;
    void* m_Value;
};

struct InterfaceMap
{
    void* m_pBegin;
    Pair* m_pEnd;

    Pair* LowerBound(const ETCInterfaceType* key);
};

struct ScriptVTable
{
    unsigned char m_Pad00[0x2C];
    CThing* (__fastcall* m_GetThing)(const ScriptThing*);
    unsigned char m_Pad30[0xFC];
    char (__fastcall* m_IsValidThing)(const ScriptThing*);
};

struct ScriptThing
{
    const ScriptVTable* m_pVTable;
};

struct VillageThing
{
    unsigned char m_Pad00[0x24];
    unsigned char m_Flags24;
    unsigned char m_Pad25[0x1F];
    InterfaceMap m_Map44;
};

struct WorldVTable
{
    unsigned char m_Pad00[0x30];
    CWorldMap* (__fastcall* m_GetWorldMap)(void*, CThing*);
};

struct World
{
    const WorldVTable* m_pVTable;
};

struct InterfaceSelf
{
    void* m_Pad00;
    World* m_pWorld;
};

class CWorldMap
{
public:
    void PopulateRegionWithVillagers();
};

class CGameScriptInterface
{
public:
    void RepopulateVillage(const ScriptThing& thing) const;
};

static Pair* g_lowerBoundResult;
static void* g_thingObject;
static ETCInterfaceType g_lowerBoundKey;
static int g_validCalls;
static int g_getThingCalls;
static int g_worldMapCalls;
static int g_populateCalls;

static char __fastcall IsValid(const ScriptThing*)
{
    ++g_validCalls;
    return 1;
}

static CThing* __fastcall GetThing(const ScriptThing*)
{
    ++g_getThingCalls;
    return (CThing*)g_thingObject;
}

static CWorldMap* __fastcall GetWorldMap(void*, CThing*)
{
    ++g_worldMapCalls;
    static CWorldMap map;
    return &map;
}

Pair* InterfaceMap::LowerBound(const ETCInterfaceType* key)
{
    g_lowerBoundKey = *key;
    return g_lowerBoundResult;
}

void CWorldMap::PopulateRegionWithVillagers()
{
    ++g_populateCalls;
}

void CGameScriptInterface::RepopulateVillage(const ScriptThing& thing) const
{
    const ScriptThing* scriptThing = &thing;
    if (scriptThing->m_pVTable->m_IsValidThing(scriptThing) != 0)
    {
        CThing* thingObject = scriptThing->m_pVTable->m_GetThing(scriptThing);
        VillageThing* village = (VillageThing*)thingObject;
        if ((village->m_Flags24 & 0x04) != 0)
        {
            ETCInterfaceType type = (ETCInterfaceType)0x22;
            Pair* record = ((InterfaceMap*)&village->m_Map44)->LowerBound(&type);
            if (record == village->m_Map44.m_pEnd || type < record->m_Key)
                record = village->m_Map44.m_pEnd;
            if (record->m_Value != 0)
            {
                InterfaceSelf* self = (InterfaceSelf*)this;
                CWorldMap* worldMap = self->m_pWorld->m_pVTable->m_GetWorldMap(
                    self->m_pWorld, thingObject);
                worldMap->PopulateRegionWithVillagers();
            }
        }
    }
}

static void Reset()
{
    g_validCalls = 0;
    g_getThingCalls = 0;
    g_worldMapCalls = 0;
    g_populateCalls = 0;
    g_lowerBoundKey = (ETCInterfaceType)0;
}

int main()
{
    ScriptVTable scriptVTable;
    ScriptThing scriptThing;
    VillageThing village;
    Pair record;
    Pair end;
    WorldVTable worldVTable;
    World world;
    InterfaceSelf self;
    scriptVTable.m_GetThing = GetThing;
    scriptVTable.m_IsValidThing = IsValid;
    scriptThing.m_pVTable = &scriptVTable;
    worldVTable.m_GetWorldMap = GetWorldMap;
    world.m_pVTable = &worldVTable;
    self.m_Pad00 = 0;
    self.m_pWorld = &world;
    record.m_Key = (ETCInterfaceType)0x22;
    record.m_Value = &village;
    end.m_Key = (ETCInterfaceType)0;
    end.m_Value = 0;
    village.m_Flags24 = 0x04;
    village.m_Map44.m_pEnd = &end;
    g_thingObject = &village;

    Reset();
    g_lowerBoundResult = &record;
    ((CGameScriptInterface*)&self)->RepopulateVillage(scriptThing);
    if (g_validCalls != 1 || g_getThingCalls != 1 ||
        g_worldMapCalls != 1 || g_populateCalls != 1 ||
        g_lowerBoundKey != (ETCInterfaceType)0x22)
        return 1;

    Reset();
    village.m_Flags24 = 0;
    ((CGameScriptInterface*)&self)->RepopulateVillage(scriptThing);
    if (g_worldMapCalls != 0 || g_populateCalls != 0)
        return 2;

    Reset();
    village.m_Flags24 = 0x04;
    g_lowerBoundResult = &end;
    ((CGameScriptInterface*)&self)->RepopulateVillage(scriptThing);
    if (g_worldMapCalls != 0 || g_populateCalls != 0)
        return 3;

    printf("FSE2_008a1c50_TEST PASS\n");
    return 0;
}
