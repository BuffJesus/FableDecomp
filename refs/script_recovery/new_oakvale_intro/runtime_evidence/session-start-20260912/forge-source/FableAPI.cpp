#include "FableAPI.h"
#include <Shlwapi.h>
#include <d3d9.h>
#include <sstream>
#include <memory>
#include <cfloat>
#include "EntityScriptingAPI.h"
#pragma comment(lib, "shlwapi.lib")

// =====================================================================================
// ForgeFSE addition: LOCAL DETAIL collection-load telemetry
// =====================================================================================
typedef bool (__thiscall *tLocalDetailCollectionLoad)(void*, void*, const void*);
static tLocalDetailCollectionLoad LocalDetailCollectionLoad_Original = nullptr;
static bool g_localDetailCollectionProbeInstalled = false;
static unsigned long g_localDetailCollectionProbeCalls = 0;

static DWORD ProbeReadDword(const void* base, size_t offset, DWORD fallback) {
    __try { return *reinterpret_cast<const DWORD*>(
        reinterpret_cast<const BYTE*>(base) + offset); }
    __except (EXCEPTION_EXECUTE_HANDLER) { return fallback; }
}

static BYTE ProbeReadByte(const void* base, size_t offset, BYTE fallback) {
    __try { return *(reinterpret_cast<const BYTE*>(base) + offset); }
    __except (EXCEPTION_EXECUTE_HANDLER) { return fallback; }
}

static bool ProbeWriteByte(void* base, size_t offset, BYTE value) {
    __try {
        *(reinterpret_cast<BYTE*>(base) + offset) = value;
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}

static bool __fastcall LocalDetailCollectionLoad_Detour(
    void* self, void*, void* stream, const void* palette) {
    const DWORD before = stream ? ProbeReadDword(stream, 4, 0xffffffffu) : 0xffffffffu;
    const bool result = LocalDetailCollectionLoad_Original(self, stream, palette);
    // Forge opens hundreds of retail collections before the custom map is
    // streamed. Keep enough headroom to capture ForgeTest's on-demand block.
    if (g_localDetailCollectionProbeCalls++ < 5000) {
        DWORD enabled = 0, type = 0, first = 0, after = 0xffffffffu;
        if (self) {
            enabled = ProbeReadByte(self, 0x14, 0xff);
            type = ProbeReadDword(self, 4, 0xffffffffu);
            first = ProbeReadDword(self, 8, 0xffffffffu);
        }
        if (stream) after = ProbeReadDword(stream, 4, 0xffffffffu);
        std::stringstream ss;
        ss << "[LocalDetailCollectionProbe] result=" << (result ? 1 : 0)
           << " enabled=" << enabled << " type=0x" << std::hex << type
           << " first=0x" << first << std::dec
           << " stream=" << before << "->" << after;
        LogToFile(ss.str());
    }
    return result;
}

static __declspec(naked) void LocalDetailCollectionLoad_NakedStub() {
    __asm { jmp LocalDetailCollectionLoad_Detour }
}

bool InstallLocalDetailCollectionProbeHook() {
    if (g_localDetailCollectionProbeInstalled) return true;
    if (!g_fableBase) return false;
    const DWORD target = ASLR<DWORD>(0x00BDE680);
    const size_t prologueSize = 6; // 53 56 8B 74 24 0C
    const BYTE expected[6] = {0x53,0x56,0x8b,0x74,0x24,0x0c};
    if (memcmp(reinterpret_cast<void*>(target), expected, 6) != 0) {
        LogToFile("!!! [LocalDetailCollectionProbe] unexpected retail prologue.");
        return false;
    }
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) return false;
    memcpy(tramp, reinterpret_cast<void*>(target), prologueSize);
    tramp[prologueSize] = 0xE9;
    const DWORD backRel = target + DWORD(prologueSize) -
                          reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    LocalDetailCollectionLoad_Original =
        reinterpret_cast<tLocalDetailCollectionLoad>(tramp);
    BYTE patch[6] = {0xE9,0,0,0,0,0x90};
    const DWORD fwdRel = reinterpret_cast<DWORD>(&LocalDetailCollectionLoad_NakedStub) -
                         target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) return false;
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);
    g_localDetailCollectionProbeInstalled = true;
    LogToFile("[LocalDetailCollectionProbe] Hook installed at CObjectTypeCollection::Load.");
    return true;
}

// =====================================================================================
// ForgeFSE addition: repeated-static-mesh render telemetry
// =====================================================================================
typedef DWORD (__thiscall *tRepeatedMeshRenderPrimitive)(void*, void*, DWORD, void*);
static tRepeatedMeshRenderPrimitive RepeatedMeshRenderPrimitive_Original = nullptr;
static bool g_repeatedMeshRenderProbeInstalled = false;
static unsigned long g_repeatedMeshRenderProbeCalls = 0;
static volatile void* g_repeatedMeshProbeActivePrimitive = nullptr;
static volatile unsigned long g_repeatedMeshProbeSubPrimitiveCalls = 0;
static bool g_repeatedMeshProbeForcedManagerEnabled = false;

static float ProbeReadFloat(const void* base, size_t offset, float fallback) {
    __try { return *reinterpret_cast<const float*>(
        reinterpret_cast<const BYTE*>(base) + offset); }
    __except (EXCEPTION_EXECUTE_HANDLER) { return fallback; }
}

static DWORD __fastcall RepeatedMeshRenderPrimitive_Detour(
    void* self, void*, void* primitive, DWORD renderLayer, void* subPrimitiveChain) {
    const float x = ProbeReadFloat(primitive, 0x70, -FLT_MAX);
    const float y = ProbeReadFloat(primitive, 0x74, -FLT_MAX);
    const bool forgeTest = x >= 3320.0f && x <= 3400.0f &&
                           y >= 2296.0f && y <= 2376.0f;
    if (forgeTest && g_repeatedMeshRenderProbeCalls++ < 2000) {
        std::stringstream ss;
        ss << "[RepeatedMeshRenderProbe] ENTER primitive=0x" << std::hex
           << reinterpret_cast<DWORD>(primitive)
           << " meshInfo=0x" << ProbeReadDword(primitive, 0x50, 0)
           << " objects=" << std::dec << ProbeReadDword(primitive, 0x54, 0)
           << " sphere=(" << x << "," << y << ","
           << ProbeReadFloat(primitive, 0x78, 0.0f) << ","
           << ProbeReadFloat(primitive, 0x7c, 0.0f) << ")"
           << " normals=0x" << std::hex << ProbeReadDword(primitive, 0x8c, 0)
           << " matrices=0x" << ProbeReadDword(primitive, 0x90, 0)
           << " offsets=0x" << ProbeReadDword(primitive, 0x94, 0)
           << " subsections=0x" << ProbeReadDword(primitive, 0x98, 0)
           << " renderInfo=0x" << ProbeReadDword(primitive, 0xa4, 0)
           << std::dec << " layer=" << renderLayer;
        LogToFile(ss.str());
    }
    const unsigned long subCallsBefore = g_repeatedMeshProbeSubPrimitiveCalls;
    void* previousActive = const_cast<void*>(g_repeatedMeshProbeActivePrimitive);
    if (forgeTest) {
        g_repeatedMeshProbeActivePrimitive = primitive;
        if (!ProbeReadByte(self, 8, 0xff)) {
            if (ProbeWriteByte(self, 8, 1)) {
                if (!g_repeatedMeshProbeForcedManagerEnabled) {
                    LogToFile("[RepeatedMeshRenderProbe] DIAG forced manager+0x08 0->1.");
                    g_repeatedMeshProbeForcedManagerEnabled = true;
                }
            } else {
                LogToFile("!!! [RepeatedMeshRenderProbe] failed to write manager+0x08.");
            }
        }
    }
    const DWORD result = RepeatedMeshRenderPrimitive_Original(
        self, primitive, renderLayer, subPrimitiveChain);
    if (forgeTest) g_repeatedMeshProbeActivePrimitive = previousActive;
    if (forgeTest && g_repeatedMeshRenderProbeCalls <= 2000) {
        std::stringstream ss;
        ss << "[RepeatedMeshRenderProbe] EXIT result=" << result
           << " subPrimitiveCalls="
           << (g_repeatedMeshProbeSubPrimitiveCalls - subCallsBefore)
           << " primitive=0x" << std::hex << reinterpret_cast<DWORD>(primitive);
        LogToFile(ss.str());
    }
    return result;
}

typedef DWORD (__thiscall *tRepeatedMeshRenderSubPrimitive)(void*, void*, void*);
static tRepeatedMeshRenderSubPrimitive RepeatedMeshRenderSubPrimitive_Original = nullptr;

static DWORD __fastcall RepeatedMeshRenderSubPrimitive_Detour(
    void* self, void*, void* subPrimitive, void* renderContext) {
    if (g_repeatedMeshProbeActivePrimitive) {
        ++g_repeatedMeshProbeSubPrimitiveCalls;
    }
    return RepeatedMeshRenderSubPrimitive_Original(self, subPrimitive, renderContext);
}

static __declspec(naked) void RepeatedMeshRenderSubPrimitive_NakedStub() {
    __asm { jmp RepeatedMeshRenderSubPrimitive_Detour }
}

static bool InstallRepeatedMeshRenderSubPrimitiveProbeHook() {
    const DWORD target = ASLR<DWORD>(0x00B9AC30);
    const size_t prologueSize = 6; // 81 EC C4 00 00 00: sub esp, 0xc4
    const BYTE expected[6] = {0x81,0xec,0xc4,0x00,0x00,0x00};
    if (memcmp(reinterpret_cast<void*>(target), expected, 6) != 0) {
        LogToFile("!!! [RepeatedMeshRenderProbe] unexpected RenderSubPrimitive prologue.");
        return false;
    }
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) return false;
    memcpy(tramp, reinterpret_cast<void*>(target), prologueSize);
    tramp[prologueSize] = 0xE9;
    const DWORD backRel = target + DWORD(prologueSize) -
                          reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    RepeatedMeshRenderSubPrimitive_Original =
        reinterpret_cast<tRepeatedMeshRenderSubPrimitive>(tramp);
    BYTE patch[6] = {0xE9,0,0,0,0,0x90};
    const DWORD fwdRel = reinterpret_cast<DWORD>(&RepeatedMeshRenderSubPrimitive_NakedStub) -
                         target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) return false;
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);
    LogToFile("[RepeatedMeshRenderProbe] RenderSubPrimitive hook installed.");
    return true;
}

static __declspec(naked) void RepeatedMeshRenderPrimitive_NakedStub() {
    __asm { jmp RepeatedMeshRenderPrimitive_Detour }
}

bool InstallRepeatedMeshRenderProbeHook() {
    if (g_repeatedMeshRenderProbeInstalled) return true;
    if (!g_fableBase) return false;
    const DWORD target = ASLR<DWORD>(0x00B9A4A0);
    const size_t prologueSize = 6; // 81 EC 80 02 00 00: sub esp, 0x280
    const BYTE expected[6] = {0x81,0xec,0x80,0x02,0x00,0x00};
    if (memcmp(reinterpret_cast<void*>(target), expected, 6) != 0) {
        LogToFile("!!! [RepeatedMeshRenderProbe] unexpected retail prologue.");
        return false;
    }
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) return false;
    memcpy(tramp, reinterpret_cast<void*>(target), prologueSize);
    tramp[prologueSize] = 0xE9;
    const DWORD backRel = target + DWORD(prologueSize) -
                          reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    RepeatedMeshRenderPrimitive_Original =
        reinterpret_cast<tRepeatedMeshRenderPrimitive>(tramp);
    BYTE patch[6] = {0xE9,0,0,0,0,0x90};
    const DWORD fwdRel = reinterpret_cast<DWORD>(&RepeatedMeshRenderPrimitive_NakedStub) -
                         target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) return false;
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);
    g_repeatedMeshRenderProbeInstalled = true;
    if (!InstallRepeatedMeshRenderSubPrimitiveProbeHook()) return false;
    LogToFile("[RepeatedMeshRenderProbe] Hook installed at RenderPrimitive.");
    return true;
}

std::string g_fseBasePath = "";

void InitializeFSEPaths(HMODULE hMod) {
    char dllPath[MAX_PATH] = { 0 };
    GetModuleFileNameA(hMod, dllPath, MAX_PATH);
    PathRemoveFileSpecA(dllPath);

    g_fseBasePath = std::string(dllPath) + "/FSE";
}

void LogCScriptThingDetails(const std::string& context, CScriptThing* pThing) {
    std::stringstream ss;
    ss << context << ": CScriptThing* = 0x" << std::hex << reinterpret_cast<uintptr_t>(pThing);
    if (!pThing) {
        ss << " (NULL)";
        LogToFile(ss.str());
        return;
    }

    ss << ", pVTable = 0x" << std::hex << reinterpret_cast<uintptr_t>(pThing->pVTable);
    if (pThing->pVTable == g_pCScriptThingVTable) {
        ss << " (Matches Known VTable)";
        CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
        if (pVTable->GetPThingUniqueID) {
            ss << ", WorldUID = " << std::dec << pVTable->GetPThingUniqueID(pThing);
        }
        if (pVTable->GetPos) {
            const C3DVector* pPos = pVTable->GetPos(pThing);
            if (pPos) {
                ss << ", WrapperPos = (" << pPos->x << "," << pPos->y << "," << pPos->z << ")";
            }
            else {
                ss << ", WrapperPos = NULL";
            }
        }
    }
    else {
        ss << " (!!! MISMATCH !!! Expected: 0x" << std::hex << reinterpret_cast<uintptr_t>(g_pCScriptThingVTable) << ")";
    }

    ss << ", pImp.Data = 0x" << std::hex << reinterpret_cast<uintptr_t>(pThing->pImp.Data);
    ss << ", pImp.Info = 0x" << std::hex << reinterpret_cast<uintptr_t>(pThing->pImp.Info);

    if (pThing->pImp.Info) {
        ss << ", Info->RefCount = " << std::dec << pThing->pImp.Info->RefCount;
        ss << ", Info->DeleteFunc = 0x" << std::hex << reinterpret_cast<uintptr_t>(pThing->pImp.Info->DeleteFunc);
        ss << ", Info->Data = 0x" << std::hex << reinterpret_cast<uintptr_t>(pThing->pImp.Info->Data);
        if (pThing->pImp.Info->Data != pThing->pImp.Data) {
            ss << " (!!! Info->Data != pImp.Data !!!)";
        }
    }
    else {
        ss << ", Info = NULL";
    }
    LogToFile(ss.str());
}

std::string GetLogFilePath() {
    if (g_fseBasePath.empty()) return "C:/Temp/FableQuestHook_fallback.log";
    return g_fseBasePath + "/FableScriptExtender.log";
}

std::string GetQuestsConfigPath() {
    return g_fseBasePath + "/quests.lua";
}

std::string GetScriptPath(const std::string& scriptFile) {
    return g_fseBasePath + "/" + scriptFile + ".lua";
}

std::shared_ptr<CScriptThing> WrapScriptThingOutput(CScriptThing* pThingBuffer, bool retainShallowCopy) {
    if (!pThingBuffer) return nullptr;

    // We must validate the result. IsNull check is safest.
    if (pThingBuffer->pVTable) {
        // --- THIS IS THE CORRECTED LINE ---
        auto* pVTable = reinterpret_cast<CScriptThingVTable*>(pThingBuffer->pVTable);
        // --- END CORRECTION ---

        if (pVTable->IsNull && pVTable->IsNull(pThingBuffer)) {
            Game_free(pThingBuffer); // Free the buffer, it's a null object
            return nullptr;
        }
    }
    else {
        // No VTable, invalid.
        Game_free(pThingBuffer);
        return nullptr;
    }

    // A retail API that fills an output CScriptThing has already acquired this
    // reference through CBaseIntelligentPointer assignment. Only a raw shallow
    // copy from a borrowed container element needs an additional retain.
    if (retainShallowCopy && pThingBuffer->pImp.Info) {
        pThingBuffer->pImp.Info->RefCount++;
    }
    else if (!pThingBuffer->pImp.Info) {
        LogToFile("!!! WARNING: WrapScriptThingOutput - Thing has no pImp.Info! Cannot increment ref count.");
    }

    // Return a shared_ptr that manages the heap-allocated buffer
    // and decrements the game's ref count on deletion.
    return std::shared_ptr<CScriptThing>(pThingBuffer, [](CScriptThing* p) {
        if (p && p->pImp.Info && p->pImp.Info->RefCount > 0) {
            p->pImp.Info->RefCount--;
        }
        if (p && Game_free) {
            Game_free(p);
        }
        });
}

DWORD g_fableBase = 0;

void* pEmptyDataAlloc = nullptr;
void** g_pEntityScriptBindingVTable = nullptr;
void** g_pCScriptThingVTable = nullptr;
void** g_pCScriptGameResourceObjectScriptedThingBaseVTable = nullptr;
void** g_pMovieObjectVTable = nullptr;
t_malloc                                Game_malloc = nullptr;
t_free                                  Game_free = nullptr;
tCCharString_Constructor_Literal        CCharString_Construct_Literal = nullptr;
tCCharString_Constructor_Copy           CCharString_Construct_Copy = nullptr;
tCCharString_Destructor                 CCharString_Destroy = nullptr;
tCheckSection                           CheckSection = nullptr;
tAddScript                              AddScript = nullptr;
tSetScriptActiveStatus                  SetScriptActiveStatus_Func = nullptr;
tCScriptBase_Constructor                CScriptBase_Construct = nullptr;
tCSpawnedFunc_Constructor               CSpawnedFunc_Construct = nullptr;
tAddSpawnedFunction                     AddSpawnedFunction_func = nullptr;
tScriptAllocFunc                        pSunnyvaleDataAlloc_func = nullptr;
CGameScriptInterfaceBase**              g_pDSTGame = nullptr;
tRunCutsceneMacro                       RunCutsceneMacro_Func = nullptr;
tFadeOutAndKillEntity                   FadeOutAndKillEntity_Func = nullptr;
tAddLogbookStoryEntry                   AddLogbookStoryEntry_Func = nullptr;
tAddEntityScriptBinding                 AddEntityScriptBinding_API = nullptr;
tPostAddScriptedEntities_CScriptBase    PostAddScriptedEntities_CScriptBase_API = nullptr;
tIsActiveThreadTerminating_Entity       IsActiveThreadTerminating_Entity_API = nullptr;
tIsActiveThreadTerminating_Quest        IsActiveThreadTerminating_Quest_API = nullptr;
tStdMap_Constructor                     StdMap_Construct_API = nullptr;
tStdMap_OperatorBracket                 StdMap_OperatorBracket_API = nullptr;
tStdMap_Destructor                      StdMap_Destroy_API = nullptr;
tStdMap_Bool_OperatorBracket            StdMap_Bool_OperatorBracket_API = nullptr;
tStdMap_Bool_Destructor                 StdMap_Bool_Destroy_API = nullptr;
tStdMap_String_Construct                StdMap_String_Construct_API = nullptr;
tStdMap_String_OperatorBracket          StdMap_String_OperatorBracket_API = nullptr;
tStdMap_String_Destructor               StdMap_String_Destroy_API = nullptr;
tCCharString_AssignmentLiteral          CCharString_AssignLiteral_API = nullptr;
tAddLogbookStoryEntryString             AddLogbookStoryEntryString_Func = nullptr;
tAddLogbookTutorialEntry                AddLogbookTutorialEntry_Func = nullptr;
tCBaseObject_Constructor                CBaseObject_Construct_API = nullptr;
tCBaseObject_AssignmentOperator         CBaseObject_Assign_API = nullptr;
tCBaseObject_Destructor                 CBaseObject_Destroy_API = nullptr;
tCleanupPImp                            CleanupMoviePImp_API = nullptr;
tMovieResource_Destructor               MovieResource_Destroy_API = nullptr;
tInitScriptObjectHelper1                InitScriptObjectHelper1_API = nullptr;
tInitScriptObjectHelper2                InitScriptObjectHelper2_API = nullptr;
tCCharString_AssignmentOperator         CCharString_Assign_API = nullptr;
tCCharString_ToConstChar                CCharString_ToConstChar_API = nullptr;
tCCharString_OperatorPlus               CCharString_OperatorPlus_API = nullptr;
tGFCharStringToInt                      GFCharStringToInt_API = nullptr;
tGFIntToCharString                      GFIntToCharString_API = nullptr;
tIsDistanceBetweenThingsUnder           IsDistanceBetweenThingsUnder_API = nullptr;
tIsDistanceBetweenThingsOver            IsDistanceBetweenThingsOver_API = nullptr;
tCPersistContext_Transfer_bool          CPersistContext_Transfer_bool_API = nullptr;
tCPersistContext_Transfer_int           CPersistContext_Transfer_int_API = nullptr;
tCPersistContext_Transfer_string        CPersistContext_Transfer_string_API = nullptr;
tCPersistContext_Transfer_float         CPersistContext_Transfer_float_API = nullptr;
tCPersistContext_Transfer_uint          CPersistContext_Transfer_uint_API = nullptr;
tCSGROSTB_Destructor                    CSGROSTB_Destroy_API = nullptr;
tGetLanguage                            GetLanguage_API = nullptr; // ForgeFSE addition
tCWorldMap_SetPlayerPos                 CWorldMap_SetPlayerPos_API = nullptr; // ForgeFSE addition
tCWorldMap_GetMapNumberFromScriptName   CWorldMap_GetMapNumberFromScriptName_API = nullptr; // ForgeFSE addition
tCWorldMap_ActivateNavMap               CWorldMap_ActivateNavMap_API = nullptr; // ForgeFSE addition
tCWorldMap_LoadRegion                   CWorldMap_LoadRegion_API = nullptr; // ForgeFSE addition
tCWorldMap_GetRegionNumberMapIsIn       CWorldMap_GetRegionNumberMapIsIn_API = nullptr; // ForgeFSE addition
tCWorldMap_GetGroundSizeZAt             CWorldMap_GetGroundSizeZAt_API = nullptr; // ForgeFSE addition
tCWorld_SetAsLoadingRegion              CWorld_SetAsLoadingRegion_API = nullptr; // ForgeFSE addition
tCWorld_HandleMoveHeroToRegionGameEvent CWorld_HandleMoveHeroToRegionGameEvent_API = nullptr; // ForgeFSE addition
tGetSoundIndexFromSymbol                 GetSoundIndexFromSymbol_API = nullptr; // ForgeFSE addition (play-sound-by-id isolation)
void**                                   g_pMainGameComponent = nullptr;        // ForgeFSE addition (DAT_013b86a0)
tSoundPlayer_PlayIndexRange              SoundPlayer_PlayIndexRange_API = nullptr; // ForgeFSE addition
tInterfaceMap_LowerBound                 InterfaceMap_LowerBound_API = nullptr; // ForgeFSE addition

// A Lua-requested transition is consumed at the next CMainGameComponent
// update boundary. Quest callbacks and the update loop are both main-thread
// code, so this intentionally remains a small single-threaded handoff.
static bool g_retailTransitionPending = false;
static void* g_retailTransitionWorld = nullptr;
static float g_retailTransitionPos[3] = {};
static int g_retailTransitionMapSlot = 0;

void InitializeFableAPI() {
    LogToFile("--- Initializing Fable API Pointers ---");
    g_fableBase = (DWORD)GetModuleHandleA(NULL);
    if (!g_fableBase) {
        LogToFile("!!! CRITICAL: GetModuleHandleA failed! Can't initialize API. !!!");
        return;
    }

    std::stringstream ss;
    ss << "Module Base Address (g_fableBase): 0x" << std::hex << g_fableBase;
    LogToFile(ss.str());

    Game_malloc = ASLR<t_malloc>(0xBFEA0E);
    Game_free = ASLR<t_free>(0xBFEA14);
    CCharString_Construct_Literal = ASLR<tCCharString_Constructor_Literal>(0x99EBF0);
    CCharString_Construct_Copy = ASLR<tCCharString_Constructor_Copy>(0x99EC30);
    CCharString_Destroy = ASLR<tCCharString_Destructor>(0x99EAE0);
    CheckSection = ASLR<tCheckSection>(0xCB5AC0);
    AddScript = ASLR<tAddScript>(0xCB5C90);
    SetScriptActiveStatus_Func = ASLR<tSetScriptActiveStatus>(0xCBFAB8);
    CScriptBase_Construct = ASLR<tCScriptBase_Constructor>(0xCB8110);
    CSpawnedFunc_Construct = ASLR<tCSpawnedFunc_Constructor>(0xCDD450);
    AddSpawnedFunction_func = ASLR<tAddSpawnedFunction>(0xCB7E50);
    pEmptyDataAlloc = ASLR<void*>(0xCD4AC0);
    pSunnyvaleDataAlloc_func = ASLR<tScriptAllocFunc>(0xCDBD20);
    g_pDSTGame = ASLR<CGameScriptInterfaceBase**>(0x143E8F8);
    AddEntityScriptBinding_API = ASLR<tAddEntityScriptBinding>(0xCB8230);
    PostAddScriptedEntities_CScriptBase_API = ASLR<tPostAddScriptedEntities_CScriptBase>(0xCB8930);
    g_pEntityScriptBindingVTable = ASLR<void**>(0x12EA57C);
    IsActiveThreadTerminating_Entity_API = ASLR<tIsActiveThreadTerminating_Entity>(0xF35B30);
    IsActiveThreadTerminating_Quest_API = ASLR<tIsActiveThreadTerminating_Quest>(0xCB7940);
    RunCutsceneMacro_Func = ASLR<tRunCutsceneMacro>(0xCBFB7D);
    FadeOutAndKillEntity_Func = ASLR<tFadeOutAndKillEntity>(0xCBE716);
    AddLogbookStoryEntry_Func = ASLR<tAddLogbookStoryEntry>(0xCBE87F);
    StdMap_Construct_API = ASLR<tStdMap_Constructor>(0xCDBF70);
    StdMap_Destroy_API = ASLR<tStdMap_Destructor>(0xCDBFB0);
    // Ported from upstream FSE 2026-09-02.
    StdMap_Bool_OperatorBracket_API = ASLR<tStdMap_Bool_OperatorBracket>(0x8ADF10);
    StdMap_Bool_Destroy_API = ASLR<tStdMap_Bool_Destructor>(0x7ADD50);
    // Instruction-backed specializations used by retail cutscene input maps:
    // CNOVI_Bully::Main 0x00DBCA94/0x00DBCAF9/0x00DBCC98 and literal
    // CCharString assignment at 0x00DBCB00.
    StdMap_String_Construct_API = ASLR<tStdMap_String_Construct>(0x9AC2D0);
    StdMap_String_OperatorBracket_API = ASLR<tStdMap_String_OperatorBracket>(0x9AC700);
    StdMap_String_Destroy_API = ASLR<tStdMap_String_Destructor>(0x9AC310);
    CCharString_AssignLiteral_API = ASLR<tCCharString_AssignmentLiteral>(0x99EFE0);
    AddLogbookStoryEntryString_Func = ASLR<tAddLogbookStoryEntryString>(0xCBE960);
    // Retail helper used by New Oakvale's first-deed path: appends _TITLE,
    // submits category 2, then yields through the game-script interface.
    AddLogbookTutorialEntry_Func = ASLR<tAddLogbookTutorialEntry>(0xCBE9EE);
    StdMap_OperatorBracket_API = ASLR<tStdMap_OperatorBracket>(0xCD3D2E);
    CBaseObject_Construct_API = ASLR<tCBaseObject_Constructor>(0x99A380);
    // Verified against retail EmptyGrave::Main disassembly: this is the specialized
    // CScriptGameResourceObjectScriptedThingBase assignment used by cutscene actor maps.
    CBaseObject_Assign_API = ASLR<tCBaseObject_AssignmentOperator>(0x8ABD10);
    CBaseObject_Destroy_API = ASLR<tCBaseObject_Destructor>(0x99A430);
    g_pMovieObjectVTable = ASLR<void**>(0x1260EF4);
    CleanupMoviePImp_API = ASLR<tCleanupPImp>(0x6E7AB0);
    // Retail CScriptGameResourceObjectMovieBase non-deleting destructor. It
    // releases pImp.Info, clears pImp, restores the base vtable, and chains to
    // CBaseObject::~CBaseObject (verified at 0x006E7B80).
    MovieResource_Destroy_API = ASLR<tMovieResource_Destructor>(0x6E7B80);
    InitScriptObjectHelper1_API = ASLR<tInitScriptObjectHelper1>(0xCD23B9);
    InitScriptObjectHelper2_API = ASLR<tInitScriptObjectHelper2>(0xCD2770);
    g_pCScriptGameResourceObjectScriptedThingBaseVTable = ASLR<void**>(0x127094C);
    g_pCScriptThingVTable = ASLR<void**>(0x1238C8C);
    CCharString_Assign_API = ASLR<tCCharString_AssignmentOperator>(0x99EFB0);
    CCharString_ToConstChar_API = ASLR<tCCharString_ToConstChar>(0x99E4C0);
    GFCharStringToInt_API = ASLR<tGFCharStringToInt>(0x99E7F0);
    GFIntToCharString_API = ASLR<tGFIntToCharString>(0x99F830);
    CCharString_OperatorPlus_API = ASLR<tCCharString_OperatorPlus>(0x99F690);
    IsDistanceBetweenThingsUnder_API = ASLR<tIsDistanceBetweenThingsUnder>(0xCBE2FF);
    // CNOVI_TeddyGirl::Main calls this exact helper at 0x00DAFC34.
    IsDistanceBetweenThingsOver_API = ASLR<tIsDistanceBetweenThingsOver>(0xCBE3EA);
    CPersistContext_Transfer_bool_API = ASLR<tCPersistContext_Transfer_bool>(0x4045C0);   
    CPersistContext_Transfer_int_API = ASLR<tCPersistContext_Transfer_int>(0x410BE0);   
    CPersistContext_Transfer_string_API = ASLR<tCPersistContext_Transfer_string>(0x4109A0);
    CPersistContext_Transfer_float_API = ASLR<tCPersistContext_Transfer_float>(0x410620);   
    CPersistContext_Transfer_uint_API = ASLR<tCPersistContext_Transfer_uint>(0x4106F0); 
    CSGROSTB_Destroy_API = ASLR<tCSGROSTB_Destructor>(0x7E74D0);

    // --- ForgeFSE additions ---
    GetLanguage_API = ASLR<tGetLanguage>(0x415070); // NLocalisation::GetLanguage
    CWorldMap_SetPlayerPos_API = ASLR<tCWorldMap_SetPlayerPos>(0x5063E0);              // CWorldMap::SetPlayerPos (map stream + hero move)
    CWorldMap_GetMapNumberFromScriptName_API = ASLR<tCWorldMap_GetMapNumberFromScriptName>(0x4FB880); // CWorldMap::GetMapNumberFromScriptName
    CWorldMap_ActivateNavMap_API = ASLR<tCWorldMap_ActivateNavMap>(0x50AF10);          // CWorldMap::ActivateNavMap (activate hot-streamed navmesh -> unfreezes hero)
    CWorldMap_LoadRegion_API = ASLR<tCWorldMap_LoadRegion>(0x500540);                   // CWorldMap::LoadRegion (SAFE synchronous force-load, no adjacency check)
    CWorldMap_GetRegionNumberMapIsIn_API = ASLR<tCWorldMap_GetRegionNumberMapIsIn>(0x4FC190); // CWorldMap::GetRegionNumberMapIsIn (map slot -> 1-based owning region index)
    CWorldMap_GetGroundSizeZAt_API = ASLR<tCWorldMap_GetGroundSizeZAt>(0x63A300);       // CWorldMap::GetGroundSizeZAt (ground/collision surface height at world X,Y)
    CWorld_SetAsLoadingRegion_API = ASLR<tCWorld_SetAsLoadingRegion>(0x49E2C0);         // CWorld::SetAsLoadingRegion (REAL cross-region transition: force-load + go-live + nav + hero move)
    CWorld_HandleMoveHeroToRegionGameEvent_API = ASLR<tCWorld_HandleMoveHeroToRegionGameEvent>(0x49EAF0); // CWorld::HandleMoveHeroToRegionGameEvent (retail caller of SetAsLoadingRegion)

    // --- ForgeFSE addition: play-sound-by-numeric-id isolation binding ---
    GetSoundIndexFromSymbol_API = ASLR<tGetSoundIndexFromSymbol>(0xA01920); // CASoundBank::GetSoundIndexFromSymbol
    g_pMainGameComponent = ASLR<void**>(0x13B86A0);                          // DAT_013b86a0 CMainGameComponent*
    SoundPlayer_PlayIndexRange_API = ASLR<tSoundPlayer_PlayIndexRange>(0x66B280); // FUN_0066b280 (variation play by index range)
    InterfaceMap_LowerBound_API = ASLR<tInterfaceMap_LowerBound>(0x40F020);  // CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound

    LogToFile("--- Fable API Pointers Initialized ---");
}

bool QueueRetailRegionTransition(void* pWorld, const float pos[3], int mapSlot) {
    if (!pWorld || !pos || !CWorld_SetAsLoadingRegion_API) {
        LogToFile("!!! [RetailTransition] queue rejected: world, position, or API is unavailable.");
        return false;
    }
    if (g_retailTransitionPending) {
        LogToFile("!!! [RetailTransition] queue rejected: another transition is already pending.");
        return false;
    }
    g_retailTransitionWorld = pWorld;
    g_retailTransitionPos[0] = pos[0];
    g_retailTransitionPos[1] = pos[1];
    g_retailTransitionPos[2] = pos[2];
    g_retailTransitionMapSlot = mapSlot;
    g_retailTransitionPending = true;

    std::stringstream ss;
    ss << "[RetailTransition] queued for main update: slot " << mapSlot
       << " target (" << pos[0] << "," << pos[1] << "," << pos[2] << ").";
    LogToFile(ss.str());
    return true;
}

void ProcessQueuedRetailRegionTransition() {
    if (!g_retailTransitionPending) return;

    void* pWorld = g_retailTransitionWorld;
    const float pos[3] = {
        g_retailTransitionPos[0], g_retailTransitionPos[1], g_retailTransitionPos[2]
    };
    const int mapSlot = g_retailTransitionMapSlot;
    g_retailTransitionPending = false;
    g_retailTransitionWorld = nullptr;
    g_retailTransitionMapSlot = 0;

    std::stringstream ss;
    ss << "[RetailTransition] arming from CMainGameComponent::Update: slot " << mapSlot
       << " target (" << pos[0] << "," << pos[1] << "," << pos[2] << ").";
    LogToFile(ss.str());
    CWorld_SetAsLoadingRegion_API(pWorld, pos, 0.0f, true, true, true);
    LogToFile("[RetailTransition] CWorld::SetAsLoadingRegion returned at engine update boundary.");
}

// CMainGameComponent::Update @ retail 0x00418289 has a six-byte prologue:
// push ebp; mov ebp,esp; sub esp,34h.
typedef void(__thiscall* tMainGameComponent_Update)(void* pThis);
static tMainGameComponent_Update MainGameComponent_Update_Original = nullptr;
static bool g_retailUpdateTransitionHookInstalled = false;

static void __fastcall MainGameComponent_Update_Detour(void* pThis, void*) {
    MainGameComponent_Update_Original(pThis);
    // Arm only after the complete retail update has unwound. This gives the
    // loader its own following frame for state 1, instead of changing the
    // world while the current update is still traversing its old region.
    ProcessQueuedRetailRegionTransition();
}

static __declspec(naked) void MainGameComponent_Update_NakedStub() {
    __asm { jmp MainGameComponent_Update_Detour }
}

bool InstallRetailUpdateTransitionHook() {
    if (g_retailUpdateTransitionHookInstalled) return true;
    if (!g_fableBase) {
        LogToFile("!!! [RetailTransition] update hook rejected: game base is unavailable.");
        return false;
    }

    const DWORD target = ASLR<DWORD>(0x00418289);
    constexpr size_t prologueSize = 6;
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) {
        LogToFile("!!! [RetailTransition] update trampoline allocation failed.");
        return false;
    }

    memcpy(tramp, reinterpret_cast<const void*>(target), prologueSize);
    tramp[prologueSize] = 0xE9;
    const DWORD backRel = target + static_cast<DWORD>(prologueSize) -
                          reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    MainGameComponent_Update_Original = reinterpret_cast<tMainGameComponent_Update>(tramp);

    BYTE patch[prologueSize] = { 0xE9, 0, 0, 0, 0, 0x90 };
    const DWORD fwdRel = reinterpret_cast<DWORD>(&MainGameComponent_Update_NakedStub) - target - 5;
    memcpy(patch + 1, &fwdRel, 4);

    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) {
        VirtualFree(tramp, 0, MEM_RELEASE);
        MainGameComponent_Update_Original = nullptr;
        LogToFile("!!! [RetailTransition] update hook VirtualProtect failed.");
        return false;
    }
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);

    g_retailUpdateTransitionHookInstalled = true;
    std::stringstream ss;
    ss << "[RetailTransition] CMainGameComponent::Update hook installed at 0x"
       << std::hex << target << " (trampoline 0x" << reinterpret_cast<DWORD>(tramp) << ").";
    LogToFile(ss.str());
    return true;
}

// ===== ForgeFSE: clear the quest-card viewport's scratch surface (garble fix) =====
// Found via live decompilation of Fable.exe (no guessing): the quest card renders into a REUSED
// scratch-buffer surface through CEngineSurfaceCompositionManager::RenderToTexture @ 0x00B2B180 ->
// CSurfaceCompositor::StartComposition @ 0x00F39730, which binds the surface via CRenderTarget::
// AttachTarget -- NOT CDisplayManager::SetRenderTarget (that's why every RT-bind hook missed it).
// That scratch surface is never cleared, so stale content (a leftover "candle scene") + uninitialised
// VRAM show around the small 3D card mesh = the garble. RenderToTexture has exactly ONE caller (the
// disk-cache/card path), so we flag while inside it and clear the just-bound surface right after
// StartComposition binds it -- scoped so nothing else is touched.

typedef void(__thiscall* tClearRenderTarget)(void* pDispMgr, void* pColour, unsigned long flags);
static tClearRenderTarget ClearRenderTarget_Fn = nullptr;      // 0x009BE420 (flags bit2 = clear colour)
static void** g_ppDisplayManager = nullptr;                    // *0x01436E1C = CDisplayManager*
static volatile long g_inCardRenderToTexture = 0;
static bool g_cardClearHooksInstalled = false;

#pragma pack(push, 1)
struct FseRGBColour { unsigned char c0, c1, c2, a; };          // 4 bytes, alpha at +3
#pragma pack(pop)

// Generic 5/6-byte inline hook installer; fills *pOriginal with a call-through trampoline.
static bool InstallInlineHook(DWORD target, void* detourStub, size_t prologueSize,
                              void** pOriginal, const char* tag) {
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 32, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) { LogToFile(std::string("!!! [CardClear] tramp alloc failed: ") + tag); return false; }
    memcpy(tramp, reinterpret_cast<const void*>(target), prologueSize);
    tramp[prologueSize] = 0xE9;
    const DWORD backRel = target + (DWORD)prologueSize - reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    *pOriginal = tramp;
    BYTE patch[8]; memset(patch, 0x90, prologueSize); patch[0] = 0xE9;
    const DWORD fwdRel = reinterpret_cast<DWORD>(detourStub) - target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize, PAGE_EXECUTE_READWRITE, &oldProt)) {
        VirtualFree(tramp, 0, MEM_RELEASE); *pOriginal = nullptr;
        LogToFile(std::string("!!! [CardClear] VirtualProtect failed: ") + tag); return false;
    }
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);
    return true;
}

// ===== ForgeFSE: retail cutscene-skip diagnostic boundary =====
// Static proof for these entry points and prologue lengths is recorded in
// refs/script_recovery/new_oakvale_intro/runtime_evidence/
// cutscene-skip-native-analysis-20260909.md. The hooks are installed once but remain passive
// outside Begin/EndCutsceneSkipDiagnostics, so native quests and ordinary gameplay are untouched.
static volatile LONG g_cutsceneSkipDiagActive = 0;
static bool g_cutsceneSkipDiagHooksInstalled = false;
static bool g_cutsceneSpeakDiagHookInstalled = false;
static bool g_cutsceneMovementDiagHooksInstalled = false;
static const char* g_cutsceneSkipDiagMacro = nullptr;

typedef void(__thiscall* tDiagSetCutsceneMode)(void*, bool, bool);
typedef void(__thiscall* tDiagScriptedThingSpeak)(void*, const CScriptThing*, const char*,
    ETextGroupSelectionMethod, bool, bool, bool);
typedef void(__thiscall* tDiagScriptedThingMoveToPosition)(
    void*, const C3DVector*, float, EScriptEntityMoveType, bool, bool);
typedef void(__thiscall* tDiagScriptedThingMoveToThing)(
    void*, const CScriptThing*, float, EScriptEntityMoveType, bool, bool, bool, bool);
typedef bool(__thiscall* tDiagScriptedThingIsPerformingScriptTask)(void*);
static tDiagSetCutsceneMode DiagSetCutsceneMode_Original = nullptr;
static tDiagScriptedThingSpeak DiagScriptedThingSpeak_Original = nullptr;
static tDiagScriptedThingMoveToPosition DiagScriptedThingMoveToPosition_Original = nullptr;
static tDiagScriptedThingMoveToThing DiagScriptedThingMoveToThing_Original = nullptr;
static tDiagScriptedThingIsPerformingScriptTask DiagScriptedThingIsPerformingScriptTask_Original = nullptr;

static void LogCutsceneSkipManagerState(const char* eventName) {
    const auto* const singleton = ASLR<uintptr_t*>(0x013B8790);
    const uintptr_t manager = singleton ? *singleton : 0;
    std::stringstream ss;
    ss << "[CutsceneSkipDiag] " << eventName << " macro="
       << (g_cutsceneSkipDiagMacro ? g_cutsceneSkipDiagMacro : "<none>")
       << " manager=0x" << std::hex << manager;
    if (manager) {
        const unsigned char* b = reinterpret_cast<const unsigned char*>(manager);
        ss << std::dec
           << " inCutscene=" << static_cast<unsigned>(b[0xF6])
           << " skipped=" << static_cast<unsigned>(b[0xF7])
           << " skippable=" << static_cast<unsigned>(b[0xF8])
           << " skippableWhilePaused=" << static_cast<unsigned>(b[0xF9])
           << " alternateInput=" << static_cast<unsigned>(b[0xFA])
           << " promptPrimary=" << static_cast<unsigned>(b[0xFB])
           << " promptSkipScene=" << static_cast<unsigned>(b[0xFC])
           << " skipTimerActive=" << static_cast<unsigned>(b[0x100]);
    }
    LogToFile(ss.str());
}

static void __fastcall DiagSetCutsceneMode_Detour(void* pThis, void*, bool inCutscene, bool skippable) {
    DiagSetCutsceneMode_Original(pThis, inCutscene, skippable);
    if (InterlockedCompareExchange(&g_cutsceneSkipDiagActive, 0, 0)) {
        std::stringstream ss;
        ss << "SetCutsceneMode(inCutscene=" << (inCutscene ? 1 : 0)
           << ", second=" << (skippable ? 1 : 0) << ")";
        LogCutsceneSkipManagerState(ss.str().c_str());
    }
}
static __declspec(naked) void DiagSetCutsceneMode_NakedStub() { __asm { jmp DiagSetCutsceneMode_Detour } }

// Last common boundary before CActionTalkToThing construction. Retail's generated
// father cutscene pushes (false, true, false) for every direct Speak; log the live
// values without changing them.
static void __fastcall DiagScriptedThingSpeak_Detour(void* pThis, void*,
    const CScriptThing* pTarget, const char* dialogueKey, ETextGroupSelectionMethod method,
    bool makeTargetListen, bool soundIn2D, bool overScreenFade) {
    if (InterlockedCompareExchange(&g_cutsceneSkipDiagActive, 0, 0)) {
        std::stringstream ss;
        ss << "[CutsceneSpeechDiag] macro="
           << (g_cutsceneSkipDiagMacro ? g_cutsceneSkipDiagMacro : "<none>")
           << " controller=0x" << std::hex << reinterpret_cast<uintptr_t>(pThis)
           << " target=0x" << reinterpret_cast<uintptr_t>(pTarget)
           << std::dec << " method=" << static_cast<int>(method)
           << " listen=" << (makeTargetListen ? 1 : 0)
           << " soundIn2D=" << (soundIn2D ? 1 : 0)
           << " overFade=" << (overScreenFade ? 1 : 0)
           << " key=" << (dialogueKey ? dialogueKey : "<null>");
        LogToFile(ss.str());
    }
    DiagScriptedThingSpeak_Original(pThis, pTarget, dialogueKey, method,
        makeTargetListen, soundIn2D, overScreenFade);
}
static __declspec(naked) void DiagScriptedThingSpeak_NakedStub() {
    __asm { jmp DiagScriptedThingSpeak_Detour }
}

// Observe the exact retail task boundary used by RunCutsceneMacro_Func. This is
// deliberately passive outside the Begin/End bracket and never changes operands
// or return values. It distinguishes a rejected MoveToPosition from a task that
// starts and is later cancelled, which position-only probes cannot do.
static void __fastcall DiagScriptedThingMoveToPosition_Detour(void* pThis, void*,
    const C3DVector* pPos, float radius, EScriptEntityMoveType moveType, bool b1, bool b2) {
    if (InterlockedCompareExchange(&g_cutsceneSkipDiagActive, 0, 0)) {
        std::stringstream ss;
        ss << "[CutsceneMoveDiag] issue macro="
           << (g_cutsceneSkipDiagMacro ? g_cutsceneSkipDiagMacro : "<none>")
           << " controller=0x" << std::hex << reinterpret_cast<uintptr_t>(pThis)
           << std::dec << " destination=(";
        if (pPos) ss << pPos->x << ',' << pPos->y << ',' << pPos->z;
        else ss << "<null>";
        ss << ") radius=" << radius << " moveType=" << static_cast<int>(moveType)
           << " b1=" << (b1 ? 1 : 0) << " b2=" << (b2 ? 1 : 0);
        LogToFile(ss.str());
    }
    DiagScriptedThingMoveToPosition_Original(pThis, pPos, radius, moveType, b1, b2);
}
static __declspec(naked) void DiagScriptedThingMoveToPosition_NakedStub() {
    __asm { jmp DiagScriptedThingMoveToPosition_Detour }
}

static void __fastcall DiagScriptedThingMoveToThing_Detour(void* pThis, void*,
    const CScriptThing* pTarget, float radius, EScriptEntityMoveType moveType,
    bool b1, bool b2, bool b3, bool b4) {
    if (InterlockedCompareExchange(&g_cutsceneSkipDiagActive, 0, 0)) {
        std::stringstream ss;
        ss << "[CutsceneMoveDiag] issueThing macro="
           << (g_cutsceneSkipDiagMacro ? g_cutsceneSkipDiagMacro : "<none>")
           << " controller=0x" << std::hex << reinterpret_cast<uintptr_t>(pThis)
           << " target=0x" << reinterpret_cast<uintptr_t>(pTarget)
           << std::dec << " radius=" << radius << " moveType=" << static_cast<int>(moveType)
           << " b1=" << (b1 ? 1 : 0) << " b2=" << (b2 ? 1 : 0)
           << " b3=" << (b3 ? 1 : 0) << " b4=" << (b4 ? 1 : 0);
        LogToFile(ss.str());
    }
    DiagScriptedThingMoveToThing_Original(pThis, pTarget, radius, moveType, b1, b2, b3, b4);
}
static __declspec(naked) void DiagScriptedThingMoveToThing_NakedStub() {
    __asm { jmp DiagScriptedThingMoveToThing_Detour }
}

static bool __fastcall DiagScriptedThingIsPerformingScriptTask_Detour(void* pThis, void*) {
    const bool result = DiagScriptedThingIsPerformingScriptTask_Original(pThis);
    if (InterlockedCompareExchange(&g_cutsceneSkipDiagActive, 0, 0)) {
        std::stringstream ss;
        ss << "[CutsceneMoveDiag] poll macro="
           << (g_cutsceneSkipDiagMacro ? g_cutsceneSkipDiagMacro : "<none>")
           << " controller=0x" << std::hex << reinterpret_cast<uintptr_t>(pThis)
           << std::dec << " active=" << (result ? 1 : 0);
        LogToFile(ss.str());
    }
    return result;
}
static __declspec(naked) void DiagScriptedThingIsPerformingScriptTask_NakedStub() {
    __asm { jmp DiagScriptedThingIsPerformingScriptTask_Detour }
}

bool InstallCutsceneSkipDiagnosticHooks() {
    if (g_cutsceneSkipDiagHooksInstalled) return true;
    const DWORD setMode = ASLR<DWORD>(0x0089D600);
    const DWORD scriptedThingSpeak = ASLR<DWORD>(0x00904780);
    const DWORD scriptedThingMove = ASLR<DWORD>(0x00903BC0);
    const DWORD scriptedThingMoveToThing = ASLR<DWORD>(0x00903E20);
    const DWORD scriptedThingPoll = ASLR<DWORD>(0x00904EE0);
    const BYTE setModeExpected[5] = {0x53, 0x8b, 0x5c, 0x24, 0x08};
    const BYTE speakExpected[6] = {0x56, 0x8d, 0x71, 0x14, 0x8b, 0xce};
    const BYTE moveExpected[6] = {0x83, 0xec, 0x08, 0x53, 0x8b, 0xd9};
    const BYTE moveToThingExpected[7] = {0x55, 0x8b, 0xe9, 0x57, 0x8d, 0x7d, 0x14};
    const BYTE pollExpected[7] = {0x51, 0x56, 0x8d, 0x71, 0x14, 0x8b, 0xce};
    if (memcmp(reinterpret_cast<const void*>(setMode), setModeExpected, sizeof(setModeExpected)) ||
        memcmp(reinterpret_cast<const void*>(scriptedThingSpeak), speakExpected, sizeof(speakExpected)) ||
        memcmp(reinterpret_cast<const void*>(scriptedThingMove), moveExpected, sizeof(moveExpected)) ||
        memcmp(reinterpret_cast<const void*>(scriptedThingMoveToThing), moveToThingExpected, sizeof(moveToThingExpected)) ||
        memcmp(reinterpret_cast<const void*>(scriptedThingPoll), pollExpected, sizeof(pollExpected))) {
        LogToFile("!!! [CutsceneSkipDiag] retail prologue mismatch; no diagnostic hooks installed.");
        return false;
    }
    const bool a = InstallInlineHook(setMode, &DiagSetCutsceneMode_NakedStub, 5,
        reinterpret_cast<void**>(&DiagSetCutsceneMode_Original), "CutsceneSkip.SetCutsceneMode");
    g_cutsceneSpeakDiagHookInstalled = InstallInlineHook(scriptedThingSpeak,
        &DiagScriptedThingSpeak_NakedStub, 6,
        reinterpret_cast<void**>(&DiagScriptedThingSpeak_Original), "CutsceneSpeech.ScriptedThingSpeak");
    const bool moveHook = InstallInlineHook(scriptedThingMove,
        &DiagScriptedThingMoveToPosition_NakedStub, 6,
        reinterpret_cast<void**>(&DiagScriptedThingMoveToPosition_Original), "CutsceneMove.MoveToPosition");
    const bool moveToThingHook = InstallInlineHook(scriptedThingMoveToThing,
        &DiagScriptedThingMoveToThing_NakedStub, 7,
        reinterpret_cast<void**>(&DiagScriptedThingMoveToThing_Original), "CutsceneMove.MoveToThing");
    const bool pollHook = InstallInlineHook(scriptedThingPoll,
        &DiagScriptedThingIsPerformingScriptTask_NakedStub, 7,
        reinterpret_cast<void**>(&DiagScriptedThingIsPerformingScriptTask_Original), "CutsceneMove.IsPerformingScriptTask");
    g_cutsceneMovementDiagHooksInstalled = moveHook && moveToThingHook && pollHook;
    g_cutsceneSkipDiagHooksInstalled = a;
    LogToFile(g_cutsceneSkipDiagHooksInstalled
        ? "[CutsceneSkipDiag] passive SetCutsceneMode boundary hook installed."
        : "!!! [CutsceneSkipDiag] hook installation incomplete; diagnostic bracketing disabled.");
    LogToFile(g_cutsceneSpeakDiagHookInstalled
        ? "[CutsceneSpeechDiag] passive retail Speak boundary hook installed."
        : "!!! [CutsceneSpeechDiag] passive Speak boundary hook installation failed.");
    LogToFile(g_cutsceneMovementDiagHooksInstalled
        ? "[CutsceneMoveDiag] passive retail movement boundary hooks installed."
        : "!!! [CutsceneMoveDiag] passive movement hook installation failed.");
    return g_cutsceneSkipDiagHooksInstalled;
}

void BeginCutsceneSkipDiagnostics(const char* macroName) {
    if (!g_cutsceneSkipDiagHooksInstalled) return;
    g_cutsceneSkipDiagMacro = macroName;
    InterlockedExchange(&g_cutsceneSkipDiagActive, 1);
    LogCutsceneSkipManagerState("begin");
}

void EndCutsceneSkipDiagnostics() {
    if (!g_cutsceneSkipDiagHooksInstalled) return;
    LogCutsceneSkipManagerState("end");
    InterlockedExchange(&g_cutsceneSkipDiagActive, 0);
    g_cutsceneSkipDiagMacro = nullptr;
}

// The card's stale in-RAM entry never re-renders because UpdateDiskCacheTexture @ 0x00B2D330
// short-circuits: iVar14 = RBTree_FindLE(...); if (found && IsNearlyIdentical(...)) return false; -- it
// skips RenderToTexture. So we (A) flag while inside UpdateDiskCacheTexture, (B) force IsNearlyIdentical
// @ 0x00B2B050 to return FALSE while that flag is set -> the gate falls through and the card RE-RENDERS
// every view, and (C) clear the scratch surface right after StartComposition @ 0x00F39730 binds it (same
// flag) so the re-render lands clean. IsNearlyIdentical has a 2nd caller (GetComposedSurface) -- the flag
// leaves it untouched.
static volatile long g_inUDC = 0;
static int g_udcFires = 0, g_scClears = 0;

// Hook A: UpdateDiskCacheTexture @ 0x00B2D330 (prologue 83 EC 48 A1 98 6E 43 01 = 8 bytes).
// __thiscall(this, CCompositeSurfaceCache*, ulong, bool) = 3 stack args, returns bool.
typedef char(__thiscall* tUpdateDiskCacheTexture)(void*, void*, unsigned long, char);
static tUpdateDiskCacheTexture UpdateDiskCacheTexture_Original = nullptr;
static char __fastcall UpdateDiskCacheTexture_Detour(void* pThis, void* /*edx*/, void* a1, unsigned long a2, char a3) {
    InterlockedIncrement(&g_inUDC);
    if (g_udcFires < 6) { std::stringstream s; s << "[CardClear] DIAG UpdateDiskCacheTexture #" << ++g_udcFires; LogToFile(s.str()); }
    char r = UpdateDiskCacheTexture_Original(pThis, a1, a2, a3);
    InterlockedDecrement(&g_inUDC);
    return r;
}
static __declspec(naked) void UpdateDiskCacheTexture_NakedStub() { __asm { jmp UpdateDiskCacheTexture_Detour } }

// Hook B: IsNearlyIdentical @ 0x00B2B050 (prologue 51 8B 44 24 08 = 5 bytes).
// __thiscall(this, arg1, morph, float) = 3 stack args. Return FALSE while inside UpdateDiskCacheTexture.
typedef char(__thiscall* tIsNearlyIdentical)(void*, void*, void*, float);
static tIsNearlyIdentical IsNearlyIdentical_Original = nullptr;
static char __fastcall IsNearlyIdentical_Detour(void* pThis, void* /*edx*/, void* a1, void* a2, float a3) {
    if (g_inUDC > 0) return 0;   // force "not identical" -> the card re-renders instead of reusing the cache
    return IsNearlyIdentical_Original(pThis, a1, a2, a3);
}
static __declspec(naked) void IsNearlyIdentical_NakedStub() { __asm { jmp IsNearlyIdentical_Detour } }

// Hook C: StartComposition @ 0x00F39730 (prologue 83 EC 20 53 55 = 5 bytes). __thiscall(this, bool).
// After the original binds the scratch surface, clear it while inside UpdateDiskCacheTexture.
typedef void(__thiscall* tStartComposition)(void*, char);
static tStartComposition StartComposition_Original = nullptr;
static void __fastcall StartComposition_Detour(void* pThis, void* /*edx*/, char param1) {
    StartComposition_Original(pThis, param1);            // binds the scratch surface (AttachTarget)
    if (g_inUDC > 0 && ClearRenderTarget_Fn && g_ppDisplayManager && *g_ppDisplayManager) {
        if (g_scClears < 6) { std::stringstream s; s << "[CardClear] DIAG StartComposition clear #" << ++g_scClears; LogToFile(s.str()); }
        FseRGBColour black = { 0, 0, 0, 0xFF };
        ClearRenderTarget_Fn(*g_ppDisplayManager, &black, 4);   // wipe stale scratch before the card composites
    }
}
static __declspec(naked) void StartComposition_NakedStub() { __asm { jmp StartComposition_Detour } }

bool InstallCardRTClearHook() {
    if (g_cardClearHooksInstalled) return true;
    if (!g_fableBase) { LogToFile("!!! [CardClear] rejected: game base unavailable."); return false; }
    ClearRenderTarget_Fn = ASLR<tClearRenderTarget>(0x009BE420);
    g_ppDisplayManager   = ASLR<void**>(0x01436E1C);
    bool a = InstallInlineHook(ASLR<DWORD>(0x00B2D330), reinterpret_cast<void*>(&UpdateDiskCacheTexture_NakedStub),
                               8, reinterpret_cast<void**>(&UpdateDiskCacheTexture_Original), "UpdateDiskCacheTexture");
    bool b = InstallInlineHook(ASLR<DWORD>(0x00B2B050), reinterpret_cast<void*>(&IsNearlyIdentical_NakedStub),
                               5, reinterpret_cast<void**>(&IsNearlyIdentical_Original), "IsNearlyIdentical");
    bool c = InstallInlineHook(ASLR<DWORD>(0x00F39730), reinterpret_cast<void*>(&StartComposition_NakedStub),
                               5, reinterpret_cast<void**>(&StartComposition_Original), "StartComposition");
    g_cardClearHooksInstalled = a && b && c;
    LogToFile(g_cardClearHooksInstalled
        ? "[CardClear] 3 hooks installed: UpdateDiskCacheTexture + IsNearlyIdentical(force re-render) + StartComposition(clear)."
        : "!!! [CardClear] one or more hooks FAILED to install.");
    return g_cardClearHooksInstalled;
}

// =====================================================================================
// ForgeFSE addition: MAP-OPEN NAME-ALIAS detour (ForgeTest terrain white-out route 2)
// =====================================================================================
// Trampoline detour on CEngineMap::OpenRetailStaticMap @ retail 0x00B41E50.
//
// Mechanism (proven, work/terrain_path/mapopen_proven_decomp.log):
//   - OpenRetailStaticMap is __thiscall; this (CEngineMap*) arrives in ECX.
//   - It reads the CEngineMap's OWN name -- a CCharString at (this + 0x1c) -- and resolves the
//     graphic/scenery BANK entry (via CreateNewBankMap @0x009D58D0) and terrain THEMES from it.
//   - ForgeTest's donor-cloned bank/theme indices are registered ONLY under the donor's name, so
//     opening "ForgeTest.lev" binds the wrong entry -> shared region bank corrupted -> white-out.
//
// This detour swaps (this+0x1c) from the SOURCE name ("ForgeTest.lev") to the DONOR name
// ("Darkwood9_Leadout_01") for exactly one OpenRetailStaticMap call, then restores the original
// CCharString, so the sub-map opens AS the donor (bank + theme both resolve to donor-valid data).
// It uses the game's own CCharString copy-ctor/assign/destructor so refcounts stay correct.
//
// The 5-byte prologue at 0x00B41E50 is:  83 EC 64 (SUB ESP,0x64)  53 (PUSH EBX)  55 (PUSH EBP)
// -- all position-independent, so it relocates cleanly into a trampoline that replays those bytes
// and JMPs to 0x00B41E55. We overwrite 0x00B41E50 with a 5-byte JMP to a naked stub that funnels
// into the C++ handler (preserving __thiscall: ECX=this).

tOpenRetailStaticMap OpenRetailStaticMap_Original = nullptr; // -> trampoline (replayed prologue + jmp back)

// Runtime alias config (guarded by the empty-source check; no lock -- map-open is single-threaded).
static std::string g_aliasSourceName; // e.g. "ForgeTest.lev" (empty == disabled)
static std::string g_aliasDonorName;  // e.g. "Darkwood9_Leadout_01"
static bool        g_aliasHookInstalled = false;
// Observational identity used by the later foreground-decode probe.  Classify
// decoded layers by their owning map, never by the coordinates being tested.
static DWORD       g_forgeTestLandscapeMap = 0;

static void LogStaticMapProbeResult(void* pEngineMap) {
    const char* base = reinterpret_cast<const char*>(pEngineMap);
    const unsigned long openState = *reinterpret_cast<const unsigned long*>(base + 0x20);
    const float* bounds = reinterpret_cast<const float*>(base + 0x24);
    const void* asyncEntry = *reinterpret_cast<void* const*>(base + 0x3c);
    unsigned long bankIndex = 0xffffffffUL;
    if (asyncEntry) {
        bankIndex = *reinterpret_cast<const unsigned long*>(
            reinterpret_cast<const char*>(asyncEntry) + 0x8);
    }
    std::stringstream ss;
    ss << "[StaticMapProbe] opened state=" << openState
       << " bankIndex=" << bankIndex
       << " bounds=(" << bounds[0] << "," << bounds[1] << "," << bounds[2]
       << ")..(" << bounds[3] << "," << bounds[4] << "," << bounds[5] << ")";
    LogToFile(ss.str());
}

void SetMapResourceAlias(const std::string& sourceName, const std::string& donorName) {
    g_aliasSourceName = sourceName;
    g_aliasDonorName = donorName;
    LogToFile("[MapResourceAlias] alias set: '" + sourceName + "' -> '" + donorName + "'");
}

void ClearMapResourceAlias() {
    LogToFile("[MapResourceAlias] alias cleared (was '" + g_aliasSourceName + "' -> '" + g_aliasDonorName + "')");
    g_aliasSourceName.clear();
    g_aliasDonorName.clear();
}

bool LoadStartupMapResourceAlias() {
    const std::string path = g_fseBasePath + "/map_resource_alias.ini";
    if (GetFileAttributesA(path.c_str()) == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    char source[MAX_PATH] = { 0 };
    char donor[MAX_PATH] = { 0 };
    GetPrivateProfileStringA("MapResourceAlias", "Source", "", source,
                             static_cast<DWORD>(sizeof(source)), path.c_str());
    GetPrivateProfileStringA("MapResourceAlias", "Donor", "", donor,
                             static_cast<DWORD>(sizeof(donor)), path.c_str());
    if (source[0] == '\0' || donor[0] == '\0') {
        LogToFile("!!! [MapResourceAlias] map_resource_alias.ini exists but "
                  "[MapResourceAlias] Source/Donor is incomplete; startup alias disabled.");
        return false;
    }

    SetMapResourceAlias(source, donor);
    LogToFile("[MapResourceAlias] startup alias loaded from '" + path +
              "' before static-map bootstrap.");
    return true;
}

// The actual C++ handler. __fastcall so ECX(this)/EDX map onto the first two params; the naked
// stub arranges the stack so the 3 real stack args follow. Returns nothing (original is void).
static void __fastcall OpenRetailStaticMap_Detour(void* pEngineMap, void* /*edx*/,
                                                  void* pBankFile, void* pDataStream, void* pManifestMap) {
    // Map-open telemetry is unconditional. Alias mutation remains optional and disabled when its
    // extra CCharString APIs are unavailable.
    if (!pEngineMap || !CCharString_ToConstChar_API) {
        OpenRetailStaticMap_Original(pEngineMap, pBankFile, pDataStream, pManifestMap);
        return;
    }

    // The CEngineMap name is a CCharString at this+0x1c (disasm: LEA EAX,[ESI+0x1c]).
    CCharString* pName = reinterpret_cast<CCharString*>(reinterpret_cast<char*>(pEngineMap) + 0x1c);

    const char* pCur = nullptr;
    // Guard the raw read: a null/garbage pStringData would fault the const-char accessor.
    if (pName->pStringData) {
        pCur = CCharString_ToConstChar_API(pName);
    }

    LogToFile(std::string("[StaticMapProbe] opening '") + (pCur ? pCur : "<null>") + "'.");
    const bool forgeTest = pCur && std::string(pCur).find("ForgeTest") != std::string::npos;
    if (forgeTest) {
        g_forgeTestLandscapeMap = reinterpret_cast<DWORD>(pEngineMap);
    }

    if (g_aliasSourceName.empty() || !CCharString_Construct_Copy ||
        !CCharString_Assign_API || !CCharString_Destroy) {
        OpenRetailStaticMap_Original(pEngineMap, pBankFile, pDataStream, pManifestMap);
        if (forgeTest) LogStaticMapProbeResult(pEngineMap);
        return;
    }

    bool matched = (pCur != nullptr) && (g_aliasSourceName == pCur);

    if (!matched) {
        OpenRetailStaticMap_Original(pEngineMap, pBankFile, pDataStream, pManifestMap);
        if (forgeTest) LogStaticMapProbeResult(pEngineMap);
        return;
    }

    LogToFile(std::string("[MapResourceAlias] OpenRetailStaticMap: name '") + pCur +
              "' matches source; substituting donor '" + g_aliasDonorName + "' for this call.");

    // Save the original CCharString by value (a proper refcounted copy) so we can restore it.
    CCharString saved = { 0 };
    CCharString_Construct_Copy(&saved, pName);

    // Build the donor CCharString and copy-assign it into this+0x1c (ref-safe overwrite).
    CCharString donor = { 0 };
    CCharString_Construct_Literal(&donor, g_aliasDonorName.c_str(), -1);
    CCharString_Assign_API(pName, &donor);

    // Run the real open under the donor identity.
    OpenRetailStaticMap_Original(pEngineMap, pBankFile, pDataStream, pManifestMap);

    // Restore the original name, then release our temporaries.
    CCharString_Assign_API(pName, &saved);
    CCharString_Destroy(&donor);
    CCharString_Destroy(&saved);

    LogToFile("[MapResourceAlias] OpenRetailStaticMap: donor open complete; original name restored.");
    if (forgeTest) {
        LogStaticMapProbeResult(pEngineMap);
    }
}

// Naked entry the 5-byte JMP lands on. Preserves __thiscall (ECX=this) and forwards to the
// __fastcall handler. Stack on entry (after the game's CALL): [ret][arg1 bank][arg2 stream][arg3 map].
// __fastcall wants ECX=this, EDX=unused, then (arg1,arg2,arg3) on the stack in that order -- which is
// exactly the incoming layout. So we simply jump to the handler; it returns straight to the caller.
// The handler is `void` and the original is a `ret 0xC` __thiscall; a __fastcall handler with 3 stack
// args also cleans 0xC on return, so caller stack balance is preserved.
static __declspec(naked) void OpenRetailStaticMap_NakedStub() {
    __asm {
        jmp OpenRetailStaticMap_Detour
    }
}

bool InstallMapResourceAliasHook() {
    if (g_aliasHookInstalled) return true;
    if (!g_fableBase) {
        LogToFile("!!! [MapResourceAlias] InstallMapResourceAliasHook: g_fableBase not set (call InitializeFableAPI first).");
        return false;
    }

    const DWORD target = ASLR<DWORD>(0x00B41E50); // CEngineMap::OpenRetailStaticMap entry

    // Allocate an executable trampoline: [replayed 5-byte prologue][JMP back to target+5].
    // 5 (prologue) + 5 (jmp) = 10 bytes; round to 16.
    BYTE* tramp = (BYTE*)VirtualAlloc(nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!tramp) {
        LogToFile("!!! [MapResourceAlias] VirtualAlloc for trampoline failed.");
        return false;
    }

    // Prologue bytes at 0x00B41E50: 83 EC 64 (SUB ESP,0x64) 53 (PUSH EBX) 55 (PUSH EBP).
    // (Position-independent; safe to copy verbatim from the live image.)
    memcpy(tramp, (const void*)target, 5);
    // JMP rel32 from (tramp+5) to (target+5).
    tramp[5] = 0xE9;
    DWORD backRel = (DWORD)(target + 5) - (DWORD)(tramp + 5) - 5;
    memcpy(tramp + 6, &backRel, 4);

    OpenRetailStaticMap_Original = (tOpenRetailStaticMap)tramp;

    // Overwrite the target's first 5 bytes with JMP rel32 -> naked stub.
    BYTE patch[5] = { 0xE9, 0, 0, 0, 0 };
    DWORD fwdRel = (DWORD)&OpenRetailStaticMap_NakedStub - target - 5;
    memcpy(patch + 1, &fwdRel, 4);

    DWORD oldProt;
    if (!VirtualProtect((LPVOID)target, 5, PAGE_EXECUTE_READWRITE, &oldProt)) {
        LogToFile("!!! [MapResourceAlias] VirtualProtect(target) failed; hook NOT installed.");
        VirtualFree(tramp, 0, MEM_RELEASE);
        OpenRetailStaticMap_Original = nullptr;
        return false;
    }
    memcpy((void*)target, patch, 5);
    VirtualProtect((LPVOID)target, 5, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), (LPCVOID)target, 5);

    g_aliasHookInstalled = true;
    std::stringstream ss;
    ss << "[MapResourceAlias] Hook installed at OpenRetailStaticMap 0x" << std::hex << target
       << " (trampoline 0x" << (DWORD)tramp << ").";
    LogToFile(ss.str());
    return true;
}

// =====================================================================================
// ForgeFSE addition: LANDSCAPE PATCH TEXTURE-RESOLUTION telemetry
// =====================================================================================
// UpdateForeground delegates its material-ready decision to LoadTextures @ 0x00BF4130.
// That function indexes the global table at [*DAT_01436e98 + 0x954] using the patch's
// u16 list at +0x0c/+0x10 and clears flag bit 0 when a handle/resource is unavailable.
// The hook is observational: it calls the original first, snapshots the same gate, and logs
// only a patch's first state or a state transition.

tLandscapePatchLoadTextures LandscapePatchLoadTextures_Original = nullptr;
static bool g_landscapeTextureProbeInstalled = false;
static std::map<void*, unsigned long long> g_landscapeTextureProbeStates;
static unsigned long g_landscapeTextureProbeLogCount = 0;

struct LandscapeTextureProbeSnapshot {
    bool safe;
    unsigned char flags;
    unsigned short indexCount;
    unsigned long tableCount;
    unsigned short indices[16];
    unsigned char states[16]; // 0=ready, 1=OOB, 2=no handle, 3=no resource, 4=resource not ready
    unsigned short captured;
    unsigned short missing;
};

static void CaptureLandscapeTextureProbe(void* pPatch, LandscapeTextureProbeSnapshot* out) {
    memset(out, 0, sizeof(*out));
    __try {
        const char* patch = reinterpret_cast<const char*>(pPatch);
        out->flags = *reinterpret_cast<const unsigned char*>(patch + 0x3c);
        out->indexCount = *reinterpret_cast<const unsigned short*>(patch + 0x10);
        const unsigned short* indices =
            *reinterpret_cast<const unsigned short* const*>(patch + 0x0c);

        const DWORD engine = *ASLR<DWORD*>(0x01436E98);
        const DWORD table = engine ? *reinterpret_cast<const DWORD*>(engine + 0x954) : 0;
        const DWORD begin = table ? *reinterpret_cast<const DWORD*>(table + 0x1e0) : 0;
        const DWORD end = table ? *reinterpret_cast<const DWORD*>(table + 0x1e4) : 0;
        out->tableCount = (begin && end >= begin) ? (end - begin) / 0x2c : 0;
        out->captured = out->indexCount < 16 ? out->indexCount : 16;

        for (unsigned short i = 0; i < out->captured; ++i) {
            const unsigned short index = indices ? indices[i] : 0xffff;
            out->indices[i] = index;
            unsigned char state = 0;
            if (!indices || index >= out->tableCount) {
                state = 1;
            } else {
                const DWORD handle = *reinterpret_cast<const DWORD*>(begin + index * 0x2c + 0x28);
                if (!handle) state = 2;
                else {
                    const DWORD resource = *reinterpret_cast<const DWORD*>(handle + 0x08);
                    if (!resource) state = 3;
                    else if (*reinterpret_cast<const DWORD*>(resource + 0x1c) == 0) state = 4;
                }
            }
            out->states[i] = state;
            if (state != 0) ++out->missing;
        }
        out->safe = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        out->safe = false;
    }
}

// --- ForgeTest texture pin (fix for the camera-driven foreground vanish) ---------------------
// The terrain's foreground textures are GLOBAL + evictable; ForgeTest's host region is not
// preloaded, so the streamer evicts them under view pressure -> LoadTextures returns not-ready
// -> RenderForeground clears flag 0x04 and aborts -> vanish. Fix: for ForgeTest patches only,
// force-load (CFrameData::BeginLoading @0x009FDE80) + keep-most-recently-used (CResource::SetAsMRU
// @0x009FC450) each referenced texture BEFORE the original readiness check runs. Texture-agnostic
// (holds future custom textures too). Retail addresses verified vs rebuild/manifest.
typedef void(__thiscall* tCFrameDataBeginLoading)(void* pFrameData, unsigned long lod, unsigned long priority);
typedef void(__thiscall* tCResourceSetAsMRU)(void* pResource);
static tCFrameDataBeginLoading CFrameData_BeginLoading = nullptr;
static tCResourceSetAsMRU      CResource_SetAsMRU = nullptr;

static int g_pinDiagCount = 0;
static int g_pinFireCount = 0;
static int g_pinTargetDiagCount = 0;
// SEH-only helper: read candidate map-pointer offsets into PODs (no C++ objects allowed in __try).
static bool ReadPinDiag(void* pPatch, DWORD out[6]) {
    __try {
        const char* p = reinterpret_cast<const char*>(pPatch);
        out[0]=*reinterpret_cast<const DWORD*>(p+0x04); out[1]=*reinterpret_cast<const DWORD*>(p+0x08);
        out[2]=*reinterpret_cast<const DWORD*>(p+0x14); out[3]=*reinterpret_cast<const DWORD*>(p+0x18);
        out[4]=*reinterpret_cast<const DWORD*>(p+0x1c); out[5]=*reinterpret_cast<const DWORD*>(p+0x20);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
static bool ReadPinBounds(void* pPatch, float out[4]) {
    __try {
        const char* p = reinterpret_cast<const char*>(pPatch);
        out[0] = *reinterpret_cast<const float*>(p + 0x20);
        out[1] = *reinterpret_cast<const float*>(p + 0x24);
        out[2] = *reinterpret_cast<const float*>(p + 0x2c);
        out[3] = *reinterpret_cast<const float*>(p + 0x30);
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
// SEH-only helper: does the pin; returns true if the patch belonged to ForgeTest (gate matched).
static bool DoPinForgeTestPatch(void* pPatch) {
    __try {
        const char* patch = reinterpret_cast<const char*>(pPatch);
        const DWORD patchMap = *reinterpret_cast<const DWORD*>(patch + 0x04);
        // The retail patch object carries its world-space AABB at +0x20..+0x34;
        // this is the same field set captured by the foreground render probe.
        // ForgeTest's authored map bounds are measured from its opened LEV:
        // (3328,2296)..(3392,2360).  Use the object identity when available,
        // but retain a coordinate gate because the patch's +0x04 owner pointer
        // is not the CEngineMap pointer on every LoadTextures call observed.
        const float minX = *reinterpret_cast<const float*>(patch + 0x20);
        const float minY = *reinterpret_cast<const float*>(patch + 0x24);
        const float maxX = *reinterpret_cast<const float*>(patch + 0x2c);
        const float maxY = *reinterpret_cast<const float*>(patch + 0x30);
        const bool coordinateMatch =
            minX >= 3328.0f && maxX <= 3392.0f &&
            minY >= 2296.0f && maxY <= 2360.0f;
        const bool identityMatch =
            g_forgeTestLandscapeMap && patchMap == g_forgeTestLandscapeMap;
        if (!identityMatch && !coordinateMatch) return false;
        const unsigned short indexCount = *reinterpret_cast<const unsigned short*>(patch + 0x10);
        const unsigned short* indices = *reinterpret_cast<const unsigned short* const*>(patch + 0x0c);
        if (!indices) return true;
        const DWORD engine = *ASLR<DWORD*>(0x01436E98);
        const DWORD table = engine ? *reinterpret_cast<const DWORD*>(engine + 0x954) : 0;
        const DWORD begin = table ? *reinterpret_cast<const DWORD*>(table + 0x1e0) : 0;
        const DWORD endp = table ? *reinterpret_cast<const DWORD*>(table + 0x1e4) : 0;
        if (!begin || endp < begin) return true;
        const DWORD tableCount = (endp - begin) / 0x2c;
        const unsigned short cap = indexCount < 64 ? indexCount : 64;
        for (unsigned short i = 0; i < cap; ++i) {
            const unsigned short index = indices[i];
            if (index >= tableCount) continue;
            const DWORD handle = *reinterpret_cast<const DWORD*>(begin + index * 0x2c + 0x28);
            if (!handle) continue;
            const DWORD resource = *reinterpret_cast<const DWORD*>(handle + 0x08);
            if (!resource) continue;
            CFrameData_BeginLoading(reinterpret_cast<void*>(resource), 0, 0); // lod 0, high priority
            if (CResource_SetAsMRU) CResource_SetAsMRU(reinterpret_cast<void*>(resource));
        }
        return true;
    } __except (EXCEPTION_EXECUTE_HANDLER) { return false; }
}
static void PinForgeTestPatchTextures(void* pPatch) {
    if (!CFrameData_BeginLoading) return;
    if (g_pinDiagCount < 12 && g_forgeTestLandscapeMap) {
        DWORD d[6];
        if (ReadPinDiag(pPatch, d)) {
            ++g_pinDiagCount;
            std::stringstream ds;
            ds << "[PinDiag] ftMap=0x" << std::hex << g_forgeTestLandscapeMap
               << " p+04=0x" << d[0] << " p+08=0x" << d[1] << " p+14=0x" << d[2]
               << " p+18=0x" << d[3] << " p+1c=0x" << d[4] << " p+20=0x" << d[5];
            LogToFile(ds.str());
        }
    }
    if (g_pinTargetDiagCount < 16) {
        float bounds[4];
        if (ReadPinBounds(pPatch, bounds)) {
            const float minX = bounds[0];
            const float minY = bounds[1];
            const float maxX = bounds[2];
            const float maxY = bounds[3];
            if (minX >= 3328.0f && maxX <= 3392.0f &&
                minY >= 2296.0f && maxY <= 2360.0f) {
                ++g_pinTargetDiagCount;
                std::stringstream ds;
                ds << "[PinDiag] TARGET bounds=(" << minX << "," << minY
                   << ")->(" << maxX << "," << maxY << ")";
                LogToFile(ds.str());
            }
        }
    }
    if (DoPinForgeTestPatch(pPatch) && g_pinFireCount < 8) {
        ++g_pinFireCount; LogToFile("[PinDiag] PIN FIRED for ForgeTest patch");
    }
}

static bool __fastcall LandscapePatchLoadTextures_Detour(
    void* pPatch, void* /*edx*/, float distanceSquared) {
    PinForgeTestPatchTextures(pPatch); // pin ForgeTest textures resident before the readiness check
    const bool result = LandscapePatchLoadTextures_Original(pPatch, distanceSquared);
    LandscapeTextureProbeSnapshot snap;
    CaptureLandscapeTextureProbe(pPatch, &snap);

    unsigned long long signature = result ? 1ULL : 0ULL;
    signature |= static_cast<unsigned long long>(snap.flags) << 8;
    signature |= static_cast<unsigned long long>(snap.indexCount) << 16;
    signature |= static_cast<unsigned long long>(snap.missing) << 32;
    signature |= static_cast<unsigned long long>(snap.safe ? 1 : 0) << 63;
    const auto previous = g_landscapeTextureProbeStates.find(pPatch);
    if (g_landscapeTextureProbeLogCount < 512 &&
        (previous == g_landscapeTextureProbeStates.end() || previous->second != signature)) {
        g_landscapeTextureProbeStates[pPatch] = signature;
        ++g_landscapeTextureProbeLogCount;
        std::stringstream ss;
        ss << "[LandscapeTextureProbe] patch=0x" << std::hex
           << reinterpret_cast<DWORD>(pPatch) << std::dec
           << " distSq=" << distanceSquared << " result=" << (result ? 1 : 0)
           << " flags=0x" << std::hex << static_cast<unsigned>(snap.flags) << std::dec
           << " indices=" << snap.indexCount << " table=" << snap.tableCount
           << " missing=" << snap.missing << " safe=" << (snap.safe ? 1 : 0)
           << " values=[";
        for (unsigned short i = 0; i < snap.captured; ++i) {
            if (i) ss << ',';
            ss << snap.indices[i] << ':' << static_cast<unsigned>(snap.states[i]);
        }
        if (snap.indexCount > snap.captured) ss << ",...";
        ss << ']';
        LogToFile(ss.str());
    }
    return result;
}

static __declspec(naked) void LandscapePatchLoadTextures_NakedStub() {
    __asm { jmp LandscapePatchLoadTextures_Detour }
}

bool InstallLandscapeTextureProbeHook() {
    if (g_landscapeTextureProbeInstalled) return true;
    if (!g_fableBase) {
        LogToFile("!!! [LandscapeTextureProbe] g_fableBase not initialized.");
        return false;
    }
    // Bind the texture-pin APIs (retail addresses, verified vs rebuild/manifest).
    CFrameData_BeginLoading = ASLR<tCFrameDataBeginLoading>(0x009FDE80);
    CResource_SetAsMRU = ASLR<tCResourceSetAsMRU>(0x009FC450);

    const DWORD target = ASLR<DWORD>(0x00BF4130);
    const size_t prologueSize = 7; // 51 55 8B E9 8A 45 3C: whole instructions
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) {
        LogToFile("!!! [LandscapeTextureProbe] trampoline allocation failed.");
        return false;
    }
    memcpy(tramp, reinterpret_cast<const void*>(target), prologueSize);
    tramp[prologueSize] = 0xE9;
    const DWORD backRel = target + static_cast<DWORD>(prologueSize) -
                          reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    LandscapePatchLoadTextures_Original =
        reinterpret_cast<tLandscapePatchLoadTextures>(tramp);

    BYTE patch[7] = {0xE9, 0, 0, 0, 0, 0x90, 0x90};
    const DWORD fwdRel = reinterpret_cast<DWORD>(&LandscapePatchLoadTextures_NakedStub) -
                         target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) {
        VirtualFree(tramp, 0, MEM_RELEASE);
        LandscapePatchLoadTextures_Original = nullptr;
        LogToFile("!!! [LandscapeTextureProbe] VirtualProtect failed.");
        return false;
    }
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);
    g_landscapeTextureProbeInstalled = true;
    std::stringstream ss;
    ss << "[LandscapeTextureProbe] Hook installed at 0x" << std::hex << target
       << " (trampoline 0x" << reinterpret_cast<DWORD>(tramp) << ").";
    LogToFile(ss.str());
    return true;
}

// =====================================================================================
// ForgeFSE addition: LANDSCAPE BACKGROUND DRAW telemetry
// =====================================================================================
// CLandscapeBackgroundPatch::Render @ 0x00BE6F70 binds the patch's composed CTexture
// at +0xB8 and submits the CIndexBuffer/+0xBC and CVertexBufferWin32/+0xC0.  This is a
// separate stage from CEngineLandscapePatch::LoadTextures: the latter proves source
// texture resources are ready, while this hook proves what the background draw consumes.

tLandscapeBackgroundPatchRender LandscapeBackgroundPatchRender_Original = nullptr;
static bool g_landscapeBackgroundRenderProbeInstalled = false;
static std::map<void*, unsigned long long> g_landscapeBackgroundRenderProbeStates;
static unsigned long g_landscapeBackgroundRenderProbeLogCount = 0;

struct LandscapeBackgroundRenderProbeSnapshot {
    bool safe;
    DWORD texture;
    DWORD d3dTexture;
    DWORD textureBytes;
    DWORD indexBuffer;
    DWORD d3dIndexBuffer;
    DWORD indexBytes;
    DWORD vertexBuffer;
    DWORD d3dVertexBuffer;
    DWORD vertexBytes;
    DWORD vertexType;
    unsigned char vertexStride;
    float minZ;
    float maxZ;
    unsigned short vertexCount;
    unsigned short polyCount;
    unsigned short mapX;
    unsigned short mapY;
    unsigned short width;
    unsigned short height;
    unsigned char lodBand;
    bool sharedIndexBuffer;
    bool waterOnly;
    bool defaultSection;
    unsigned char bridgeCount;
};

static void CaptureLandscapeBackgroundRenderProbe(
    void* pPatch, LandscapeBackgroundRenderProbeSnapshot* out) {
    memset(out, 0, sizeof(*out));
    __try {
        const char* patch = reinterpret_cast<const char*>(pPatch);
        out->texture = *reinterpret_cast<const DWORD*>(patch + 0x0b8);
        if (out->texture) {
            out->d3dTexture = *reinterpret_cast<const DWORD*>(out->texture + 0x00);
            out->textureBytes =
                *reinterpret_cast<const DWORD*>(out->texture + 0x04) & 0x0fffffff;
        }

        out->indexBuffer = *reinterpret_cast<const DWORD*>(patch + 0x0bc);
        if (out->indexBuffer) {
            out->d3dIndexBuffer = *reinterpret_cast<const DWORD*>(out->indexBuffer + 0x04);
            out->indexBytes = *reinterpret_cast<const DWORD*>(out->indexBuffer + 0x08);
        }

        out->vertexBuffer = *reinterpret_cast<const DWORD*>(patch + 0x0c0);
        if (out->vertexBuffer) {
            out->d3dVertexBuffer = *reinterpret_cast<const DWORD*>(out->vertexBuffer + 0x04);
            out->vertexBytes = *reinterpret_cast<const DWORD*>(out->vertexBuffer + 0x08);
            out->vertexType = *reinterpret_cast<const DWORD*>(out->vertexBuffer + 0x0c);
            out->vertexStride = *reinterpret_cast<const unsigned char*>(out->vertexBuffer + 0x10);
        }

        out->minZ = *reinterpret_cast<const float*>(patch + 0x0f8);
        out->maxZ = *reinterpret_cast<const float*>(patch + 0x0fc);
        out->vertexCount = *reinterpret_cast<const unsigned short*>(patch + 0x104);
        out->polyCount = *reinterpret_cast<const unsigned short*>(patch + 0x106);
        out->mapX = *reinterpret_cast<const unsigned short*>(patch + 0x10c);
        out->mapY = *reinterpret_cast<const unsigned short*>(patch + 0x10e);
        out->width = *reinterpret_cast<const unsigned short*>(patch + 0x110);
        out->height = *reinterpret_cast<const unsigned short*>(patch + 0x112);
        out->lodBand = *reinterpret_cast<const unsigned char*>(patch + 0x118);
        out->sharedIndexBuffer = *reinterpret_cast<const bool*>(patch + 0x119);
        out->waterOnly = *reinterpret_cast<const bool*>(patch + 0x11a);
        out->defaultSection = *reinterpret_cast<const bool*>(patch + 0x11b);
        for (unsigned i = 0; i < 4; ++i) {
            if (*reinterpret_cast<const DWORD*>(patch + 0x0d4 + i * 4)) {
                ++out->bridgeCount;
            }
        }
        out->safe = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        out->safe = false;
    }
}

static bool __fastcall LandscapeBackgroundPatchRender_Detour(
    void* pPatch, void* /*edx*/, bool renderFlag) {
    const bool result = LandscapeBackgroundPatchRender_Original(pPatch, renderFlag);
    LandscapeBackgroundRenderProbeSnapshot snap;
    CaptureLandscapeBackgroundRenderProbe(pPatch, &snap);

    unsigned long long signature = static_cast<unsigned long long>(snap.texture);
    signature ^= static_cast<unsigned long long>(snap.d3dTexture) << 32;
    signature ^= static_cast<unsigned long long>(snap.vertexBuffer) << 7;
    signature ^= static_cast<unsigned long long>(snap.indexBuffer) << 19;
    signature ^= static_cast<unsigned long long>(snap.vertexCount) << 3;
    signature ^= static_cast<unsigned long long>(snap.polyCount) << 27;
    signature ^= static_cast<unsigned long long>(snap.lodBand) << 56;
    signature ^= static_cast<unsigned long long>(result ? 1 : 0) << 63;
    const auto previous = g_landscapeBackgroundRenderProbeStates.find(pPatch);
    if (g_landscapeBackgroundRenderProbeLogCount < 4096 &&
        (previous == g_landscapeBackgroundRenderProbeStates.end() || previous->second != signature)) {
        g_landscapeBackgroundRenderProbeStates[pPatch] = signature;
        ++g_landscapeBackgroundRenderProbeLogCount;
        std::stringstream ss;
        ss << "[LandscapeBackgroundRenderProbe] patch=0x" << std::hex
           << reinterpret_cast<DWORD>(pPatch)
           << " tex=0x" << snap.texture << " d3dTex=0x" << snap.d3dTexture
           << " ib=0x" << snap.indexBuffer << " d3dIB=0x" << snap.d3dIndexBuffer
           << " vb=0x" << snap.vertexBuffer << " d3dVB=0x" << snap.d3dVertexBuffer
           << std::dec << " texBytes=" << snap.textureBytes
           << " ibBytes=" << snap.indexBytes << " vbBytes=" << snap.vertexBytes
           << " vtype=" << snap.vertexType
           << " stride=" << static_cast<unsigned>(snap.vertexStride)
           << " map=(" << snap.mapX << ',' << snap.mapY << ')'
           << " size=(" << snap.width << ',' << snap.height << ')'
           << " z=(" << snap.minZ << ',' << snap.maxZ << ')'
           << " vertices=" << snap.vertexCount << " polys=" << snap.polyCount
           << " lod=" << static_cast<unsigned>(snap.lodBand)
           << " bridges=" << static_cast<unsigned>(snap.bridgeCount)
           << " sharedIB=" << (snap.sharedIndexBuffer ? 1 : 0)
           << " water=" << (snap.waterOnly ? 1 : 0)
           << " default=" << (snap.defaultSection ? 1 : 0)
           << " arg=" << (renderFlag ? 1 : 0)
           << " result=" << (result ? 1 : 0)
           << " safe=" << (snap.safe ? 1 : 0);
        LogToFile(ss.str());
    }
    return result;
}

static __declspec(naked) void LandscapeBackgroundPatchRender_NakedStub() {
    __asm { jmp LandscapeBackgroundPatchRender_Detour }
}

bool InstallLandscapeBackgroundRenderProbeHook() {
    if (g_landscapeBackgroundRenderProbeInstalled) return true;
    if (!g_fableBase) {
        LogToFile("!!! [LandscapeBackgroundRenderProbe] g_fableBase not initialized.");
        return false;
    }
    const DWORD target = ASLR<DWORD>(0x00BE6F70);
    const size_t prologueSize = 6; // 83 EC 4C | 56 | 8B F1: whole instructions
    const BYTE expected[6] = {0x83, 0xec, 0x4c, 0x56, 0x8b, 0xf1};
    if (memcmp(reinterpret_cast<const void*>(target), expected, sizeof(expected)) != 0) {
        LogToFile("!!! [LandscapeBackgroundRenderProbe] unexpected retail prologue; hook not installed.");
        return false;
    }
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) {
        LogToFile("!!! [LandscapeBackgroundRenderProbe] trampoline allocation failed.");
        return false;
    }
    memcpy(tramp, reinterpret_cast<const void*>(target), prologueSize);
    tramp[prologueSize] = 0xe9;
    const DWORD backRel = target + static_cast<DWORD>(prologueSize) -
                          reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    LandscapeBackgroundPatchRender_Original =
        reinterpret_cast<tLandscapeBackgroundPatchRender>(tramp);

    BYTE patch[6] = {0xe9, 0, 0, 0, 0, 0x90};
    const DWORD fwdRel = reinterpret_cast<DWORD>(&LandscapeBackgroundPatchRender_NakedStub) -
                          target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) {
        VirtualFree(tramp, 0, MEM_RELEASE);
        LandscapeBackgroundPatchRender_Original = nullptr;
        LogToFile("!!! [LandscapeBackgroundRenderProbe] VirtualProtect failed.");
        return false;
    }
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);
    g_landscapeBackgroundRenderProbeInstalled = true;
    std::stringstream ss;
    ss << "[LandscapeBackgroundRenderProbe] Hook installed at 0x" << std::hex << target
       << " (trampoline 0x" << reinterpret_cast<DWORD>(tramp) << ").";
    LogToFile(ss.str());
    return true;
}

// =====================================================================================
// ForgeFSE addition: AUTHORED LANDSCAPE FOREGROUND DRAW telemetry
// =====================================================================================
// The foreground path is distinct from CLandscapeBackgroundPatch::Render.  Each
// CEngineLandscapePatch owns a linked list of CLandscapeLayerMesh objects; this probe
// records that list at the exact draw entry without changing any game state.

tLandscapePatchRenderForeground LandscapePatchRenderForeground_Original = nullptr;
static bool g_landscapeForegroundRenderProbeInstalled = false;
static std::map<void*, unsigned long long> g_landscapeForegroundRenderProbeStates;
static unsigned long g_landscapeForegroundRenderProbeLogCount = 0;

struct LandscapeForegroundLayerProbeSnapshot {
    DWORD layer;
    long foregroundTextureIndex;
    long backgroundTextureIndex;
    long bumpMapTextureIndex;
    float selfIllumination;
    DWORD foregroundTexture;
    DWORD backgroundTexture;
    DWORD bumpMap;
    DWORD foregroundFrameData;
    DWORD backgroundFrameData;
    DWORD bumpFrameData;
    DWORD foregroundGraphicData;
    DWORD backgroundGraphicData;
    DWORD bumpGraphicData;
    DWORD foregroundTextureHandle;
    DWORD backgroundTextureHandle;
    DWORD bumpTextureHandle;
    DWORD mappingTextureHandle;
    long minTextureMipMap;
    long minBumpMapMipMap;
    long mappingDirection;
    long foregroundTextureMipmapLevel;
    long foregroundBumpmapTextureMipmapLevel;
    DWORD indexBuffer;
    DWORD d3dIndexBuffer;
    DWORD indexBytes;
    DWORD vertexBuffer;
    DWORD d3dVertexBuffer;
    DWORD vertexBytes;
    DWORD vertexType;
    unsigned char vertexStride;
    DWORD map;
    unsigned short vertexCount;
    unsigned short polyCount;
    bool usesSharedIndexBuffer;
    DWORD proceduralVertexBuffer;
    DWORD vertices;
};

struct LandscapeForegroundRenderProbeSnapshot {
    bool safe;
    unsigned char flags;
    unsigned short mapX;
    unsigned short mapY;
    float minX;
    float minY;
    float minZ;
    float maxX;
    float maxY;
    float maxZ;
    DWORD layerList;
    unsigned char layerCount;
    bool layerCycle;
    LandscapeForegroundLayerProbeSnapshot layers[8];
};

struct LandscapeVertexBufferReadbackProbe {
    bool safe;
    bool descValid;
    bool readAttempted;
    bool readValid;
    bool skippedWriteOnly;
    HRESULT descResult;
    HRESULT lockResult;
    DWORD usage;
    DWORD pool;
    DWORD size;
    DWORD fvf;
    unsigned short sampledVertices;
    unsigned long hash;
    unsigned short minX;
    unsigned short minY;
    unsigned short maxX;
    unsigned short maxY;
    float minZ;
    float maxZ;
    float minNormalZ;
    float maxNormalZ;
};

static void InspectLandscapeVertexBuffer(
    DWORD d3dVertexBuffer, unsigned short requestedVertices,
    LandscapeVertexBufferReadbackProbe* out) {
    memset(out, 0, sizeof(*out));
    out->descResult = E_FAIL;
    out->lockResult = E_NOTIMPL;
    out->minX = out->minY = 0xffff;
    out->minZ = out->minNormalZ = FLT_MAX;
    out->maxZ = out->maxNormalZ = -FLT_MAX;
    __try {
        IDirect3DVertexBuffer9* vb =
            reinterpret_cast<IDirect3DVertexBuffer9*>(d3dVertexBuffer);
        if (!vb) {
            out->safe = true;
            return;
        }
        D3DVERTEXBUFFER_DESC desc;
        memset(&desc, 0, sizeof(desc));
        out->descResult = vb->GetDesc(&desc);
        if (FAILED(out->descResult)) {
            out->safe = true;
            return;
        }
        out->descValid = true;
        out->usage = desc.Usage;
        out->pool = static_cast<DWORD>(desc.Pool);
        out->size = desc.Size;
        out->fvf = desc.FVF;

        // Microsoft documents D3DLOCK_READONLY for reading existing contents,
        // but explicitly says not to use it on D3DUSAGE_WRITEONLY buffers.
        if ((desc.Usage & D3DUSAGE_WRITEONLY) != 0) {
            out->skippedWriteOnly = true;
            out->safe = true;
            return;
        }

        out->readAttempted = true;
        void* data = nullptr;
        out->lockResult = vb->Lock(0, 0, &data, D3DLOCK_READONLY);
        if (FAILED(out->lockResult) || !data) {
            out->safe = true;
            return;
        }

        // PDB: CTVertexLandscapeForegroundBase is 24 bytes:
        // u16 X/Y, float Z, float NormalX/Y/Z, four blend bytes.
        const unsigned stride = 24;
        const unsigned available = desc.Size / stride;
        const unsigned count = requestedVertices < available ? requestedVertices : available;
        const unsigned char* bytes = reinterpret_cast<const unsigned char*>(data);
        unsigned long hash = 2166136261u; // FNV-1a, diagnostic identity only.
        for (unsigned i = 0; i < desc.Size; ++i) {
            hash ^= bytes[i];
            hash *= 16777619u;
        }
        out->hash = hash;
        for (unsigned i = 0; i < count; ++i) {
            const unsigned char* v = bytes + i * stride;
            unsigned short x, y;
            float z, nz;
            memcpy(&x, v + 0x00, sizeof(x));
            memcpy(&y, v + 0x02, sizeof(y));
            memcpy(&z, v + 0x04, sizeof(z));
            memcpy(&nz, v + 0x10, sizeof(nz));
            if (x < out->minX) out->minX = x;
            if (x > out->maxX) out->maxX = x;
            if (y < out->minY) out->minY = y;
            if (y > out->maxY) out->maxY = y;
            if (z < out->minZ) out->minZ = z;
            if (z > out->maxZ) out->maxZ = z;
            if (nz < out->minNormalZ) out->minNormalZ = nz;
            if (nz > out->maxNormalZ) out->maxNormalZ = nz;
        }
        out->sampledVertices = static_cast<unsigned short>(count);
        out->readValid = true;
        vb->Unlock();
        out->safe = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        out->safe = false;
    }
}

static void CaptureGraphicInfoFrameBinding(
    DWORD frame, DWORD* frameData, DWORD* graphicData, DWORD* textureHandle) {
    *frameData = 0;
    *graphicData = 0;
    *textureHandle = 0;
    if (!frame) return;

    // RenderForeground resolves a CGraphicInfoFrame through these exact links:
    // frame+0x08 -> CFrameData, CFrameData+0x1C -> CGraphicFrameData, then
    // CGraphicFrameData+0x08 -> the binding whose first word is passed to
    // IDirect3DDevice9::SetTexture.  Recording every link distinguishes a
    // merely allocated frame from a texture that can actually be submitted.
    *frameData = *reinterpret_cast<const DWORD*>(frame + 0x08);
    if (!*frameData) return;
    *graphicData = *reinterpret_cast<const DWORD*>(*frameData + 0x1c);
    if (!*graphicData) return;
    const DWORD binding = *reinterpret_cast<const DWORD*>(*graphicData + 0x08);
    if (!binding) return;
    *textureHandle = *reinterpret_cast<const DWORD*>(binding);
}

static DWORD CaptureLandscapeMappingTextureHandle(long mappingDirection) {
    if (mappingDirection < 0 || mappingDirection > 4 || !g_fableBase) return 0;
    // RenderForeground binds stage 0 from the five-entry lookup at
    // landscape-renderer+0x5BC, indexed by MappingDirection with 8-byte slots.
    const DWORD renderer = *ASLR<DWORD*>(0x01436EA8);
    if (!renderer) return 0;
    return *reinterpret_cast<const DWORD*>(
        renderer + 0x5bc + static_cast<DWORD>(mappingDirection) * 8);
}

static void CaptureLandscapeForegroundRenderProbe(
    void* pPatch, LandscapeForegroundRenderProbeSnapshot* out) {
    memset(out, 0, sizeof(*out));
    __try {
        const char* patch = reinterpret_cast<const char*>(pPatch);
        out->layerList = *reinterpret_cast<const DWORD*>(patch + 0x08);
        out->minX = *reinterpret_cast<const float*>(patch + 0x20);
        out->minY = *reinterpret_cast<const float*>(patch + 0x24);
        out->minZ = *reinterpret_cast<const float*>(patch + 0x28);
        out->maxX = *reinterpret_cast<const float*>(patch + 0x2c);
        out->maxY = *reinterpret_cast<const float*>(patch + 0x30);
        out->maxZ = *reinterpret_cast<const float*>(patch + 0x34);
        out->mapX = *reinterpret_cast<const unsigned short*>(patch + 0x38);
        out->mapY = *reinterpret_cast<const unsigned short*>(patch + 0x3a);
        out->flags = *reinterpret_cast<const unsigned char*>(patch + 0x3c);

        DWORD layer = out->layerList;
        DWORD visited[8] = {0};
        while (layer && out->layerCount < 8) {
            for (unsigned char i = 0; i < out->layerCount; ++i) {
                if (visited[i] == layer) {
                    out->layerCycle = true;
                    layer = 0;
                    break;
                }
            }
            if (!layer) break;
            const unsigned char index = out->layerCount;
            visited[index] = layer;
            LandscapeForegroundLayerProbeSnapshot& dst = out->layers[index];
            dst.layer = layer;
            dst.foregroundTextureIndex = *reinterpret_cast<const long*>(layer + 0x04);
            dst.backgroundTextureIndex = *reinterpret_cast<const long*>(layer + 0x08);
            dst.bumpMapTextureIndex = *reinterpret_cast<const long*>(layer + 0x0c);
            dst.selfIllumination = *reinterpret_cast<const float*>(layer + 0x10);
            dst.foregroundTexture = *reinterpret_cast<const DWORD*>(layer + 0x14);
            dst.backgroundTexture = *reinterpret_cast<const DWORD*>(layer + 0x18);
            dst.bumpMap = *reinterpret_cast<const DWORD*>(layer + 0x1c);
            CaptureGraphicInfoFrameBinding(
                dst.foregroundTexture, &dst.foregroundFrameData,
                &dst.foregroundGraphicData, &dst.foregroundTextureHandle);
            CaptureGraphicInfoFrameBinding(
                dst.backgroundTexture, &dst.backgroundFrameData,
                &dst.backgroundGraphicData, &dst.backgroundTextureHandle);
            CaptureGraphicInfoFrameBinding(
                dst.bumpMap, &dst.bumpFrameData,
                &dst.bumpGraphicData, &dst.bumpTextureHandle);
            dst.minTextureMipMap = *reinterpret_cast<const long*>(layer + 0x20);
            dst.minBumpMapMipMap = *reinterpret_cast<const long*>(layer + 0x24);
            dst.mappingDirection = *reinterpret_cast<const long*>(layer + 0x28);
            dst.mappingTextureHandle =
                CaptureLandscapeMappingTextureHandle(dst.mappingDirection);
            dst.foregroundTextureMipmapLevel = *reinterpret_cast<const long*>(layer + 0x2c);
            dst.foregroundBumpmapTextureMipmapLevel = *reinterpret_cast<const long*>(layer + 0x30);
            dst.indexBuffer = *reinterpret_cast<const DWORD*>(layer + 0x34);
            if (dst.indexBuffer) {
                dst.d3dIndexBuffer = *reinterpret_cast<const DWORD*>(dst.indexBuffer + 0x04);
                dst.indexBytes = *reinterpret_cast<const DWORD*>(dst.indexBuffer + 0x08);
            }
            dst.vertexBuffer = *reinterpret_cast<const DWORD*>(layer + 0x38);
            if (dst.vertexBuffer) {
                dst.d3dVertexBuffer = *reinterpret_cast<const DWORD*>(dst.vertexBuffer + 0x04);
                dst.vertexBytes = *reinterpret_cast<const DWORD*>(dst.vertexBuffer + 0x08);
                dst.vertexType = *reinterpret_cast<const DWORD*>(dst.vertexBuffer + 0x0c);
                dst.vertexStride = *reinterpret_cast<const unsigned char*>(dst.vertexBuffer + 0x10);
            }
            dst.map = *reinterpret_cast<const DWORD*>(layer + 0x40);
            dst.vertexCount = *reinterpret_cast<const unsigned short*>(layer + 0x44);
            dst.polyCount = *reinterpret_cast<const unsigned short*>(layer + 0x46);
            dst.usesSharedIndexBuffer = *reinterpret_cast<const bool*>(layer + 0x48);
            dst.proceduralVertexBuffer = *reinterpret_cast<const DWORD*>(layer + 0x4c);
            dst.vertices = *reinterpret_cast<const DWORD*>(layer + 0x50);
            ++out->layerCount;
            layer = *reinterpret_cast<const DWORD*>(layer + 0x3c);
        }
        if (layer) out->layerCycle = true; // More than eight layers is also noteworthy.
        out->safe = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        out->safe = false;
    }
}

static void __fastcall LandscapePatchRenderForeground_Detour(
    void* pPatch, void* /*edx*/) {
    LandscapeForegroundRenderProbeSnapshot before;
    CaptureLandscapeForegroundRenderProbe(pPatch, &before);
    LandscapePatchRenderForeground_Original(pPatch);

    unsigned long long signature = static_cast<unsigned long long>(before.flags);
    signature ^= static_cast<unsigned long long>(before.layerList) << 8;
    signature ^= static_cast<unsigned long long>(before.layerCount) << 40;
    signature ^= static_cast<unsigned long long>(before.safe ? 1 : 0) << 63;
    for (unsigned char i = 0; i < before.layerCount; ++i) {
        const LandscapeForegroundLayerProbeSnapshot& layer = before.layers[i];
        signature ^= static_cast<unsigned long long>(layer.layer) << ((i * 7) & 31);
        signature ^= static_cast<unsigned long long>(layer.d3dVertexBuffer) << ((i * 11) & 31);
        signature ^= static_cast<unsigned long long>(layer.d3dIndexBuffer) << ((i * 13) & 31);
        signature ^= static_cast<unsigned long long>(layer.vertexCount) << ((i * 5) & 47);
        signature ^= static_cast<unsigned long long>(layer.polyCount) << ((i * 3 + 17) & 47);
    }

    const auto previous = g_landscapeForegroundRenderProbeStates.find(pPatch);
    if (g_landscapeForegroundRenderProbeLogCount < 4096 &&
        (previous == g_landscapeForegroundRenderProbeStates.end() || previous->second != signature)) {
        g_landscapeForegroundRenderProbeStates[pPatch] = signature;
        ++g_landscapeForegroundRenderProbeLogCount;
        std::stringstream ss;
        ss << "[LandscapeForegroundRenderProbe] patch=0x" << std::hex
           << reinterpret_cast<DWORD>(pPatch) << " layerHead=0x" << before.layerList
           << std::dec << " map=(" << before.mapX << ',' << before.mapY << ')'
           << " bounds=(" << before.minX << ',' << before.minY << ',' << before.minZ
           << ")->(" << before.maxX << ',' << before.maxY << ',' << before.maxZ << ')'
           << " flags=0x" << std::hex << static_cast<unsigned>(before.flags) << std::dec
           << " layers=" << static_cast<unsigned>(before.layerCount)
           << " truncatedOrCycle=" << (before.layerCycle ? 1 : 0)
           << " safe=" << (before.safe ? 1 : 0);
        LogToFile(ss.str());

        for (unsigned char i = 0; i < before.layerCount; ++i) {
            const LandscapeForegroundLayerProbeSnapshot& layer = before.layers[i];
            const bool heightfieldBox =
                before.minX >= 2784.0f && before.maxX <= 2816.0f &&
                before.minY >= 2560.0f && before.maxY <= 2592.0f;
            LandscapeVertexBufferReadbackProbe readback;
            memset(&readback, 0, sizeof(readback));
            if (heightfieldBox) {
                InspectLandscapeVertexBuffer(
                    layer.d3dVertexBuffer, layer.vertexCount, &readback);
            }
            std::stringstream ls;
            ls << "[LandscapeForegroundLayerProbe] patch=0x" << std::hex
               << reinterpret_cast<DWORD>(pPatch) << " layer=0x" << layer.layer
               << std::dec << " ordinal=" << static_cast<unsigned>(i)
               << " texIdx=(" << layer.foregroundTextureIndex << ','
               << layer.backgroundTextureIndex << ',' << layer.bumpMapTextureIndex << ')'
               << " illum=" << layer.selfIllumination
               << std::hex << " frames=(0x" << layer.foregroundTexture << ",0x"
               << layer.backgroundTexture << ",0x" << layer.bumpMap << ')'
               << " frameData=(0x" << layer.foregroundFrameData << ",0x"
               << layer.backgroundFrameData << ",0x" << layer.bumpFrameData << ')'
               << " graphicData=(0x" << layer.foregroundGraphicData << ",0x"
               << layer.backgroundGraphicData << ",0x" << layer.bumpGraphicData << ')'
               << " textureHandles=(0x" << layer.foregroundTextureHandle << ",0x"
               << layer.backgroundTextureHandle << ",0x" << layer.bumpTextureHandle << ')'
               << " mappingTexture=0x" << layer.mappingTextureHandle
               << " ib=0x" << layer.indexBuffer << " d3dIB=0x" << layer.d3dIndexBuffer
               << " vb=0x" << layer.vertexBuffer << " d3dVB=0x" << layer.d3dVertexBuffer
               << " mapPtr=0x" << layer.map << std::dec
               << " ibBytes=" << layer.indexBytes << " vbBytes=" << layer.vertexBytes
               << " vtype=" << layer.vertexType
               << " stride=" << static_cast<unsigned>(layer.vertexStride)
               << " vertices=" << layer.vertexCount << " polys=" << layer.polyCount
               << " minMips=(" << layer.minTextureMipMap << ',' << layer.minBumpMapMipMap << ')'
               << " mapping=" << layer.mappingDirection
               << " fgMip=" << layer.foregroundTextureMipmapLevel
               << " bumpMip=" << layer.foregroundBumpmapTextureMipmapLevel
               << " sharedIB=" << (layer.usesSharedIndexBuffer ? 1 : 0)
               << std::hex << " procVB=0x" << layer.proceduralVertexBuffer
               << " cpuVertices=0x" << layer.vertices;
            if (heightfieldBox) {
                ls << " vbDescHr=0x" << std::hex
                   << static_cast<unsigned long>(readback.descResult)
                   << " vbUsage=0x" << readback.usage
                   << " vbPool=" << std::dec << readback.pool
                   << " vbDescBytes=" << readback.size
                   << " vbFVF=0x" << std::hex << readback.fvf
                   << " vbWriteOnly=" << std::dec
                   << (readback.skippedWriteOnly ? 1 : 0)
                   << " vbLockHr=0x" << std::hex
                   << static_cast<unsigned long>(readback.lockResult)
                   << " vbRead=" << std::dec << (readback.readValid ? 1 : 0)
                   << " vbSample=" << readback.sampledVertices
                   << " vbHash=0x" << std::hex << readback.hash << std::dec;
                if (readback.readValid) {
                    ls << " gpuXY=(" << readback.minX << ',' << readback.minY
                       << ")->(" << readback.maxX << ',' << readback.maxY << ')'
                       << " gpuZ=(" << readback.minZ << ',' << readback.maxZ << ')'
                       << " gpuNormalZ=(" << readback.minNormalZ << ','
                       << readback.maxNormalZ << ')';
                }
                ls << " vbReadSafe=" << (readback.safe ? 1 : 0);
            }
            LogToFile(ls.str());
        }
    }
}

static __declspec(naked) void LandscapePatchRenderForeground_NakedStub() {
    __asm { jmp LandscapePatchRenderForeground_Detour }
}

bool InstallLandscapeForegroundRenderProbeHook() {
    if (g_landscapeForegroundRenderProbeInstalled) return true;
    if (!g_fableBase) {
        LogToFile("!!! [LandscapeForegroundRenderProbe] g_fableBase not initialized.");
        return false;
    }
    const DWORD target = ASLR<DWORD>(0x00BF4570);
    const size_t prologueSize = 9; // 81 EC F0 02 00 00 | 55 | 8B E9
    const BYTE expected[9] = {0x81, 0xec, 0xf0, 0x02, 0x00, 0x00, 0x55, 0x8b, 0xe9};
    if (memcmp(reinterpret_cast<const void*>(target), expected, sizeof(expected)) != 0) {
        LogToFile("!!! [LandscapeForegroundRenderProbe] unexpected retail prologue; hook not installed.");
        return false;
    }
    BYTE* tramp = reinterpret_cast<BYTE*>(VirtualAlloc(
        nullptr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE));
    if (!tramp) {
        LogToFile("!!! [LandscapeForegroundRenderProbe] trampoline allocation failed.");
        return false;
    }
    memcpy(tramp, reinterpret_cast<const void*>(target), prologueSize);
    tramp[prologueSize] = 0xe9;
    const DWORD backRel = target + static_cast<DWORD>(prologueSize) -
                          reinterpret_cast<DWORD>(tramp + prologueSize) - 5;
    memcpy(tramp + prologueSize + 1, &backRel, 4);
    LandscapePatchRenderForeground_Original =
        reinterpret_cast<tLandscapePatchRenderForeground>(tramp);

    BYTE patch[9] = {0xe9, 0, 0, 0, 0, 0x90, 0x90, 0x90, 0x90};
    const DWORD fwdRel = reinterpret_cast<DWORD>(&LandscapePatchRenderForeground_NakedStub) -
                          target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), prologueSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) {
        VirtualFree(tramp, 0, MEM_RELEASE);
        LandscapePatchRenderForeground_Original = nullptr;
        LogToFile("!!! [LandscapeForegroundRenderProbe] VirtualProtect failed.");
        return false;
    }
    memcpy(reinterpret_cast<void*>(target), patch, prologueSize);
    VirtualProtect(reinterpret_cast<void*>(target), prologueSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), prologueSize);
    g_landscapeForegroundRenderProbeInstalled = true;
    std::stringstream ss;
    ss << "[LandscapeForegroundRenderProbe] Hook installed at 0x" << std::hex << target
       << " (trampoline 0x" << reinterpret_cast<DWORD>(tramp) << ").";
    LogToFile(ss.str());
    return true;
}

// =====================================================================================
// ForgeFSE addition: AUTHORED LANDSCAPE FOREGROUND DECODE telemetry
// =====================================================================================
// CLandscapeLayerMesh::LoadForeground @ retail 0x00BFE050 decodes each source
// vertex directly into a buffer returned by the game's existing Lock call. At
// 0x00BFE6B4 EDI points one-past the decoded 24-byte stream and [ESP+0x1C]
// still holds the CLandscapeLayerMesh. The original instructions then recover
// the layer and its CVertexBuffer before calling Unlock at 0x00BFE6BB.

static bool g_landscapeForegroundDecodeProbeInstalled = false;
static DWORD g_landscapeForegroundDecodeProbeReturn = 0;
static unsigned long g_landscapeForegroundDecodeProbeForgeLogCount = 0;
static unsigned long g_landscapeForegroundDecodeProbeRetailLogCount = 0;

struct LandscapeForegroundDecodedSnapshot {
    DWORD layer;
    DWORD map;
    long textureIndices[3];
    unsigned short vertexCount;
    unsigned short minX, minY, maxX, maxY;
    float minZ, maxZ;
    float minNx, minNy, minNz;
    float maxNx, maxNy, maxNz;
    float minNormalLength, maxNormalLength;
    unsigned char minCliffV, maxCliffV;
    unsigned char minCliffU, maxCliffU;
    unsigned char minBlend, maxBlend;
    float firstZ, firstNz, middleZ, middleNz, lastZ, lastNz;
    unsigned long hash;
    bool finite;
    bool forgeTest;
    bool targetCoordinates;
    bool donorCoordinates;
};

static bool InspectLandscapeForegroundDecodedVertices(
    const unsigned char* layer, const unsigned char* decodedEnd,
    LandscapeForegroundDecodedSnapshot* out) {
    memset(out, 0, sizeof(*out));
    __try {
        if (!layer || !decodedEnd) return false;
        const unsigned short vertexCount =
            *reinterpret_cast<const unsigned short*>(layer + 0x44);
        if (vertexCount == 0 || vertexCount > 4096) return false;
        const size_t byteCount = static_cast<size_t>(vertexCount) * 24;
        if (reinterpret_cast<DWORD>(decodedEnd) < byteCount) return false;
        const unsigned char* vertices = decodedEnd - byteCount;

        unsigned short minX = 0xffff, minY = 0xffff, maxX = 0, maxY = 0;
        float minZ = FLT_MAX, maxZ = -FLT_MAX;
        float minNx = FLT_MAX, minNy = FLT_MAX, minNz = FLT_MAX;
        float maxNx = -FLT_MAX, maxNy = -FLT_MAX, maxNz = -FLT_MAX;
        float minNormalLength = FLT_MAX, maxNormalLength = -FLT_MAX;
        unsigned char minCliffV = 0xff, maxCliffV = 0;
        unsigned char minCliffU = 0xff, maxCliffU = 0;
        unsigned char minBlend = 0xff, maxBlend = 0;
        unsigned long hash = 2166136261u;
        bool finite = true;
        for (size_t i = 0; i < byteCount; ++i) {
            hash ^= vertices[i];
            hash *= 16777619u;
        }
        for (unsigned i = 0; i < vertexCount; ++i) {
            const unsigned char* v = vertices + static_cast<size_t>(i) * 24;
            unsigned short x, y;
            float z, nx, ny, nz;
            memcpy(&x, v + 0x00, sizeof(x));
            memcpy(&y, v + 0x02, sizeof(y));
            memcpy(&z, v + 0x04, sizeof(z));
            memcpy(&nx, v + 0x08, sizeof(nx));
            memcpy(&ny, v + 0x0c, sizeof(ny));
            memcpy(&nz, v + 0x10, sizeof(nz));
            const unsigned char cliffV = v[0x14];
            const unsigned char cliffU = v[0x15];
            const unsigned char blend = v[0x16];
            finite = finite && _finite(z) && _finite(nx) && _finite(ny) && _finite(nz);
            if (x < minX) minX = x;
            if (x > maxX) maxX = x;
            if (y < minY) minY = y;
            if (y > maxY) maxY = y;
            if (z < minZ) minZ = z;
            if (z > maxZ) maxZ = z;
            if (nx < minNx) minNx = nx;
            if (nx > maxNx) maxNx = nx;
            if (ny < minNy) minNy = ny;
            if (ny > maxNy) maxNy = ny;
            if (nz < minNz) minNz = nz;
            if (nz > maxNz) maxNz = nz;
            const float normalLength = std::sqrt(nx * nx + ny * ny + nz * nz);
            if (normalLength < minNormalLength) minNormalLength = normalLength;
            if (normalLength > maxNormalLength) maxNormalLength = normalLength;
            if (cliffV < minCliffV) minCliffV = cliffV;
            if (cliffV > maxCliffV) maxCliffV = cliffV;
            if (cliffU < minCliffU) minCliffU = cliffU;
            if (cliffU > maxCliffU) maxCliffU = cliffU;
            if (blend < minBlend) minBlend = blend;
            if (blend > maxBlend) maxBlend = blend;
        }

        const unsigned mid = vertexCount / 2;
        const unsigned char* first = vertices;
        const unsigned char* middle = vertices + static_cast<size_t>(mid) * 24;
        const unsigned char* last = vertices + static_cast<size_t>(vertexCount - 1) * 24;
        out->layer = reinterpret_cast<DWORD>(layer);
        out->map = *reinterpret_cast<const DWORD*>(layer + 0x40);
        out->textureIndices[0] = *reinterpret_cast<const long*>(layer + 0x04);
        out->textureIndices[1] = *reinterpret_cast<const long*>(layer + 0x08);
        out->textureIndices[2] = *reinterpret_cast<const long*>(layer + 0x0c);
        out->vertexCount = vertexCount;
        out->minX = minX; out->minY = minY; out->maxX = maxX; out->maxY = maxY;
        out->minZ = minZ; out->maxZ = maxZ;
        out->minNx = minNx; out->minNy = minNy; out->minNz = minNz;
        out->maxNx = maxNx; out->maxNy = maxNy; out->maxNz = maxNz;
        out->minNormalLength = minNormalLength;
        out->maxNormalLength = maxNormalLength;
        out->minCliffV = minCliffV; out->maxCliffV = maxCliffV;
        out->minCliffU = minCliffU; out->maxCliffU = maxCliffU;
        out->minBlend = minBlend; out->maxBlend = maxBlend;
        memcpy(&out->firstZ, first + 0x04, sizeof(out->firstZ));
        memcpy(&out->firstNz, first + 0x10, sizeof(out->firstNz));
        memcpy(&out->middleZ, middle + 0x04, sizeof(out->middleZ));
        memcpy(&out->middleNz, middle + 0x10, sizeof(out->middleNz));
        memcpy(&out->lastZ, last + 0x04, sizeof(out->lastZ));
        memcpy(&out->lastNz, last + 0x10, sizeof(out->lastNz));
        out->hash = hash;
        out->finite = finite;
        out->targetCoordinates = minX >= 2784 && maxX <= 2816 &&
                                 minY >= 2560 && maxY <= 2592;
        out->donorCoordinates = minX >= 2816 && maxX <= 2848 &&
                                minY >= 2368 && maxY <= 2400;
        out->forgeTest = out->map != 0 && out->map == g_forgeTestLandscapeMap;
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

static void __cdecl CaptureLandscapeForegroundDecodedVertices(
    const unsigned char* layer, const unsigned char* decodedEnd) {
    LandscapeForegroundDecodedSnapshot snap;
    if (!InspectLandscapeForegroundDecodedVertices(layer, decodedEnd, &snap)) return;
    if (snap.forgeTest) {
        if (g_landscapeForegroundDecodeProbeForgeLogCount >= 256) return;
        ++g_landscapeForegroundDecodeProbeForgeLogCount;
    } else {
        // A small control set from the ordinary terrain loaded before/around
        // ForgeTest makes the same run useful for decode-range comparison.
        if (g_landscapeForegroundDecodeProbeRetailLogCount >= 8) return;
        ++g_landscapeForegroundDecodeProbeRetailLogCount;
    }
    std::stringstream ss;
    ss << "[LandscapeForegroundDecodeProbe] scope="
       << (snap.forgeTest ? "ForgeTest" : "retail-control")
       << " layer=0x" << std::hex << snap.layer
       << " map=0x" << snap.map
       << " texIdx=(" << std::dec << snap.textureIndices[0] << ','
       << snap.textureIndices[1] << ',' << snap.textureIndices[2] << ')'
       << " vertices=" << snap.vertexCount
       << " xy=(" << snap.minX << ',' << snap.minY << ")->("
       << snap.maxX << ',' << snap.maxY << ')'
       << " coordScope=" << (snap.targetCoordinates ? "target" :
                              (snap.donorCoordinates ? "donor" : "other"))
       << " z=(" << snap.minZ << ',' << snap.maxZ << ')'
       << " normalX=(" << snap.minNx << ',' << snap.maxNx << ')'
       << " normalY=(" << snap.minNy << ',' << snap.maxNy << ')'
       << " normalZ=(" << snap.minNz << ',' << snap.maxNz << ')'
       << " normalLength=(" << snap.minNormalLength << ',' << snap.maxNormalLength << ')'
       << " cliffV=(" << static_cast<unsigned>(snap.minCliffV) << ','
       << static_cast<unsigned>(snap.maxCliffV) << ')'
       << " cliffU=(" << static_cast<unsigned>(snap.minCliffU) << ','
       << static_cast<unsigned>(snap.maxCliffU) << ')'
       << " blend=(" << static_cast<unsigned>(snap.minBlend) << ','
       << static_cast<unsigned>(snap.maxBlend) << ')'
       << " samplesZ/Nz=(" << snap.firstZ << '/' << snap.firstNz << ','
       << snap.middleZ << '/' << snap.middleNz << ',' << snap.lastZ << '/' << snap.lastNz << ')'
       << " finite=" << (snap.finite ? 1 : 0)
       << " hash=0x" << std::hex << snap.hash;
    LogToFile(ss.str());
}

static __declspec(naked) void LandscapeForegroundDecode_NakedStub() {
    __asm {
        pushfd
        pushad
        // After PUSHFD/PUSHAD, saved EDI is [ESP] and the original hook-site
        // stack begins at ESP+0x24. The layer local was [original ESP+0x1C].
        mov eax, [esp + 40h]
        mov edx, [esp]
        push edx
        push eax
        call CaptureLandscapeForegroundDecodedVertices
        add esp, 8
        popad
        popfd

        // Exact seven bytes displaced from retail 0x00BFE6B4.
        mov esi, [esp + 1ch]
        mov ecx, [esi + 38h]
        jmp dword ptr [g_landscapeForegroundDecodeProbeReturn]
    }
}

bool InstallLandscapeForegroundDecodeProbeHook() {
    if (g_landscapeForegroundDecodeProbeInstalled) return true;
    if (!g_fableBase) {
        LogToFile("!!! [LandscapeForegroundDecodeProbe] g_fableBase not initialized.");
        return false;
    }
    const DWORD target = ASLR<DWORD>(0x00BFE6B4);
    const size_t patchSize = 7;
    const BYTE expected[7] = {0x8b, 0x74, 0x24, 0x1c, 0x8b, 0x4e, 0x38};
    if (memcmp(reinterpret_cast<const void*>(target), expected, sizeof(expected)) != 0) {
        LogToFile("!!! [LandscapeForegroundDecodeProbe] unexpected retail bytes; hook not installed.");
        return false;
    }

    BYTE patch[7] = {0xe9, 0, 0, 0, 0, 0x90, 0x90};
    const DWORD fwdRel = reinterpret_cast<DWORD>(&LandscapeForegroundDecode_NakedStub) -
                         target - 5;
    memcpy(patch + 1, &fwdRel, 4);
    g_landscapeForegroundDecodeProbeReturn = target + static_cast<DWORD>(patchSize);
    DWORD oldProt = 0;
    if (!VirtualProtect(reinterpret_cast<void*>(target), patchSize,
                        PAGE_EXECUTE_READWRITE, &oldProt)) {
        LogToFile("!!! [LandscapeForegroundDecodeProbe] VirtualProtect failed.");
        return false;
    }
    memcpy(reinterpret_cast<void*>(target), patch, patchSize);
    VirtualProtect(reinterpret_cast<void*>(target), patchSize, oldProt, &oldProt);
    FlushInstructionCache(GetCurrentProcess(), reinterpret_cast<void*>(target), patchSize);

    g_landscapeForegroundDecodeProbeInstalled = true;
    std::stringstream ss;
    ss << "[LandscapeForegroundDecodeProbe] Hook installed at 0x" << std::hex << target
       << " (return 0x" << g_landscapeForegroundDecodeProbeReturn << ").";
    LogToFile(ss.str());
    return true;
}
