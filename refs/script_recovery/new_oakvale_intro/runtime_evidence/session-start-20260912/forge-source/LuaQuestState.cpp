#include "LuaQuestState.h"
#include "LuaQuestHost.h"
#include "GameInterface.h"
#include "LuaEntityHost.h"
#include "sol/sol.hpp" 
#include <vector>
#include "EntityScriptingAPI.h"
#include "FableAPI.h"
#include "MasterQuest.h"
#include "LuaManager.h"
#include <string>
#include <set>
#include <sstream>

// Ported from upstream FSE (eeeeeAeoN/FableScriptExtender @ master, 2026-09-02).
// This fork's own bindings take std::shared_ptr<CScriptThing>& directly; the ported
// upstream bodies accept a raw sol::object, so they need this coercion.
static CScriptThing* GetScriptThingFromSolObject(sol::object obj) {
    if (obj.is<std::shared_ptr<CScriptThing>>()) {
        return obj.as<std::shared_ptr<CScriptThing>>().get();
    }
    else if (obj.is<CScriptThing*>()) {
        return obj.as<CScriptThing*>();
    }
    return nullptr;
}

void LuaQuestState::LogReceivedPointer(CScriptThing* pThing, const std::string& context) {
    LogToFile("===== LogReceivedPointer START =====");

    std::stringstream ss;
    ss << "    Context: " << context;
    ss << ", RECEIVED pThing address: 0x" << std::hex << reinterpret_cast<uintptr_t>(pThing);
    LogToFile(ss.str());

    LogToFile("===== LogReceivedPointer END =====");
}

struct CWideString {
    void* PStringData;
};

static_assert(sizeof(CWideString) == 4, "Retail CWideString must remain a single 32-bit pointer");

namespace {
    typedef CWideString* (__thiscall* tRetailWideStringCtor)(CWideString* self, const wchar_t* text);
    typedef void (__thiscall* tRetailWideStringDtor)(CWideString* self);

    bool Utf8ToWide(const std::string& input, std::wstring& output) {
        if (input.empty()) {
            output.clear();
            return true;
        }

        const int required = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
            input.data(), static_cast<int>(input.size()), nullptr, 0);
        if (required <= 0) return false;

        output.resize(required);
        return MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS,
            input.data(), static_cast<int>(input.size()), &output[0], required) == required;
    }

    class RetailWideString {
    public:
        RetailWideString() : m_value{}, m_dtor(nullptr), m_constructed(false) {}
        ~RetailWideString() {
            if (m_constructed && m_dtor) m_dtor(&m_value);
        }

        bool Construct(const std::wstring& text, tRetailWideStringCtor ctor, tRetailWideStringDtor dtor) {
            if (!ctor || !dtor) return false;
            ctor(&m_value, text.c_str());
            m_dtor = dtor;
            m_constructed = true;
            return true;
        }

        const CWideString* Get() const { return &m_value; }

    private:
        RetailWideString(const RetailWideString&);
        RetailWideString& operator=(const RetailWideString&);
        CWideString m_value;
        tRetailWideStringDtor m_dtor;
        bool m_constructed;
    };
}

struct CRGBFloatColour {
    float B, G, R, A;
};

static CRGBFloatColour ParseFloatColorTable(sol::table colorTable) {
    sol::optional<float> optR = colorTable["R"];
    sol::optional<float> optG = colorTable["G"];
    sol::optional<float> optB = colorTable["B"];
    sol::optional<float> optA = colorTable["A"];
    return {
        optB.value_or(1.0f), // Field 1: B
        optG.value_or(1.0f), // Field 2: G
        optR.value_or(1.0f), // Field 3: R
        optA.value_or(1.0f)  // Field 4: A
    };
}

static CRGBColour ParseColorTable(sol::table colorTable) {
    auto component = [&colorTable](const char* upper, const char* lower, const char* longName, unsigned char fallback) {
        sol::optional<unsigned char> value = colorTable[upper];
        if (!value) value = colorTable[lower];
        if (!value && longName) value = colorTable[longName];
        return value.value_or(fallback);
    };
    const unsigned char r = component("R", "r", "red", 255);
    const unsigned char g = component("G", "g", "green", 255);
    const unsigned char b = component("B", "b", "blue", 255);
    const unsigned char a = component("A", "a", "alpha", 255);
    // CRGBColour is BGRA in the retail ABI.  The PDB-derived CRGBColour_u_0_s_0 layout and
    // the native quest-info-bar colour writes independently place B/G/R/A at offsets 0/1/2/3.
    return { b, g, r, a };
}

static void LogRetailCutsceneState(const std::string& phase, const std::string& cutsceneName) {
    const auto* const guiSingletonSlot = ASLR<uintptr_t*>(0x013B8790);
    const uintptr_t gui = guiSingletonSlot ? *guiSingletonSlot : 0;
    const auto* const actionMode = ASLR<unsigned char*>(0x0143E8F4);

    std::ostringstream ss;
    ss << "    [CutsceneState] " << phase << " '" << cutsceneName << "'"
       << " gui=0x" << std::hex << gui;
    if (gui) {
        const auto* const bytes = reinterpret_cast<const unsigned char*>(gui);
        ss << std::dec
           << " actionMode=" << static_cast<unsigned int>(actionMode ? *actionMode : 0)
           << " guiActionMode=" << static_cast<unsigned int>(bytes[0xF4])
           << " inCutscene=" << static_cast<unsigned int>(bytes[0xF6])
           << " skippable=" << static_cast<unsigned int>(bytes[0xF8])
           << " skippableWhilePaused=" << static_cast<unsigned int>(bytes[0xF9])
           << " alternateSkipInput=" << static_cast<unsigned int>(bytes[0xFA]);
    }
    LogToFile(ss.str());
}

static sol::table ConvertGossipMapToLua(const std::map<CCharString, bool>* pMap, sol::state_view lua) {
    sol::table resultTable = lua.create_table();
    if (!pMap || !CCharString_ToConstChar_API) {
        // Log("!!! ERROR: ConvertGossipMapToLua - pMap or CCharString_ToConstChar_API is null!");
        return resultTable;
    }
    for (const auto& pair : *pMap) {
        const char* keyText = CCharString_ToConstChar_API(&pair.first);
        if (keyText) {
            resultTable[std::string(keyText)] = pair.second;
        }
        // Do NOT destroy pair.first, it is const and owned by the map
    }
    return resultTable;
}

static sol::table ConvertGossipVectorToLua(const std::vector<CCharString>* pVec, sol::state_view lua) {
    sol::table resultTable = lua.create_table();
    if (!pVec || !CCharString_ToConstChar_API) {
        // Log("!!! ERROR: ConvertGossipVectorToLua - pVec or CCharString_ToConstChar_API is null!");
        return resultTable;
    }
    int luaTableIndex = 1;
    for (const auto& str : *pVec) {
        const char* text = CCharString_ToConstChar_API(&str);
        if (text) {
            resultTable[luaTableIndex++] = std::string(text);
        }
        // Do NOT destroy str, it is const and owned by the vector
    }
    return resultTable;
}

LuaQuestState::LuaQuestState(LuaQuestHost* parent, CGameScriptInterfaceBase* gameInterface)
    : m_pParentHost(parent), m_pGameInterface(gameInterface) {
    LogToFile("    LuaQuestState object created.");
}

void LuaQuestState::Log(const std::string& message) {
    if (m_pParentHost) {
        std::string full_message = "[" + m_pParentHost->GetScriptName() + "] " + message;
        LogToFile(full_message);
    }
}

sol::table LuaQuestState::GetFSEInfo(sol::this_state s) {
    sol::state_view lua(s);
    sol::table result = lua.create_table();
    const FSEDiagnostics diagnostics = GetFSEDiagnostics();
    result["fseVersion"] = diagnostics.fseVersion;
    result["apiVersion"] = diagnostics.apiVersion;
    result["manifestVersion"] = diagnostics.manifestVersion;
    result["buildId"] = diagnostics.buildId;
    result["loadCount"] = diagnostics.loadCount;
    result["questDefinitionCount"] = diagnostics.questDefinitionCount;
    result["registeredQuestCount"] = diagnostics.registeredQuestCount;
    result["entityAllocatorScriptCount"] = diagnostics.entityAllocatorScriptCount;
    result["manifestFunctionCount"] = diagnostics.manifestFunctionCount;
    result["scriptsRegisteredWithGame"] = diagnostics.scriptsRegisteredWithGame;

    sol::table capabilities = lua.create_table();
    capabilities["api_manifest_v1"] = true;
    capabilities["get_fse_info_v2"] = true;
    capabilities["thread_regions_v1"] = true;
    capabilities["async_actions_v1"] = true;
    capabilities["nonblocking_actions_v1"] = true;
    result["capabilities"] = capabilities;

    sol::table limits = lua.create_table();
    limits["maxQuestScripts"] = static_cast<int>(diagnostics.maxQuestScripts);
    limits["maxEntityScripts"] = static_cast<int>(diagnostics.maxEntityScripts);
    limits["maxTotalScripts"] = static_cast<int>(diagnostics.maxTotalScripts);
    result["limits"] = limits;

    sol::table gameCompatibility = lua.create_table();
    gameCompatibility["status"] = diagnostics.gameCompatibilityStatus;
    gameCompatibility["detectedBuild"] = "steam-retail";
    gameCompatibility["message"] = diagnostics.gameCompatibilityMessage;
    gameCompatibility["compatible"] = diagnostics.gameBuildCompatible;
    result["gameCompatibility"] = gameCompatibility;

    return result;
}

void LuaQuestState::CreateThread(const std::string& luaFunctionName, sol::table args) {
    if (!m_pParentHost) {
        return;
    }

    std::string regionName = "Class"; 
    std::vector<sol::object> threadArgsVec;

    if (args.valid() && args.get_type() == sol::type::table) {

        regionName = args.get_or("region", std::string("Class"));

        sol::object argsForThreadObj = args["args"];
        if (argsForThreadObj.valid() && argsForThreadObj.get_type() != sol::type::lua_nil) {
            if (argsForThreadObj.is<sol::table>()) {
                sol::table threadArgsTable = argsForThreadObj.as<sol::table>();
                for (size_t i = 1; i <= threadArgsTable.size(); ++i) {
                    threadArgsVec.push_back(threadArgsTable[i]);
                }
            }
            else {
                threadArgsVec.push_back(argsForThreadObj);
            }
        }
    }
    m_pParentHost->CreateThread(luaFunctionName, regionName, threadArgsVec);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateEffectOnThing(const std::string& effectName, CScriptThing* pTarget, const std::string& boneName,
    sol::optional<bool> independent, sol::optional<bool> alwaysUpdate) {
    LogToFile("===== CreateEffectOnThing START =====");

    std::stringstream ss;
    ss << "    RECEIVED pTarget address: 0x" << std::hex << reinterpret_cast<uintptr_t>(pTarget);
    LogToFile(ss.str());

    if (!m_pGameInterface || !CreateEffect_OnThing_API || !pTarget || !Game_malloc) { // <-- Added Game_malloc check
        LogToFile("!!! ERROR: CreateEffectOnThing prerequisites failed.");
        LogToFile("===== CreateEffectOnThing END (Failure) =====");
        return nullptr; // <-- Return nullptr
    }

    FableString fsEffect(effectName.c_str());
    FableString fsBone(boneName.c_str());
    FableString fsScript("");

    bool finalIndependent = independent.value_or(false);
    bool finalAlwaysUpdate = alwaysUpdate.value_or(false);

    CScriptThing* pResultEffect = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultEffect) {
        LogToFile("!!! ERROR: Failed to allocate memory for result effect buffer!");
        LogToFile("===== CreateEffectOnThing END (Failure) =====");
        return nullptr; // <-- Return nullptr
    }
    memset(pResultEffect, 0, sizeof(CScriptThing));

    LogToFile("    Calling CreateEffect_OnThing_API...");
    try {
        CreateEffect_OnThing_API(m_pGameInterface, pResultEffect, fsEffect, pTarget, fsBone, fsScript,
            finalIndependent, finalAlwaysUpdate);
        LogToFile("    API call completed successfully.");
    }
    catch (...) {
        LogToFile("!!! CATASTROPHIC FAILURE: Exception caught during CreateEffect_OnThing_API call!");
        Game_free(pResultEffect); // Free the buffer on exception
        return nullptr; // <-- Return nullptr
    }

    LogToFile("===== CreateEffectOnThing END (Success, returning handle) =====");
    return WrapScriptThingOutput(pResultEffect);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateEffectAtPos(const std::string& effectName, sol::table position,
    sol::optional<float> angle, sol::optional<bool> independent, sol::optional<bool> alwaysUpdate) {

    if (!m_pGameInterface || !CreateEffect_AtPosition_API || !Game_malloc) {
        Log("!!! ERROR: CreateEffectAtPos - API function or Game_malloc not available!");
        return nullptr;
    }

    FableString fsEffect(effectName.c_str());
    FableString fsScript("");

    // --- THIS IS THE FIX ---
    // Added "const C3DVector" to define the variable.
    const C3DVector pos = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };
    // --- END FIX ---

    float finalAngle = angle.value_or(0.0f);
    bool finalIndependent = independent.value_or(false);
    bool finalAlwaysUpdate = alwaysUpdate.value_or(false);

    CScriptThing* pResultEffect = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (pResultEffect) {
        memset(pResultEffect, 0, sizeof(CScriptThing));

        // The API call is now safe because &pos is a "const C3DVector*"
        CreateEffect_AtPosition_API(m_pGameInterface, pResultEffect, fsEffect, &pos, fsScript,
            finalAngle, finalIndependent, finalAlwaysUpdate);

        return WrapScriptThingOutput(pResultEffect);
    }
    else {
        Log("!!! ERROR: CreateEffectAtPos - Failed to allocate memory for result CScriptThing!");
        return nullptr;
    }
}

void LuaQuestState::ShowMessage(const std::string& message, float duration) {
    if (m_pGameInterface && ShowMessageAndSuspend_API) {
        FableString fsMessage(message.c_str());
        ShowMessageAndSuspend_API(m_pGameInterface, fsMessage, duration);
    }
}

void LuaQuestState::AddQuestCard(const std::string& questCardObjectName, const std::string& questScriptName, bool replayable, bool skip_GM_message) {
    if (m_pGameInterface && AddQuestCard_API) {
        FableString fqn(questCardObjectName.c_str());
        FableString fqd(questScriptName.c_str());
        // Pass the correctly named parameters to the API function
        AddQuestCard_API(m_pGameInterface, fqn, fqd, replayable, skip_GM_message);
    }
}

void LuaQuestState::GiveQuestCardDirectly(const std::string& questCardObjectName, const std::string& questName, bool flag) {
    if (m_pGameInterface && GiveHeroQuestCardDirectly_API) {
        FableString cardObject(questCardObjectName.c_str());
        FableString scriptName(questName.c_str());
        GiveHeroQuestCardDirectly_API(m_pGameInterface, cardObject, scriptName, flag);
    }
}

void LuaQuestState::ShowMessageWithButtons(const std::string& message, const std::string& button1Response, const std::string& button2Response, float displayTime) {
    if (m_pGameInterface && ShowMessageWithButtons_API) {
        FableString fsMessage(message.c_str());
        FableString fsButton1(button1Response.c_str());
        FableString fsButton2(button2Response.c_str());

        // Add logging
        Log("Showing message with buttons: Msg='" + message + "', Btn1='" + button1Response + "', Btn2='" + button2Response + "', Time=" + std::to_string(displayTime));

        ShowMessageWithButtons_API(m_pGameInterface, fsMessage, fsButton1, fsButton2, displayTime);
    }
    else {
        Log("!!! ERROR: ShowMessageWithButtons - API function not available!");
    }
}

void LuaQuestState::DeactivateQuest(const std::string& questName, unsigned int delay) {
    if (m_pGameInterface && DeactivateQuest_API) {
        FableString fq(questName.c_str());
        DeactivateQuest_API(m_pGameInterface, fq, delay);
    }
}

std::shared_ptr<CScriptThing> LuaQuestState::GetHero() {
    if (!m_pGameInterface || !GetHero_API) {
        LogToFile("!!! ERROR: GetHero - API not available!");
        return nullptr;
    }

    // 1. Get the game's actual, internal hero handle pointer
    CScriptThing* pGameHero = GetHero_API(m_pGameInterface);
    if (!pGameHero) {
        LogToFile("!!! ERROR: GetHero - API returned null!");
        return nullptr;
    }

    // The returned handle is owned by the engine. Lua borrows it; adding a shared owner here
    // leaks one reference per frame in polling-heavy quest scripts until Lua garbage collection.
    return std::shared_ptr<CScriptThing>(pGameHero, [](CScriptThing*) {});
}

void LuaQuestState::AddQuestRegion(const std::string& questName, const std::string& regionName) { if (m_pGameInterface && AddQuestRegion_API) { FableString fq(questName.c_str()), fr(regionName.c_str()); AddQuestRegion_API(m_pGameInterface, fq, fr); } }

bool LuaQuestState::IsRegionLoaded(const std::string& regionName) { if (m_pGameInterface && IsRegionLoaded_API) { FableString fr(regionName.c_str()); return IsRegionLoaded_API(m_pGameInterface, fr); } return false; }

bool LuaQuestState::IsRegionDefLoaded(const std::string& regionDefName) {
    if (m_pGameInterface && IsRegionDefLoaded_API) {
        FableString fr(regionDefName.c_str());
        return IsRegionDefLoaded_API(m_pGameInterface, fr.get());
    }
    Log("!!! WARNING: IsRegionDefLoaded - API not available; returning false.");
    return false;
}

// ForgeFSE diagnostic bindings (2026-07-20): state readers so a Lua test can log exactly
// why the hero is frozen after arriving in a custom region. Each null-guards the resolved
// fn ptr and returns false on failure so a bad call can't crash.
// IsHeroControlledByPlayer @ CGSI vtable[394] (retail 0x0088E980) -> bool() const.
bool LuaQuestState::IsHeroControlledByPlayer() {
    if (m_pGameInterface && IsHeroControlledByPlayer_API) { return IsHeroControlledByPlayer_API(m_pGameInterface); }
    Log("!!! WARNING: IsHeroControlledByPlayer - API not available; returning false.");
    return false;
}
// IsEntityUnderScriptedControl @ CGSI vtable[9] (retail 0x008960C0) -> bool(CScriptThing const&) const.
bool LuaQuestState::IsEntityUnderScriptedControl(const std::shared_ptr<CScriptThing>& spThing) {
    if (m_pGameInterface && IsEntityUnderScriptedControl_API) {
        CScriptThing* pThing = spThing.get();
        if (pThing) { return IsEntityUnderScriptedControl_API(m_pGameInterface, pThing); }
        Log("!!! WARNING: IsEntityUnderScriptedControl - thing is null; returning false.");
    } else {
        Log("!!! WARNING: IsEntityUnderScriptedControl - API not available; returning false.");
    }
    return false;
}
// IsRegionLoadedAndPreloaded @ CGSI vtable[13] (retail 0x00890550) -> bool(CCharString const&) const.
bool LuaQuestState::IsRegionLoadedAndPreloaded(const std::string& regionName) {
    if (m_pGameInterface && IsRegionLoadedAndPreloaded_API) { FableString fr(regionName.c_str()); return IsRegionLoadedAndPreloaded_API(m_pGameInterface, fr); }
    Log("!!! WARNING: IsRegionLoadedAndPreloaded - API not available; returning false.");
    return false;
}

void LuaQuestState::ActivateQuest(const std::string& questName) { if (m_pGameInterface && ActivateQuest_API) { FableString fq(questName.c_str()); ActivateQuest_API(m_pGameInterface, fq); } }

void LuaQuestState::ActivateQuestWithoutLoadingResources(const std::string& questName) {
    if (!m_pGameInterface || !ActivateQuestWithoutLoadingResources_API) {
        Log("!!! ERROR: ActivateQuestWithoutLoadingResources - API not available!");
        return;
    }
    FableString fq(questName.c_str());
    ActivateQuestWithoutLoadingResources_API(m_pGameInterface, fq.get());
}

void LuaQuestState::ActivateMultipleQuestsWithoutLoadingResources(sol::table questNames) {
    if (!m_pGameInterface || !ActivateMultipleQuestsWithoutLoadingResources_API) {
        Log("!!! ERROR: ActivateMultipleQuestsWithoutLoadingResources - API not available!");
        return;
    }

    // Retail CArray<T> is three pointers: begin, end, allocation end. The quest manager
    // consumes the array synchronously. FableString owns each referenced string for the call.
    std::vector<std::unique_ptr<FableString>> owners;
    std::vector<CCharString> values;
    owners.reserve(questNames.size());
    values.reserve(questNames.size());
    // Read the Lua sequence by numeric index. lua_next iteration is not ordered, while the
    // source CArray preserves the quest order written by retail gameflow.
    const std::size_t count = questNames.size();
    for (std::size_t i = 1; i <= count; ++i) {
        sol::object value = questNames.get<sol::object>(i);
        if (!value.is<std::string>()) continue;
        owners.emplace_back(new FableString(value.as<std::string>().c_str()));
        values.push_back(owners.back()->m_charString);
    }
    CCharString* begin = values.empty() ? nullptr : values.data();
    struct RetailCArrayCCharString {
        CCharString* begin;
        CCharString* end;
        CCharString* allocationEnd;
    } array = { begin, begin ? begin + values.size() : nullptr,
                begin ? begin + values.capacity() : nullptr };
    ActivateMultipleQuestsWithoutLoadingResources_API(
        m_pGameInterface, reinterpret_cast<const CArray<CCharString>*>(&array));
}

void LuaQuestState::PrepareQuestsWhenFinalQuestIsActivated(const std::string& finalQuestName) {
    if (!m_pGameInterface || !PrepareQuestsWhenFinalQuestIsActivated_API) {
        Log("!!! ERROR: PrepareQuestsWhenFinalQuestIsActivated - API not available!");
        return;
    }
    FableString fq(finalQuestName.c_str());
    PrepareQuestsWhenFinalQuestIsActivated_API(m_pGameInterface, fq.get());
}

void LuaQuestState::PrepareQuestsWhenFinalQuestIsCompleted() {
    if (m_pGameInterface && PrepareQuestsWhenFinalQuestIsCompleted_API) {
        PrepareQuestsWhenFinalQuestIsCompleted_API(m_pGameInterface);
    } else {
        Log("!!! ERROR: PrepareQuestsWhenFinalQuestIsCompleted - API not available!");
    }
}

bool LuaQuestState::IsXbox() {
    return m_pGameInterface && IsXbox_API ? IsXbox_API(m_pGameInterface) : false;
}

void LuaQuestState::SetQuestGoldReward(const std::string& questName, int amount) { if (m_pGameInterface && SetQuestCardGoldReward_API) { FableString fq(questName.c_str()); SetQuestCardGoldReward_API(m_pGameInterface, fq, amount); } }

void LuaQuestState::SetQuestRenownReward(const std::string& questName, int amount) { if (m_pGameInterface && SetQuestCardRenownReward_API) { FableString fq(questName.c_str()); SetQuestCardRenownReward_API(m_pGameInterface, fq, amount); } }

void LuaQuestState::SetQuestCardObjective(const std::string& qn, const std::string& ot, const std::string& r1, const std::string& r2) { if (m_pGameInterface && SetQuestCardObjective_API) { FableString fqn(qn.c_str()), fot(ot.c_str()), fr1(r1.c_str()), fr2(r2.c_str()); SetQuestCardObjective_API(m_pGameInterface, fqn, fot, fr1, fr2); } }

void LuaQuestState::SetQuestWorldMapOffset(const std::string& questName, int x, int y) {
    if (m_pGameInterface && SetQuestWorldMapOffset_API) {
        FableString fsQuest(questName.c_str());
        C2DCoordI offset = { x, y };
        SetQuestWorldMapOffset_API(m_pGameInterface, fsQuest.get(), &offset);
    }
    else {
        Log("!!! ERROR: SetQuestWorldMapOffset - API function not available!");
    }
}

void LuaQuestState::HeroReceiveMessageFromGuildMaster(const std::string& messageKey, const std::string& graphicName, bool highPriority, bool alwaysPlay) {
    if (m_pGameInterface && HeroReceiveMessageFromGuildMaster_API) {
        FableString fsMessage(messageKey.c_str());
        FableString fsGraphic(graphicName.c_str());
        HeroReceiveMessageFromGuildMaster_API(m_pGameInterface, fsMessage.get(), fsGraphic.get(), highPriority, alwaysPlay); // Pass bools directly
    }
    else {
        Log("!!! ERROR: HeroReceiveMessageFromGuildMaster - API function not available!");
    }
}

bool LuaQuestState::IsLevelLoaded(const std::string& levelName) {
    if (m_pGameInterface && IsLevelLoaded_API) {
        FableString fsLevel(levelName.c_str());
        return IsLevelLoaded_API(m_pGameInterface, fsLevel.get());
    }
    Log("!!! ERROR: IsLevelLoaded - API function not available!");
    return false;
}

void LuaQuestState::OverrideMusic(int musicSetType, bool isCutscene, bool forcePlay) {
    if (m_pGameInterface && OverrideMusic_API) {
        // It casts the integer to the enum type here in C++
        OverrideMusic_API(m_pGameInterface, static_cast<EMusicSetType>(musicSetType), isCutscene, forcePlay);
    }
    else {
        Log("!!! ERROR: OverrideMusic - API function not available!");
    }
}

int LuaQuestState::RegisterTimer() {
    if (m_pGameInterface && RegisterTimer_API) {
        int timerId = RegisterTimer_API(m_pGameInterface);
        Log("Registered game timer with ID: " + std::to_string(timerId));
        return timerId;
    }
    Log("!!! ERROR: RegisterTimer - API function not available!");
    return -1; // Return an invalid ID on error
}

void LuaQuestState::SetTimer(int timerId, int timeValue) {
    if (m_pGameInterface && SetTimer_API) {
        SetTimer_API(m_pGameInterface, timerId, timeValue);
        // Log("Set game timer ID " + std::to_string(timerId) + " to value: " + std::to_string(timeValue)); // Optional logging
    }
    else {
        Log("!!! ERROR: SetTimer - API function not available!");
    }
}

int LuaQuestState::GetTimer(int timerId) {
    if (m_pGameInterface && GetTimer_API) {
        int value = GetTimer_API(m_pGameInterface, timerId);
        // Log("Got game timer ID " + std::to_string(timerId) + " value: " + std::to_string(value)); // Optional logging
        return value;
    }
    Log("!!! ERROR: GetTimer - API function not available!");
    return -1; // Return an invalid value on error
}

void LuaQuestState::KickOffQuestStartScreen(const std::string& questName, bool isStory, bool isGold) { if (m_pGameInterface && KickOffQuestStartScreen_API) { FableString fq(questName.c_str()); KickOffQuestStartScreen_API(m_pGameInterface, fq, isStory, isGold); } }

std::string LuaQuestState::GetActiveQuestName() { if (m_pGameInterface && GetActiveQuestName_API) { CCharString r = { 0 }; GetActiveQuestName_API(m_pGameInterface, &r); if (r.pStringData) { char* t = *(char**)r.pStringData; if (t) { std::string n(t); CCharString_Destroy(&r); return n; } } } return ""; }

void LuaQuestState::Pause(float duration) {
    if (m_pGameInterface && Pause_API) {
        Pause_API(m_pGameInterface, duration);
    }
}

void LuaQuestState::SetQuestAsFailed(const std::string& questName, bool showScreen, const std::string& wideMessage, bool useWideMessage) {
    // wideMessage parameter is ignored for now as the C++ code used an empty string.
    if (m_pGameInterface && SetQuestAsFailed_API) {
        FableString fsQuest(questName.c_str());

        // Create a CWideString on the stack representing an empty string.
        // Initialize its pointer to nullptr, mimicking an empty default state.
        CWideString cwsEmptyMessage = { nullptr };

        // The C++ called SetQuestAsFailed(..., &v23, 1). v23 was empty string, 1 means use it.
        // We pass the address of our empty CWideString and the useWideMessage flag (true).
        SetQuestAsFailed_API(m_pGameInterface, fsQuest.get(), showScreen, &cwsEmptyMessage, true);

        // No explicit destruction needed for cwsEmptyMessage as it's stack-allocated
        // and doesn't manage heap memory directly in this simple case.
    }
    else {
        Log("!!! ERROR: SetQuestAsFailed - API function not available!");
    }
}

void LuaQuestState::StopOverrideMusic(bool isCutscene) {
    if (m_pGameInterface && StopOverrideMusic_API) {
        StopOverrideMusic_API(m_pGameInterface, isCutscene);
    }
    else {
        Log("!!! ERROR: StopOverrideMusic - API function not available!");
    }
}

void LuaQuestState::ResetPlayerCreatureCombatMultiplier() {
    if (m_pGameInterface && ResetPlayerCreatureCombatMultiplier_API) {
        ResetPlayerCreatureCombatMultiplier_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ResetPlayerCreatureCombatMultiplier - API function not available!");
    }
}

void LuaQuestState::AddRumourCategory(const std::string& categoryName) {
    if (m_pGameInterface && AddRumourCategory_API) {
        FableString fsCategory(categoryName.c_str());
        AddRumourCategory_API(m_pGameInterface, fsCategory.get());
    }
    else {
        Log("!!! ERROR: AddRumourCategory - API function not available!");
    }
}

void LuaQuestState::AddNewRumourToCategory(const std::string& categoryName, const std::string& rumourTextKey) {
    if (m_pGameInterface && AddNewRumourToCategory_API) {
        FableString fsCategory(categoryName.c_str());
        FableString fsRumour(rumourTextKey.c_str());
        AddNewRumourToCategory_API(m_pGameInterface, fsCategory.get(), fsRumour.get());
    }
    else {
        Log("!!! ERROR: AddNewRumourToCategory - API function not available!");
    }
}

void LuaQuestState::AddGossipFactionToCategory(const std::string& categoryName, const std::string& factionName) {
    if (m_pGameInterface && AddGossipFactionToCategory_API) {
        FableString fsCategory(categoryName.c_str());
        FableString fsFaction(factionName.c_str());
        AddGossipFactionToCategory_API(m_pGameInterface, fsCategory.get(), fsFaction.get());
    }
    else {
        Log("!!! ERROR: AddGossipFactionToCategory - API function not available!");
    }
}

void LuaQuestState::EntitySetCutsceneBehaviour(CScriptThing* pThing, int behaviour) {
    if (m_pGameInterface && EntitySetCutsceneBehaviour_API && pThing) {
        EntitySetCutsceneBehaviour_API(m_pGameInterface, pThing, static_cast<ECutsceneBehaviour>(behaviour));
    }
    else {
        if (!pThing) Log("!!! ERROR: EntitySetCutsceneBehaviour - pThing is null!");
        else Log("!!! ERROR: EntitySetCutsceneBehaviour - API function not available!");
    }
}

void LuaQuestState::CameraDefault() {
    if (m_pGameInterface && CameraDefault_API) {
        CameraDefault_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: CameraDefault - API function not available!");
    }
}

std::shared_ptr<CScriptThing> LuaQuestState::GetNearestWithDefName(const std::shared_ptr<CScriptThing>& spRelativeTo, const std::string& defName) {
    if (!m_pGameInterface || !GetNearestWithDefName_API || !Game_malloc) {
        Log("!!! ERROR: GetNearestWithDefName - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pRelativeTo = spRelativeTo.get();
    if (!pRelativeTo) {
        Log("!!! ERROR: GetNearestWithDefName - pRelativeTo is null!");
        return nullptr;
    }

    FableString fsDefName(defName.c_str());

    // 1. Malloc buffer (Pattern from GetThingWithScriptName)
    CScriptThing* pHeapResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pHeapResult) {
        Log("!!! ERROR: GetNearestWithDefName - Failed to allocate heap buffer!");
        return nullptr;
    }
    memset(pHeapResult, 0, sizeof(CScriptThing));

    // 2. Call API (fills buffer)
    GetNearestWithDefName_API(m_pGameInterface, pHeapResult, pRelativeTo, fsDefName);

    // 3. Wrap, validate, and return the buffer (WrapScriptThingOutput handles frees on failure)
    return WrapScriptThingOutput(pHeapResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::GetHeroTargetedThing() {
    if (!m_pGameInterface || !GetHeroTargetedThing_API || !Game_malloc) {
        Log("!!! ERROR: GetHeroTargetedThing - API or memory allocator not available!");
        return nullptr;
    }

    // 1. Malloc buffer
    CScriptThing* pHeapResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pHeapResult) {
        Log("!!! ERROR: GetHeroTargetedThing - Failed to allocate heap buffer!");
        return nullptr;
    }
    memset(pHeapResult, 0, sizeof(CScriptThing));

    // 2. Call API (fills buffer)
    GetHeroTargetedThing_API(m_pGameInterface, pHeapResult);

    // 3. Wrap, validate, and return the buffer
    return WrapScriptThingOutput(pHeapResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::GetNearestWithScriptName(const std::shared_ptr<CScriptThing>& spRelativeTo, const std::string& scriptName) {
    if (!m_pGameInterface || !GetNearestWithScriptName_API || !Game_malloc) {
        Log("!!! ERROR: GetNearestWithScriptName - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pRelativeTo = spRelativeTo.get();
    if (!pRelativeTo) {
        Log("!!! ERROR: GetNearestWithScriptName - pRelativeTo is null!");
        return nullptr;
    }

    FableString fsName(scriptName.c_str());

    // 1. Malloc buffer
    CScriptThing* pHeapResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pHeapResult) {
        Log("!!! ERROR: GetNearestWithScriptName - Failed to allocate heap buffer!");
        return nullptr;
    }
    memset(pHeapResult, 0, sizeof(CScriptThing));

    // 2. Call API (fills buffer)
    GetNearestWithScriptName_API(m_pGameInterface, pHeapResult, pRelativeTo, fsName);

    // 3. Wrap, validate, and return
    return WrapScriptThingOutput(pHeapResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::GetFurthestWithScriptName(const std::shared_ptr<CScriptThing>& spRelativeTo, const std::string& scriptName) {
    if (!m_pGameInterface || !GetFurthestWithScriptName_API || !Game_malloc) {
        Log("!!! ERROR: GetFurthestWithScriptName - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pRelativeTo = spRelativeTo.get();
    if (!pRelativeTo) {
        Log("!!! ERROR: GetFurthestWithScriptName - pRelativeTo is null!");
        return nullptr;
    }

    FableString fsName(scriptName.c_str());

    // 1. Malloc buffer
    CScriptThing* pHeapResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pHeapResult) {
        Log("!!! ERROR: GetFurthestWithScriptName - Failed to allocate heap buffer!");
        return nullptr;
    }
    memset(pHeapResult, 0, sizeof(CScriptThing));

    // 2. Call API (fills buffer)
    GetFurthestWithScriptName_API(m_pGameInterface, pHeapResult, pRelativeTo, fsName);

    // 3. Wrap, validate, and return
    return WrapScriptThingOutput(pHeapResult);
}

sol::table LuaQuestState::GetAllThingsWithDefName(const std::string& defName, sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetAllThingsWithDefName_API || !Game_malloc) {
        Log("!!! ERROR: GetAllThingsWithDefName - API or memory allocator not available!");
        return resultTable;
    }

    FableString fsName(defName.c_str());
    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(10); // Reserve some space

    // Call API - Fills the vector with COPIES
    int count = GetAllThingsWithDefName_API(m_pGameInterface, fsName, &thingsVectorCopies);

    if (count > 0) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            // 1. Malloc a new buffer for this copy
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: GetAllThingsWithDefName - Failed to allocate heap buffer for vector item!");
                continue; // Skip this one
            }
            // 2. Copy the data from the vector into our heap buffer
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing));

            // 3. Wrap the heap buffer in our smart pointer
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);

            // 4. Add to table if valid
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
            // (WrapScriptThingOutput handles freeing pHeapCopy if it's invalid)
        }
    }
    return resultTable;
}

sol::table LuaQuestState::GetAllThingsWithScriptName(const std::string& scriptName, sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    // Check prerequisites
    if (!m_pGameInterface || !GetAllThingsWithScriptName_API || !Game_malloc) {
        Log("!!! ERROR: GetAllThingsWithScriptName prerequisites (API func or malloc) not available!");
        return resultTable;
    }

    FableString fsName(scriptName.c_str());
    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(10); // Reserve some space

    // Call game API - Fills thingsVectorCopies with COPIES
    int count = GetAllThingsWithScriptName_API(m_pGameInterface, fsName.get(), &thingsVectorCopies);
    Log("GetAllThingsWithScriptName: API returned " + std::to_string(count) + " copies for '" + scriptName + "'.");

    if (count > 0) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            // 1. Allocate a NEW stable copy on the heap
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: Failed to allocate heap memory for shared_ptr copy!");
                continue;
            }
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing)); // Copy the validated data

            // 2. Wrap, validate, and get shared_ptr (WrapScriptThingOutput handles frees on failure)
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);

            // 3. Add to table if valid
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
        }
    }
    return resultTable;
}

std::shared_ptr<CScriptThing> LuaQuestState::GetThingWithUID(const std::string& uid_str) { // Parameter changed
    if (!m_pGameInterface || !GetThingWithUID_API || !Game_malloc || !Game_free) {
        Log("!!! ERROR: GetThingWithUID - API function or memory allocator not available!");
        return nullptr;
    }

    // --- ADD THIS CONVERSION BLOCK ---
    unsigned __int64 uid = 0;
    try {
        // Convert the string to an unsigned long long (unsigned __int64)
        uid = std::stoull(uid_str);
    }
    catch (const std::exception& e) {
        Log("!!! ERROR: GetThingWithUID - Failed to convert UID string '" + uid_str + "' to number. " + e.what());
        return nullptr;
    }
    // --- END ADDITION ---


    // 1. Malloc buffer
    CScriptThing* pResultHeap = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultHeap) {
        Log("!!! ERROR: GetThingWithUID - Failed to allocate memory for result CScriptThing!");
        return nullptr;
    }
    memset(pResultHeap, 0, sizeof(CScriptThing));

    // Updated log to show both string and converted number
    Log("GetThingWithUID: Received UID string: \"" + uid_str + "\", converted to: " + std::to_string(uid) + ". Calling API...");

    // 2. Call the API function with the *converted* UID
    GetThingWithUID_API(m_pGameInterface, pResultHeap, uid);

    // 3. Wrap, validate, and return
    return WrapScriptThingOutput(pResultHeap);
}

bool LuaQuestState::IsThingWithThisUIDAlive(const std::string& uid_str) {
    if (!m_pGameInterface || !IsThingWithThisUIDAlive_API) {
        Log("!!! ERROR: IsThingWithThisUIDAlive - API function not available!");
        return false;
    }

    unsigned __int64 uid = 0;
    try {
        std::size_t parsed = 0;
        uid = std::stoull(uid_str, &parsed);
        if (parsed != uid_str.size()) {
            throw std::invalid_argument("trailing characters");
        }
    }
    catch (const std::exception& e) {
        Log("!!! ERROR: IsThingWithThisUIDAlive - Failed to convert UID string '" + uid_str + "' to number. " + e.what());
        return false;
    }

    return IsThingWithThisUIDAlive_API(m_pGameInterface, uid);
}

std::shared_ptr<CScriptThing> LuaQuestState::GetThingWithScriptNameAtRegion(const std::string& scriptName, const std::string& regionName) {
    if (!m_pGameInterface || !GetThingWithScriptName_ByNameAndRegion_API || !Game_malloc) {
        Log("!!! ERROR: GetThingWithScriptNameAtRegion - API or memory allocator not available!");
        return nullptr;
    }

    FableString fsScript(scriptName.c_str());
    FableString fsRegion(regionName.c_str());

    // 1. Malloc buffer
    CScriptThing* pHeapResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pHeapResult) {
        Log("!!! ERROR: GetThingWithScriptNameAtRegion - Failed to allocate memory!");
        return nullptr;
    }
    memset(pHeapResult, 0, sizeof(CScriptThing));

    // 2. Call API (fills buffer)
    GetThingWithScriptName_ByNameAndRegion_API(m_pGameInterface, pHeapResult, fsScript, fsRegion);

    // 3. Wrap, validate, and return
    return WrapScriptThingOutput(pHeapResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::GetFurthestWithDefName(const std::shared_ptr<CScriptThing>& spRelativeTo, const std::string& defName) {
    if (!m_pGameInterface || !GetFurthestWithDefName_API || !Game_malloc) {
        Log("!!! ERROR: GetFurthestWithDefName - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pRelativeTo = spRelativeTo.get();
    if (!pRelativeTo) {
        Log("!!! ERROR: GetFurthestWithDefName - pRelativeTo is null!");
        return nullptr;
    }

    FableString fsDefName(defName.c_str());

    // 1. Malloc buffer
    CScriptThing* pHeapResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pHeapResult) {
        Log("!!! ERROR: GetFurthestWithDefName - Failed to allocate heap buffer!");
        return nullptr;
    }
    memset(pHeapResult, 0, sizeof(CScriptThing));

    // 2. Call API (fills buffer)
    GetFurthestWithDefName_API(m_pGameInterface, pHeapResult, pRelativeTo, fsDefName);

    // 3. Wrap, validate, and return
    return WrapScriptThingOutput(pHeapResult);
}

void LuaQuestState::EntitySetInFaction(CScriptThing* pThing, const std::string& factionName) {
    if (!pThing) {
        Log("!!! ERROR: EntitySetInFaction - pThing is null!");
        return;
    }
    if (m_pGameInterface && EntitySetInFaction_API) {
        FableString fsFaction(factionName.c_str());
        EntitySetInFaction_API(m_pGameInterface, pThing, fsFaction.get());
    }
    else {
        Log("!!! ERROR: EntitySetInFaction - API function not available!");
    }
}

void LuaQuestState::SetQuestAsCompleted(const std::string& questName, bool showScreen, bool updateLogbook, bool showQuestCompleteScreen) {
    // --- DEBUG LOG 1 ---
    std::stringstream ss_pre;
    ss_pre << "    [C++] SetQuestAsCompleted: ENTER name='" << questName
           << "' flags=" << showScreen << "," << updateLogbook << "," << showQuestCompleteScreen
           << ". PActiveThread: 0x" << std::hex << (m_pParentHost ? m_pParentHost->base.PActiveThread : nullptr);
    LogToFile(ss_pre.str());
    // --- END DEBUG ---

    if (m_pGameInterface && SetQuestAsCompleted_API) {
        FableString fsQuest(questName.c_str());
        SetQuestAsCompleted_API(m_pGameInterface, fsQuest.get(), showScreen, updateLogbook, showQuestCompleteScreen);
    }
    else {
        Log("!!! ERROR: SetQuestAsCompleted - API function not available!");
    }

    // --- DEBUG LOG 2 ---
    std::stringstream ss_post;
    ss_post << "    [C++] SetQuestAsCompleted: EXIT. PActiveThread: 0x" << std::hex << (m_pParentHost ? m_pParentHost->base.PActiveThread : nullptr);
    LogToFile(ss_post.str());
    // --- END DEBUG ---
}

void LuaQuestState::DeactivateQuestLater(const std::string& questName, unsigned int delayFrames) {
    if (m_pGameInterface && DeactivateQuestLater_API) {
        FableString fsQuest(questName.c_str());
        DeactivateQuestLater_API(m_pGameInterface, fsQuest.get(), delayFrames); // C++ called with 0 delay
    }
    else {
        Log("!!! ERROR: DeactivateQuestLater - API function not available!");
    }
}

int LuaQuestState::EntityGetStockItemPrice(CScriptThing* pThing) {
    if (m_pGameInterface && EntityGetStockItemPrice_API && pThing) {
        return EntityGetStockItemPrice_API(m_pGameInterface, pThing);
    }
    if (!pThing) Log("!!! ERROR: EntityGetStockItemPrice - pThing is null!");
    else Log("!!! ERROR: EntityGetStockItemPrice - API function not available!");
    return -1; // Return an error indicator
}

void LuaQuestState::EntitySetStockItemPrice(CScriptThing* pThing, int price) {
    if (m_pGameInterface && EntitySetStockItemPrice_API && pThing) {
        EntitySetStockItemPrice_API(m_pGameInterface, pThing, price);
    }
    else {
        if (!pThing) Log("!!! ERROR: EntitySetStockItemPrice - pThing is null!");
        else Log("!!! ERROR: EntitySetStockItemPrice - API function not available!");
    }
}

void LuaQuestState::EntitySetAsForSale(CScriptThing* pThing, bool bForSale) {
    if (m_pGameInterface && EntitySetAsForSale_API && pThing) {
        EntitySetAsForSale_API(m_pGameInterface, pThing, bForSale);
    }
    else {
        if (!pThing) Log("!!! ERROR: EntitySetAsForSale - pThing is null!");
        else Log("!!! ERROR: EntitySetAsForSale - API function not available!");
    }
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateObjectWithRotation(const std::string& defName, sol::table position, float rotation, const std::string& scriptName) {
    if (m_pGameInterface && CreateObject_WithRotation_API && Game_malloc) {
        FableString fsDef(defName.c_str());
        FableString fsScript(scriptName.c_str());

        sol::optional<float> optX = position["x"];
        sol::optional<float> optY = position["y"];
        sol::optional<float> optZ = position["z"];
        C3DVector pos = { optX.value_or(0.0f), optY.value_or(0.0f), optZ.value_or(0.0f) };

        CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
        if (pResult) {
            memset(pResult, 0, sizeof(CScriptThing));
            CreateObject_WithRotation_API(m_pGameInterface, pResult, fsDef, &pos, rotation, fsScript);

            // Wrap, validate, and return. This will free pResult if it's invalid.
            return WrapScriptThingOutput(pResult);
        }
        else {
            Log("!!! ERROR: CreateObjectWithRotation - Failed to allocate memory for result CScriptThing!");
        }
    }
    else {
        Log("!!! ERROR: CreateObjectWithRotation - API function or Game_malloc not available!");
    }
    return nullptr;
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateObject(const std::string& defName, sol::table position, const std::string& scriptName) {
    if (m_pGameInterface && CreateObject_NoRotation_API && Game_malloc) {
        FableString fsDef(defName.c_str());
        FableString fsScript(scriptName.c_str());

        sol::optional<float> optX = position["x"];
        sol::optional<float> optY = position["y"];
        sol::optional<float> optZ = position["z"];
        C3DVector pos = { optX.value_or(0.0f), optY.value_or(0.0f), optZ.value_or(0.0f) };

        CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
        if (pResult) {
            memset(pResult, 0, sizeof(CScriptThing));
            CreateObject_NoRotation_API(m_pGameInterface, pResult, fsDef, &pos, fsScript);

            // Wrap, validate, and return.
            return WrapScriptThingOutput(pResult);
        }
        else {
            Log("!!! ERROR: CreateObject - Failed to allocate memory for result CScriptThing!");
        }
    }
    else {
        Log("!!! ERROR: CreateObject - API function or Game_malloc not available!");
    }
    return nullptr;
}

unsigned int LuaQuestState::PlaySoundOnThing(std::shared_ptr<CScriptThing> pThing, const std::string& soundName) {
    // Use .get() to get the raw pointer from the shared_ptr
    CScriptThing* pRawThing = pThing.get();

    if (m_pGameInterface && PlaySoundOnThing_API && pRawThing) {
        FableString fsSound(soundName.c_str());
        // Basic null check for the thing
        bool isNull = true;
        if (pRawThing->pImp.Data && pRawThing->pImp.Data->pVTable) {
            CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pRawThing->pImp.Data->pVTable);
            if (pVTable->IsNull) {
                isNull = pVTable->IsNull(reinterpret_cast<CGameScriptThing*>(pRawThing->pImp.Data));
            }
        }
        Log("Attempting to call PlaySoundOnThing_API for sound: " + soundName);
        if (!isNull) {
            // Pass the raw pointer to the game's API
            return PlaySoundOnThing_API(m_pGameInterface, pRawThing, fsSound);
        }
        else {
            Log("!!! ERROR: PlaySoundOnThing - pThing is null or invalid!");
            return 0; // 0 is often an invalid sound handle ID
        }
    }
    else {
        if (!pRawThing) Log("!!! ERROR: PlaySoundOnThing - pThing is null!");
        else Log("!!! ERROR: PlaySoundOnThing - API function not available!");
        return 0; // Return an invalid sound handle ID
    }
}

unsigned int LuaQuestState::Play2DSound(const std::string& soundName) {
    if (m_pGameInterface && Play2DSound_API) {
        FableString fsSound(soundName.c_str());
        Log("Playing 2D sound: " + soundName);
        return Play2DSound_API(m_pGameInterface, fsSound);
    }
    else {
        Log("!!! ERROR: Play2DSound - API function not available!");
        return 0; // Return an invalid sound handle ID
    }
}

unsigned int LuaQuestState::PlaySoundAtPos(sol::table position, const std::string& soundName) {
    if (m_pGameInterface && PlaySoundAtPos_API) {
        FableString fsSound(soundName.c_str());

        // Extract position safely
        sol::optional<float> optX = position["x"];
        sol::optional<float> optY = position["y"];
        sol::optional<float> optZ = position["z"];
        C3DVector pos = {
            optX ? *optX : 0.0f,
            optY ? *optY : 0.0f,
            optZ ? *optZ : 0.0f
        };

        Log("Playing sound '" + soundName + "' at pos: " + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ", " + std::to_string(pos.z));
        return PlaySoundAtPos_API(m_pGameInterface, &pos, fsSound);
    }
    else {
        Log("!!! ERROR: PlaySoundAtPos - API function not available!");
        return 0; // Return an invalid sound handle ID
    }
}

// ---------------------------------------------------------------------------
// PLAY-SOUND-BY-NUMERIC-ID isolation binding (ForgeFSE addition)
//
// PlaySoundIdOnThing(thing, id) fires bank sample `id` DIRECTLY on `thing`,
// bypassing the criteria->CRC->id resolver. It replicates
// CGameScriptInterface::PlaySoundOnThing @ retail 0x008a0a20 (decomp:
// ghidra_out/pson_full.c) but supplies the numeric id in place of the symbol
// lookup. Use it to isolate a silent swapped .lug clip:
//   * meow plays  -> clip + decode + AL pipeline are fine  => event ROUTING is the bug
//   * meow silent but a known-retail control id is audible -> CLIP-SPECIFIC AL failure
//   * both silent -> the play path / binding is wrong (not the clip)
// ---------------------------------------------------------------------------
unsigned int LuaQuestState::PlaySoundIdOnThing(std::shared_ptr<CScriptThing> pThing, int soundId) {
    CScriptThing* pRawThing = pThing.get();
    if (!pRawThing) { Log("!!! PlaySoundIdOnThing: pThing is null"); return 0; }
    if (!SoundPlayer_PlayIndexRange_API || !InterfaceMap_LowerBound_API) {
        Log("!!! PlaySoundIdOnThing: sound APIs not available"); return 0;
    }
    if (soundId <= 0) { Log("!!! PlaySoundIdOnThing: invalid id " + std::to_string(soundId)); return 0; }

    // pRawThing is the CScriptThing whose pVTable is the game's CScriptThing vtable.
    // Guard: valid vtable pointer.
    void** pThingVtbl = pRawThing->pVTable;
    if (!pThingVtbl) { Log("!!! PlaySoundIdOnThing: thing has no vtable"); return 0; }

    Log("PlaySoundIdOnThing: firing bank sample id " + std::to_string(soundId));

    // Step 1 (decomp): cVar2 = param_1->vtbl[75]()  (IsValid / not-null); if 0, bail.
    typedef char(__thiscall* tThing_IsValid)(void* pThing);
    tThing_IsValid pfnIsValid = reinterpret_cast<tThing_IsValid>(pThingVtbl[75]); // 300/4
    if (!pfnIsValid || pfnIsValid(pRawThing) == 0) {
        Log("!!! PlaySoundIdOnThing: thing IsValid()==false"); return 0;
    }

    // Step 2 (decomp): iface = param_1->vtbl[11]();  (returns the CTCBase interface owner)
    //                  if (!(*(uint*)(iface+0x20) & 0x10000)) bail;
    typedef int(__thiscall* tThing_GetInterfaceOwner)(void* pThing);
    tThing_GetInterfaceOwner pfnOwner = reinterpret_cast<tThing_GetInterfaceOwner>(pThingVtbl[11]); // 0x2c/4
    if (!pfnOwner) { Log("!!! PlaySoundIdOnThing: no interface-owner vfn"); return 0; }
    int iface = pfnOwner(pRawThing);
    if (!iface) { Log("!!! PlaySoundIdOnThing: interface owner null"); return 0; }
    if ((*(unsigned int*)(iface + 0x20) & 0x10000) == 0) {
        Log("!!! PlaySoundIdOnThing: thing has no sound-emitter interface flag (0x10000)"); return 0;
    }

    // Step 3 (decomp): it = LowerBound(iface+0x44, key=ETCInterfaceType 0x10);
    //   *it -> pair<key, CTCBase* soundPlayer>.  The value (soundPlayer) is at *(it+4).
    int key = 0x10; // ETCInterfaceType of the CTCSoundPlayer / sound-emitter component
    // Iterator is a std::_Vector_iterator (single pointer under the hood). Retail
    // LowerBound writes the iterator to *pOutIter and returns it too.
    void* pIterOut = nullptr;
    InterfaceMap_LowerBound_API(reinterpret_cast<void*>(iface + 0x44), &pIterOut, &key);
    // pIterOut now points at a pair<ETCInterfaceType, CTCBase*>; the map entry stride
    // is 8 (u32 key + ptr). The resolved CTCSoundPlayer* is the pair's value at +4.
    if (!pIterOut) { Log("!!! PlaySoundIdOnThing: LowerBound returned null iterator"); return 0; }
    // Match the decomp's clamp: if the found key != requested key, retail falls back to
    // end(); we conservatively require the key to match before dereferencing.
    int foundKey = *reinterpret_cast<int*>(pIterOut);
    if (foundKey != key) {
        Log("!!! PlaySoundIdOnThing: sound-emitter interface not present on thing (key " +
            std::to_string(foundKey) + " != 0x10)"); return 0;
    }
    void* pSoundPlayer = *reinterpret_cast<void**>(reinterpret_cast<char*>(pIterOut) + 4);
    if (!pSoundPlayer) { Log("!!! PlaySoundIdOnThing: resolved sound player null"); return 0; }

    // Step 4 (decomp): build CSoundPair{id,id} and CSoundInit, then
    //   FUN_0066b280(&pair, &init, 1,0,0)  with ECX = pSoundPlayer.
    long soundPair[2] = { (long)soundId, (long)soundId };
    // CSoundInit stack layout from the decomp (uStack_20..uStack_4, 8 dwords):
    //   +0x00 vol   = 1.0f (0x3f800000)
    //   +0x04 pitch = 1.0f (0x3f800000)
    //   +0x08 = 0
    //   +0x0C flags = 0x8010
    //   +0x10 = 1
    //   +0x14 = 1
    //   +0x18 = 0
    //   +0x1C handle = 0xffffffff
    unsigned int soundInit[8];
    soundInit[0] = 0x3f800000; // volume 1.0f
    soundInit[1] = 0x3f800000; // pitch  1.0f
    soundInit[2] = 0;
    soundInit[3] = 0x8010;     // flags
    soundInit[4] = 1;
    soundInit[5] = 1;
    soundInit[6] = 0;
    soundInit[7] = 0xffffffff; // handle = none yet

    SoundPlayer_PlayIndexRange_API(pSoundPlayer, soundPair, soundInit, 1, 0, 0);
    Log("PlaySoundIdOnThing: dispatched id " + std::to_string(soundId));
    return (unsigned int)soundId;
}

// Convenience: play a specific bank sample id on the HERO (self-contained isolation test).
unsigned int LuaQuestState::PlaySoundId(int soundId) {
    std::shared_ptr<CScriptThing> hero = GetHero();
    if (!hero) { Log("!!! PlaySoundId: GetHero() returned null"); return 0; }
    return PlaySoundIdOnThing(hero, soundId);
}

// Resolve a sound symbol/criteria string to its numeric bank id (for logging /
// picking a known-retail control id). Returns 0 if not found.
int LuaQuestState::GetSoundIdFromSymbol(const std::string& symbol) {
    if (!GetSoundIndexFromSymbol_API || !g_pMainGameComponent || !*g_pMainGameComponent) {
        Log("!!! GetSoundIdFromSymbol: API/global not available"); return 0;
    }
    void* pBank = *reinterpret_cast<void**>(reinterpret_cast<char*>(*g_pMainGameComponent) + 0x10);
    if (!pBank) { Log("!!! GetSoundIdFromSymbol: sample bank null"); return 0; }
    FableString fs(symbol.c_str());
    long id = GetSoundIndexFromSymbol_API(pBank, fs);
    Log("GetSoundIdFromSymbol('" + symbol + "') = " + std::to_string(id));
    return (int)id;
}

float LuaQuestState::GetHeroAge() {
    if (m_pGameInterface && GetHeroAge_API) {
        return GetHeroAge_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: GetHeroAge - API function not available!");
        return -1.0f; // Return an error value
    }
}

void LuaQuestState::EntitySetAppearanceSeed(CScriptThing* pThing, unsigned int seed) {
    if (m_pGameInterface && EntitySetAppearanceSeed_API && pThing) {
        // Basic null check for the thing
        bool isNull = true;
        if (pThing->pImp.Data && pThing->pImp.Data->pVTable) {
            CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pThing->pImp.Data->pVTable);
            if (pVTable->IsNull) {
                isNull = pVTable->IsNull(reinterpret_cast<CGameScriptThing*>(pThing->pImp.Data));
            }
        }
        if (!isNull) {
            EntitySetAppearanceSeed_API(m_pGameInterface, pThing, seed);
        }
        else {
            Log("!!! ERROR: EntitySetAppearanceSeed - pThing is null or invalid!");
        }
    }
    else {
        if (!pThing) Log("!!! ERROR: EntitySetAppearanceSeed - pThing is null!");
        else Log("!!! ERROR: EntitySetAppearanceSeed - API function not available!");
    }
}

int LuaQuestState::GetWorldFrame() {
    if (m_pGameInterface && GetWorldFrame_API) {
        return GetWorldFrame_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: GetWorldFrame - API function not available!");
        return -1; // Return an error value
    }
}

void LuaQuestState::ShowOnScreenMessageWithFont(sol::table position, const std::string& message, sol::table color, const std::string& fontName) {
    if (m_pGameInterface && ShowOnScreenMessage_WithFont_API) {
        FableString fsMessage(message.c_str());
        FableString fsFont(fontName.c_str());

        // Extract position safely
        sol::optional<float> optX = position["x"];
        sol::optional<float> optY = position["y"];
        C2DVector pos = { optX ? *optX : 0.0f, optY ? *optY : 0.0f };

        // Extract color safely (assuming R, G, B, A are 0-255)
        sol::optional<unsigned char> optR = color["R"];
        sol::optional<unsigned char> optG = color["G"];
        sol::optional<unsigned char> optB = color["B"];
        sol::optional<unsigned char> optA = color["A"];
        CRGBColour col = {
            optR ? *optR : (unsigned char)255,
            optG ? *optG : (unsigned char)255,
            optB ? *optB : (unsigned char)255,
            optA ? *optA : (unsigned char)255 // Default to opaque white
        };

        ShowOnScreenMessage_WithFont_API(m_pGameInterface, &pos, fsMessage, &col, fsFont);

    }
    else {
        Log("!!! ERROR: ShowOnScreenMessageWithFont - API function not available!");
    }
}

void LuaQuestState::GiveHeroItem(CScriptThing* pItemThing) {
    if (m_pGameInterface && GiveHeroItem_API && pItemThing) {
        // Check if the item is valid before giving
        bool isNull = true;
        CCharString defNameResult = { 0 };
        const char* defNameCStr = "UNKNOWN";

        // *** FIX: Cast to CGameScriptThingVTable* ***
        if (pItemThing->pImp.Data && pItemThing->pImp.Data->pVTable) { // Check implementation pointer and its vtable
            CGameScriptThingVTable* pVTable = reinterpret_cast<CGameScriptThingVTable*>(pItemThing->pImp.Data->pVTable);
            // ********************************************
            if (pVTable->IsNull) {
                isNull = pVTable->IsNull(reinterpret_cast<CGameScriptThing*>(pItemThing->pImp.Data)); // Call IsNull on the implementation object
            }

            if (!isNull && pVTable->GetDefName && CCharString_ToConstChar_API) {
                // Call GetDefName on the implementation object
                pVTable->GetDefName(reinterpret_cast<CGameScriptThing*>(pItemThing->pImp.Data), &defNameResult);
                if (defNameResult.pStringData) {
                    defNameCStr = CCharString_ToConstChar_API(&defNameResult);
                }
            }
        }
        else {
            Log("!!! WARNING: GiveHeroItem - pItemThing->pImp.Data or its VTable is null!");
            isNull = true; // Treat as null if implementation details are missing
        }


        if (!isNull) {
            Log("Giving hero item: (DefName: " + std::string(defNameCStr) + ")");
            GiveHeroItem_API(m_pGameInterface, pItemThing); // API still takes the CScriptThing* wrapper
        }
        else {
            Log("!!! ERROR: GiveHeroItem - Attempted to give a null or invalid CScriptThing* (implementation check failed)!");
        }

        if (defNameResult.pStringData && CCharString_Destroy) {
            CCharString_Destroy(&defNameResult);
        }

    }
    else {
        if (!pItemThing) Log("!!! ERROR: GiveHeroItem - pItemThing is null!");
        else Log("!!! ERROR: GiveHeroItem - API function not available!");
    }
}

void LuaQuestState::SetHeroGuideShowsQuestCards(bool show) {
    if (m_pGameInterface && SetHeroGuideToShowQuestCardsWhenSpokenTo_API) {
        SetHeroGuideToShowQuestCardsWhenSpokenTo_API(m_pGameInterface, show);
        Log("Set Hero Guide shows quest cards: " + std::string(show ? "true" : "false"));
    }
    else {
        Log("!!! ERROR: SetHeroGuideShowsQuestCards - API function not available!");
    }
}

int LuaQuestState::EntityGetBossPhase(CScriptThing* pThing) {
    if (m_pGameInterface && EntityGetBossPhase_API && pThing) {
        return EntityGetBossPhase_API(m_pGameInterface, pThing);
    }
    if (!pThing) Log("!!! ERROR: EntityGetBossPhase - pThing is null!");
    else Log("!!! ERROR: EntityGetBossPhase - API function not available!");
    return -1; // Return invalid phase on error
}

bool LuaQuestState::GiveHeroTutorial(int category) {
    if (m_pGameInterface && GiveHeroTutorial_API) {
        return GiveHeroTutorial_API(m_pGameInterface, static_cast<ETutorialCategory>(category));
    }
    Log("!!! ERROR: GiveHeroTutorial - API function not available!");
    return false;
}

void LuaQuestState::PlayCutscene(const std::string& cutsceneName, sol::table actors) {
    if (m_pParentHost && IsActiveThreadTerminating_Quest_API && IsActiveThreadTerminating_Quest_API(&m_pParentHost->base)) {
        Log("!!! PlayCutscene aborted - Thread is terminating.");
        return; 
    }

    if (!m_pGameInterface || !RunCutsceneMacro_Func || !StartScriptingEntity_API || !StdMap_Construct_API ||
        !StdMap_OperatorBracket_API || !CBaseObject_Construct_API || !CBaseObject_Assign_API ||
        !g_pMovieObjectVTable || !StdMap_Destroy_API || !Game_free || !StartMovieSequence_API ||
        !PauseAllNonScriptedEntities_API || !MovieResource_Destroy_API) {
        Log("!!! ERROR: PlayCutscene failed - one or more required API functions are not initialized!");
        return;
    }

    Log("--- Playing Cutscene: " + cutsceneName + " ---");

    void* pMapBuffer = Game_malloc(0x24);
    if (!pMapBuffer) {
        Log("!!! ERROR: Failed to allocate map buffer!");
        return;
    }
    Log("    [RunCutsceneWithSetup] Constructing actor map...");
    StdMap_Construct_API(pMapBuffer);
    Log("    [RunCutsceneWithSetup] Actor map constructed.");

    for (auto kvp : actors) {
        std::string actorName = kvp.first.as<std::string>();
        Log("    [RunCutsceneWithSetup] Preparing actor '" + actorName + "'.");
        CScriptThing* actorThing = kvp.second.as<CScriptThing*>();

        if (!actorThing) {
            Log("!!! ERROR: PlayCutscene received a nil actor for name: " + actorName);
            continue;
        }

        FableString fsActorName(actorName.c_str());
        CScriptGameResourceObjectScriptedThingBase* pMapEntry = StdMap_OperatorBracket_API(pMapBuffer, fsActorName);
        CBaseObject_Assign_API(pMapEntry, actorThing);

        Log("    [PlayCutscene] Actor '" + actorName + "' assigned to map.");
    }

    char movieObjectBuffer[16];
    memset(movieObjectBuffer, 0, 16);
    auto* pMovieObject = reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(movieObjectBuffer);
    CBaseObject_Construct_API(pMovieObject);
    pMovieObject->pVTable = g_pMovieObjectVTable;

    FableString classString("");
    Log("    Starting movie sequence...");
    StartMovieSequence_API(m_pGameInterface, classString, reinterpret_cast<CScriptGameResourceObjectMovieBase*>(pMovieObject)); // Start the sequence

    Log("    [PlayCutscene] Pausing all non-scripted entities...");
    PauseAllNonScriptedEntities_API(m_pGameInterface, true);

    Log("    [PlayCutscene] >>>>> ENTERING BLOCKING RunCutsceneMacro_Func for '" + cutsceneName + "' <<<<<");
    FableString cutsceneNameStr(cutsceneName.c_str());
    BeginCutsceneSkipDiagnostics(cutsceneName.c_str());
    RunCutsceneMacro_Func(cutsceneNameStr, pMapBuffer, nullptr, nullptr, false, true);
    EndCutsceneSkipDiagnostics();
    Log("    [PlayCutscene] >>>>> EXITED BLOCKING RunCutsceneMacro_Func <<<<<");

    Log("    [PlayCutscene] Unpausing all non-scripted entities...");
    PauseAllNonScriptedEntities_API(m_pGameInterface, false);

    Log("    Destructing movie object...");
    MovieResource_Destroy_API(reinterpret_cast<CScriptGameResourceObjectMovieBase*>(pMovieObject));

    Log("    Destructing map object...");
    StdMap_Destroy_API(pMapBuffer);
    Game_free(pMapBuffer);

    Log("--- Cutscene Finished ---");
}

void LuaQuestState::FixMovieSequenceCamera(bool isFixed) {
    if (m_pGameInterface && FixMovieSequenceCamera_API) {
        FixMovieSequenceCamera_API(m_pGameInterface, isFixed);
    }
}

// ===== Cutscene session, ported from upstream FSE (eeeeeAeoN/FableScriptExtender
// @ master, 2026-09-02). Replaces this fork's own session: the Lua argument
// order of RunCutscene was (name, setupCondition, skippable), the reverse of
// what the retail script ports call, the flag map was always nullptr, and the
// actors were never placed under script control. =====
bool LuaQuestState::StartCutscene(sol::table actors, sol::optional<sol::table> flags, sol::optional<bool> fixCamera) {
    if (m_bCutsceneSessionActive) {
        Log("!!! WARNING: StartCutscene called while another cutscene session is already active! Ending previous session.");
        EndCutscene();
    }

    if (!m_pGameInterface || !RunCutsceneMacro_Func || !StdMap_Construct_API ||
        !StdMap_OperatorBracket_API || !CBaseObject_Construct_API || !CBaseObject_Assign_API ||
        !g_pMovieObjectVTable || !StdMap_Destroy_API || !Game_free || !StartMovieSequence_API ||
        !PauseAllNonScriptedEntities_API || !MovieResource_Destroy_API || !CSGROSTB_Destroy_API) {
        Log("!!! ERROR: StartCutscene failed - one or more required API functions are not initialized!");
        return false;
    }

    m_pCutsceneMapBuffer = Game_malloc(0x24);
    if (!m_pCutsceneMapBuffer) {
        Log("!!! ERROR: StartCutscene failed to allocate map buffer!");
        return false;
    }
    StdMap_Construct_API(m_pCutsceneMapBuffer);

    m_cutsceneAcquiredActors.clear();

    for (auto kvp : actors) {
        std::string actorName = kvp.first.as<std::string>();
        CScriptThing* actorThing = nullptr;
        if (kvp.second.is<CScriptThing*>()) {
            actorThing = kvp.second.as<CScriptThing*>();
        } else if (kvp.second.is<std::shared_ptr<CScriptThing>>()) {
            actorThing = kvp.second.as<std::shared_ptr<CScriptThing>>().get();
        }

        if (!actorThing) {
            Log("!!! ERROR: StartCutscene received a nil actor for name: " + actorName);
            continue;
        }

        ActiveCutsceneActor aa;
        memset(&aa.handle, 0, sizeof(CScriptGameResourceObjectScriptedThingBase));
        CBaseObject_Construct_API(&aa.handle);
        aa.handle.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
        aa.handle.pImp.Data = nullptr;
        aa.handle.pImp.Info = nullptr;

        if (InitScriptObjectHelper1_API && InitScriptObjectHelper2_API) {
            if (InitScriptObjectHelper1_API(&aa.handle)) {
                InitScriptObjectHelper2_API(&aa.handle);
            }
        }

        if (auto* existingHandle = LuaEntityAPI::FindControlHandle(actorThing)) {
            CBaseObject_Assign_API(&aa.handle, existingHandle);
            Log("    [StartCutscene] Reused existing script-control handle for actor '" + actorName + "'.");
        } else if (StartScriptingEntity_API) {
            aa.acquired = StartScriptingEntity_API(m_pGameInterface, actorThing, &aa.handle, SCRIPT_AI_PRIORITY_VERY_HIGH);
            if (aa.acquired) {
                Log("    [StartCutscene] Script control acquired for actor '" + actorName + "'.");
            } else {
                Log("    [StartCutscene] WARNING: Failed to acquire script control for actor '" + actorName + "'.");
            }
        }

        FableString fsActorName(actorName.c_str());
        CScriptGameResourceObjectScriptedThingBase* pMapEntry = StdMap_OperatorBracket_API(m_pCutsceneMapBuffer, fsActorName);
        CBaseObject_Assign_API(pMapEntry, &aa.handle);

        m_cutsceneAcquiredActors.push_back(aa);
        Log("    [StartCutscene] Actor '" + actorName + "' assigned to map.");
    }

    m_pCutsceneFlagsBuffer = nullptr;
    if (StdMap_Construct_API) {
        m_pCutsceneFlagsBuffer = Game_malloc(0x24);
        if (m_pCutsceneFlagsBuffer) {
            StdMap_Construct_API(m_pCutsceneFlagsBuffer);
            if (flags.has_value() && StdMap_Bool_OperatorBracket_API) {
                sol::table flagTable = flags.value();
                for (auto kvp : flagTable) {
                    std::string flagName = kvp.first.as<std::string>();
                    bool flagVal = kvp.second.as<bool>();
                    FableString fsFlagName(flagName.c_str());
                    bool* pVal = StdMap_Bool_OperatorBracket_API(m_pCutsceneFlagsBuffer, fsFlagName);
                    if (pVal) {
                        *pVal = flagVal;
                    }
                }
            }
        }
    }

    memset(m_cutsceneMovieObjectBuffer, 0, sizeof(m_cutsceneMovieObjectBuffer));
    auto* pMovieObject = reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(m_cutsceneMovieObjectBuffer);
    CBaseObject_Construct_API(pMovieObject);
    pMovieObject->pVTable = g_pMovieObjectVTable;

    FableString classString("");
    Log("    [StartCutscene] Starting movie sequence...");
    StartMovieSequence_API(m_pGameInterface, classString, reinterpret_cast<CScriptGameResourceObjectMovieBase*>(pMovieObject));

    Log("    [StartCutscene] Pausing all non-scripted entities...");
    PauseAllNonScriptedEntities_API(m_pGameInterface, true);

    m_bCutsceneCameraFixed = fixCamera.value_or(true);
    if (m_bCutsceneCameraFixed && FixMovieSequenceCamera_API) {
        Log("    [StartCutscene] Fixing movie sequence camera...");
        FixMovieSequenceCamera_API(m_pGameInterface, true);
    }

    m_bCutsceneSessionActive = true;
    return true;
}

bool LuaQuestState::RunCutscene(const std::string& macroName, sol::optional<bool> skippable, sol::optional<bool> setupcond) {
    if (!m_bCutsceneSessionActive || !RunCutsceneMacro_Func) {
        Log("!!! ERROR: RunCutscene called without an active cutscene session!");
        return false;
    }

    bool bSkippable = skippable.value_or(true);
    bool bSetupCond = setupcond.value_or(false);

    Log("    [RunCutscene] >>>>> ENTERING BLOCKING RunCutsceneMacro_Func for '" + macroName + "' (skippable=" + (bSkippable ? "true" : "false") + ") <<<<<");
    FableString cutsceneNameStr(macroName.c_str());
    BeginCutsceneSkipDiagnostics(macroName.c_str());
    RunCutsceneMacro_Func(cutsceneNameStr, m_pCutsceneMapBuffer, m_pCutsceneFlagsBuffer, nullptr, bSetupCond, bSkippable);
    EndCutsceneSkipDiagnostics();
    Log("    [RunCutscene] >>>>> EXITED BLOCKING RunCutsceneMacro_Func for '" + macroName + "' <<<<<");
    return true;
}

void LuaQuestState::EndCutscene() {
    if (!m_bCutsceneSessionActive) {
        return;
    }

    if (m_bCutsceneCameraFixed && FixMovieSequenceCamera_API) {
        Log("    [EndCutscene] Releasing movie sequence camera...");
        FixMovieSequenceCamera_API(m_pGameInterface, false);
    }

    if (PauseAllNonScriptedEntities_API && m_pGameInterface) {
        Log("    [EndCutscene] Unpausing all non-scripted entities...");
        PauseAllNonScriptedEntities_API(m_pGameInterface, false);
    }

    auto* pMovieObject = reinterpret_cast<CScriptGameResourceObjectMovieBase*>(m_cutsceneMovieObjectBuffer);
    if (MovieResource_Destroy_API) {
        Log("    [EndCutscene] Destructing movie resource object...");
        MovieResource_Destroy_API(reinterpret_cast<CScriptGameResourceObjectMovieBase*>(pMovieObject));
    }

    Log("    [EndCutscene] Releasing cutscene actor handles...");
    for (auto& aa : m_cutsceneAcquiredActors) {
        // Retail destroys each stack CScriptGameResourceObjectScriptedThingBase
        // with its derived destructor (0x007E74D0). Calling only the base
        // destructor leaves the pImp.Info reference at +0x0c alive and can keep
        // an acquired actor, including the Hero, under script control.
        CSGROSTB_Destroy_API(&aa.handle);
    }
    m_cutsceneAcquiredActors.clear();

    if (m_pCutsceneFlagsBuffer) {
        Log("    [EndCutscene] Destructing flags map object...");
        if (StdMap_Bool_Destroy_API) {
            StdMap_Bool_Destroy_API(m_pCutsceneFlagsBuffer);
        } else if (StdMap_Destroy_API) {
            StdMap_Destroy_API(m_pCutsceneFlagsBuffer);
        }
        Game_free(m_pCutsceneFlagsBuffer);
        m_pCutsceneFlagsBuffer = nullptr;
    }

    if (m_pCutsceneMapBuffer) {
        Log("    [EndCutscene] Destructing actor map object...");
        StdMap_Destroy_API(m_pCutsceneMapBuffer);
        Game_free(m_pCutsceneMapBuffer);
        m_pCutsceneMapBuffer = nullptr;
    }

    m_bCutsceneSessionActive = false;
    Log("--- Cutscene Session Ended ---");
}

bool LuaQuestState::GetCutsceneFlag(const std::string& flagName) {
    if (m_pCutsceneFlagsBuffer && StdMap_Bool_OperatorBracket_API) {
        FableString fsFlagName(flagName.c_str());
        bool* pVal = StdMap_Bool_OperatorBracket_API(m_pCutsceneFlagsBuffer, fsFlagName);
        if (pVal) {
            return *pVal;
        }
    }
    return false;
}

void LuaQuestState::SetCutsceneActionMode(bool isActionMode, const std::string& actionName) {
    if (m_pGameInterface && SetCutsceneActionMode_API) {
        FableString fsActionName(actionName.c_str());
        SetCutsceneActionMode_API(m_pGameInterface, isActionMode, fsActionName.get());
    } else {
        Log("!!! ERROR: SetCutsceneActionMode - API function not available!");
    }
}

void LuaQuestState::RunCutsceneWithSetup(const std::string& cutsceneName, sol::table actors, sol::optional<sol::table> flags, sol::optional<bool> setupcond, sol::optional<bool> skippable, sol::optional<bool> fixCamera, sol::this_state state) {
    if (m_pParentHost && IsActiveThreadTerminating_Quest_API && IsActiveThreadTerminating_Quest_API(&m_pParentHost->base)) {
        Log("!!! RunCutsceneWithSetup aborted - Thread is terminating.");
        return; 
    }

    if (!m_pGameInterface || !RunCutsceneMacro_Func || !StartScriptingEntity_API || !StdMap_Construct_API ||
        !StdMap_OperatorBracket_API || !CBaseObject_Construct_API || !CBaseObject_Assign_API ||
        !g_pMovieObjectVTable || !StdMap_Destroy_API || !Game_free || !StartMovieSequence_API ||
        !PauseAllNonScriptedEntities_API || !MovieResource_Destroy_API || !CSGROSTB_Destroy_API ||
        !NewScriptFrame_API || !IsActiveThreadTerminating_API) {
        Log("!!! ERROR: RunCutsceneWithSetup failed - one or more required API functions are not initialized!");
        return;
    }

    Log("--- Running Cutscene with Custom Setup: " + cutsceneName + " ---");

    void* pMapBuffer = Game_malloc(0x24);
    if (!pMapBuffer) {
        Log("!!! ERROR: Failed to allocate map buffer!");
        return;
    }
    StdMap_Construct_API(pMapBuffer);

    struct AcquiredActor {
        CScriptGameResourceObjectScriptedThingBase handle;
        bool acquired = false;
    };
    std::vector<AcquiredActor> acquiredActors;

    for (auto kvp : actors) {
        std::string actorName = kvp.first.as<std::string>();
        CScriptThing* actorThing = nullptr;
        if (kvp.second.is<CScriptThing*>()) {
            actorThing = kvp.second.as<CScriptThing*>();
        } else if (kvp.second.is<std::shared_ptr<CScriptThing>>()) {
            actorThing = kvp.second.as<std::shared_ptr<CScriptThing>>().get();
        }

        if (!actorThing) {
            Log("!!! ERROR: RunCutsceneWithSetup received a nil actor for name: " + actorName);
            continue;
        }

        AcquiredActor aa;
        memset(&aa.handle, 0, sizeof(CScriptGameResourceObjectScriptedThingBase));
        CBaseObject_Construct_API(&aa.handle);
        Log("    [RunCutsceneWithSetup] Base handle constructed for actor '" + actorName + "'.");
        aa.handle.pVTable = g_pCScriptGameResourceObjectScriptedThingBaseVTable;
        aa.handle.pImp.Data = nullptr;
        aa.handle.pImp.Info = nullptr;

        if (InitScriptObjectHelper1_API && InitScriptObjectHelper2_API) {
            Log("    [RunCutsceneWithSetup] Initializing script handle for actor '" + actorName + "'.");
            if (InitScriptObjectHelper1_API(&aa.handle)) {
                InitScriptObjectHelper2_API(&aa.handle);
            }
            Log("    [RunCutsceneWithSetup] Script handle initialized for actor '" + actorName + "'.");
        }

        if (auto* existingHandle = LuaEntityAPI::FindControlHandle(actorThing)) {
            CBaseObject_Assign_API(&aa.handle, existingHandle);
            Log("    [RunCutsceneWithSetup] Reused existing script-control handle for actor '" + actorName + "'.");
        } else if (StartScriptingEntity_API) {
            Log("    [RunCutsceneWithSetup] Acquiring script control for actor '" + actorName + "'.");
            while (!StartScriptingEntity_API(m_pGameInterface, actorThing, &aa.handle,
                                             SCRIPT_AI_PRIORITY_VERY_HIGH)) {
                NewScriptFrame_API(m_pGameInterface);
                const bool interfaceTerminating = IsActiveThreadTerminating_API(m_pGameInterface);
                const bool questTerminating = m_pParentHost && IsActiveThreadTerminating_Quest_API &&
                    IsActiveThreadTerminating_Quest_API(&m_pParentHost->base);
                if (interfaceTerminating || questTerminating) {
                    Log("    [RunCutsceneWithSetup] Actor acquisition aborted because the active thread is terminating.");
                    CSGROSTB_Destroy_API(&aa.handle);
                    for (auto& acquiredActor : acquiredActors) {
                        CSGROSTB_Destroy_API(&acquiredActor.handle);
                    }
                    StdMap_Destroy_API(pMapBuffer);
                    Game_free(pMapBuffer);
                    return;
                }
            }
            aa.acquired = true;
            Log("    [RunCutsceneWithSetup] Script control acquired for actor '" + actorName + "'.");
        }

        FableString fsActorName(actorName.c_str());
        CScriptGameResourceObjectScriptedThingBase* pMapEntry = StdMap_OperatorBracket_API(pMapBuffer, fsActorName);
        CBaseObject_Assign_API(pMapEntry, &aa.handle);

        acquiredActors.push_back(aa);
        {
            std::ostringstream ss;
            ss << "    [RunCutsceneWithSetup] Actor identity '" << actorName
               << "': thing=0x" << std::hex << reinterpret_cast<uintptr_t>(actorThing)
               << " thingData=0x" << reinterpret_cast<uintptr_t>(actorThing->pImp.Data)
               << " resourceData=0x" << reinterpret_cast<uintptr_t>(aa.handle.pImp.Data)
               << " resourceInfo=0x" << reinterpret_cast<uintptr_t>(aa.handle.pImp.Info);
            Log(ss.str());
        }
        Log("    [RunCutsceneWithSetup] Actor '" + actorName + "' assigned to map.");
    }

    void* pFlagsBuffer = nullptr;
    void* pInputArgsBuffer = nullptr;
    if (flags.has_value()) {
        sol::table flagTable = flags.value();
        for (auto kvp : flagTable) {
            std::string key = kvp.first.as<std::string>();
            sol::object value = kvp.second.as<sol::object>();
            if (value.is<bool>() && StdMap_Bool_OperatorBracket_API) {
                if (!pFlagsBuffer) {
                    pFlagsBuffer = Game_malloc(0x24);
                    if (pFlagsBuffer) StdMap_Construct_API(pFlagsBuffer);
                }
                if (pFlagsBuffer) {
                    FableString fsKey(key.c_str());
                    bool flagVal = value.as<bool>();
                    bool* pVal = StdMap_Bool_OperatorBracket_API(pFlagsBuffer, fsKey);
                    if (pVal) *pVal = flagVal;
                    Log("    [RunCutsceneWithSetup] Boolean flag '" + key + "' set to " + (flagVal ? "true" : "false"));
                }
            } else if (value.is<std::string>() && StdMap_String_Construct_API &&
                       StdMap_String_OperatorBracket_API && CCharString_AssignLiteral_API) {
                if (!pInputArgsBuffer) {
                    pInputArgsBuffer = Game_malloc(0x24);
                    if (pInputArgsBuffer) StdMap_String_Construct_API(pInputArgsBuffer);
                }
                if (pInputArgsBuffer) {
                    std::string replacement = value.as<std::string>();
                    FableString fsKey(key.c_str());
                    CCharString* pVal = StdMap_String_OperatorBracket_API(pInputArgsBuffer, fsKey);
                    if (pVal) CCharString_AssignLiteral_API(pVal, replacement.c_str());
                    Log("    [RunCutsceneWithSetup] Input substitution '" + key + "' set to '" + replacement + "'.");
                }
            } else {
                Log("!!! WARNING: RunCutsceneWithSetup ignored unsupported setup value for key '" + key + "'.");
            }
        }
    }

    // Retail callers commonly open and configure the movie sequence before invoking the
    // cutscene macro. Reuse that sequence instead of attempting to acquire the engine's
    // single movie resource a second time (which waits forever on itself).
    lua_State* const luaState = state;
    const auto callerMovie = m_movieHandlesByLuaState.find(luaState);
    const bool ownsMovieSequence =
        callerMovie == m_movieHandlesByLuaState.end() || callerMovie->second == nullptr;
    char movieObjectBuffer[16] = {};
    CScriptGameResourceObjectScriptedThingBase* pMovieObject = nullptr;
    bool bFixCamera = fixCamera.value_or(true);
    if (ownsMovieSequence) {
        pMovieObject = reinterpret_cast<CScriptGameResourceObjectScriptedThingBase*>(movieObjectBuffer);
        CBaseObject_Construct_API(pMovieObject);
        pMovieObject->pVTable = g_pMovieObjectVTable;

        FableString classString("");
        Log("    [RunCutsceneWithSetup] Starting movie sequence...");
        StartMovieSequence_API(m_pGameInterface, classString, reinterpret_cast<CScriptGameResourceObjectMovieBase*>(pMovieObject));

        Log("    [RunCutsceneWithSetup] Pausing all non-scripted entities...");
        PauseAllNonScriptedEntities_API(m_pGameInterface, true);

        if (bFixCamera && FixMovieSequenceCamera_API) {
            Log("    [RunCutsceneWithSetup] Fixing movie sequence camera...");
            FixMovieSequenceCamera_API(m_pGameInterface, true);
        }
    } else {
        Log("    [RunCutsceneWithSetup] Reusing caller-owned movie sequence.");
    }

    bool bSetupCond = setupcond.value_or(false);
    bool bSkippable = skippable.value_or(true);

    {
        std::ostringstream ss;
        ss << "    [RunCutsceneABI] macro='" << cutsceneName << "'"
           << " setupOptional=" << (setupcond.has_value() ? 1 : 0)
           << " setup=" << (bSetupCond ? 1 : 0)
           << " skippableOptional=" << (skippable.has_value() ? 1 : 0)
           << " skippable=" << (bSkippable ? 1 : 0)
           << " nativeStackOrder=flags,input,setup,skippable";
        Log(ss.str());
    }

    Log("    [RunCutsceneWithSetup] >>>>> ENTERING BLOCKING RunCutsceneMacro_Func for '" + cutsceneName + "' <<<<<");
    FableString cutsceneNameStr(cutsceneName.c_str());
    LogRetailCutsceneState("before", cutsceneName);
    BeginCutsceneSkipDiagnostics(cutsceneName.c_str());
    RunCutsceneMacro_Func(cutsceneNameStr, pMapBuffer, pFlagsBuffer, pInputArgsBuffer, bSetupCond, bSkippable);
    EndCutsceneSkipDiagnostics();
    LogRetailCutsceneState("after", cutsceneName);
    Log("    [RunCutsceneWithSetup] >>>>> EXITED BLOCKING RunCutsceneMacro_Func <<<<<");

    if (ownsMovieSequence && bFixCamera && FixMovieSequenceCamera_API) {
        Log("    [RunCutsceneWithSetup] Releasing movie sequence camera...");
        FixMovieSequenceCamera_API(m_pGameInterface, false);
    }

    if (ownsMovieSequence) {
        Log("    [RunCutsceneWithSetup] Unpausing all non-scripted entities...");
        PauseAllNonScriptedEntities_API(m_pGameInterface, false);

        Log("    Destructing movie object...");
        MovieResource_Destroy_API(reinterpret_cast<CScriptGameResourceObjectMovieBase*>(pMovieObject));
    }

    Log("    Releasing cutscene actor handles...");
    for (auto& aa : acquiredActors) {
        // This is a derived scripted-resource object, not merely CBaseObject.
        // The retail destructor releases pImp.Info and then chains to the base.
        CSGROSTB_Destroy_API(&aa.handle);
    }

    if (pFlagsBuffer) {
        Log("    Destructing flags map object...");
        if (StdMap_Bool_Destroy_API) {
            StdMap_Bool_Destroy_API(pFlagsBuffer);
        } else if (StdMap_Destroy_API) {
            StdMap_Destroy_API(pFlagsBuffer);
        }
        Game_free(pFlagsBuffer);
    }

    if (pInputArgsBuffer) {
        Log("    Destructing cutscene input-substitution map...");
        StdMap_String_Destroy_API(pInputArgsBuffer);
        Game_free(pInputArgsBuffer);
    }

    Log("    Destructing actor map object...");
    StdMap_Destroy_API(pMapBuffer);
    Game_free(pMapBuffer);

    Log("--- Cutscene Execution Completed ---");
}


void LuaQuestState::SetCreatureGeneratorsEnabled(const std::string& regionName, bool enabled) {
    if (m_pGameInterface && SetCreatureGeneratorsEnabled_API) {
        FableString fsRegion(regionName.c_str());
        SetCreatureGeneratorsEnabled_API(m_pGameInterface, fsRegion, enabled);
        Log("Set creature generators in '" + regionName + "' to " + (enabled ? "enabled" : "disabled") + ".");
    }
}

bool LuaQuestState::IsObjectInHeroPossession(const std::string& objectDefName) {
    if (m_pGameInterface && IsObjectInThingsPossession_API && GetHero_API) {
        FableString fsObject(objectDefName.c_str());
        CScriptThing* pHero = GetHero_API(m_pGameInterface);
        if (pHero) {
            return IsObjectInThingsPossession_API(m_pGameInterface, fsObject, pHero);
        }
    }
    return false;
}

bool LuaQuestState::IsObjectInThingsPossession(const std::string& objectDefName, const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsObjectInThingsPossession_API) {
        Log("!!! ERROR: IsObjectInThingsPossession - API not available!");
        return false;
    }

    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsObjectInThingsPossession - thing is null!");
        return false;
    }

    FableString fsObject(objectDefName.c_str());
    return IsObjectInThingsPossession_API(m_pGameInterface, fsObject.get(), pThing);
}

std::string SafeGetDefName(CScriptThing* pThing) {
    if (!pThing || !pThing->pVTable || !CCharString_ToConstChar_API || !CCharString_Destroy) {
        return "INVALID_THING_PTR";
    }
    CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
    if (!pVTable->GetDefName) {
        return "NO_GETDEFNAME_FUNC";
    }
    CCharString defNameResult = { 0 };
    pVTable->GetDefName(pThing, &defNameResult);
    if (defNameResult.pStringData) {
        const char* nameCStr = CCharString_ToConstChar_API(&defNameResult);
        if (nameCStr) {
            std::string name(nameCStr);
            CCharString_Destroy(&defNameResult);
            return name;
        }
        CCharString_Destroy(&defNameResult); // Destroy even if ToConstChar fails
    }
    return "GETDEFNAME_FAILED";
}

std::shared_ptr<CScriptThing> LuaQuestState::GetThingWithScriptName(const std::string& scriptName) {
    LogToFile("===== GetThingWithScriptName START: '" + scriptName + "' =====");

    if (!m_pGameInterface || !GetThingWithScriptName_ByName_API || !Game_malloc || !Game_free) {
        Log("!!! ERROR: Prerequisites not available!");
        return nullptr;
    }
    if (!g_pCScriptThingVTable) {
        Log("!!! ERROR: g_pCScriptThingVTable is null!");
        return nullptr;
    }

    FableString fsScriptName(scriptName.c_str());

    CScriptThing* pHeapResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pHeapResult) {
        Log("!!! ERROR: Failed to allocate heap memory!");
        return nullptr;
    }
    memset(pHeapResult, 0, sizeof(CScriptThing));

    LogToFile("    Calling GetThingWithScriptName_ByName_API with heap buffer...");
    CScriptThing* pApiReturn = GetThingWithScriptName_ByName_API(m_pGameInterface, pHeapResult, fsScriptName);
    LogToFile("    API call returned.");

    LogCScriptThingDetails("    Heap Buffer After API", pHeapResult);

    bool isNull = true;
    if (pHeapResult->pVTable == g_pCScriptThingVTable) {
        CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pHeapResult->pVTable);
        if (pVTable->IsNull) {
            isNull = pVTable->IsNull(pHeapResult);
        }
    }

    if (!isNull && pHeapResult->pImp.Info) {
        LogToFile("    Object is valid. Adopting API-owned reference (RefCount="
            + std::to_string(pHeapResult->pImp.Info->RefCount) + ").");
        LogToFile("===== GetThingWithScriptName END (Success) =====");

        // Return shared_ptr with custom deleter
        return std::shared_ptr<CScriptThing>(pHeapResult, [](CScriptThing* p) {
            LogToFile("===== shared_ptr Deleter Called =====");
            std::stringstream ss;
            ss << "    Cleaning up CScriptThing* = 0x" << std::hex << reinterpret_cast<uintptr_t>(p);
            LogToFile(ss.str());

            if (p && p->pImp.Info && p->pImp.Info->RefCount > 0) {
                LogToFile("    Current RefCount: " + std::to_string(p->pImp.Info->RefCount));
                p->pImp.Info->RefCount--;
                LogToFile("    Decremented to: " + std::to_string(p->pImp.Info->RefCount));
            }

            if (p && Game_free) {
                Game_free(p);
                LogToFile("    Freed heap allocation.");
            }
            LogToFile("===== shared_ptr Deleter Complete =====");
            });
    }
    else {
        LogToFile("    Object not found or is null.");
        Game_free(pHeapResult);
        LogToFile("===== GetThingWithScriptName END (Not Found) =====");
        return nullptr;
    }
}

sol::table LuaQuestState::GetThingDebugInfo(CScriptThing* pThing, sol::this_state s) {
    sol::state_view lua(s);
    sol::table info = lua.create_table();
    LogToFile("===== GetThingDebugInfo START =====");
    LogCScriptThingDetails("    Input Thing", pThing); // Reuse the detailed logger

    if (!pThing) {
        info["valid"] = false;
        info["error"] = "Input CScriptThing* is NULL";
        LogToFile("    Input Thing is NULL.");
        LogToFile("===== GetThingDebugInfo END =====");
        return info;
    }

    info["address"] = reinterpret_cast<uintptr_t>(pThing);
    info["vtable_address"] = reinterpret_cast<uintptr_t>(pThing->pVTable);
    info["imp_data_address"] = reinterpret_cast<uintptr_t>(pThing->pImp.Data);
    info["imp_info_address"] = reinterpret_cast<uintptr_t>(pThing->pImp.Info);
    info["vtable_matches_known"] = (pThing->pVTable == g_pCScriptThingVTable);

    if (pThing->pImp.Info) {
        info["ref_count"] = pThing->pImp.Info->RefCount;
        info["deleter_func_address"] = reinterpret_cast<uintptr_t>(pThing->pImp.Info->DeleteFunc);
        info["info_data_address"] = reinterpret_cast<uintptr_t>(pThing->pImp.Info->Data);
        info["info_data_matches_imp_data"] = (pThing->pImp.Info->Data == pThing->pImp.Data);
    }
    else {
        info["ref_count"] = "N/A (Info NULL)";
    }

    // Attempt IsNull check
    bool isNull = true;
    std::string isNullError = "OK";
    if (pThing->pVTable == g_pCScriptThingVTable) {
        CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
        if (pVTable->IsNull) {
            try {
                isNull = pVTable->IsNull(pThing);
            }
            catch (...) {
                isNull = true; // Assume null on exception
                isNullError = "Exception during IsNull call";
                LogToFile("!!! EXCEPTION during IsNull call in GetThingDebugInfo !!!");
            }
        }
        else {
            isNullError = "IsNull function pointer in VTable is NULL";
            isNull = true;
        }
    }
    else {
        isNullError = "VTable mismatch or NULL";
        isNull = true;
    }
    info["is_null_check"] = isNull;
    info["is_null_error"] = isNullError;


    // Attempt SafeGetDefName
    std::string defName = SafeGetDefName(pThing);
    info["def_name"] = defName;

    info["valid"] = !isNull && (pThing->pVTable == g_pCScriptThingVTable) && pThing->pImp.Info; // Basic validity check

    LogToFile("===== GetThingDebugInfo END =====");
    return info;
}

void LuaQuestState::ReleaseThing(CScriptThing* pThing) {
    LogToFile("===== ReleaseThing (Direct Pointer - No Free) START =====");
    LogCScriptThingDetails("    Input Handle", pThing);

    if (!pThing) {
        LogToFile("!!! WARNING: ReleaseThing called with NULL pointer!");
        LogToFile("===== ReleaseThing (Direct Pointer - No Free) END =====");
        return;
    }

    // Crucially, only decrement the reference count
    if (pThing->pImp.Info) {
        LogToFile("    Current RefCount: " + std::to_string(pThing->pImp.Info->RefCount));
        if (pThing->pImp.Info->RefCount > 0) {
            pThing->pImp.Info->RefCount--; // DECREMENT REF COUNT
            LogToFile("    Decremented RefCount. New RefCount: " + std::to_string(pThing->pImp.Info->RefCount));
        }
        else {
            LogToFile("!!! WARNING: ReleaseThing - RefCount was already 0 or negative!");
        }
    }
    else {
        LogToFile("!!! WARNING: ReleaseThing - pImp.Info is NULL! Cannot decrement RefCount.");
    }

    // DO NOT FREE pThing HERE - it points to game memory or a game-managed object
    LogToFile("    (Skipping Game_free as pointer is assumed to be game-managed)");
    LogToFile("===== ReleaseThing (Direct Pointer - No Free) END =====");
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateCreature(const std::string& defName, sol::table position, const std::string& scriptName) {
    if (m_pGameInterface && CreateCreature_API && Game_malloc) {
        FableString fsDef(defName.c_str());
        FableString fsScript(scriptName.c_str());
        C3DVector pos = { position["x"].get_or(0.0f), position["y"].get_or(0.0f), position["z"].get_or(0.0f) };

        CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
        if (pResult) {
            memset(pResult, 0, sizeof(CScriptThing));
            CreateCreature_API(m_pGameInterface, pResult, fsDef, &pos, fsScript, false);

            // Wrap, validate, and return.
            return WrapScriptThingOutput(pResult);
        }
        else {
            Log("!!! ERROR: CreateCreature - Failed to allocate memory!");
        }
    }
    else {
        Log("!!! ERROR: CreateCreature - API or Game_malloc not available!");
    }
    return nullptr;
}

void LuaQuestState::EntitySetTargetable(CScriptThing* pThing, bool isTargetable) {
    if (m_pGameInterface && EntitySetTargetable_API && pThing) {
        EntitySetTargetable_API(m_pGameInterface, pThing, isTargetable);
    }
}

void LuaQuestState::EntityTeleportToThing(CScriptThing* pThingToMove, CScriptThing* pTargetThing, sol::optional<bool> b3) {
    if (m_pGameInterface && EntityTeleportToThing_API && pThingToMove && pTargetThing) {
        EntityTeleportToThing_API(m_pGameInterface, pThingToMove, pTargetThing, b3.value_or(false));
    }
}

int LuaQuestState::RetailRandModulo(int modulus) {
    if (modulus <= 0) {
        Log("!!! ERROR: RetailRandModulo requires a positive modulus.");
        return 0;
    }
    // Every reconstructed New Oakvale random branch calls the retail executable's
    // MSVCR71 rand import thunk at 0x00BFEB16. Calling the thunk preserves the
    // process-wide retail RNG stream; the plugin CRT and Lua RNG are different streams.
    using tRetailRand = int(__cdecl*)();
    tRetailRand retailRand = ASLR<tRetailRand>(0x00BFEB16);
    return retailRand() % modulus;
}

float LuaQuestState::GetHealth(CScriptThing* pThing) {
    if (m_pGameInterface && GetHealth_API && pThing) {
        return GetHealth_API(m_pGameInterface, pThing);
    }
    return 0.0f;
}

void LuaQuestState::EntitySetAsDrawable(CScriptThing* pThing, bool isDrawable) {
    if (m_pGameInterface && EntitySetAsDrawable_API && pThing) {
        EntitySetAsDrawable_API(m_pGameInterface, pThing, isDrawable);
    }
}

void LuaQuestState::GiveHeroObject(const std::string& objectDefName, int amount) {
    if (m_pGameInterface && GiveHeroObject_API) {
        FableString fsObject(objectDefName.c_str());
        GiveHeroObject_API(m_pGameInterface, fsObject, amount, false);
    }
}

bool LuaQuestState::IsDistanceBetweenThingsUnder(sol::object thing1, sol::object thing2, float distance) {
    // Check if the API function pointer is valid
    if (!IsDistanceBetweenThingsUnder_API) {
        return false;
    }

    // Entity callbacks receive `me` as a raw CScriptThing*, while GetHero/GetThing return
    // shared_ptr userdata. Normalize both representations explicitly before the retail fastcall.
    // Relying on sol's implicit conversion here made this adjacent native helper inconsistent with
    // CanThingBe_Seen_ByOtherThing and could feed the distance test the userdata wrapper rather
    // than the contained retail CScriptThing.
    CScriptThing* pThing1 = GetScriptThingFromSolObject(thing1);
    CScriptThing* pThing2 = GetScriptThingFromSolObject(thing2);
    if (!pThing1 || !pThing2) {
        return false;
    }

    // Call the game's API function directly using the raw pointers
    try {
        return IsDistanceBetweenThingsUnder_API(pThing1, pThing2, distance);
    }
    catch (...) {
        return false; // Return false if the API call throws an exception
    }
}

bool LuaQuestState::IsDistanceBetweenThingsOver(sol::object thing1, sol::object thing2, float distance) {
    CScriptThing* pThing1 = GetScriptThingFromSolObject(thing1);
    CScriptThing* pThing2 = GetScriptThingFromSolObject(thing2);
    if (!IsDistanceBetweenThingsOver_API || !pThing1 || !pThing2) {
        return false;
    }

    try {
        return IsDistanceBetweenThingsOver_API(pThing1, pThing2, distance);
    }
    catch (...) {
        return false;
    }
}

void LuaQuestState::RemoveThing(sol::object thing, sol::optional<bool> immediate, sol::optional<bool> removeFromWorld) {
    CScriptThing* pThing = GetScriptThingFromSolObject(thing);
    if (m_pGameInterface && RemoveThing_API && pThing) {
        RemoveThing_API(m_pGameInterface, pThing, immediate.value_or(false), removeFromWorld.value_or(true));
    }
}

void LuaQuestState::FadeOutAndKillEntity(CScriptThing* pThing, bool bKillAfterFade, float fOverThisManySeconds, bool bKillPersistent) {
    if (pThing && FadeOutAndKillEntity_Func) {
        // Argument order matches retail: bool, then float. See the typedef comment.
        FadeOutAndKillEntity_Func(pThing, bKillAfterFade, fOverThisManySeconds, bKillPersistent ? 1 : 0);
    }
}

void LuaQuestState::AddLogbookStoryEntry(int storyId) {
    if (AddLogbookStoryEntry_Func) {
        AddLogbookStoryEntry_Func(storyId);
    }
}

void LuaQuestState::EntitySetAsKillable(CScriptThing* pThing, bool isKillable, sol::optional<bool> forceDead) {
    if (m_pGameInterface && EntitySetAsKillable_API && pThing) EntitySetAsKillable_API(m_pGameInterface, pThing, isKillable, forceDead.value_or(false));
}

void LuaQuestState::EntitySetOpinionReactionMask(CScriptThing* pThing, const std::string& maskName) {
    if (m_pGameInterface && EntitySetOpinionReactionMask_API && pThing) {
        FableString fsMask(maskName.c_str());
        EntitySetOpinionReactionMask_API(m_pGameInterface, pThing, fsMask);
    }
}

void LuaQuestState::SetThingHasInformation(CScriptThing* pThing, sol::optional<bool> wander, sol::optional<bool> wave, sol::optional<bool> continueAI) {
    if (m_pGameInterface && SetThingHasInformation_API && pThing) {
        SetThingHasInformation_API(m_pGameInterface, pThing, wander.value_or(false), wave.value_or(true), continueAI.value_or(false));
    }
}

void LuaQuestState::EntitySetFacingAngleTowardsThing(sol::object thing, sol::object target, sol::optional<bool> snap) {
    CScriptThing* pThing = GetScriptThingFromSolObject(thing);
    CScriptThing* pTarget = GetScriptThingFromSolObject(target);
    if (m_pGameInterface && EntitySetFacingAngleTowardsThing_API && pThing && pTarget) {
        EntitySetFacingAngleTowardsThing_API(m_pGameInterface, pThing, pTarget, snap.value_or(false));
    }
}

void LuaQuestState::GiveHeroYesNoQuestion(const std::string& question, const std::string& button1, const std::string& button2, const std::string& button3, sol::optional<bool> b5) {
    Log("--- GiveHeroYesNoQuestion START ---");
    Log("    Question: " + question);
    Log("    Button 1: " + button1);
    Log("    Button 2: " + button2);
    Log("    Button 3: " + button3);

    if (!m_pGameInterface || !GiveHeroYesNoQuestion_API) {
        Log("!!! ERROR: GiveHeroYesNoQuestion - API function not available!");
        return;
    }

    // 2. Prepare FableStrings
    FableString fsQ(question.c_str());
    FableString fsB1(button1.c_str());
    FableString fsB2(button2.c_str());
    FableString fsB3(button3.c_str());

    GiveHeroYesNoQuestion_API(m_pGameInterface, fsQ, fsB1, fsB2, fsB3, b5.value_or(true));
    Log("--- GiveHeroYesNoQuestion END ---");
}

void LuaQuestState::TakeObjectFromHero(const std::string& objectDefName) {
    if (m_pGameInterface && TakeObjectFromHero_API) {
        FableString fsObj(objectDefName.c_str());
        TakeObjectFromHero_API(m_pGameInterface, fsObj);
    }
}

void LuaQuestState::GiveHeroGold(int amount) {
    if (m_pGameInterface && GiveHeroGold_API) GiveHeroGold_API(m_pGameInterface, amount);
}

void LuaQuestState::GiveHeroMorality(float amount) {
    if (m_pGameInterface && GiveHeroMorality_API) GiveHeroMorality_API(m_pGameInterface, amount);
}

void LuaQuestState::GiveHeroExperience(int amount) {
    if (m_pGameInterface && GiveHeroExperience_API) GiveHeroExperience_API(m_pGameInterface, amount);
}

void LuaQuestState::ClearThingHasInformation(CScriptThing* pThing) {
    if (m_pGameInterface && ClearThingHasInformation_API && pThing) ClearThingHasInformation_API(m_pGameInterface, pThing);
}

void LuaQuestState::FadeScreenOut(float fadeTime, float holdTime) {
    if (m_pGameInterface && FadeScreenOut_API) {
        CRGBColour black = { 0, 0, 0, 255 };
        FadeScreenOut_API(m_pGameInterface, fadeTime, holdTime, black);
    }
}

void LuaQuestState::FadeScreenIn() {
    if (m_pGameInterface && FadeScreenIn_API) FadeScreenIn_API(m_pGameInterface);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateCreatureNearby(const std::string& defName, sol::table position, float radius, const std::string& scriptName) {
    if (m_pGameInterface && CreateCreatureNearby_API && Game_malloc) {
        FableString fsDef(defName.c_str());
        FableString fsScript(scriptName.c_str());
        C3DVector pos = { position["x"].get_or(0.0f), position["y"].get_or(0.0f), position["z"].get_or(0.0f) };

        CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
        if (pResult) {
            memset(pResult, 0, sizeof(CScriptThing));
            CreateCreatureNearby_API(m_pGameInterface, pResult, fsDef, &pos, radius, fsScript, false);

            // Wrap, validate, and return.
            return WrapScriptThingOutput(pResult);
        }
        else {
            Log("!!! ERROR: CreateCreatureNearby - Failed to allocate memory!");
        }
    }
    else {
        Log("!!! ERROR: CreateCreatureNearby - API or Game_malloc not available!");
    }
    return nullptr;
}

void LuaQuestState::EntitySetAllStategroupsEnabled(CScriptThing* pThing, bool enabled) {
    if (m_pGameInterface && EntitySetAllStategroupsEnabled_API && pThing) {
        EntitySetAllStategroupsEnabled_API(m_pGameInterface, pThing, enabled);
    }
}

void LuaQuestState::PauseAllNonScriptedEntities(bool isPaused) {
    if (m_pGameInterface && PauseAllNonScriptedEntities_API) {
        PauseAllNonScriptedEntities_API(m_pGameInterface, isPaused);
    }
}

void LuaQuestState::AddEntityBinding(const std::string& entityScriptName, const std::string& scriptFileName)
{
    Log("Binding entity '" + entityScriptName + "' to script '" + scriptFileName + "'...");
    m_pendingBindings.push_back({ entityScriptName, scriptFileName });
}

void LuaQuestState::FinalizeEntityBindings()
{
    if (!m_pParentHost || !AddEntityScriptBinding_API || !PostAddScriptedEntities_CScriptBase_API || !PostAddScriptedEntities_API)
    {
        Log("!!! ERROR: FinalizeEntityBindings failed - required API functions are not initialized!");
        return;
    }

    Log("Finalizing all entity bindings...");

    for (const auto& bindingInfo : m_pendingBindings)
    {
        tEntityScriptAllocFunc pAllocFunc = GetEntityAllocatorForScript(bindingInfo.scriptFile);
        if (pAllocFunc)
        {
            void* pBindingBuffer = Game_malloc(sizeof(CEntityScriptBindingBase));
            if (pBindingBuffer)
            {
                memset(pBindingBuffer, 0, sizeof(CEntityScriptBindingBase));
                CEntityScriptBindingBase* pBinding = static_cast<CEntityScriptBindingBase*>(pBindingBuffer);

                pBinding->pVTable = g_pEntityScriptBindingVTable;
                CCharString_Construct_Literal(&pBinding->EntityScriptName, bindingInfo.entityName.c_str(), -1);
                pBinding->pParentScript = &m_pParentHost->base;
                pBinding->pAllocFunc = pAllocFunc;
                pBinding->bSomething = true;

                AddEntityScriptBinding_API(&m_pParentHost->base, pBinding);
                Log("    Binding successful.");
            }
        }
        else
        {
            Log("!!! ERROR: Could not find allocator for entity script '" + bindingInfo.scriptFile + "'");
        }
    }

    PostAddScriptedEntities_CScriptBase_API(&m_pParentHost->base);
    PostAddScriptedEntities_API(m_pGameInterface);
    Log("    Finalization complete.");

    m_pendingBindings.clear();
}

bool LuaQuestState::PersistTransferBool(void* pContext, const std::string& name, bool value, sol::optional<bool> defaultValue)
{
    LogToFile("    [PERSIST] Transferring bool '" + name + "'...");
    CPersistContext* pRealContext = static_cast<CPersistContext*>(pContext);
    bool tempValue = value;
    const bool transferDefault = defaultValue.value_or(false);

    if (CPersistContext_Transfer_bool_API) {
        CPersistContext_Transfer_bool_API(pRealContext, name.c_str(), &tempValue, &transferDefault);
        LogToFile("    [PERSIST] ...Value is now: " + std::string(tempValue ? "true" : "false"));
    }
    else {
        LogToFile("    [PERSIST] ...ERROR: CPersistContext_Transfer_bool_API is null!");
    }
    return tempValue;
}

int LuaQuestState::PersistTransferInt(void* pContext, const std::string& name, int value)
{
    LogToFile("    [PERSIST] Transferring int '" + name + "'...");
    CPersistContext* pRealContext = static_cast<CPersistContext*>(pContext);
    int tempValue = value;
    const int defaultValue = value;

    if (CPersistContext_Transfer_int_API) {
        CPersistContext_Transfer_int_API(pRealContext, name.c_str(), &tempValue, &defaultValue);
        LogToFile("    [PERSIST] ...Value is now: " + std::to_string(tempValue));
    }
    else {
        LogToFile("    [PERSIST] ...ERROR: CPersistContext_Transfer_int_API is null!");
    }
    return tempValue;
}

std::string LuaQuestState::PersistTransferString(void* pContext, const std::string& name, const std::string& value)
{
    LogToFile("    [PERSIST] Transferring string '" + name + "'...");
    CPersistContext* pRealContext = static_cast<CPersistContext*>(pContext);

    FableString fsValue(value.c_str());
    FableString fsDefault(value.c_str()); 

    if (CPersistContext_Transfer_string_API) {
        CPersistContext_Transfer_string_API(pRealContext, name.c_str(), fsValue.get(), fsDefault);
        const char* resultText = CCharString_ToConstChar_API(fsValue.get());
        if (resultText) {
            std::string finalStr(resultText);
            LogToFile("    [PERSIST] ...Value is now: " + finalStr);
            return finalStr;
        }
    }
    else {
        LogToFile("    [PERSIST] ...ERROR: CPersistContext_Transfer_string_API is null!");
    }
    return value;
}

float LuaQuestState::PersistTransferFloat(void* pContext, const std::string& name, float value)
{
    LogToFile("    [PERSIST] Transferring float '" + name + "'...");
    CPersistContext* pRealContext = static_cast<CPersistContext*>(pContext);
    float tempValue = value;
    const float defaultValue = value;

    if (CPersistContext_Transfer_float_API) {
        CPersistContext_Transfer_float_API(pRealContext, name.c_str(), &tempValue, &defaultValue);
        LogToFile("    [PERSIST] ...Value is now: " + std::to_string(tempValue));
    }
    else {
        LogToFile("    [PERSIST] ...ERROR: CPersistContext_Transfer_float_API is null!");
    }
    return tempValue;
}

unsigned int LuaQuestState::PersistTransferUInt(void* pContext, const std::string& name, unsigned int value)
{
    LogToFile("    [PERSIST] Transferring uint '" + name + "'...");
    CPersistContext* pRealContext = static_cast<CPersistContext*>(pContext);
    unsigned int tempValue = value;
    const unsigned int defaultValue = value;

    if (CPersistContext_Transfer_uint_API) {
        CPersistContext_Transfer_uint_API(pRealContext, name.c_str(), &tempValue, &defaultValue);
        LogToFile("    [PERSIST] ...Value is now: " + std::to_string(tempValue));
    }
    else {
        LogToFile("    [PERSIST] ...ERROR: CPersistContext_Transfer_uint_API is null!");
    }
    return tempValue;
}

std::string LuaQuestState::GetNamespacedKey(const std::string& key) {
    if (m_pParentHost) {
        // This turns "isFinished" into "MyFirstQuest:isFinished"
        return m_pParentHost->GetScriptName() + ":" + key;
    }
    Log("!!! ERROR: GetNamespacedKey - m_pParentHost is null! Using fallback.");
    return "UNKNOWN_QUEST:" + key;
}

void LuaQuestState::SetStateBool(const std::string& key, bool value) {
    // No mutex. Get the namespaced key.
    std::string namespacedKey = GetNamespacedKey(key);
    // Call the global manager
    LuaManager::GetInstance().SetGlobalStateBool(namespacedKey, value);
}

void LuaQuestState::SetStateInt(const std::string& key, int value) {
    std::string namespacedKey = GetNamespacedKey(key);
    LuaManager::GetInstance().SetGlobalStateInt(namespacedKey, value);
}

void LuaQuestState::SetStateString(const std::string& key, const std::string& value) {
    std::string namespacedKey = GetNamespacedKey(key);
    LuaManager::GetInstance().SetGlobalStateString(namespacedKey, value);
}

int LuaQuestState::GetStateInt(const std::string& key) {
    std::string namespacedKey = GetNamespacedKey(key);
    return LuaManager::GetInstance().GetGlobalStateInt(namespacedKey);
}

bool LuaQuestState::GetStateBool(const std::string& key) {
    std::string namespacedKey = GetNamespacedKey(key);
    return LuaManager::GetInstance().GetGlobalStateBool(namespacedKey);
}

std::string LuaQuestState::GetStateString(const std::string& key) {
    std::string namespacedKey = GetNamespacedKey(key);
    return LuaManager::GetInstance().GetGlobalStateString(namespacedKey);
}

void LuaQuestState::SetGlobalBool(const std::string& key, bool value) {
    LuaManager::GetInstance().SetGlobalStateBool(key, value);
}

void LuaQuestState::SetGlobalInt(const std::string& key, int value) {
    LuaManager::GetInstance().SetGlobalStateInt(key, value);
}

void LuaQuestState::SetGlobalString(const std::string& key, const std::string& value) {
    LuaManager::GetInstance().SetGlobalStateString(key, value);
}

int LuaQuestState::GetGlobalInt(const std::string& key) {
    return LuaManager::GetInstance().GetGlobalStateInt(key);
}

bool LuaQuestState::GetGlobalBool(const std::string& key) {
    return LuaManager::GetInstance().GetGlobalStateBool(key);
}

std::string LuaQuestState::GetGlobalString(const std::string& key) {
    return LuaManager::GetInstance().GetGlobalStateString(key);
}

bool LuaQuestState::NewScriptFrame(CScriptThing* pMe) {
    if (m_pGameInterface && NewScriptFrame_API) {
        NewScriptFrame_API(m_pGameInterface);
    }

    if (IsActiveThreadTerminating_Entity_API) {
        LuaEntityHost* pEntityHost = (LuaEntityHost*)((char*)pMe - offsetof(LuaEntityHost, m_Me));

        return !IsActiveThreadTerminating_Entity_API(pEntityHost);
    }
    return true;
}

bool LuaQuestState::NewScriptFrame() {
    std::string scriptName = "UNKNOWN_SCRIPT";
    if (m_pParentHost) {
        scriptName = m_pParentHost->GetScriptName();
    }

    if (m_pGameInterface && NewScriptFrame_API) {
        NewScriptFrame_API(m_pGameInterface);
    }

    if (IsActiveThreadTerminating_Quest_API) {
        return !IsActiveThreadTerminating_Quest_API(&m_pParentHost->base);
    }

    return true;
}

bool LuaQuestState::IsActiveThreadTerminating() {
    return m_pParentHost && IsActiveThreadTerminating_Quest_API &&
        IsActiveThreadTerminating_Quest_API(&m_pParentHost->base);
}

sol::object LuaQuestState::GetMasterGameState(const std::string& flagName, sol::this_state s) {
    sol::state_view lua(s);

    if (!m_pParentHost) { /* ... */ return sol::make_object(lua, sol::nil); }

    CQ_SunnyvaleMasterData* pMaster = m_pParentHost->GetMasterData();

    if (!pMaster) { /* ... */ return sol::make_object(lua, sol::nil); }

    // --- Boolean Flags ---
    if (flagName == "HenchmanSacrificedBodge") return sol::make_object(lua, pMaster->HenchmanSacrificedBodge);
    if (flagName == "HauntedBarrowFieldsCompleted") return sol::make_object(lua, pMaster->HauntedBarrowFieldsCompleted);
    if (flagName == "RandomEntitySheriffAlive") return sol::make_object(lua, pMaster->RandomEntitySheriffAlive);
    if (flagName == "RandomEntityLieutenantAlive") return sol::make_object(lua, pMaster->RandomEntityLieutenantAlive);
    if (flagName == "BanditCampTwinbladeKilled") return sol::make_object(lua, pMaster->BanditCampTwinbladeKilled);
    if (flagName == "FriendOfForeman") return sol::make_object(lua, pMaster->FriendOfForeman);
    if (flagName == "BridgeOpened") return sol::make_object(lua, pMaster->BridgeOpened);
    if (flagName == "GrannyMemoryReturned") return sol::make_object(lua, pMaster->GrannyMemoryReturned);
    if (flagName == "IsLunaHuman") return sol::make_object(lua, pMaster->IsLunaHuman);
    if (flagName == "CondemnedManDead") return sol::make_object(lua, pMaster->CondemnedManDead);
    if (flagName == "CondemnedManForgiven") return sol::make_object(lua, pMaster->CondemnedManForgiven);
    if (flagName == "CondemnedManMeetsBodyGuard") return sol::make_object(lua, pMaster->CondemnedManMeetsBodyGuard);
    if (flagName == "CondemnedManMeetsBodyGuardCutSceneStart") return sol::make_object(lua, pMaster->CondemnedManMeetsBodyGuardCutSceneStart);
    if (flagName == "CondemnedManMeetsBodyGuardCutSceneFinished") return sol::make_object(lua, pMaster->CondemnedManMeetsBodyGuardCutSceneFinished);
    if (flagName == "SeenAbbeyMotherAtGuild") return sol::make_object(lua, pMaster->SeenAbbeyMotherAtGuild);
    if (flagName == "DefeatedThunder") return sol::make_object(lua, pMaster->DefeatedThunder);
    if (flagName == "LostToThunder") return sol::make_object(lua, pMaster->LostToThunder);
    if (flagName == "KilledThunder") return sol::make_object(lua, pMaster->KilledThunder);
    if (flagName == "CollectedSoulFromArena") return sol::make_object(lua, pMaster->CollectedSoulFromArena);
    if (flagName == "KilledBriar") return sol::make_object(lua, pMaster->KilledBriar);
    if (flagName == "CollectedSoulFromMother") return sol::make_object(lua, pMaster->CollectedSoulFromMother);
    if (flagName == "KilledGM") return sol::make_object(lua, pMaster->KilledGM);
    if (flagName == "CollectedSoulFromNostro") return sol::make_object(lua, pMaster->CollectedSoulFromNostro);
    if (flagName == "WhisperKilledByHero") return sol::make_object(lua, pMaster->WhisperKilledByHero);
    if (flagName == "ArenaFinished") return sol::make_object(lua, pMaster->ArenaFinished);
    if (flagName == "GatesRequireClosing") return sol::make_object(lua, pMaster->GatesRequireClosing);
    if (flagName == "GatesRequireOpening") return sol::make_object(lua, pMaster->GatesRequireOpening);
    if (flagName == "TrophyDealerHeroSpokenToDemonDoors") return sol::make_object(lua, pMaster->TrophyDealerHeroSpokenToDemonDoors);
    if (flagName == "MotherDodgingKraken") return sol::make_object(lua, pMaster->MotherDodgingKraken);
    if (flagName == "JackBossBattleHeroGoodAtEnd") return sol::make_object(lua, pMaster->JackBossBattleHeroGoodAtEnd);
    if (flagName == "WifeLeavingYou") return sol::make_object(lua, pMaster->WifeLeavingYou);
    if (flagName == "WifeLeftYou") return sol::make_object(lua, pMaster->WifeLeftYou);
    if (flagName == "StartNextScene") return sol::make_object(lua, pMaster->StartNextScene);
    if (flagName == "ScorpionsDestroyed") return sol::make_object(lua, pMaster->ScorpionsDestroyed);
    if (flagName == "ScorpionsDestroyedCutscenePlayed") return sol::make_object(lua, pMaster->ScorpionsDestroyedCutscenePlayed);
    if (flagName == "SkillTrainingStarted") return sol::make_object(lua, pMaster->SkillTrainingStarted);
    if (flagName == "WillTrainingStarted") return sol::make_object(lua, pMaster->WillTrainingStarted);
    if (flagName == "MovingDummiesNeeded") return sol::make_object(lua, pMaster->MovingDummiesNeeded);
    if (flagName == "MeleeApprenticeNeededForCutscene") return sol::make_object(lua, pMaster->MeleeApprenticeNeededForCutscene);
    if (flagName == "GuildWarningOccuring") return sol::make_object(lua, pMaster->GuildWarningOccuring);
    if (flagName == "SkillTestOccuring") return sol::make_object(lua, pMaster->SkillTestOccuring);
    if (flagName == "WillTestOccuring") return sol::make_object(lua, pMaster->WillTestOccuring);
    if (flagName == "SkillRepeating") return sol::make_object(lua, pMaster->SkillRepeating);
    if (flagName == "SkillRepeatKnown") return sol::make_object(lua, pMaster->SkillRepeatKnown);
    if (flagName == "SkillDummyReset") return sol::make_object(lua, pMaster->SkillDummyReset);
    if (flagName == "HeroTakingGuildTest") return sol::make_object(lua, pMaster->HeroTakingGuildTest);
    if (flagName == "SwordInTheStoneComplete") return sol::make_object(lua, pMaster->SwordInTheStoneComplete);
    if (flagName == "AmbushTradersAllGuardsDead") return sol::make_object(lua, pMaster->AmbushTradersAllGuardsDead);
    if (flagName == "AmbushTradersAllTradersDead") return sol::make_object(lua, pMaster->AmbushTradersAllTradersDead);
    if (flagName == "AmbushTradersSpyDead") return sol::make_object(lua, pMaster->AmbushTradersSpyDead);
    if (flagName == "StruckDealWithLadyGrey") return sol::make_object(lua, pMaster->StruckDealWithLadyGrey);
    if (flagName == "HeroExposedLadyGrey") return sol::make_object(lua, pMaster->HeroExposedLadyGrey);
    if (flagName == "HeroMarriedLadyGrey") return sol::make_object(lua, pMaster->HeroMarriedLadyGrey);
    if (flagName == "BountyHuntWithinTimeLimit") return sol::make_object(lua, pMaster->BountyHuntWithinTimeLimit);
    if (flagName == "BountyHuntDecapitation") return sol::make_object(lua, pMaster->BountyHuntDecapitation);
    if (flagName == "BountyHuntTimeLimitExceeded") return sol::make_object(lua, pMaster->BountyHuntTimeLimitExceeded);
    if (flagName == "BreakSiegeFinished") return sol::make_object(lua, pMaster->BreakSiegeFinished);
    if (flagName == "WhiteBalverineFinished") return sol::make_object(lua, pMaster->WhiteBalverineFinished);
    if (flagName == "MadBomberFinished") return sol::make_object(lua, pMaster->MadBomberFinished);
    if (flagName == "BodyGuardsMustStandAndWait") return sol::make_object(lua, pMaster->BodyGuardsMustStandAndWait);
    if (flagName == "BodyGuardsInLimbo") return sol::make_object(lua, pMaster->BodyGuardsInLimbo);
    if (flagName == "WifeHeroFlirting") return sol::make_object(lua, pMaster->WifeHeroFlirting);
    if (flagName == "PrisonRaceWonByHero") return sol::make_object(lua, pMaster->PrisonRaceWonByHero);
    if (flagName == "WasPrisonRaceEverWonByHero") return sol::make_object(lua, pMaster->WasPrisonRaceEverWonByHero);
    if (flagName == "PrisonKeyStolenByHero") return sol::make_object(lua, pMaster->PrisonKeyStolenByHero);
    if (flagName == "BooksPreviouslyOpened1") return sol::make_object(lua, pMaster->BooksPreviouslyOpened[0]);
    if (flagName == "BooksPreviouslyOpened2") return sol::make_object(lua, pMaster->BooksPreviouslyOpened[1]);
    if (flagName == "BooksPreviouslyOpened3") return sol::make_object(lua, pMaster->BooksPreviouslyOpened[2]);
    if (flagName == "PrisonTorturedBefore") return sol::make_object(lua, pMaster->PrisonTorturedBefore);
    if (flagName == "PrisonGameDoneBefore") return sol::make_object(lua, pMaster->PrisonGameDoneBefore);
    if (flagName == "HeroDollsScriptUsingTeacher") return sol::make_object(lua, pMaster->HeroDollsScriptUsingTeacher);
    if (flagName == "TimeAdvancePointTriggered") return sol::make_object(lua, pMaster->TimeAdvancePointTriggered);
    if (flagName == "OFBRCratesStolen") return sol::make_object(lua, pMaster->OFBRCratesStolen);
    if (flagName == "OFBR_NoCratesWereStolen") return sol::make_object(lua, pMaster->OFBR_NoCratesWereStolen);
    if (flagName == "DarkwoodAllTradersAlive") return sol::make_object(lua, pMaster->DarkwoodAllTradersAlive);
    if (flagName == "MadBomberNoBombsExplode") return sol::make_object(lua, pMaster->MadBomberNoBombsExplode);
    if (flagName == "HobbeCavePerpertratorKilled") return sol::make_object(lua, pMaster->HobbeCavePerpertratorKilled);
    if (flagName == "HobbeCaveBoyUnharmed") return sol::make_object(lua, pMaster->HobbeCaveBoyUnharmed);
    if (flagName == "BanditCampKillNoBandits") return sol::make_object(lua, pMaster->BanditCampKillNoBandits);
    if (flagName == "BreakSiegeNoAssistance") return sol::make_object(lua, pMaster->BreakSiegeNoAssistance);
    if (flagName == "BreakSiegeKilledLeader") return sol::make_object(lua, pMaster->BreakSiegeKilledLeader);
    if (flagName == "OrchardFarmBanditKilled") return sol::make_object(lua, pMaster->OrchardFarmBanditKilled);
    if (flagName == "OrchardFarmGuardKilled") return sol::make_object(lua, pMaster->OrchardFarmGuardKilled);
    if (flagName == "BanditCampKillManyBandits") return sol::make_object(lua, pMaster->BanditCampKillManyBandits);
    if (flagName == "HobbeContestKillMoreHobbes") return sol::make_object(lua, pMaster->HobbeContestKillMoreHobbes);
    if (flagName == "GraveyardKillAllInnocents") return sol::make_object(lua, pMaster->GraveyardKillAllInnocents);
    if (flagName == "HangingTreeBanditKilled") return sol::make_object(lua, pMaster->HangingTreeBanditKilled);
    if (flagName == "HangingTreeGuardKilled") return sol::make_object(lua, pMaster->HangingTreeGuardKilled);
    if (flagName == "TCGKillNoBandits") return sol::make_object(lua, pMaster->TCGKillNoBandits);
    if (flagName == "TCGMadeTimeLimit") return sol::make_object(lua, pMaster->TCGMadeTimeLimit);
    if (flagName == "TCEKeepBanditFollowerAlive") return sol::make_object(lua, pMaster->TCEKeepBanditFollowerAlive);
    if (flagName == "TCEMadeTimeLimit") return sol::make_object(lua, pMaster->TCEMadeTimeLimit);
    if (flagName == "RansomVictimVictimNoDamage") return sol::make_object(lua, pMaster->RansomVictimVictimNoDamage);
    if (flagName == "RansomVictimKidnappersKilled") return sol::make_object(lua, pMaster->RansomVictimKidnappersKilled);
    if (flagName == "RansomVictimSaveVictim") return sol::make_object(lua, pMaster->RansomVictimSaveVictim);
    if (flagName == "RansomVictimHaveVictimKilled") return sol::make_object(lua, pMaster->RansomVictimHaveVictimKilled);
    if (flagName == "MinionCampBriarNoDamage") return sol::make_object(lua, pMaster->MinionCampBriarNoDamage);
    if (flagName == "FireHeartFreeAllPrisoners") return sol::make_object(lua, pMaster->FireHeartFreeAllPrisoners);
    if (flagName == "FireHeartKillAllPrisoners") return sol::make_object(lua, pMaster->FireHeartKillAllPrisoners);
    if (flagName == "TCETimeLimitBoastTaken") return sol::make_object(lua, pMaster->TCETimeLimitBoastTaken);
    if (flagName == "TCGTimeLimitBoastTaken") return sol::make_object(lua, pMaster->TCGTimeLimitBoastTaken);
    if (flagName == "HeroWoreMask") return sol::make_object(lua, pMaster->HeroWoreMask);
    if (flagName == "SingingStonesInSync") return sol::make_object(lua, pMaster->SingingStonesInSync);

    // --- Integer Flags ---
    if (flagName == "PostSavePosition") return sol::make_object(lua, static_cast<int>(pMaster->PostSavePosition));
    if (flagName == "HenchmanCommentaryState") return sol::make_object(lua, pMaster->HenchmanCommentaryState);
    if (flagName == "HenchmanOverrideState") return sol::make_object(lua, pMaster->HenchmanOverrideState);
    if (flagName == "RandomEntityVillagerMax") return sol::make_object(lua, pMaster->RandomEntityVillagerMax);
    if (flagName == "RandomEntityGuardMax") return sol::make_object(lua, pMaster->RandomEntityGuardMax);
    if (flagName == "RandomEntityBanditMax") return sol::make_object(lua, pMaster->RandomEntityBanditMax);
    if (flagName == "RandomEntityVillagerCount") return sol::make_object(lua, pMaster->RandomEntityVillagerCount);
    if (flagName == "RandomEntityGuardCount") return sol::make_object(lua, pMaster->RandomEntityGuardCount);
    if (flagName == "RandomEntityBanditCount") return sol::make_object(lua, pMaster->RandomEntityBanditCount);
    if (flagName == "ArcheryStateCurrent") return sol::make_object(lua, pMaster->ArcheryStateCurrent);
    if (flagName == "ArcheryStateRequired") return sol::make_object(lua, pMaster->ArcheryStateRequired);
    if (flagName == "ArcheryHighScore") return sol::make_object(lua, pMaster->ArcheryHighScore);
    if (flagName == "OrchardFarmRaidLastCompleted") return sol::make_object(lua, pMaster->OrchardFarmRaidLastCompleted);
    if (flagName == "OrchardFarmTraderEscortCounter") return sol::make_object(lua, pMaster->OrchardFarmTraderEscortCounter);
    if (flagName == "DeliveredSoul") return sol::make_object(lua, pMaster->DeliveredSoul);
    if (flagName == "HeroDrunkness") return sol::make_object(lua, pMaster->HeroDrunkness);
    if (flagName == "JackBossBattleResult") return sol::make_object(lua, pMaster->JackBossBattleResult);
    if (flagName == "CurrentScene") return sol::make_object(lua, pMaster->CurrentScene);
    if (flagName == "CurrentChapter") return sol::make_object(lua, pMaster->CurrentChapter);
    if (flagName == "SkillScore") return sol::make_object(lua, pMaster->SkillScore);
    if (flagName == "HighestSkillScore") return sol::make_object(lua, pMaster->HighestSkillScore);
    if (flagName == "WillScore") return sol::make_object(lua, pMaster->WillScore);
    if (flagName == "GlobalMeleeGrade") return sol::make_object(lua, pMaster->GlobalMeleeGrade);
    if (flagName == "GlobalSkillGrade") return sol::make_object(lua, pMaster->GlobalSkillGrade);
    if (flagName == "GlobalWillGrade") return sol::make_object(lua, pMaster->GlobalWillGrade);
    if (flagName == "AmbushTradersKillCount") return sol::make_object(lua, pMaster->AmbushTradersKillCount);
    if (flagName == "AmbushTradersBanditHireCount") return sol::make_object(lua, pMaster->AmbushTradersBanditHireCount);
    if (flagName == "AchievementsWorthyOfSong") return sol::make_object(lua, pMaster->AchievementsWorthyOfSong);
    if (flagName == "PrisonRaceNumber") return sol::make_object(lua, pMaster->PrisonRaceNumber);
    if (flagName == "MaxChickenKickingScore") return sol::make_object(lua, pMaster->MaxChickenKickingScore);
    if (flagName == "StoryTellerSpecialStories") return sol::make_object(lua, pMaster->StoryTellerSpecialStories);
    if (flagName == "StoryTellerToldSpecialStories") return sol::make_object(lua, pMaster->StoryTellerToldSpecialStories);
    if (flagName == "HobbeCaveInnocentSacrificed") return sol::make_object(lua, pMaster->HobbeCaveInnocentSacrificed);
    if (flagName == "DarkwoodPickpocketedAllTraders") return sol::make_object(lua, pMaster->DarkwoodPickpocketedAllTraders);

    // --- Float Flags ---
    if (flagName == "RandomEntityVillagerPercentage") return sol::make_object(lua, pMaster->RandomEntityVillagerPercentage);
    if (flagName == "RandomEntityGuardPercentage") return sol::make_object(lua, pMaster->RandomEntityGuardPercentage);
    if (flagName == "RandomEntityBanditPercentage") return sol::make_object(lua, pMaster->RandomEntityBanditPercentage);
    if (flagName == "VillagerAngryRating") return sol::make_object(lua, pMaster->VillagerAngryRating);

    // --- String Flags ---
    if (flagName == "TeddySolution") {
        const char* value = (pMaster->TeddySolution.pStringData && CCharString_ToConstChar_API)
            ? CCharString_ToConstChar_API(&pMaster->TeddySolution) : "";
        return sol::make_object(lua, std::string(value ? value : ""));
    }

    Log("! GetMasterGameState WARNING: Unknown flag requested: '" + flagName + "'");
    return sol::make_object(lua, sol::nil);
}

void LuaQuestState::SetThingPersistent(CScriptThing* pThing, bool isPersistent) {
    if (m_pGameInterface && SetThingPersistent_API && pThing) {
        bool isNull = true;
        if (pThing->pVTable) {
            CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
            if (pVTable->IsNull) isNull = pVTable->IsNull(pThing);
        }
        if (!isNull) {
            SetThingPersistent_API(m_pGameInterface, pThing, isPersistent);
        }
        else {
            Log("!!! ERROR: SetThingPersistent - pThing is null or invalid!");
        }
    }
    else {
        if (!pThing) Log("!!! ERROR: SetThingPersistent - pThing is null!");
        else Log("!!! ERROR: SetThingPersistent - API function not available!");
    }
}

void LuaQuestState::EntitySetAsDamageable(CScriptThing* pThing, bool isDamageable) {
    // Note: CScriptThing* is passed directly to the API
    if (m_pGameInterface && EntitySetAsDamageable_API && pThing) {
        bool isNull = true;
        if (pThing->pVTable) {
            CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
            if (pVTable->IsNull) isNull = pVTable->IsNull(pThing);
        }
        if (!isNull) {
            EntitySetAsDamageable_API(m_pGameInterface, pThing, isDamageable);
        }
        else {
            Log("!!! ERROR: EntitySetAsDamageable - pThing is null or invalid!");
        }
    }
    else {
        if (!pThing) Log("!!! ERROR: EntitySetAsDamageable - pThing is null!");
        else Log("!!! ERROR: EntitySetAsDamageable - API function not available!");
    }
}

void LuaQuestState::EntitySetAsToAddToComboMultiplierWhenHit(CScriptThing* pThing, bool doesAddToCombo) {
    // Note: CScriptThing* is passed directly to the API
    if (m_pGameInterface && EntitySetAsToAddToComboMultiplierWhenHit_API && pThing) {
        bool isNull = true;
        if (pThing->pVTable) {
            CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
            if (pVTable->IsNull) isNull = pVTable->IsNull(pThing);
        }
        if (!isNull) {
            EntitySetAsToAddToComboMultiplierWhenHit_API(m_pGameInterface, pThing, doesAddToCombo);
        }
        else {
            Log("!!! ERROR: EntitySetAsToAddToComboMultiplierWhenHit - pThing is null or invalid!");
        }
    }
    else {
        if (!pThing) Log("!!! ERROR: EntitySetAsToAddToComboMultiplierWhenHit - pThing is null!");
        else Log("!!! ERROR: EntitySetAsToAddToComboMultiplierWhenHit - API function not available!");
    }
}

void LuaQuestState::SetRegionExitAsActive(CScriptThing* pThing, bool isActive) {
    // Note: CScriptThing* is passed directly to the API
    if (m_pGameInterface && SetRegionExitAsActive_API && pThing) {
        bool isNull = true;
        if (pThing->pVTable) {
            CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
            if (pVTable->IsNull) isNull = pVTable->IsNull(pThing);
        }
        if (!isNull) {
            // Assuming pThing points to a valid Region Exit Thing
            SetRegionExitAsActive_API(m_pGameInterface, pThing, isActive);
        }
        else {
            Log("!!! ERROR: SetRegionExitAsActive - pThing is null or invalid!");
        }
    }
    else {
        if (!pThing) Log("!!! ERROR: SetRegionExitAsActive - pThing is null!");
        else Log("!!! ERROR: SetRegionExitAsActive - API function not available!");
    }
}

float LuaQuestState::GetHeroFatness() {
    if (m_pGameInterface && GetHeroFatness_API) {
        return GetHeroFatness_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: GetHeroFatness - API function not available!");
        return 0.0f; // Return a default value
    }
}

void LuaQuestState::EntityFadeOut(CScriptThing* pThing, float duration) {
    // Note: CScriptThing* is passed directly to the API
    if (m_pGameInterface && EntityFadeOut_API && pThing) {
        bool isNull = true;
        if (pThing->pVTable) {
            CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
            if (pVTable->IsNull) isNull = pVTable->IsNull(pThing);
        }
        if (!isNull) {
            EntityFadeOut_API(m_pGameInterface, pThing, duration);
        }
        else {
            Log("!!! ERROR: EntityFadeOut - pThing is null or invalid!");
        }
    }
    else {
        if (!pThing) Log("!!! ERROR: EntityFadeOut - pThing is null!");
        else Log("!!! ERROR: EntityFadeOut - API function not available!");
    }
}

void LuaQuestState::OpenDoor(CScriptThing* pThing) {
    // Note: CScriptThing* is passed directly to the API
    if (m_pGameInterface && OpenDoor_API && pThing) {
        bool isNull = true;
        if (pThing->pVTable) {
            CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
            if (pVTable->IsNull) isNull = pVTable->IsNull(pThing);
        }
        if (!isNull) {
            // Assuming pThing points to a valid Door Thing
            OpenDoor_API(m_pGameInterface, pThing);
        }
        else {
            Log("!!! ERROR: OpenDoor - pThing is null or invalid!");
        }
    }
    else {
        if (!pThing) Log("!!! ERROR: OpenDoor - pThing is null!");
        else Log("!!! ERROR: OpenDoor - API function not available!");
    }
}

// ---- ForgeFSE additions (DB-verified vtable slots + demangled signatures, 2026-07-21) ----
// Shared null/vtable-validity check, mirroring the OpenDoor guard above.
static bool ForgeThingIsValid(CScriptThing* pThing) {
    if (!pThing) return false;
    if (pThing->pVTable) {
        CScriptThingVTable* pVTable = reinterpret_cast<CScriptThingVTable*>(pThing->pVTable);
        if (pVTable->IsNull) return !pVTable->IsNull(pThing);
    }
    return true;
}

void LuaQuestState::CloseDoor(CScriptThing* pThing) {
    if (m_pGameInterface && CloseDoor_API && ForgeThingIsValid(pThing)) CloseDoor_API(m_pGameInterface, pThing);
    else Log("!!! ERROR: CloseDoor - null thing or API unavailable!");
}

void LuaQuestState::OpenHouseDoors(CScriptThing* pHouse) {
    if (m_pGameInterface && OpenHouseDoors_API && ForgeThingIsValid(pHouse)) OpenHouseDoors_API(m_pGameInterface, pHouse);
    else Log("!!! ERROR: OpenHouseDoors - null thing or API unavailable!");
}

void LuaQuestState::CloseHouseDoors(CScriptThing* pHouse) {
    if (m_pGameInterface && CloseHouseDoors_API && ForgeThingIsValid(pHouse)) CloseHouseDoors_API(m_pGameInterface, pHouse);
    else Log("!!! ERROR: CloseHouseDoors - null thing or API unavailable!");
}

void LuaQuestState::JamDoor(CScriptThing* pDoor) {
    if (m_pGameInterface && JamDoor_API && ForgeThingIsValid(pDoor)) JamDoor_API(m_pGameInterface, pDoor);
    else Log("!!! ERROR: JamDoor - null thing or API unavailable!");
}

void LuaQuestState::SetHouseOwnedByPlayer(CScriptThing* pHouse, bool bOwned, bool bRented) {
    if (m_pGameInterface && SetHouseOwnedByPlayer_API && ForgeThingIsValid(pHouse)) SetHouseOwnedByPlayer_API(m_pGameInterface, pHouse, bOwned, bRented);
    else Log("!!! ERROR: SetHouseOwnedByPlayer - null thing or API unavailable!");
}

int LuaQuestState::AddNewConversation(CScriptThing* pThing, bool b1, bool b2) {
    if (m_pGameInterface && AddNewConversation_API && ForgeThingIsValid(pThing)) return AddNewConversation_API(m_pGameInterface, pThing, b1, b2);
    Log("!!! ERROR: AddNewConversation - null thing or API unavailable!");
    return -1; // invalid conversation id
}

void LuaQuestState::AddPersonToConversation(int conversationID, CScriptThing* pPerson) {
    if (m_pGameInterface && AddPersonToConversation_API && ForgeThingIsValid(pPerson)) AddPersonToConversation_API(m_pGameInterface, conversationID, pPerson);
    else Log("!!! ERROR: AddPersonToConversation - null thing or API unavailable!");
}

void LuaQuestState::HeroGoFishing(bool bGo) {
    if (m_pGameInterface && HeroGoFishing_API) HeroGoFishing_API(m_pGameInterface, bGo);
    else Log("!!! ERROR: HeroGoFishing - API unavailable!");
}

void LuaQuestState::HeroGoDigging() {
    if (m_pGameInterface && HeroGoDigging_API) HeroGoDigging_API(m_pGameInterface);
    else Log("!!! ERROR: HeroGoDigging - API unavailable!");
}

void LuaQuestState::HeroStopDigging() {
    if (m_pGameInterface && HeroStopDigging_API) HeroStopDigging_API(m_pGameInterface);
    else Log("!!! ERROR: HeroStopDigging - API unavailable!");
}

bool LuaQuestState::IsDPadButtonHeldForExpression(const std::string& expressionName) {
    if (m_pGameInterface && IsDPadButtonHeldForExpression_API) {
        FableString expression(expressionName.c_str());
        return IsDPadButtonHeldForExpression_API(m_pGameInterface, expression);
    }
    Log("!!! ERROR: IsDPadButtonHeldForExpression - API unavailable!");
    return false;
}

bool LuaQuestState::IsFishingSpotEnabled(CScriptThing* pSpot) {
    if (m_pGameInterface && IsFishingSpotEnabled_API && pSpot)
        return IsFishingSpotEnabled_API(m_pGameInterface, pSpot);
    Log("!!! ERROR: IsFishingSpotEnabled - API unavailable or spot is null!");
    return false;
}

void LuaQuestState::DisableFishingSpot(CScriptThing* pSpot) {
    if (m_pGameInterface && DisableFishingSpot_API && pSpot)
        DisableFishingSpot_API(m_pGameInterface, pSpot);
    else Log("!!! ERROR: DisableFishingSpot - API unavailable or spot is null!");
}

void LuaQuestState::UpdateFishWeight(CScriptThing* pSpot) {
    if (m_pGameInterface && UpdateFishWeight_API && pSpot)
        UpdateFishWeight_API(m_pGameInterface, pSpot);
    else Log("!!! ERROR: UpdateFishWeight - API unavailable or spot is null!");
}

void LuaQuestState::SetToDisplayTutorialsDuringCutscenes(bool display) {
    if (m_pGameInterface && SetToDisplayTutorialsDuringCutscenes_API)
        SetToDisplayTutorialsDuringCutscenes_API(m_pGameInterface, display);
    else Log("!!! ERROR: SetToDisplayTutorialsDuringCutscenes - API unavailable!");
}

void LuaQuestState::SetCutsceneMode(bool inCutscene, bool unknown) {
    if (m_pGameInterface && SetCutsceneMode_API)
        SetCutsceneMode_API(m_pGameInterface, inCutscene, unknown);
    else Log("!!! ERROR: SetCutsceneMode - API unavailable!");
}

int LuaQuestState::GetHeroFishingLevel() {
    if (m_pGameInterface && GetHeroFishingLevel_API) return GetHeroFishingLevel_API(m_pGameInterface);
    Log("!!! ERROR: GetHeroFishingLevel - API unavailable!");
    return -1;
}

int LuaQuestState::GetHeroGold() {
    if (m_pGameInterface && GetHeroGold_API) {
        return GetHeroGold_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: GetHeroGold - API function not available!");
        return -1; // Return an error indicator
    }
}

// ForgeFSE addition: returns the engine language id (e.g. 9 = TEXT_ENGLISH_MAIN,
// 0xC = TEXT_FRENCH_MAIN). Wraps NLocalisation::GetLanguage @ retail 0x00415070
// (no-arg getter; engine caches the result in a global on first call).
int LuaQuestState::GetGameLanguage() {
    if (GetLanguage_API) {
        return static_cast<int>(GetLanguage_API());
    }
    else {
        Log("!!! ERROR: GetGameLanguage - API function not available!");
        return -1; // Return an error indicator
    }
}

// ForgeFSE addition: obtain the live CWorldMap* from the CGameScriptInterface.
// CORRECTED 2026-07-20 (was slot 13 / 0x34 -> crash 0xC0000005 in the vtable path).
// Evidence: the RETAIL hero region-transition body itself,
// CGameScriptInterface::EntityTeleportToPosition @0x0089B780
// (FableTLC ghidra_out/scriptvm_decomp.c ~L18147-18221), resolves the CWorldMap
// FOUR times as:
//     pWorld    = *(CWorld**)(pInterface + 4);
//     pWorldMap = (*(void**(**)())( *(void***)pWorld + 0x30/4 ))();   // slot 12
// i.e. CWorld vtable byte offset 0x30 == slot 12, NOT 0x34/slot 13. Every proven
// consumer in the transition path (GetRegionNumberMapIsIn, StartMovePlayerPos) is fed
// the slot-12 result. Slot 13 (0x34) is a DIFFERENT vtable entry; calling it as the
// world-map getter is what access-violates inside ResolveWorldMap (WER offset +0x200599).
// ForgeFSE addition: obtain the live CWorld* from the CGameScriptInterface.
// The CWorld is stored directly at pInterface+4 (proven: EntityTeleportToPosition reads
// pWorld = *(CWorld**)(pInterface + 4) before calling its vtable[12] world-map getter).
// This is the `this` for CWorld::SetAsLoadingRegion / HandleMoveHeroToRegionGameEvent.
static void* ResolveWorld(CGameScriptInterfaceBase* pInterface) {
    if (!pInterface) return nullptr;
    return *reinterpret_cast<void**>(reinterpret_cast<char*>(pInterface) + 4);
}

static void* ResolveWorldMap(CGameScriptInterfaceBase* pInterface) {
    if (!pInterface) return nullptr;
    // pInterface layout: [0]=pVTable, [+4]=CWorld*
    void* pWorld = ResolveWorld(pInterface);
    if (!pWorld) return nullptr;
    void** pWorldVTable = *reinterpret_cast<void***>(pWorld);
    if (!pWorldVTable) return nullptr;
    typedef void* (__thiscall* tGetWorldMap)(void* pWorld);
    tGetWorldMap pGetWorldMap = reinterpret_cast<tGetWorldMap>(pWorldVTable[12]); // 0x30 (proven)
    if (!pGetWorldMap) return nullptr;
    return pGetWorldMap(pWorld);
}

struct StaticBindingProbeResult {
    bool ok;
    long count;
    unsigned long state;
    unsigned long bankIndex;
    float bounds[6];
};

static StaticBindingProbeResult ProbeStaticBinding(void* pWorldMap, int mapSlot) {
    StaticBindingProbeResult out = {};
    out.bankIndex = 0xffffffffUL;
    __try {
        char* wm = reinterpret_cast<char*>(pWorldMap);
        void** begin = *reinterpret_cast<void***>(wm + 0x20);
        void** end = *reinterpret_cast<void***>(wm + 0x24);
        out.count = static_cast<long>(end - begin);
        if (!begin || mapSlot <= 0 || mapSlot > out.count) return out;
        char* map = reinterpret_cast<char*>(begin[mapSlot - 1]);
        out.state = *reinterpret_cast<unsigned long*>(map + 0x20);
        const float* bounds = reinterpret_cast<float*>(map + 0x24);
        for (int i = 0; i < 6; ++i) out.bounds[i] = bounds[i];
        void* asyncEntry = *reinterpret_cast<void**>(map + 0x3c);
        if (asyncEntry) out.bankIndex = *reinterpret_cast<unsigned long*>(reinterpret_cast<char*>(asyncEntry) + 0x8);
        out.ok = true;
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        out.ok = false;
    }
    return out;
}

// ForgeFSE addition: relocate the hero to an arbitrary map/region.
//
// This deliberately uses the proven synchronous bridge rather than relying on the
// retail EntityTeleportToPosition event path to finish an isolated custom region.
// The bridge is the sequence recovered and validated in the 2026-07-20 live run:
//   GetRegionNumberMapIsIn -> LoadRegion(immediate, force) -> SetPlayerPos
//   -> ActivateNavMap -> EntityTeleportToPosition.
// LoadRegion makes the destination region resident; SetPlayerPos performs the world
// map state handoff; ActivateNavMap makes the destination navigation graph live; and
// the retail hero helper finally updates the hero thing. Each step is retained here
// because the later steps are not interchangeable with the earlier ones for a distant
// map, and the helper alone was disproven by the owner-corrected ForgeTest probe.
void LuaQuestState::GoToMapSlot(int mapSlot, float x, float y, float z) {
    GoToMapSlotPrepare(mapSlot, x, y, z);

    if (!m_pGameInterface || !EntityTeleportToPosition_API || !GetHero_API) {
        return;
    }
    CScriptThing* pHero = GetHero_API(m_pGameInterface);
    if (!pHero) {
        Log("!!! WARNING: GoToMapSlot - GetHero returned null; hero not teleported.");
        return;
    }

    // Keep the historical single-call behavior for existing scripts. New
    // scripts that need a frame boundary should call GoToMapSlotPrepare,
    // yield, then invoke EntityTeleportToPosition explicitly.
    void* pWorldMap = ResolveWorldMap(m_pGameInterface);
    if (!pWorldMap || !CWorldMap_GetGroundSizeZAt_API) {
        return;
    }
    const C3DVector groundProbe = { x, y, 0.0f };
    const float groundZ = CWorldMap_GetGroundSizeZAt_API(
        pWorldMap, reinterpret_cast<const float*>(&groundProbe));
    if (groundZ <= -9999.0f) {
        return;
    }
    const C3DVector pos = { x, y, groundZ + 1.0f };
    EntityTeleportToPosition_API(m_pGameInterface, pHero, &pos, 0.0f, true, true);
    Log("GoToMapSlot: retail EntityTeleportToPosition completed.");
}

// ForgeFSE diagnostic: log the live region-vector size. This decides the
// region-142 blocker (SESSION_HANDOFF_2026-08-11_64X64.md): if the count is the
// vanilla 141 while the .bwd stores 142/143, the runtime vector is truncated/capped
// and a dedicated region past the vanilla count can never resolve; if it is 143 the
// data is loaded and the miss is a different mechanism. Region vector: begin at
// CWorldMap+0x2c, end at +0x30, element stride 0x58.
// POD-only SEH helpers (C2712: __try cannot share a function with C++ objects
// that need unwinding, so the raw reads live here and the caller does the logging).
struct RegionVectorProbe {
    bool ok;
    long regionCount;
    unsigned long beginAddr;
    unsigned long endAddr;
    long mapSpanBytes;
};

static RegionVectorProbe ReadRegionVector(void* pWorldMap) {
    RegionVectorProbe out = {};
    __try {
        char* wm = reinterpret_cast<char*>(pWorldMap);
        char* begin = *reinterpret_cast<char**>(wm + 0x2c);
        char* end = *reinterpret_cast<char**>(wm + 0x30);
        char* mapBegin = *reinterpret_cast<char**>(wm + 0x20);
        char* mapEnd = *reinterpret_cast<char**>(wm + 0x24);
        out.beginAddr = reinterpret_cast<unsigned long>(begin);
        out.endAddr = reinterpret_cast<unsigned long>(end);
        out.regionCount = begin ? static_cast<long>((end - begin) / 0x58) : -1;
        out.mapSpanBytes = static_cast<long>(mapEnd - mapBegin);
        out.ok = true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        out.ok = false;
    }
    return out;
}

static bool CallGetRegionNumber(void* pWorldMap, int mapSlot, long& regionOut) {
    __try {
        regionOut = CWorldMap_GetRegionNumberMapIsIn_API(pWorldMap, mapSlot);
        return true;
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        return false;
    }
}

int LuaQuestState::ProbeRegionVectorSize() {
    if (!m_pGameInterface) {
        Log("!!! ERROR: ProbeRegionVectorSize - game interface not available!");
        return -1;
    }
    void* pWorldMap = ResolveWorldMap(m_pGameInterface);
    if (!pWorldMap) {
        Log("!!! ERROR: ProbeRegionVectorSize - could not resolve CWorldMap.");
        return -1;
    }
    const RegionVectorProbe p = ReadRegionVector(pWorldMap);
    if (!p.ok) {
        Log("!!! ERROR: ProbeRegionVectorSize - SEH reading region vector.");
        return -1;
    }
    std::stringstream ss;
    ss << "ProbeRegionVectorSize: regions begin=0x" << std::hex << p.beginAddr
       << " end=0x" << p.endAddr << std::dec << " stride=0x58 -> count="
       << p.regionCount << " ; maps span=" << p.mapSpanBytes << " bytes";
    Log(ss.str());
    return static_cast<int>(p.regionCount);
}

// ForgeFSE diagnostic: resolve which region owns a map slot, via the retail
// CWorldMap::GetRegionNumberMapIsIn @0x4FC190 (0 = not found). Pair with
// Runtime evidence has measured 142 loaded regions for the appended ForgeTest
// world. Pair this count with the ownership result; 142 + owner 0 indicates a
// membership/lookup failure, while owner 142 proves the dedicated region.
int LuaQuestState::ProbeRegionContainsSlot(int mapSlot) {
    if (!m_pGameInterface || !CWorldMap_GetRegionNumberMapIsIn_API) {
        Log("!!! ERROR: ProbeRegionContainsSlot - APIs unavailable.");
        return -1;
    }
    void* pWorldMap = ResolveWorldMap(m_pGameInterface);
    if (!pWorldMap) {
        Log("!!! ERROR: ProbeRegionContainsSlot - could not resolve CWorldMap.");
        return -1;
    }
    long region = 0;
    if (!CallGetRegionNumber(pWorldMap, mapSlot, region)) {
        Log("!!! ERROR: ProbeRegionContainsSlot - SEH in GetRegionNumberMapIsIn.");
        return -1;
    }
    std::stringstream ss;
    ss << "ProbeRegionContainsSlot(" << mapSlot
       << "): GetRegionNumberMapIsIn -> region " << region << " (0=not found).";
    Log(ss.str());
    return static_cast<int>(region);
}

void LuaQuestState::GoToMapSlotPrepare(int mapSlot, float x, float y, float z) {
    if (!m_pGameInterface) {
        Log("!!! ERROR: GoToMapSlotPrepare - game interface not available!");
        return;
    }
    if (mapSlot <= 0) {
        Log("!!! ERROR: GoToMapSlotPrepare - invalid map slot (must be >= 1).");
        return;
    }
    if (!CWorldMap_GetRegionNumberMapIsIn_API || !CWorldMap_LoadRegion_API ||
        !CWorldMap_SetPlayerPos_API || !CWorldMap_ActivateNavMap_API ||
        !CWorldMap_GetGroundSizeZAt_API) {
        Log("!!! ERROR: GoToMapSlotPrepare - map transition APIs unavailable.");
        return;
    }

    void* pWorldMap = ResolveWorldMap(m_pGameInterface);
    if (!pWorldMap) {
        Log("!!! ERROR: GoToMapSlotPrepare - could not resolve CWorldMap.");
        return;
    }

    const long regionIndex =
        CWorldMap_GetRegionNumberMapIsIn_API(pWorldMap, mapSlot);
    {
        std::stringstream ss;
        ss << "GoToMapSlotPrepare: slot " << mapSlot << " -> resolved region "
           << regionIndex << " target (" << x << "," << y << "," << z << ").";
        Log(ss.str());
    }

    if (regionIndex <= 0) {
        Log("!!! ERROR: GoToMapSlotPrepare - destination slot has no owning region.");
        return;
    }

    CWorldMap_LoadRegion_API(pWorldMap, regionIndex, 0 /* immediate */, true /* force */);
    {
        std::stringstream ss;
        ss << "GoToMapSlotPrepare: CWorldMap::LoadRegion(region=" << regionIndex
           << ", immediate, force=true) returned.";
        Log(ss.str());
    }

    // The proven bridge snaps the destination to the loaded collision surface.
    // Query only after LoadRegion: before that point the target sub-map is not
    // resident and the engine returns its outside-map sentinel.
    const C3DVector groundProbe = { x, y, 0.0f };
    const float groundZ = CWorldMap_GetGroundSizeZAt_API(
        pWorldMap, reinterpret_cast<const float*>(&groundProbe));
    if (groundZ <= -9999.0f) {
        Log("!!! ERROR: GoToMapSlotPrepare - destination ground query returned outside-map sentinel.");
        return;
    }
    const C3DVector pos = { x, y, groundZ + 1.0f };
    {
        std::stringstream ss;
        ss << "GoToMapSlotPrepare: spawn Z resolved to " << pos.z
           << " (ground=" << groundZ << ", passed=" << z << ").";
        Log(ss.str());
    }
    CWorldMap_SetPlayerPos_API(pWorldMap, static_cast<long>(mapSlot),
                               reinterpret_cast<const float*>(&pos), 0 /* immediate */);
    Log("GoToMapSlotPrepare: CWorldMap::SetPlayerPos completed.");

    CWorldMap_ActivateNavMap_API(pWorldMap, static_cast<long>(mapSlot));
    Log("GoToMapSlotPrepare: CWorldMap::ActivateNavMap completed; hero move deferred.");
}

void LuaQuestState::GoToMapSlotRetailTransition(int mapSlot, float x, float y, float z) {
    if (!m_pGameInterface || !CWorld_SetAsLoadingRegion_API) {
        Log("!!! ERROR: GoToMapSlotRetailTransition - transition APIs unavailable.");
        return;
    }

    void* pWorld = ResolveWorld(m_pGameInterface);
    if (!pWorld) {
        Log("!!! ERROR: GoToMapSlotRetailTransition - could not resolve world.");
        return;
    }

    // This is the retail event vector. Do not ground-snap here: the target
    // region is not resident yet, so a pre-load ground query is meaningless.
    const C3DVector pos = { x, y, z };
    {
        std::stringstream ss;
        ss << "GoToMapSlotRetailTransition: slot " << mapSlot
           << " target (" << pos.x << "," << pos.y << "," << pos.z
           << ") flags=(p3=true, force=true, p5=true).";
        Log(ss.str());
    }
    const bool queued = QueueRetailRegionTransition(
        pWorld, reinterpret_cast<const float*>(&pos), mapSlot);
    Log(queued
        ? "GoToMapSlotRetailTransition: queued for the next engine update boundary."
        : "!!! ERROR: GoToMapSlotRetailTransition: queue request was rejected.");
}

void LuaQuestState::GoToMap(const std::string& mapScriptName, float x, float y, float z) {
    if (!m_pGameInterface || !CWorldMap_SetPlayerPos_API || !CWorldMap_GetMapNumberFromScriptName_API) {
        Log("!!! ERROR: GoToMap - API function not available!");
        return;
    }
    void* pWorldMap = ResolveWorldMap(m_pGameInterface);
    if (!pWorldMap) {
        Log("!!! ERROR: GoToMap - could not resolve CWorldMap from interface.");
        return;
    }
    FableString fsName(mapScriptName.c_str());
    long slot = CWorldMap_GetMapNumberFromScriptName_API(pWorldMap, fsName.get());
    if (slot <= 0) {
        std::stringstream ss;
        ss << "!!! ERROR: GoToMap - no map with script name '" << mapScriptName
           << "' (use GoToMapSlot for a known slot number).";
        Log(ss.str());
        return;
    }
    GoToMapSlot(static_cast<int>(slot), x, y, z);
}

// ForgeFSE addition: MAP-OPEN NAME-ALIAS (ForgeTest terrain white-out route 2, DIAGNOSTIC).
// Enables the OpenRetailStaticMap detour so the SOURCE map opens its bank/theme AS the DONOR.
// Ensures the trampoline is installed (idempotent) then sets the runtime alias pair.
void LuaQuestState::AliasMapResources(const std::string& sourceMapName, const std::string& donorMapName) {
    if (sourceMapName.empty() || donorMapName.empty()) {
        Log("!!! ERROR: AliasMapResources - both sourceMapName and donorMapName are required.");
        return;
    }
    if (!InstallMapResourceAliasHook()) {
        Log("!!! ERROR: AliasMapResources - could not install the OpenRetailStaticMap hook (see log).");
        return;
    }
    SetMapResourceAlias(sourceMapName, donorMapName);
    {
        std::stringstream ss;
        ss << "AliasMapResources: '" << sourceMapName << "' -> '" << donorMapName
           << "' ARMED. Call GoToMapSlot next, then ClearMapResourceAlias().";
        Log(ss.str());
    }
}

void LuaQuestState::ClearMapResourceAlias() {
    ::ClearMapResourceAlias();
    Log("ClearMapResourceAlias: map-open name-alias disabled (passthrough restored).");
}

void LuaQuestState::EntitySetThingAsEnemyOfThing(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {

    CScriptThing* pThing1 = spThing1.get();
    CScriptThing* pThing2 = spThing2.get();

    if (m_pGameInterface && EntitySetThingAsEnemyOfThing_API && pThing1 && pThing2) {
        EntitySetThingAsEnemyOfThing_API(m_pGameInterface, pThing1, pThing2);
    }
    else {
        if (!pThing1 || !pThing2) Log("!!! ERROR: EntitySetThingAsEnemyOfThing - One or both input Things are null!");
        else Log("!!! ERROR: EntitySetThingAsEnemyOfThing - API function not available!");
    }
}

void LuaQuestState::ChangeHeroHealthBy(float amount, bool canKill, bool showChange) {
    if (m_pGameInterface && ChangeHeroHealthBy_API) {
        ChangeHeroHealthBy_API(m_pGameInterface, amount, canKill, showChange);
    }
    else {
        Log("!!! ERROR: ChangeHeroHealthBy - API not available!");
    }
}

void LuaQuestState::CameraShake(float intensity, float duration) {
    if (m_pGameInterface && CameraShake_API) {
        CameraShake_API(m_pGameInterface, intensity, duration);
    }
    else {
        Log("!!! ERROR: CameraShake - API not available!");
    }
}

bool LuaQuestState::GetHeroHasMarried() {
    if (m_pGameInterface && GetHeroHasMarried_API) {
        return GetHeroHasMarried_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHasMarried - API not available!");
    return false;
}

bool LuaQuestState::GetHeroHasMurderedWife() {
    if (m_pGameInterface && GetHeroHasMurderedWife_API) {
        return GetHeroHasMurderedWife_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHasMurderedWife - API not available!");
    return false;
}

int LuaQuestState::GetHeroRenownLevel() {
    if (m_pGameInterface && GetHeroRenownLevel_API) {
        return GetHeroRenownLevel_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroRenownLevel - API not available!");
    return -1; // Indicate error
}

int LuaQuestState::GetHeroTitle() {
    if (m_pGameInterface && GetHeroTitle_API) {
        // Cast the enum return value directly to int for Lua
        return static_cast<int>(GetHeroTitle_API(m_pGameInterface));
    }
    Log("!!! ERROR: GetHeroTitle - API not available!");
    return -1; // Indicate error (assuming -1 is not a valid title ID)
}

sol::object LuaQuestState::MsgOnHeroCastSpell(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && MsgOnHeroCastSpell_API) {
        EHeroAbility abilityCasted = HERO_ABILITY_NULL; // Initialize output parameter
        bool messageTriggered = MsgOnHeroCastSpell_API(m_pGameInterface, &abilityCasted);
        if (messageTriggered) {
            // If the message is true, return the ability ID that was cast
            return sol::make_object(lua, static_cast<int>(abilityCasted));
        }
        else {
            // If the message is false, return nil
            return sol::make_object(lua, sol::nil);
        }
    }
    Log("!!! ERROR: MsgOnHeroCastSpell - API not available!");
    return sol::make_object(lua, sol::nil); // Return nil on error
}

void LuaQuestState::GiveHeroAbility(int abilityId, bool give) {
    if (m_pGameInterface && GiveHeroAbility_API) {
        // Cast the int from Lua back to the EHeroAbility enum
        EHeroAbility ability = static_cast<EHeroAbility>(abilityId);
        // Check if the casted value is within the valid range (optional but safer)
        if (ability >= HERO_ABILITY_NULL && ability < MAX_NUMBER_OF_HERO_ABILITIES) {
            GiveHeroAbility_API(m_pGameInterface, ability, give);
        }
        else {
            Log("!!! ERROR: GiveHeroAbility - Invalid Ability ID provided: " + std::to_string(abilityId));
        }
    }
    else {
        Log("!!! ERROR: GiveHeroAbility - API not available!");
    }
}

void LuaQuestState::StartMovieSequence(sol::this_state state) {
    Log("--- Starting Movie Sequence ---");

    // Check prerequisites
    if (!m_pGameInterface || !StartMovieSequence_API || !CBaseObject_Construct_API || !g_pMovieObjectVTable || !Game_malloc) {
        Log("!!! ERROR: StartMovieSequence - Prerequisites not met (API functions, VTable, or malloc missing)!");
        return;
    }

    lua_State* const luaState = state;
    CScriptGameResourceObjectMovieBase*& movieHandle = m_movieHandlesByLuaState[luaState];

    // A single retail invocation cannot start twice without destructing its local.
    if (movieHandle) {
        Log("!!! WARNING: StartMovieSequence called while a sequence is already active. Ignoring.");
        return;
    }

    // 1. Allocate memory for the handle ON THE HEAP
    movieHandle = static_cast<CScriptGameResourceObjectMovieBase*>(Game_malloc(sizeof(CScriptGameResourceObjectMovieBase)));
    if (!movieHandle) {
        m_movieHandlesByLuaState.erase(luaState);
        Log("!!! ERROR: StartMovieSequence - Failed to malloc movie handle!");
        return;
    }
    memset(movieHandle, 0, sizeof(CScriptGameResourceObjectMovieBase)); // Zero it out

    // 2. Construct the base object part
    CBaseObject_Construct_API(movieHandle);

    // 3. Set the VTable and initialize pImp (like in SpeakAndWait)
    movieHandle->pVTable = g_pMovieObjectVTable;
    movieHandle->pImp.Data = nullptr;
    movieHandle->pImp.Info = nullptr;

    // 4. Call the game API to start the sequence
    FableString classStr("");
    StartMovieSequence_API(m_pGameInterface, classStr, movieHandle);

    Log("--- Movie Sequence Started Successfully ---");
}

void LuaQuestState::EndMovieSequence(sol::this_state state) {
    Log("--- Ending Movie Sequence ---");

    lua_State* const luaState = state;
    const auto movie = m_movieHandlesByLuaState.find(luaState);
    if (movie == m_movieHandlesByLuaState.end() || !movie->second) {
        Log("!!! WARNING: EndMovieSequence called but no sequence is active. Ignoring.");
        return;
    }
    CScriptGameResourceObjectMovieBase* const movieHandle = movie->second;

    // Check prerequisites for cleanup
    if (!MovieResource_Destroy_API || !Game_free) {
        Log("!!! ERROR: EndMovieSequence - Cleanup prerequisites not met (API functions or free missing)!");
        // We might still try to free the memory, but cleanup will be incomplete.
        if (Game_free) Game_free(movieHandle);
        m_movieHandlesByLuaState.erase(movie); // Forget it even if cleanup is incomplete.
        return;
    }

    // 1. Invoke the exact retail derived destructor. Besides releasing pImp,
    // this restores the base vtable before chaining to CBaseObject::~CBaseObject.
    Log("    Destroying movie resource object...");
    MovieResource_Destroy_API(movieHandle);

    // 3. Free the heap memory we allocated
    Log("    Freeing movie handle memory...");
    Game_free(movieHandle);

    // 4. Nullify the pointer
    m_movieHandlesByLuaState.erase(movie);

    Log("--- Movie Sequence Ended Successfully ---");
}

void LuaQuestState::AbandonMovieSequence(lua_State* luaState) {
    const auto movie = m_movieHandlesByLuaState.find(luaState);
    if (movie == m_movieHandlesByLuaState.end() || !movie->second) {
        return;
    }

    Log("    [MovieRAII] Entity VM is closing with an active movie sequence; unwinding it.");
    if (m_pGameInterface && PauseAllNonScriptedEntities_API) {
        PauseAllNonScriptedEntities_API(m_pGameInterface, false);
    }

    CScriptGameResourceObjectMovieBase* const movieHandle = movie->second;
    if (MovieResource_Destroy_API) {
        MovieResource_Destroy_API(movieHandle);
    }
    if (Game_free) {
        Game_free(movieHandle);
    }
    m_movieHandlesByLuaState.erase(movie);
}

int LuaQuestState::StartAmbientConversation(const std::shared_ptr<CScriptThing>& spSpeaker, const std::shared_ptr<CScriptThing>& spListener,
    sol::optional<bool> soundIn2D, sol::optional<bool> playDuringCutscene)
{
    Log("--- StartAmbientConversation START ---");

    // 1. Check prerequisites
    if (!m_pGameInterface || !AddNewConversation_API || !AddPersonToConversation_API) {
        Log("!!! ERROR: StartAmbientConversation - Prerequisites not met (API functions missing)!");
        Log("--- StartAmbientConversation END (Failure) ---");
        return -1; // Return invalid ID
    }

    // 2. Validate pointers
    CScriptThing* pSpeaker = spSpeaker.get();
    CScriptThing* pListener = spListener.get();
    if (!pSpeaker || !pListener) {
        Log("!!! ERROR: StartAmbientConversation - Speaker or Listener is NULL!");
        Log("--- StartAmbientConversation END (Failure) ---");
        return -1;
    }

    // 3. Set defaults (based on pseudocode)
    bool bSoundIn2D = soundIn2D.value_or(false);
    bool bPlayDuringCutscene = playDuringCutscene.value_or(false);

    // 4. Call AddNewConversation
    Log("    Calling AddNewConversation...");
    int convoID = AddNewConversation_API(m_pGameInterface, pSpeaker, bSoundIn2D, bPlayDuringCutscene);

    if (convoID == -1) { // Assuming -1 is the error/invalid ID
        Log("!!! ERROR: AddNewConversation_API failed, returned -1!");
        Log("--- StartAmbientConversation END (Failure) ---");
        return -1;
    }
    Log("    New conversation created with ID: " + std::to_string(convoID));

    // 5. Call AddPersonToConversation for the listener
    // (The speaker is already added by AddNewConversation)
    Log("    Adding Listener to conversation...");
    AddPersonToConversation_API(m_pGameInterface, convoID, pListener);

    // As seen in the Trader script, sometimes you add more people (like the Hero)
    // We'll assume for this simple function, speaker + listener is enough.
    // The Lua script can call AddPersonToConversation if it needs more.
    // *Correction*: Let's add the Hero automatically, as most conversations involve them.
    // *Correction 2*: No, the 'listener' IS the Hero. Your plan is better. speaker + listener.

    Log("--- StartAmbientConversation END (Success) ---");
    return convoID;
}

void LuaQuestState::AddLineToConversation(int conversationID, const std::string& textKey, sol::object speaker,
    sol::object listener, sol::optional<bool> showSubtitle)
{
    // 1. Check prerequisites
    if (!m_pGameInterface || !AddLineToConversation_API) {
        Log("!!! ERROR: AddLineToConversation - Prerequisites not met!");
        return;
    }

    // 2. Validate pointers
    CScriptThing* pSpeaker = GetScriptThingFromSolObject(speaker);
    CScriptThing* pListener = GetScriptThingFromSolObject(listener);
    if (!pSpeaker) {
        Log("!!! ERROR: AddLineToConversation - Speaker is NULL!");
        return;
    }

    // 3. Set default for showSubtitle
    bool bShowSubtitle = showSubtitle.value_or(false);

    // 4. Prepare FableString
    FableString fsKey(textKey.c_str());

    // Retail passes an actual null CScriptThing value for an absent conversation
    // participant, not a null C++ pointer. AddLineToConversation calls a virtual
    // method on both operands even when the script-level thing is null (confirmed
    // by the warehouse-overhear crash dump at Fable+0x49076c). Preserve that ABI
    // contract for Lua nil listeners.
    CScriptThing nullListener = {};
    if (!pListener) {
        nullListener.pVTable = g_pCScriptThingVTable;
        pListener = &nullListener;
    }

    // 5. Call AddLineToConversation
    AddLineToConversation_API(m_pGameInterface, conversationID, fsKey, bShowSubtitle, pSpeaker, pListener);

    // Log("    Line '" + textKey + "' added to conversation " + std::to_string(conversationID)); // Too spammy
}

bool LuaQuestState::IsConversationActive(int conversationID)
{
    // 1. Check prerequisites
    if (!m_pGameInterface || !IsConversationActive_API) {
        Log("!!! ERROR: IsConversationActive - Prerequisites not met!");
        return false; // Return false on error
    }

    // 2. Call and return
    return IsConversationActive_API(m_pGameInterface, conversationID);
}

void LuaQuestState::RemoveConversation(int conversationID, sol::optional<bool> cutOffSpeech)
{
    Log("--- RemoveConversation START (Interrupt) ---");
    Log("    Removing conversation ID: " + std::to_string(conversationID));

    // 1. Check prerequisites
    if (!m_pGameInterface || !RemoveConversation_API) {
        Log("!!! ERROR: RemoveConversation - Prerequisites not met!");
        Log("--- RemoveConversation END (Failure) ---");
        return;
    }

    // 2. Set defaults
    // Pseudocode used 'false' for cut_off_speech
    bool bCutOffSpeech = cutOffSpeech.value_or(false);

    // 3. Call RemoveConversation
    RemoveConversation_API(m_pGameInterface, conversationID, bCutOffSpeech);
    Log("--- RemoveConversation END (Success) ---");
}

std::string LuaQuestState::GetRegionName() {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !GetRegionName_API) {
        Log("!!! ERROR: GetRegionName - API function not available!");
        return ""; // Return empty string on failure
    }

    // The API returns a const CCharString*
    const CCharString* pResult = GetRegionName_API(m_pGameInterface);

    // Pattern from existing string conversion
    if (pResult && pResult->pStringData && CCharString_ToConstChar_API) {
        const char* text = CCharString_ToConstChar_API(pResult);
        if (text) {
            // Do NOT destroy pResult, as it's const and game-owned
            return std::string(text);
        }
    }

    Log("!!! WARNING: GetRegionName - API returned null or invalid string.");
    return ""; // Return empty string on failure
}

bool LuaQuestState::MsgIsLevelLoaded(const std::string& levelName) {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !MsgIsLevelLoaded_API) {
        Log("!!! ERROR: MsgIsLevelLoaded - API function not available!");
        return false;
    }

    FableString fsLevel(levelName.c_str());
    return MsgIsLevelLoaded_API(m_pGameInterface, fsLevel);
}

bool LuaQuestState::MsgIsLevelUnloaded(const std::string& levelName) {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !MsgIsLevelUnloaded_API) {
        Log("!!! ERROR: MsgIsLevelUnloaded - API function not available!");
        return false;
    }

    FableString fsLevel(levelName.c_str());
    return MsgIsLevelUnloaded_API(m_pGameInterface, fsLevel);
}

namespace {
sol::object PollRegionMessage(sol::this_state state, CGameScriptInterfaceBase* gameInterface,
                              tMsgOnRegionLoaded poll, const char* apiName) {
    sol::state_view lua(state);
    if (!gameInterface || !poll || !CCharString_ToConstChar_API || !CCharString_Destroy) {
        LogToFile(std::string("!!! ERROR: ") + apiName + " - API prerequisites unavailable!");
        return sol::make_object(lua, sol::nil);
    }

    // Retail constructs an empty CCharString at the call site and owns the returned
    // reference. Match that ABI, copy into Lua-owned storage, then release exactly once.
    CCharString result = { 0 };
    const bool received = poll(gameInterface, &result);
    std::string regionName;
    if (result.pStringData) {
        if (const char* text = CCharString_ToConstChar_API(&result)) regionName = text;
        CCharString_Destroy(&result);
    }
    if (!received || regionName.empty()) return sol::make_object(lua, sol::nil);
    return sol::make_object(lua, regionName);
}
}

sol::object LuaQuestState::MsgOnRegionLoaded(sol::this_state s) {
    return PollRegionMessage(s, m_pGameInterface, MsgOnRegionLoaded_API, "MsgOnRegionLoaded");
}

sol::object LuaQuestState::MsgOnRegionUnloaded(sol::this_state s) {
    return PollRegionMessage(s, m_pGameInterface, MsgOnRegionUnloaded_API, "MsgOnRegionUnloaded");
}

bool LuaQuestState::MsgOnQuestCompleted(const std::string& questName) {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !MsgOnQuestCompleted_API) {
        Log("!!! ERROR: MsgOnQuestCompleted - API function not available!");
        return false;
    }

    FableString fsQuest(questName.c_str());
    return MsgOnQuestCompleted_API(m_pGameInterface, fsQuest);
}

bool LuaQuestState::MsgOnQuestFailed(const std::string& questName) {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !MsgOnQuestFailed_API) {
        Log("!!! ERROR: MsgOnQuestFailed - API function not available!");
        return false;
    }

    FableString fsQuest(questName.c_str());
    return MsgOnQuestFailed_API(m_pGameInterface, fsQuest);
}

sol::object LuaQuestState::MsgOnQuestAccepted(sol::this_state s) {
    sol::state_view lua(s);

    // Check if the API function pointer is available
    if (!m_pGameInterface || !MsgOnQuestAccepted_API) {
        Log("!!! ERROR: MsgOnQuestAccepted - API function not available!");
        return sol::make_object(lua, sol::nil);
    }

    CCharString result = { 0 }; // Output parameter
    bool wasAccepted = MsgOnQuestAccepted_API(m_pGameInterface, &result);

    if (wasAccepted) {
        // Pattern from GetActiveQuestName
        if (result.pStringData) {
            const char* text = CCharString_ToConstChar_API(&result);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&result);
                return sol::make_object(lua, finalString);
            }
            CCharString_Destroy(&result); // Cleanup even if text is null
        }
        // It was accepted but the string was bad? Return true.
        return sol::make_object(lua, true); // Fallback in case string is empty but event fired
    }

    // No quest was accepted this frame
    return sol::make_object(lua, sol::nil);
}

sol::object LuaQuestState::MsgOnHeroPickedPocket(sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !MsgOnHeroPickedPocket_API || !Game_malloc) {
        Log("!!! ERROR: MsgOnHeroPickedPocket - API function not available!");
        return sol::make_object(lua, sol::nil);
    }

    CScriptThing* pResultBuffer = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultBuffer) return sol::make_object(lua, sol::nil);
    memset(pResultBuffer, 0, sizeof(CScriptThing));

    bool eventFired = MsgOnHeroPickedPocket_API(m_pGameInterface, pResultBuffer);

    if (eventFired) {
        // --- FIX ---
        // Wrap the shared_ptr in sol::make_object
        return sol::make_object(lua, WrapScriptThingOutput(pResultBuffer));
    }

    Game_free(pResultBuffer);
    return sol::make_object(lua, sol::nil);
}

sol::object LuaQuestState::MsgOnHeroPickedLock(sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !MsgOnHeroPickedLock_API || !Game_malloc) {
        Log("!!! ERROR: MsgOnHeroPickedLock - API function not available!");
        return sol::make_object(lua, sol::nil);
    }

    CScriptThing* pResultBuffer = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultBuffer) return sol::make_object(lua, sol::nil);
    memset(pResultBuffer, 0, sizeof(CScriptThing));

    bool eventFired = MsgOnHeroPickedLock_API(m_pGameInterface, pResultBuffer);

    if (eventFired) {
        // --- FIX ---
        return sol::make_object(lua, WrapScriptThingOutput(pResultBuffer));
    }

    Game_free(pResultBuffer);
    return sol::make_object(lua, sol::nil);
}

sol::object LuaQuestState::MsgOnFishingGameFinished(sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !MsgOnFishingGameFinished_API || !Game_malloc) {
        Log("!!! ERROR: MsgOnFishingGameFinished - API function not available!");
        return sol::make_object(lua, sol::nil);
    }

    CScriptThing* pResultBuffer = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultBuffer) return sol::make_object(lua, sol::nil);
    memset(pResultBuffer, 0, sizeof(CScriptThing));

    bool eventFired = MsgOnFishingGameFinished_API(m_pGameInterface, pResultBuffer);

    if (eventFired) {
        // --- FIX ---
        return sol::make_object(lua, WrapScriptThingOutput(pResultBuffer));
    }

    Game_free(pResultBuffer);
    return sol::make_object(lua, sol::nil);
}

sol::object LuaQuestState::MsgOnTavernGameFinished(sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !MsgOnTavernGameFinished_API || !Game_malloc) {
        Log("!!! ERROR: MsgOnTavernGameFinished - API function not available!");
        return sol::make_object(lua, sol::nil);
    }

    CScriptThing* pResultBuffer = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResultBuffer) return sol::make_object(lua, sol::nil);
    memset(pResultBuffer, 0, sizeof(CScriptThing));

    bool eventFired = MsgOnTavernGameFinished_API(m_pGameInterface, (CScriptThing*)pResultBuffer);

    if (eventFired) {
        // --- FIX ---
        return sol::make_object(lua, WrapScriptThingOutput(pResultBuffer));
    }

    Game_free(pResultBuffer);
    return sol::make_object(lua, sol::nil);
}

sol::object LuaQuestState::MsgOnHeroRewardedWithItemsFrom(sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !MsgOnHeroRewardedWithItemsFrom_API) {
        Log("!!! ERROR: MsgOnHeroRewardedWithItemsFrom - API function not available!");
        return sol::make_object(lua, sol::nil);
    }

    CCharString result = { 0 }; // Output parameter
    bool eventFired = MsgOnHeroRewardedWithItemsFrom_API(m_pGameInterface, &result);

    if (eventFired) {
        // Pattern from MsgOnQuestAccepted
        if (result.pStringData) {
            const char* text = CCharString_ToConstChar_API(&result);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&result);
                return sol::make_object(lua, finalString);
            }
            CCharString_Destroy(&result);
        }
        return sol::make_object(lua, true); // Event fired but string was empty
    }

    return sol::make_object(lua, sol::nil);
}

bool LuaQuestState::IsHeroAllowedHenchmenInCurrentRegion() {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !IsHeroAllowedHenchmenInCurrentRegion_API) {
        Log("!!! ERROR: IsHeroAllowedHenchmenInCurrentRegion - API function not available!");
        return false; // Default to false if API is missing
    }

    // Call and return the API result
    return IsHeroAllowedHenchmenInCurrentRegion_API(m_pGameInterface);
}

bool LuaQuestState::IsHeroAllowedHenchmenInRegion(const std::string& regionName) {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !IsHeroAllowedHenchmenInRegion_API) {
        Log("!!! ERROR: IsHeroAllowedHenchmenInRegion - API function not available!");
        return false;
    }

    FableString fsRegion(regionName.c_str());
    return IsHeroAllowedHenchmenInRegion_API(m_pGameInterface, fsRegion);
}

void LuaQuestState::DontPopulateNextLoadedRegion() {
    // Check if the API function pointer is available
    if (!m_pGameInterface || !DontPopulateNextLoadedRegion_API) {
        Log("!!! ERROR: DontPopulateNextLoadedRegion - API function not available!");
        return;
    }

    // Call the API
    DontPopulateNextLoadedRegion_API(m_pGameInterface);
}

void LuaQuestState::DisplayQuestInfo(bool bDisplay) {
    if (!m_pGameInterface || !DisplayQuestInfo_API) {
        Log("!!! ERROR: DisplayQuestInfo - API function not available!");
        return;
    }
    // Call API as defined in the typedef
    DisplayQuestInfo_API(m_pGameInterface, bDisplay);
}

void LuaQuestState::UpdateQuestInfoCounter(int elementID, int count, int max) {
    if (!m_pGameInterface || !UpdateQuestInfoCounter_API) {
        Log("!!! ERROR: UpdateQuestInfoCounter - API function not available!");
        return;
    }

    // Call the API
    UpdateQuestInfoCounter_API(m_pGameInterface, elementID, count, max);
}

void LuaQuestState::OverrideAutomaticHouseLocking(CScriptThing* pHouse, bool overrideLocking) {
    if (!m_pGameInterface || !OverrideAutomaticHouseLocking_API || !pHouse) {
        Log("!!! ERROR: OverrideAutomaticHouseLocking - API or house not available!");
        return;
    }
    OverrideAutomaticHouseLocking_API(m_pGameInterface, pHouse, overrideLocking);
}

void LuaQuestState::UpdateOnlineScoreArchery(float score) {
    if (!m_pGameInterface || !UpdateOnlineScore_Archery_API) {
        Log("!!! ERROR: UpdateOnlineScore_Archery - API not available!");
        return;
    }
    UpdateOnlineScore_Archery_API(m_pGameInterface, score);
}

void LuaQuestState::SetQuestInfoName(const std::string& name) {
    if (!m_pGameInterface || !SetQuestInfoName_API) {
        Log("!!! ERROR: SetQuestInfoName - API function not available!");
        return;
    }
    // The typedef specified const char*, so we use .c_str()
    SetQuestInfoName_API(m_pGameInterface, name.c_str());
}

void LuaQuestState::SetQuestInfoText(const std::string& text) {
    if (!m_pGameInterface || !SetQuestInfoText_API) {
        Log("!!! ERROR: SetQuestInfoText - API function not available!");
        return;
    }
    // The typedef specified const char*, so we use .c_str()
    SetQuestInfoText_API(m_pGameInterface, text.c_str());
}

int LuaQuestState::AddQuestInfoBar(float current, float max, sol::table color1, sol::table color2, const std::string& texture, const std::string& text, float scale) {
    if (!m_pGameInterface || !AddQuestInfoBar_API) {
        Log("!!! ERROR: AddQuestInfoBar - API function not available!");
        return -1; // Return invalid ID
    }

    // Prepare parameters
    CRGBColour c1 = ParseColorTable(color1);
    CRGBColour c2 = ParseColorTable(color2);
    FableString fsTexture(texture.c_str());
    FableString fsText(text.c_str());

    const int handle = AddQuestInfoBar_API(m_pGameInterface, current, max, &c1, &c2, fsTexture, fsText, scale);
    LogToFile("    [QuestInfoEvidence] AddQuestInfoBar(current=" + std::to_string(current) +
              ", max=" + std::to_string(max) + ", texture='" + texture +
              "', scale=" + std::to_string(scale) + ") returned handle " +
              std::to_string(handle) + ".");
    return handle;
}

int LuaQuestState::AddQuestInfoBarHealth(const std::shared_ptr<CScriptThing>& spThing, sol::table color, const std::string& texture, float scale) {
    CScriptThing* pThing = spThing.get();
    if (!m_pGameInterface || !AddQuestInfoBarHealth_API) {
        Log("!!! ERROR: AddQuestInfoBarHealth - API function not available!");
        return -1; // Return invalid ID
    }

    // Get raw pointer from shared_ptr
    if (!pThing) {
        Log("!!! ERROR: AddQuestInfoBarHealth - pThing is null!");
        return -1;
    }

    // Prepare parameters
    CRGBColour c = ParseColorTable(color);
    FableString fsTexture(texture.c_str());

    return AddQuestInfoBarHealth_API(m_pGameInterface, pThing, &c, fsTexture, scale);
}

int LuaQuestState::AddQuestInfoCounter(const std::string& text, int count, float scale) {
    if (!m_pGameInterface || !AddQuestInfoCounter_ByValue_API) {
        Log("!!! ERROR: AddQuestInfoCounter - API function not available!");
        return -1; // Return invalid ID
    }

    FableString fsText(text.c_str());
    return AddQuestInfoCounter_ByValue_API(m_pGameInterface, fsText, count, scale);
}

void LuaQuestState::AddScreenMessage(const std::string& message, sol::optional<int> selectionMethod) {
    if (m_pGameInterface && AddScreenMessage_API) {
        FableString fsMessage(message.c_str());
        // Use the provided method or default to GROUP_SELECT_RANDOM_NO_REPEAT
        ETextGroupSelectionMethod eMethod = static_cast<ETextGroupSelectionMethod>(selectionMethod.value_or(GROUP_SELECT_RANDOM_NO_REPEAT));
        AddScreenMessage_API(m_pGameInterface, fsMessage.get(), eMethod);
    }
    else {
        Log("!!! ERROR: AddScreenMessage - API not available!");
    }
}

sol::object LuaQuestState::MsgOnHeroHairTypeChanged(int area, sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && MsgOnHeroHairTypeChanged_API) {
        CCharString hairNameResult = { 0 };
        // Assuming EClothingCoversArea is available from your headers
        EClothingCoversArea eArea = static_cast<EClothingCoversArea>(area);

        bool result = MsgOnHeroHairTypeChanged_API(m_pGameInterface, eArea, &hairNameResult);

        if (result) {
            // Event fired. Convert the output string.
            const char* text = CCharString_ToConstChar_API(&hairNameResult);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&hairNameResult);
                return sol::make_object(lua, finalString);
            }
            CCharString_Destroy(&hairNameResult);
            // Event fired but string was bad, return true
            return sol::make_object(lua, true);
        }
    }
    // Event did not fire, return nil
    return sol::make_object(lua, sol::nil);
}

sol::object LuaQuestState::MsgOnHeroUsedTeleporter(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && MsgOnHeroUsedTeleporter_API) {
        CCharString teleporterNameResult = { 0 };
        bool result = MsgOnHeroUsedTeleporter_API(m_pGameInterface, &teleporterNameResult);

        if (result) {
            // Event fired. Convert the output string.
            const char* text = CCharString_ToConstChar_API(&teleporterNameResult);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&teleporterNameResult);
                return sol::make_object(lua, finalString);
            }
            CCharString_Destroy(&teleporterNameResult);
            return sol::make_object(lua, true);
        }
    }
    return sol::make_object(lua, sol::nil);
}

bool LuaQuestState::MsgOnHeroUsedGuildSeal() {
    if (m_pGameInterface && MsgOnHeroUsedGuildSeal_API) {
        return MsgOnHeroUsedGuildSeal_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgOnHeroUsedGuildSeal - API not available!");
    return false;
}

bool LuaQuestState::MsgOnGameSavedManually() {
    if (m_pGameInterface && MsgOnGameSavedManually_API) {
        return MsgOnGameSavedManually_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgOnGameSavedManually - API not available!");
    return false;
}

sol::object LuaQuestState::MsgOnHeroSlept(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && MsgOnHeroSlept_API) {
        bool bWasInterrupted = false; // The output parameter
        bool result = MsgOnHeroSlept_API(m_pGameInterface, &bWasInterrupted);

        if (result) {
            // Event fired, return the output bool (e.g., true if interrupted)
            return sol::make_object(lua, bWasInterrupted);
        }
    }
    // Event did not fire, return nil
    return sol::make_object(lua, sol::nil);
}

bool LuaQuestState::MsgOnHeroFiredRangedWeapon() {
    if (m_pGameInterface && MsgOnHeroFiredRangedWeapon_API) {
        return MsgOnHeroFiredRangedWeapon_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgOnHeroFiredRangedWeapon - API not available!");
    return false;
}

std::shared_ptr<CScriptThing> LuaQuestState::TurnCreatureInto(const std::shared_ptr<CScriptThing>& spTarget, const std::string& newDefName) {
    if (!m_pGameInterface || !TurnCreatureInto_API || !Game_malloc) {
        Log("!!! ERROR: TurnCreatureInto - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: TurnCreatureInto - pTarget is null!");
        return nullptr;
    }

    FableString fsNewDefName(newDefName.c_str());

    // Allocate memory for the result
    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: TurnCreatureInto - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    // Call the API
    TurnCreatureInto_API(m_pGameInterface, pResult, pTarget, fsNewDefName.get());

    // Wrap, validate, and return
    return WrapScriptThingOutput(pResult);
}

void LuaQuestState::DeregisterTimer(int timerId) {
    if (m_pGameInterface && DeregisterTimer_API) {
        DeregisterTimer_API(m_pGameInterface, timerId);
        Log("Deregistered game timer with ID: " + std::to_string(timerId));
    }
    else {
        Log("!!! ERROR: DeregisterTimer - API not available!");
    }
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateLight(sol::table position, sol::table color, const std::string& scriptName, float innerRadius, float outerRadius, sol::optional<float> flicker) {

    // --- NEW DEBUG LOGGING ---
    std::stringstream ss_debug;
    ss_debug << "===== CreateLight C++ Wrapper START ====";
    if (m_pParentHost) {
        ss_debug << "\n    Quest: " << m_pParentHost->GetScriptName();
    }
    ss_debug << "\n    Pos: x=" << position["x"].get_or(0.0f) << ", y=" << position["y"].get_or(0.0f) << ", z=" << position["z"].get_or(0.0f);
    ss_debug << "\n    Color: R=" << color["R"].get_or(255) << ", G=" << color["G"].get_or(255) << ", B=" << color["B"].get_or(255);
    ss_debug << "\n    Script: '" << scriptName << "'";
    ss_debug << "\n    Radii: " << innerRadius << " (inner), " << outerRadius << " (outer)";
    ss_debug << "\n    Flicker: " << flicker.value_or(0.0f);
    Log(ss_debug.str());
    // --- END DEBUG LOGGING ---

    if (!m_pGameInterface || !CreateLight_API || !Game_malloc) {
        Log("!!! ERROR: CreateLight - API or memory allocator not available!");
        return nullptr;
    }

    C3DVector pos = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };

    CRGBColour col = ParseColorTable(color);
    FableString fsScriptName(scriptName.c_str());
    float fFlicker = flicker.value_or(0.0f);

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: CreateLight - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    Log("    Calling CreateLight_API...");
    CreateLight_API(m_pGameInterface, pResult, &pos, &col, fsScriptName.get(), innerRadius, outerRadius, fFlicker);
    Log("    CreateLight_API finished.");

    // WrapScriptThingOutput will validate the handle (e.g., IsNull check)
    std::shared_ptr<CScriptThing> spResult = WrapScriptThingOutput(pResult);

    // --- NEW DEBUG LOGGING ---
    if (spResult) {
        Log("    WrapScriptThingOutput SUCCESS. Returning valid handle.");
    }
    else {
        Log("!!! ERROR: WrapScriptThingOutput returned NULL. Game engine likely failed to create the light (possible script name collision?).");
    }
    Log("===== CreateLight C++ Wrapper END =====");
    // --- END DEBUG LOGGING ---

    return spResult;
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateObjectOnEntity(const std::string& defName, const std::shared_ptr<CScriptThing>& spOnThing, const std::string& scriptName) {
    if (!m_pGameInterface || !CreateObjectOnEntity_API || !Game_malloc) {
        Log("!!! ERROR: CreateObjectOnEntity - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pOnThing = spOnThing.get();
    if (!pOnThing) {
        Log("!!! ERROR: CreateObjectOnEntity - pOnThing is null!");
        return nullptr;
    }

    FableString fsDefName(defName.c_str());
    FableString fsScriptName(scriptName.c_str());

    // Allocate memory for the result
    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: CreateObjectOnEntity - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    // Call the API
    CreateObjectOnEntity_API(m_pGameInterface, pResult, fsDefName.get(), pOnThing, fsScriptName.get());

    // Wrap, validate, and return
    return WrapScriptThingOutput(pResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateCreatureOnEntity(const std::string& defName, const std::shared_ptr<CScriptThing>& spOnThing, const std::string& scriptName) {
    if (!m_pGameInterface || !CreateCreatureOnEntity_API || !Game_malloc) {
        Log("!!! ERROR: CreateCreatureOnEntity - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pOnThing = spOnThing.get();
    if (!pOnThing) {
        Log("!!! ERROR: CreateCreatureOnEntity - pOnThing is null!");
        return nullptr;
    }

    FableString fsDefName(defName.c_str());
    FableString fsScriptName(scriptName.c_str());

    // Allocate memory for the result
    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: CreateCreatureOnEntity - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    // Call the API
    CreateCreatureOnEntity_API(m_pGameInterface, pResult, fsDefName.get(), pOnThing, fsScriptName.get());

    // Wrap, validate, and return
    return WrapScriptThingOutput(pResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateExperienceOrb(sol::table position, int amount) {
    if (!m_pGameInterface || !CreateExperienceOrb_API || !Game_malloc) {
        Log("!!! ERROR: CreateExperienceOrb - API or memory allocator not available!");
        return nullptr;
    }

    C3DVector pos = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: CreateExperienceOrb - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    CreateExperienceOrb_API(m_pGameInterface, pResult, &pos, amount);

    return WrapScriptThingOutput(pResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateExplosion(const std::string& defName, sol::table position, sol::table direction, const std::string& scriptName) {
    if (!m_pGameInterface || !CreateExplosion_API || !Game_malloc) {
        Log("!!! ERROR: CreateExplosion - API or memory allocator not available!");
        return nullptr;
    }

    FableString fsDefName(defName.c_str());
    FableString fsScriptName(scriptName.c_str());

    C3DVector pos = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };

    C3DVector dir = {
        direction["x"].get_or(0.0f),
        direction["y"].get_or(0.0f),
        direction["z"].get_or(0.0f)
    };

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: CreateExplosion - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    // Calling with fsScriptName.m_charString to match the CCharString pass-by-value typedef
    CreateExplosion_API(m_pGameInterface, pResult, fsDefName.get(), &pos, &dir, fsScriptName.m_charString);

    return WrapScriptThingOutput(pResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreatePhysicalBarrier(float size, sol::table position, sol::table direction, const std::string& scriptName) {
    if (!m_pGameInterface || !CreatePhysicalBarrier_API || !Game_malloc) {
        Log("!!! ERROR: CreatePhysicalBarrier - API or memory allocator not available!");
        return nullptr;
    }

    FableString fsScriptName(scriptName.c_str());

    C3DVector pos = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };

    C3DVector dir = {
        direction["x"].get_or(0.0f),
        direction["y"].get_or(0.0f),
        direction["z"].get_or(0.0f)
    };

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: CreatePhysicalBarrier - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    // Calling with fsScriptName.m_charString to match the CCharString pass-by-value typedef
    CreatePhysicalBarrier_API(m_pGameInterface, pResult, size, &pos, &dir, fsScriptName.m_charString);

    return WrapScriptThingOutput(pResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::CreateRumble(sol::table position, float f1, float f2, const std::string& scriptName) {
    if (!m_pGameInterface || !CreateRumble_API || !Game_malloc) {
        Log("!!! ERROR: CreateRumble - API or memory allocator not available!");
        return nullptr;
    }

    FableString fsScriptName(scriptName.c_str());

    C3DVector pos = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: CreateRumble - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    // Calling with fsScriptName.m_charString to match the CCharString pass-by-value typedef
    CreateRumble_API(m_pGameInterface, pResult, &pos, f1, f2, fsScriptName.m_charString);

    return WrapScriptThingOutput(pResult);
}

void LuaQuestState::ClearAllRumbles() {
    if (m_pGameInterface && ClearAllRumbles_API) {
        ClearAllRumbles_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ClearAllRumbles - API not available!");
    }
}

void LuaQuestState::AddScreenTitleMessage(const std::string& message, float duration, sol::optional<bool> bUnknown) {
    if (m_pGameInterface && AddScreenTitleMessage_API) {
        FableString fsMessage(message.c_str());
        // Default the unknown bool to 'true' as requested
        bool bFinalUnknown = bUnknown.value_or(true);
        AddScreenTitleMessage_API(m_pGameInterface, fsMessage.get(), duration, bFinalUnknown);
    }
    else {
        Log("!!! ERROR: AddScreenTitleMessage - API not available!");
    }
}

void LuaQuestState::DisplayGameInfo(const std::string& infoKey) {
    if (m_pGameInterface && DisplayGameInfo_API) {
        FableString fsInfoKey(infoKey.c_str());
        DisplayGameInfo_API(m_pGameInterface, fsInfoKey.get());
    }
    else {
        Log("!!! ERROR: DisplayGameInfo - API not available!");
    }
}

void LuaQuestState::DisplayGameInfoText(const std::string& wideText) {
    if (!m_pGameInterface || !DisplayGameInfoText_API || !g_fableBase) {
        Log("!!! ERROR: DisplayGameInfoText - API or game base not available!");
        return;
    }
    std::wstring converted;
    if (!Utf8ToWide(wideText, converted)) {
        Log("!!! ERROR: DisplayGameInfoText - argument is not valid UTF-8.");
        return;
    }
    RetailWideString retailText;
    if (!retailText.Construct(converted, ASLR<tRetailWideStringCtor>(0x0099B6B0),
                              ASLR<tRetailWideStringDtor>(0x0099B510))) {
        Log("!!! ERROR: DisplayGameInfoText - retail CWideString construction failed.");
        return;
    }
    DisplayGameInfoText_API(m_pGameInterface, retailText.Get());
}

void LuaQuestState::AddLogBookEntry(const std::string& title, const std::string& description,
    const std::string& texture, int category) {
    if (!m_pGameInterface || !AddLogBookEntry_API || !g_fableBase) {
        Log("!!! ERROR: AddLogBookEntry - API or game base not available!");
        return;
    }
    if (category < CHeroLogBook::CHeroLogBookEntry::CATEGORY_QUEST ||
        category > CHeroLogBook::CHeroLogBookEntry::CATEGORY_COMBAT) {
        Log("!!! ERROR: AddLogBookEntry - category must be in the retail range 0..7.");
        return;
    }

    std::wstring wideTitle;
    std::wstring wideDescription;
    std::wstring wideTexture;
    if (!Utf8ToWide(title, wideTitle) || !Utf8ToWide(description, wideDescription) ||
        !Utf8ToWide(texture, wideTexture)) {
        Log("!!! ERROR: AddLogBookEntry - one or more arguments are not valid UTF-8.");
        return;
    }

    // These are the retail executable's own CWideString constructor/destructor.
    // Only wchar_t buffers cross the boundary; no modern STL object is shared.
    tRetailWideStringCtor ctor = ASLR<tRetailWideStringCtor>(0x0099B6B0);
    tRetailWideStringDtor dtor = ASLR<tRetailWideStringDtor>(0x0099B510);
    RetailWideString retailTitle;
    RetailWideString retailDescription;
    RetailWideString retailTexture;
    if (!retailTitle.Construct(wideTitle, ctor, dtor) ||
        !retailDescription.Construct(wideDescription, ctor, dtor) ||
        !retailTexture.Construct(wideTexture, ctor, dtor)) {
        Log("!!! ERROR: AddLogBookEntry - retail CWideString construction failed.");
        return;
    }

    AddLogBookEntry_API(m_pGameInterface, retailTitle.Get(), retailDescription.Get(),
        retailTexture.Get(), static_cast<CHeroLogBook::CHeroLogBookEntry::ECategory>(category));
}

bool LuaQuestState::IsSafeToDisplayGameInfo() {
    if (m_pGameInterface && IsSafeToDisplayGameInfo_API) {
        return IsSafeToDisplayGameInfo_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsSafeToDisplayGameInfo - API not available!");
    return false; // Default to 'false' (unsafe) if API is missing
}

bool LuaQuestState::DisplayTutorial(int category) {
    // This uses the same ETutorialCategory enum as GiveHeroTutorial
    if (m_pGameInterface && DisplayTutorial_API) {
        return DisplayTutorial_API(m_pGameInterface, static_cast<ETutorialCategory>(category));
    }
    Log("!!! ERROR: DisplayTutorial - API not available!");
    return false;
}

bool LuaQuestState::IsTutorialSystemEnabled() {
    if (m_pGameInterface && IsTutorialSystemEnabled_API) {
        return IsTutorialSystemEnabled_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsTutorialSystemEnabled - API not available!");
    return true; // Default to 'true' if API is missing
}

void LuaQuestState::GiveHeroWeapon(const std::string& weaponDefName, bool bEquip) {
    if (m_pGameInterface && GiveHeroWeapon_API) {
        FableString fsWeapon(weaponDefName.c_str());
        GiveHeroWeapon_API(m_pGameInterface, fsWeapon.get(), bEquip);
    }
    else {
        Log("!!! ERROR: GiveHeroWeapon - API not available!");
    }
}

void LuaQuestState::SetWeaponAsHerosActiveWeapon(const std::string& weaponDefName) {
    if (m_pGameInterface && SetWeaponAsHerosActiveWeapon_API) {
        FableString fsWeapon(weaponDefName.c_str());
        SetWeaponAsHerosActiveWeapon_API(m_pGameInterface, fsWeapon.get());
    }
    else {
        Log("!!! ERROR: SetWeaponAsHerosActiveWeapon - API not available!");
    }
}

bool LuaQuestState::GiveHeroItemsFromContainer(const std::shared_ptr<CScriptThing>& spContainerThing, bool bShowUI) {
    if (!m_pGameInterface || !GiveHeroItemsFromContainer_API) {
        Log("!!! ERROR: GiveHeroItemsFromContainer - API not available!");
        return false;
    }

    // Get the raw CScriptThing* from the shared_ptr
    CScriptThing* pContainer = spContainerThing.get();
    if (!pContainer) {
        Log("!!! ERROR: GiveHeroItemsFromContainer - pContainerThing is null!");
        return false;
    }

    // Call the API with the raw pointer
    return GiveHeroItemsFromContainer_API(m_pGameInterface, pContainer, bShowUI);
}

void LuaQuestState::SetHeroAbleToGainExperience(bool bCanGain) {
    if (m_pGameInterface && SetHeroAbleToGainExperience_API) {
        SetHeroAbleToGainExperience_API(m_pGameInterface, bCanGain);
    }
    else {
        Log("!!! ERROR: SetHeroAbleToGainExperience - API not available!");
    }
}

void LuaQuestState::SheatheHeroWeapons() {
    if (m_pGameInterface && SheatheHeroWeapons_API) {
        SheatheHeroWeapons_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: SheatheHeroWeapons - API not available!");
    }
}

void LuaQuestState::SetHeroWillAsUsable(bool bIsUsable) {
    if (m_pGameInterface && SetHeroWillAsUsable_API) {
        SetHeroWillAsUsable_API(m_pGameInterface, bIsUsable);
    }
    else {
        Log("!!! ERROR: SetHeroWillAsUsable - API not available!");
    }
}

void LuaQuestState::SetHeroWeaponsAsUsable(bool bIsUsable) {
    if (m_pGameInterface && SetHeroWeaponsAsUsable_API) {
        SetHeroWeaponsAsUsable_API(m_pGameInterface, bIsUsable);
    }
    else {
        Log("!!! ERROR: SetHeroWeaponsAsUsable - API not available!");
    }
}

void LuaQuestState::SetWeaponOutCrimeEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetWeaponOutCrimeEnabled_API) {
        SetWeaponOutCrimeEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetWeaponOutCrimeEnabled - API not available!");
    }
}

void LuaQuestState::SetGuardsIgnoreCrimes(bool bDoIgnore) {
    if (m_pGameInterface && SetGuardsIgnoreCrimes_API) {
        SetGuardsIgnoreCrimes_API(m_pGameInterface, bDoIgnore);
    }
    else {
        Log("!!! ERROR: SetGuardsIgnoreCrimes - API not available!");
    }
}

void LuaQuestState::RemoveAllHeroWeapons() {
    if (m_pGameInterface && RemoveAllHeroWeapons_API) {
        RemoveAllHeroWeapons_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: RemoveAllHeroWeapons - API not available!");
    }
}

bool LuaQuestState::IsReportedOrUnreportedCrimeKnown(const std::shared_ptr<CScriptThing>& spVillage) {
    if (!m_pGameInterface || !IsReportedOrUnreportedCrimeKnown_API) {
        Log("!!! ERROR: IsReportedOrUnreportedCrimeKnown - API not available!");
        return false;
    }

    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: IsReportedOrUnreportedCrimeKnown - pVillage thing is null!");
        return false;
    }

    return IsReportedOrUnreportedCrimeKnown_API(m_pGameInterface, pVillage);
}

void LuaQuestState::ConfiscateAllHeroItems() {
    if (m_pGameInterface && ConfiscateAllHeroItems_API) {
        ConfiscateAllHeroItems_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ConfiscateAllHeroItems - API not available!");
    }
}

void LuaQuestState::ConfiscateAllHeroWeapons() {
    if (m_pGameInterface && ConfiscateAllHeroWeapons_API) {
        ConfiscateAllHeroWeapons_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ConfiscateAllHeroWeapons - API not available!");
    }
}

void LuaQuestState::ConfiscateItemsOfTypeFromHero(const std::string& defName) {
    if (m_pGameInterface && ConfiscateItemsOfTypeFromHero_API) {
        FableString fsDefName(defName.c_str());
        ConfiscateItemsOfTypeFromHero_API(m_pGameInterface, fsDefName.get());
    }
    else {
        Log("!!! ERROR: ConfiscateItemsOfTypeFromHero - API not available!");
    }
}

void LuaQuestState::ReturnAllConfiscatedItemsToHero() {
    if (m_pGameInterface && ReturnAllConfiscatedItemsToHero_API) {
        ReturnAllConfiscatedItemsToHero_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ReturnAllConfiscatedItemsToHero - API not available!");
    }
}

void LuaQuestState::MakeHeroCarryItemInHand(const std::shared_ptr<CScriptThing>& spThing, bool setAsActive, bool setAsPersistent) {
    CScriptThing* pThing = spThing.get();
    if (!m_pGameInterface || !MakeHeroCarryItemInHand_ByThing_API) {
        Log("!!! ERROR: MakeHeroCarryItemInHand (ByThing) - API not available!");
        return;
    }

    if (!pThing) {
        Log("!!! ERROR: MakeHeroCarryItemInHand (ByThing) - pThing is null!");
        return;
    }

    MakeHeroCarryItemInHand_ByThing_API(m_pGameInterface, pThing, setAsActive, setAsPersistent);
}

void LuaQuestState::MakeHeroCarryItemInHand(const std::string& defName) {
    if (m_pGameInterface && MakeHeroCarryItemInHand_ByName_API) {
        FableString fsDefName(defName.c_str());
        MakeHeroCarryItemInHand_ByName_API(m_pGameInterface, fsDefName.get());
    }
    else {
        Log("!!! ERROR: MakeHeroCarryItemInHand (ByName) - API not available!");
    }
}

void LuaQuestState::AddTattooToHero(const std::string& tattooDefName) {
    if (m_pGameInterface && AddTattooToHero_API) {
        FableString fsTattoo(tattooDefName.c_str());
        AddTattooToHero_API(m_pGameInterface, fsTattoo.get());
    }
    else {
        Log("!!! ERROR: AddTattooToHero - API not available!");
    }
}

bool LuaQuestState::IsPlayerZTargetingThing(const std::shared_ptr<CScriptThing>& spThing) {
    CScriptThing* pThing = spThing.get();
    if (!m_pGameInterface || !IsPlayerZTargetingThing_API) {
        Log("!!! ERROR: IsPlayerZTargetingThing - API not available!");
        return false;
    }

    if (!pThing) {
        Log("!!! ERROR: IsPlayerZTargetingThing - pThing is null!");
        return false;
    }

    return IsPlayerZTargetingThing_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsPlayerCreatureBlocking() {
    if (m_pGameInterface && IsPlayerCreatureBlocking_API) {
        return IsPlayerCreatureBlocking_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsPlayerCreatureBlocking - API not available!");
    return false;
}

sol::object LuaQuestState::IsPlayerCreatureReadyToFireProjectileWeapon(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && IsPlayerCreatureReadyToFireProjectileWeapon_API) {
        float chargeAmount = 0.0f; // This is the output parameter
        bool result = IsPlayerCreatureReadyToFireProjectileWeapon_API(m_pGameInterface, &chargeAmount);

        if (result) {
            // The function returned true, so we return the charge amount
            return sol::make_object(lua, chargeAmount);
        }
        // The function returned false (not ready), so we return nil
        return sol::make_object(lua, sol::nil);
    }

    Log("!!! ERROR: IsPlayerCreatureReadyToFireProjectileWeapon - API not available!");
    return sol::make_object(lua, sol::nil);
}

int LuaQuestState::GetPlayerCreatureCombatMultiplier() {
    if (m_pGameInterface && GetPlayerCreatureCombatMultiplier_API) {
        return GetPlayerCreatureCombatMultiplier_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetPlayerCreatureCombatMultiplier - API not available!");
    return 0; // Default to 0
}

int LuaQuestState::GetPlayerCreatureCombatMultiplierRunningNumHits() {
    if (m_pGameInterface && GetPlayerCreatureCombatMultiplierRunningNumHits_API) {
        return GetPlayerCreatureCombatMultiplierRunningNumHits_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetPlayerCreatureCombatMultiplierRunningNumHits - API not available!");
    return 0; // Default to 0
}

bool LuaQuestState::IsPlayerCreatureFlourishEnabled() {
    if (m_pGameInterface && IsPlayerCreatureFlourishEnabled_API) {
        return IsPlayerCreatureFlourishEnabled_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsPlayerCreatureFlourishEnabled - API not available!");
    return false;
}

void LuaQuestState::SetPlayerCreatureOnlyTarget(const std::shared_ptr<CScriptThing>& spTarget) {
    if (!m_pGameInterface || !SetPlayerCreatureOnlyTarget_API) {
        Log("!!! ERROR: SetPlayerCreatureOnlyTarget - API not available!");
        return;
    }

    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: SetPlayerCreatureOnlyTarget - pTarget is null!");
        return;
    }

    SetPlayerCreatureOnlyTarget_API(m_pGameInterface, pTarget);
}

void LuaQuestState::ResetPlayerCreatureOnlyTarget() {
    if (m_pGameInterface && ResetPlayerCreatureOnlyTarget_API) {
        ResetPlayerCreatureOnlyTarget_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ResetPlayerCreatureOnlyTarget - API not available!");
    }
}

float LuaQuestState::GetHeroMorality() {
    if (m_pGameInterface && GetHeroMorality_API) {
        return GetHeroMorality_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroMorality - API not available!");
    return 0.0f;
}

int LuaQuestState::GetHeroMoralityCategory() {
    if (m_pGameInterface && GetHeroMoralityCategory_API) {
        // Cast the enum result to an int for Lua
        return static_cast<int>(GetHeroMoralityCategory_API(m_pGameInterface));
    }
    Log("!!! ERROR: GetHeroMoralityCategory - API not available!");
    return 0; // Default to 0 (MORALITY_NULL or MORALITY_NEUTRAL)
}

void LuaQuestState::ChangeHeroMoralityDueToTheft() {
    if (m_pGameInterface && ChangeHeroMoralityDueToTheft_API) {
        ChangeHeroMoralityDueToTheft_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ChangeHeroMoralityDueToTheft - API not available!");
    }
}

void LuaQuestState::ChangeHeroMoralityDueToPicklock() {
    if (m_pGameInterface && ChangeHeroMoralityDueToPicklock_API) {
        ChangeHeroMoralityDueToPicklock_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ChangeHeroMoralityDueToPicklock - API not available!");
    }
}

void LuaQuestState::GiveHeroRenownPoints(int amount) {
    if (m_pGameInterface && GiveHeroRenownPoints_API) {
        GiveHeroRenownPoints_API(m_pGameInterface, amount);
    }
    else {
        Log("!!! ERROR: GiveHeroRenownPoints - API not available!");
    }
}

bool LuaQuestState::IsHeroRenownLevelFull() {
    if (m_pGameInterface && IsHeroRenownLevelFull_API) {
        return IsHeroRenownLevelFull_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsHeroRenownLevelFull - API not available!");
    return false;
}

void LuaQuestState::IncreaseHeroRenownLevel() {
    if (m_pGameInterface && IncreaseHeroRenownLevel_API) {
        IncreaseHeroRenownLevel_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: IncreaseHeroRenownLevel - API not available!");
    }
}

int LuaQuestState::GetHeroStrengthLevel() {
    if (m_pGameInterface && GetHeroStrengthLevel_API) {
        return GetHeroStrengthLevel_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroStrengthLevel - API not available!");
    return 0;
}

int LuaQuestState::GetHeroSkillLevel() {
    if (m_pGameInterface && GetHeroSkillLevel_API) {
        return GetHeroSkillLevel_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroSkillLevel - API not available!");
    return 0;
}

int LuaQuestState::GetHeroWillLevel() {
    if (m_pGameInterface && GetHeroWillLevel_API) {
        return GetHeroWillLevel_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroWillLevel - API not available!");
    return 0;
}

int LuaQuestState::GetHeroStatLevel(int statType) {
    if (m_pGameInterface && GetHeroStatLevel_API) {
        EHeroTrainableStatType eStat = static_cast<EHeroTrainableStatType>(statType);
        return GetHeroStatLevel_API(m_pGameInterface, eStat);
    }
    Log("!!! ERROR: GetHeroStatLevel - API not available!");
    return 0;
}

int LuaQuestState::GetHeroStatMax(int statType) {
    if (m_pGameInterface && GetHeroStatMax_API) {
        EHeroTrainableStatType eStat = static_cast<EHeroTrainableStatType>(statType);
        return GetHeroStatMax_API(m_pGameInterface, eStat);
    }
    Log("!!! ERROR: GetHeroStatMax - API not available!");
    return 0;
}

void LuaQuestState::SetHeroAge(float age) {
    if (m_pGameInterface && SetHeroAge_API) {
        SetHeroAge_API(m_pGameInterface, age);
    }
    else {
        Log("!!! ERROR: SetHeroAge - API not available!");
    }
}

void LuaQuestState::SetHeroAsTeenager(bool bIsTeen) {
    if (m_pGameInterface && SetHeroAsTeenager_API) {
        SetHeroAsTeenager_API(m_pGameInterface, bIsTeen);
    }
    else {
        Log("!!! ERROR: SetHeroAsTeenager - API not available!");
    }
}

void LuaQuestState::SetHeroAsApprentice(bool bIsApprentice) {
    if (m_pGameInterface && SetHeroAsApprentice_API) {
        SetHeroAsApprentice_API(m_pGameInterface, bIsApprentice);
    }
    else {
        Log("!!! ERROR: SetHeroAsApprentice - API not available!");
    }
}

float LuaQuestState::GetDistanceHeroCanBeHeardFrom() {
    if (m_pGameInterface && GetDistanceHeroCanBeHeardFrom_API) {
        return GetDistanceHeroCanBeHeardFrom_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetDistanceHeroCanBeHeardFrom - API not available!");
    return 0.0f;
}

int LuaQuestState::GetHeroRoughExperienceLevel() {
    if (m_pGameInterface && GetHeroRoughExperienceLevel_API) {
        return GetHeroRoughExperienceLevel_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroRoughExperienceLevel - API not available!");
    return 0;
}

int LuaQuestState::GetHeroExperienceAvailableToSpend() {
    if (m_pGameInterface && GetHeroExperienceAvailableToSpend_API) {
        return GetHeroExperienceAvailableToSpend_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroExperienceAvailableToSpend - API not available!");
    return 0;
}

float LuaQuestState::GetHeroScariness() {
    if (m_pGameInterface && GetHeroScariness_API) {
        return GetHeroScariness_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroScariness - API not available!");
    return 0.0f;
}

float LuaQuestState::GetHeroAttractiveness() {
    if (m_pGameInterface && GetHeroAttractiveness_API) {
        return GetHeroAttractiveness_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroAttractiveness - API not available!");
    return 0.0f;
}

float LuaQuestState::GetHeroWillEnergyLevel() {
    if (m_pGameInterface && GetHeroWillEnergyLevel_API) {
        return GetHeroWillEnergyLevel_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroWillEnergyLevel - API not available!");
    return 0.0f;
}

void LuaQuestState::SetHeroWillEnergyLevel(float level) {
    if (m_pGameInterface && SetHeroWillEnergyLevel_API) {
        SetHeroWillEnergyLevel_API(m_pGameInterface, level);
    }
    else {
        Log("!!! ERROR: SetHeroWillEnergyLevel - API not available!");
    }
}

void LuaQuestState::SetHeroWillEnergyAsAbleToRefill(bool bCanRefill) {
    if (m_pGameInterface && SetHeroWillEnergyAsAbleToRefill_API) {
        SetHeroWillEnergyAsAbleToRefill_API(m_pGameInterface, bCanRefill);
    }
    else {
        Log("!!! ERROR: SetHeroWillEnergyAsAbleToRefill - API not available!");
    }
}

int LuaQuestState::GetNumberOfItemsOfTypeInInventory(const std::string& defName) {
    if (m_pGameInterface && GetNumberOfItemsOfTypeInInventory_API) {
        FableString fsDefName(defName.c_str());
        return GetNumberOfItemsOfTypeInInventory_API(m_pGameInterface, fsDefName.get());
    }
    Log("!!! ERROR: GetNumberOfItemsOfTypeInInventory - API not available!");
    return 0;
}

bool LuaQuestState::IsHeroHandLampLit() {
    if (m_pGameInterface && IsHeroHandLampLit_API) {
        return IsHeroHandLampLit_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsHeroHandLampLit - API not available!");
    return false;
}

void LuaQuestState::SetHeroHandLampAsLit(bool bIsLit) {
    if (m_pGameInterface && SetHeroHandLampAsLit_API) {
        SetHeroHandLampAsLit_API(m_pGameInterface, bIsLit);
    }
    else {
        Log("!!! ERROR: SetHeroHandLampAsLit - API not available!");
    }
}

bool LuaQuestState::IsWearingClothingItem(const std::shared_ptr<CScriptThing>& spThing, const std::string& clothingDefName) {
    CScriptThing* pThing = spThing.get();
    if (!m_pGameInterface || !IsWearingClothingItem_API) {
        Log("!!! ERROR: IsWearingClothingItem - API not available!");
        return false;
    }

    if (!pThing) {
        Log("!!! ERROR: IsWearingClothingItem - pThing is null!");
        return false;
    }

    FableString fsDefName(clothingDefName.c_str());
    return IsWearingClothingItem_API(m_pGameInterface, pThing, fsDefName.get());
}

bool LuaQuestState::IsHeroNaked() {
    if (m_pGameInterface && IsHeroNaked_API) {
        return IsHeroNaked_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsHeroNaked - API not available!");
    return false;
}

void LuaQuestState::RemoveHeroClothing() {
    if (m_pGameInterface && RemoveHeroClothing_API) {
        RemoveHeroClothing_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: RemoveHeroClothing - API not available!");
    }
}

void LuaQuestState::SetHeroAsWearing(const std::string& clothingDefName) {
    if (m_pGameInterface && SetHeroAsWearing_API) {
        FableString fsDefName(clothingDefName.c_str());
        SetHeroAsWearing_API(m_pGameInterface, fsDefName.get());
    }
    else {
        Log("!!! ERROR: SetHeroAsWearing - API not available!");
    }
}

void LuaQuestState::ChangeHeroHairstyle(const std::string& hairstyleDefName) {
    if (m_pGameInterface && ChangeHeroHairstyle_API) {
        FableString fsDefName(hairstyleDefName.c_str());
        ChangeHeroHairstyle_API(m_pGameInterface, fsDefName.get());
    }
    else {
        Log("!!! ERROR: ChangeHeroHairstyle - API not available!");
    }
}

void LuaQuestState::RemoveHeroHairstyle() {
    if (m_pGameInterface && RemoveHeroHairstyle_API) {
        RemoveHeroHairstyle_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: RemoveHeroHairstyle - API not available!");
    }
}

bool LuaQuestState::IsWearingHairstyle(const std::shared_ptr<CScriptThing>& spThing, const std::string& hairstyleDefName) {
    CScriptThing* pThing = spThing.get();
    if (!m_pGameInterface || !IsWearingHairstyle_API) {
        Log("!!! ERROR: IsWearingHairstyle - API not available!");
        return false;
    }

    if (!pThing) {
        Log("!!! ERROR: IsWearingHairstyle - pThing is null!");
        return false;
    }

    FableString fsDefName(hairstyleDefName.c_str());
    return IsWearingHairstyle_API(m_pGameInterface, pThing, fsDefName.get());
}

bool LuaQuestState::IsPlayerCarryingItemOfType(const std::string& defName) {
    if (m_pGameInterface && IsPlayerCarryingItemOfType_API) {
        FableString fsDefName(defName.c_str());
        return IsPlayerCarryingItemOfType_API(m_pGameInterface, fsDefName.get());
    }
    Log("!!! ERROR: IsPlayerCarryingItemOfType - API not available!");
    return false;
}

bool LuaQuestState::IsPlayerWieldingWeapon() {
    if (m_pGameInterface && IsPlayerWieldingWeapon_API) {
        return IsPlayerWieldingWeapon_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsPlayerWieldingWeapon - API not available!");
    return false;
}

bool LuaQuestState::IsEntityWieldingMeleeWeapon(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityWieldingMeleeWeapon_API) {
        Log("!!! ERROR: IsEntityWieldingMeleeWeapon - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityWieldingMeleeWeapon - pThing is null!");
        return false;
    }
    return IsEntityWieldingMeleeWeapon_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsEntityWieldingRangedWeapon(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityWieldingRangedWeapon_API) {
        Log("!!! ERROR: IsEntityWieldingRangedWeapon - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityWieldingRangedWeapon - pThing is null!");
        return false;
    }
    return IsEntityWieldingRangedWeapon_API(m_pGameInterface, pThing);
}

sol::object LuaQuestState::GetPreviouslyWieldedMeleeWeaponName(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetPreviouslyWieldedMeleeWeaponName_API) {
        CCharString nameResult = { 0 };
        bool result = GetPreviouslyWieldedMeleeWeaponName_API(m_pGameInterface, &nameResult);

        if (result) {
            // Event fired. Convert the output string.
            const char* text = CCharString_ToConstChar_API(&nameResult);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&nameResult);
                return sol::make_object(lua, finalString);
            }
            CCharString_Destroy(&nameResult);
            return sol::make_object(lua, true); // Succeeded but string was bad
        }
    }
    // Failed or API missing
    return sol::make_object(lua, sol::nil);
}

sol::object LuaQuestState::GetPreviouslyWieldedRangedWeaponName(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetPreviouslyWieldedRangedWeaponName_API) {
        CCharString nameResult = { 0 };
        bool result = GetPreviouslyWieldedRangedWeaponName_API(m_pGameInterface, &nameResult);

        if (result) {
            // Event fired. Convert the output string.
            const char* text = CCharString_ToConstChar_API(&nameResult);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&nameResult);
                return sol::make_object(lua, finalString);
            }
            CCharString_Destroy(&nameResult);
            return sol::make_object(lua, true);
        }
    }
    return sol::make_object(lua, sol::nil);
}

void LuaQuestState::ApplyHeroPenaltyForDeath() {
    if (m_pGameInterface && ApplyHeroPenaltyForDeath_API) {
        ApplyHeroPenaltyForDeath_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ApplyHeroPenaltyForDeath - API not available!");
    }
}

void LuaQuestState::GiveHeroTitle(const std::string& titleKey) {
    if (m_pGameInterface && GiveHeroTitle_API) {
        FableString fsTitle(titleKey.c_str());
        GiveHeroTitle_API(m_pGameInterface, fsTitle.get());
    }
    else {
        Log("!!! ERROR: GiveHeroTitle - API not available!");
    }
}

void LuaQuestState::EntitySetAsMarryable(const std::shared_ptr<CScriptThing>& spThing, bool bIsMarryable) {
    if (!m_pGameInterface || !EntitySetAsMarryable_API) {
        Log("!!! ERROR: EntitySetAsMarryable - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsMarryable - pThing is null!");
        return;
    }
    EntitySetAsMarryable_API(m_pGameInterface, pThing, bIsMarryable);
}

void LuaQuestState::EntitySetAsAbleToRegionFollowWhenMarried(const std::shared_ptr<CScriptThing>& spThing, bool bCanFollow) {
    if (!m_pGameInterface || !EntitySetAsAbleToRegionFollowWhenMarried_API) {
        Log("!!! ERROR: EntitySetAsAbleToRegionFollowWhenMarried - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsAbleToRegionFollowWhenMarried - pThing is null!");
        return;
    }
    EntitySetAsAbleToRegionFollowWhenMarried_API(m_pGameInterface, pThing, bCanFollow);
}

void LuaQuestState::EntityForceMarriageToHero(const std::shared_ptr<CScriptThing>& spThing, bool bIsForced) {
    if (!m_pGameInterface || !EntityForceMarriageToHero_API) {
        Log("!!! ERROR: EntityForceMarriageToHero - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityForceMarriageToHero - pThing is null!");
        return;
    }
    EntityForceMarriageToHero_API(m_pGameInterface, pThing, bIsForced);
}

bool LuaQuestState::IsEntityMarriedToHero(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityMarriedToHero_API) {
        Log("!!! ERROR: IsEntityMarriedToHero - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityMarriedToHero - pThing is null!");
        return false;
    }
    return IsEntityMarriedToHero_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsEntityMarriable(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityMarriable_API) {
        Log("!!! ERROR: IsEntityMarriable - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityMarriable - pThing is null!");
        return false;
    }
    return IsEntityMarriable_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsEntityWieldingWeapon(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityWieldingWeapon_API) {
        Log("!!! ERROR: IsEntityWieldingWeapon - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityWieldingWeapon - pThing is null!");
        return false;
    }
    return IsEntityWieldingWeapon_API(m_pGameInterface, pThing);
}

int LuaQuestState::GetNumberOfTimesHeroHasHadSex() {
    if (m_pGameInterface && GetNumberOfTimesHeroHasHadSex_API) {
        return GetNumberOfTimesHeroHasHadSex_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetNumberOfTimesHeroHasHadSex - API not available!");
    return 0;
}

void LuaQuestState::SetNumberOfTimesHeroHasHadSex(int count) {
    if (m_pGameInterface && SetNumberOfTimesHeroHasHadSex_API) {
        SetNumberOfTimesHeroHasHadSex_API(m_pGameInterface, count);
    }
    else {
        Log("!!! ERROR: SetNumberOfTimesHeroHasHadSex - API not available!");
    }
}

void LuaQuestState::SetHeroAsHavingHadSex(bool bHasHadSex) {
    if (m_pGameInterface && SetHeroAsHavingHadSex_API) {
        SetHeroAsHavingHadSex_API(m_pGameInterface, bHasHadSex);
    }
    else {
        Log("!!! ERROR: SetHeroAsHavingHadSex - API not available!");
    }
}

void LuaQuestState::SetHeroAsHavingHadGaySex(bool bHasHadGaySex) {
    if (m_pGameInterface && SetHeroAsHavingHadGaySex_API) {
        SetHeroAsHavingHadGaySex_API(m_pGameInterface, bHasHadGaySex);
    }
    else {
        Log("!!! ERROR: SetHeroAsHavingHadGaySex - API not available!");
    }
}

void LuaQuestState::GiveThingHeroRewardItem(const std::shared_ptr<CScriptThing>& spThing, const std::string& pItemDefName, const std::string& pBoneName) {
    if (!m_pGameInterface || !GiveThingHeroRewardItem_API) {
        Log("!!! ERROR: GiveThingHeroRewardItem - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GiveThingHeroRewardItem - pThing is null!");
        return;
    }

    FableString fsItemDefName(pItemDefName.c_str());
    FableString fsBoneName(pBoneName.c_str());
    GiveThingHeroRewardItem_API(m_pGameInterface, pThing, fsItemDefName.get(), fsBoneName.get());
}

void LuaQuestState::GiveThingItemInHand(const std::shared_ptr<CScriptThing>& spThing, const std::string& pItemDefName, bool bShow) {
    if (!m_pGameInterface || !GiveThingItemInHand_API) {
        Log("!!! ERROR: GiveThingItemInHand - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GiveThingItemInHand - pThing is null!");
        return;
    }

    FableString fsItemDefName(pItemDefName.c_str());
    GiveThingItemInHand_API(m_pGameInterface, pThing, fsItemDefName.get(), bShow);
}

void LuaQuestState::GiveThingItemInSlot(const std::shared_ptr<CScriptThing>& spThing, const std::string& pItemDefName, const std::string& pSlotName) {
    if (!m_pGameInterface || !GiveThingItemInSlot_API) {
        Log("!!! ERROR: GiveThingItemInSlot - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GiveThingItemInSlot - pThing is null!");
        return;
    }

    FableString fsItemDefName(pItemDefName.c_str());
    FableString fsSlotName(pSlotName.c_str());
    GiveThingItemInSlot_API(m_pGameInterface, pThing, fsItemDefName.get(), fsSlotName.get());
}

void LuaQuestState::GiveHeroExpression(const std::string& pExpressionName, int level, sol::optional<bool> bShowPopup) {
    if (m_pGameInterface && GiveHeroExpression_API) {
        FableString fsExpressionName(pExpressionName.c_str());
        // Default to 'true' for showing the popup
        bool bFinalShowPopup = bShowPopup.value_or(true);
        GiveHeroExpression_API(m_pGameInterface, fsExpressionName.get(), level, bFinalShowPopup);
    }
    else {
        Log("!!! ERROR: GiveHeroExpression - API not available!");
    }
}

bool LuaQuestState::HeroHasExpression(const std::string& pExpressionName) {
    if (m_pGameInterface && HeroHasExpression_API) {
        FableString fsExpressionName(pExpressionName.c_str());
        return HeroHasExpression_API(m_pGameInterface, fsExpressionName.get());
    }
    Log("!!! ERROR: HeroHasExpression - API not available!");
    return false;
}

bool LuaQuestState::IsHeroPerformingExpression(const std::string& pExpressionName) {
    if (m_pGameInterface && IsHeroPerformingExpression_API) {
        FableString fsExpressionName(pExpressionName.c_str());
        return IsHeroPerformingExpression_API(m_pGameInterface, fsExpressionName.get());
    }
    Log("!!! ERROR: IsHeroPerformingExpression - API not available!");
    return false;
}

void LuaQuestState::EntitySetAsAllowedToFollowHero(const std::shared_ptr<CScriptThing>& spThing, bool bIsAllowed) {
    if (!m_pGameInterface || !EntitySetAsAllowedToFollowHero_API) {
        Log("!!! ERROR: EntitySetAsAllowedToFollowHero - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsAllowedToFollowHero - pThing is null!");
        return;
    }
    EntitySetAsAllowedToFollowHero_API(m_pGameInterface, pThing, bIsAllowed);
}

void LuaQuestState::EntitySetAsAllowedToChangeRegionFollowingState(const std::shared_ptr<CScriptThing>& spThing, bool bIsAllowed) {
    if (!m_pGameInterface || !EntitySetAsAllowedToChangeRegionFollowingState_API) {
        Log("!!! ERROR: EntitySetAsAllowedToChangeRegionFollowingState - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsAllowedToChangeRegionFollowingState - pThing is null!");
        return;
    }
    EntitySetAsAllowedToChangeRegionFollowingState_API(m_pGameInterface, pThing, bIsAllowed);
}

void LuaQuestState::EntitySetAsRespondingToFollowAndWaitExpressions(const std::shared_ptr<CScriptThing>& spThing, bool bDoesRespond) {
    if (!m_pGameInterface || !EntitySetAsRespondingToFollowAndWaitExpressions_API) {
        Log("!!! ERROR: EntitySetAsRespondingToFollowAndWaitExpressions - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsRespondingToFollowAndWaitExpressions - pThing is null!");
        return;
    }
    EntitySetAsRespondingToFollowAndWaitExpressions_API(m_pGameInterface, pThing, bDoesRespond);
}

void LuaQuestState::EntitySetAsMirroringHeroEnemyRelationsWhileFollowing(const std::shared_ptr<CScriptThing>& spThing, bool bDoesMirror) {
    if (!m_pGameInterface || !EntitySetAsMirroringHeroEnemyRelationsWhileFollowing_API) {
        Log("!!! ERROR: EntitySetAsMirroringHeroEnemyRelationsWhileFollowing - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsMirroringHeroEnemyRelationsWhileFollowing - pThing is null!");
        return;
    }
    EntitySetAsMirroringHeroEnemyRelationsWhileFollowing_API(m_pGameInterface, pThing, bDoesMirror);
}

void LuaQuestState::TeleportAllFollowersToHeroPosition() {
    if (m_pGameInterface && TeleportAllFollowersToHeroPosition_API) {
        TeleportAllFollowersToHeroPosition_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: TeleportAllFollowersToHeroPosition - API not available!");
    }
}

void LuaQuestState::EntityTeleportToHeroPosition(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityTeleportToHeroPosition_API) {
        Log("!!! ERROR: EntityTeleportToHeroPosition - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityTeleportToHeroPosition - pThing is null!");
        return;
    }
    EntityTeleportToHeroPosition_API(m_pGameInterface, pThing);
}

void LuaQuestState::SendEntityEvent(int eventType, const std::shared_ptr<CScriptThing>& spSender, const std::shared_ptr<CScriptThing>& spReceiver) {
    if (m_pGameInterface && SendEntityEvent_API) {
        NEntityEvents::EEventType eEvent = static_cast<NEntityEvents::EEventType>(eventType);

        // .get() on a nil shared_ptr correctly returns nullptr, which is safe.
        CScriptThing* pSender = spSender.get();
        CScriptThing* pReceiver = spReceiver.get();

        // Pass nullptr for the CThing* pExtraData, as we can't safely create that from Lua
        SendEntityEvent_API(m_pGameInterface, eEvent, pSender, pReceiver, nullptr);
    }
    else {
        Log("!!! ERROR: SendEntityEvent - API not available!");
    }
}

float LuaQuestState::GetWaterHeightAtPosition(sol::table position) {
    if (m_pGameInterface && GetWaterHeightAtPosition_API) {
        C3DVector pos = {
            position["x"].get_or(0.0f),
            position["y"].get_or(0.0f),
            position["z"].get_or(0.0f)
        };
        return GetWaterHeightAtPosition_API(m_pGameInterface, &pos);
    }

    Log("!!! ERROR: GetWaterHeightAtPosition - API not available!");
    return -10000.0f; // Return a low value to indicate error
}

// ForgeFSE addition: return the GROUND (collision) surface height the hero stands on at
// world (x,y). Backed by CWorldMap::GetGroundSizeZAt @0x0063A300 on the resolved CWorldMap
// singleton (same object GoToMapSlot uses). Z of the probe vector is irrelevant to the lookup.
float LuaQuestState::GetGroundHeightAt(float x, float y) {
    if (!m_pGameInterface || !CWorldMap_GetGroundSizeZAt_API) {
        Log("!!! ERROR: GetGroundHeightAt - API not available!");
        return -10000.0f; // sentinel: low value signals failure (matches GetWaterHeightAtPosition)
    }
    void* pWorldMap = ResolveWorldMap(m_pGameInterface);
    if (!pWorldMap) {
        Log("!!! ERROR: GetGroundHeightAt - could not resolve CWorldMap.");
        return -10000.0f;
    }
    const C3DVector probe = { x, y, 0.0f };
    float h = CWorldMap_GetGroundSizeZAt_API(pWorldMap, reinterpret_cast<const float*>(&probe));
    {
        std::stringstream ss;
        ss << "GetGroundHeightAt(" << x << "," << y << ") = " << h;
        Log(ss.str());
    }
    return h;
}

float LuaQuestState::GetHeroHealthMax() {
    if (m_pGameInterface && GetHeroHealthMax_API) {
        return GetHeroHealthMax_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHealthMax - API not available!");
    return 0.0f;
}

float LuaQuestState::GetHeroHealthPercentage() {
    if (m_pGameInterface && GetHeroHealthPercentage_API) {
        return GetHeroHealthPercentage_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHealthPercentage - API not available!");
    return 0.0f;
}

int LuaQuestState::GetHeroWillEnergy() {
    if (m_pGameInterface && GetHeroWillEnergy_API) {
        return GetHeroWillEnergy_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroWillEnergy - API not available!");
    return 0;
}

int LuaQuestState::GetHeroWillEnergyMax() {
    if (m_pGameInterface && GetHeroWillEnergyMax_API) {
        return GetHeroWillEnergyMax_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroWillEnergyMax - API not available!");
    return 0;
}

int LuaQuestState::GetHeroWillEnergyPercentage() {
    if (m_pGameInterface && GetHeroWillEnergyPercentage_API) {
        return GetHeroWillEnergyPercentage_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroWillEnergyPercentage - API not available!");
    return 0;
}

void LuaQuestState::ModifyThingHealth(const std::shared_ptr<CScriptThing>& spThing, float amount, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !ModifyThingHealth_API) {
        Log("!!! ERROR: ModifyThingHealth - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ModifyThingHealth - pThing is null!");
        return;
    }
    // Default the unknown bool to 'true'
    bool bFinalUnknown = bUnknown.value_or(true);
    ModifyThingHealth_API(m_pGameInterface, pThing, amount, bFinalUnknown);
}

void LuaQuestState::EntitySetMaxHealth(const std::shared_ptr<CScriptThing>& spThing, float maxHealth, bool bSetCurrentToMax) {
    if (!m_pGameInterface || !EntitySetMaxHealth_API) {
        Log("!!! ERROR: EntitySetMaxHealth - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetMaxHealth - pThing is null!");
        return;
    }
    EntitySetMaxHealth_API(m_pGameInterface, pThing, maxHealth, bSetCurrentToMax);
}

void LuaQuestState::SetThingAsKilled(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !SetThingAsKilled_API) {
        Log("!!! ERROR: SetThingAsKilled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetThingAsKilled - pThing is null!");
        return;
    }
    SetThingAsKilled_API(m_pGameInterface, pThing);
}

void LuaQuestState::GiveHeroNewQuestObjective(const std::string& pObjectiveText, unsigned int objectiveID) {
    if (m_pGameInterface && GiveHeroNewQuestObjective_API) {
        FableString fsObjective(pObjectiveText.c_str());
        GiveHeroNewQuestObjective_API(m_pGameInterface, fsObjective.get(), objectiveID);
    }
    else {
        Log("!!! ERROR: GiveHeroNewQuestObjective - API not available!");
    }
}

void LuaQuestState::TellHeroQuestObjectiveCompleted(unsigned int objectiveID) {
    if (m_pGameInterface && TellHeroQuestObjectiveCompleted_API) {
        TellHeroQuestObjectiveCompleted_API(m_pGameInterface, objectiveID);
    }
    else {
        Log("!!! ERROR: TellHeroQuestObjectiveCompleted - API not available!");
    }
}

void LuaQuestState::TellHeroQuestObjectiveFailed(unsigned int objectiveID) {
    if (m_pGameInterface && TellHeroQuestObjectiveFailed_API) {
        TellHeroQuestObjectiveFailed_API(m_pGameInterface, objectiveID);
    }
    else {
        Log("!!! ERROR: TellHeroQuestObjectiveFailed - API not available!");
    }
}

bool LuaQuestState::IsHeroOnQuest() {
    if (m_pGameInterface && IsHeroOnQuest_API) {
        return IsHeroOnQuest_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsHeroOnQuest - API not available!");
    return false;
}

void LuaQuestState::SetGuildMasterMessages(bool bAreEnabled) {
    if (m_pGameInterface && SetGuildMasterMessages_API) {
        SetGuildMasterMessages_API(m_pGameInterface, bAreEnabled);
    }
    else {
        Log("!!! ERROR: SetGuildMasterMessages - API not available!");
    }
}

bool LuaQuestState::IsQuestActive(const std::string& questName) {
    if (m_pGameInterface && IsQuestActive_API) {
        FableString fsQuest(questName.c_str());
        return IsQuestActive_API(m_pGameInterface, fsQuest.get());
    }
    Log("!!! ERROR: IsQuestActive - API not available!");
    return false;
}

bool LuaQuestState::IsQuestRegistered(const std::string& questName) {
    if (m_pGameInterface && IsQuestRegistered_API) {
        FableString fsQuest(questName.c_str());
        return IsQuestRegistered_API(m_pGameInterface, fsQuest.get());
    }
    Log("!!! ERROR: IsQuestRegistered - API not available!");
    return false;
}

bool LuaQuestState::IsQuestCompleted(const std::string& questName) {
    if (m_pGameInterface && IsQuestCompleted_API) {
        FableString fsQuest(questName.c_str());
        return IsQuestCompleted_API(m_pGameInterface, fsQuest.get());
    }
    Log("!!! ERROR: IsQuestCompleted - API not available!");
    return false;
}

bool LuaQuestState::IsQuestFailed(const std::string& questName) {
    if (m_pGameInterface && IsQuestFailed_API) {
        FableString fsQuest(questName.c_str());
        return IsQuestFailed_API(m_pGameInterface, fsQuest.get());
    }
    Log("!!! ERROR: IsQuestFailed - API not available!");
    return false;
}

void LuaQuestState::SetQuestAsPersistent(const std::string& questName, bool bIsPersistent) {
    if (m_pGameInterface && SetQuestAsPersistent_API) {
        FableString fsQuest(questName.c_str());
        SetQuestAsPersistent_API(m_pGameInterface, fsQuest.get(), bIsPersistent);
    }
    else {
        Log("!!! ERROR: SetQuestAsPersistent - API not available!");
    }
}

std::string LuaQuestState::GetExclusiveQuestScriptName() {
    if (m_pGameInterface && GetExclusiveQuestScriptName_API) {
        // The API returns a direct, game-managed pointer
        const CCharString* pResult = GetExclusiveQuestScriptName_API(m_pGameInterface);
        if (pResult && pResult->pStringData) {
            const char* text = CCharString_ToConstChar_API(pResult);
            if (text) {
                // Return a copy of the string. DO NOT destroy the pointer.
                return std::string(text);
            }
        }
    }
    Log("!!! ERROR: GetExclusiveQuestScriptName - API not available or returned null!");
    return "";
}

void LuaQuestState::RemoveQuestCardFromGuild(const std::string& questName) {
    if (m_pGameInterface && RemoveQuestCardFromGuild_API) {
        FableString fsQuest(questName.c_str());
        RemoveQuestCardFromGuild_API(m_pGameInterface, fsQuest.get());
    }
    else {
        Log("!!! ERROR: RemoveQuestCardFromGuild - API not available!");
    }
}

void LuaQuestState::RemoveQuestCardFromHero(const std::string& questName) {
    if (m_pGameInterface && RemoveQuestCardFromHero_API) {
        FableString fsQuest(questName.c_str());
        RemoveQuestCardFromHero_API(m_pGameInterface, fsQuest.get());
    }
    else {
        Log("!!! ERROR: RemoveQuestCardFromHero - API not available!");
    }
}

void LuaQuestState::RemoveAllAvailableQuestCardsFromGuild() {
    if (m_pGameInterface && RemoveAllAvailableQuestCardsFromGuild_API) {
        RemoveAllAvailableQuestCardsFromGuild_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: RemoveAllAvailableQuestCardsFromGuild - API not available!");
    }
}

void LuaQuestState::FailAllActiveQuests() {
    if (m_pGameInterface && FailAllActiveQuests_API) {
        FailAllActiveQuests_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: FailAllActiveQuests - API not available!");
    }
}

sol::table LuaQuestState::GetAllActiveQuestInfo(sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (m_pGameInterface && GetAllActiveQuestInfo_API) {
        std::vector<CCharString> outNames;
        std::vector<CCharString> outDescriptions;

        // Call the API which fills our vectors
        GetAllActiveQuestInfo_API(m_pGameInterface, &outNames, &outDescriptions);

        // Process the results into a Lua table
        if (outNames.size() == outDescriptions.size()) {
            for (size_t i = 0; i < outNames.size(); ++i) {
                sol::table questInfo = lua.create_table();
                const char* nameText = CCharString_ToConstChar_API(&outNames[i]);
                const char* descText = CCharString_ToConstChar_API(&outDescriptions[i]);

                if (nameText) {
                    questInfo["name"] = std::string(nameText);
                }
                if (descText) {
                    questInfo["description"] = std::string(descText);
                }

                // Add to the main table (Lua tables are 1-indexed)
                resultTable[i + 1] = questInfo;

                // We must destroy the CCharString copies the game gave us
                CCharString_Destroy(&outNames[i]);
                CCharString_Destroy(&outDescriptions[i]);
            }
        }
        else {
            Log("!!! ERROR: GetAllActiveQuestInfo - Mismatched name and description counts!");
        }
    }
    else {
        Log("!!! ERROR: GetAllActiveQuestInfo - API not available!");
    }

    return resultTable;
}

void LuaQuestState::AddFeatCard(int featID, const std::string& name, const std::string& description) {
    if (m_pGameInterface && AddFeatCard_API) {
        FableString fsName(name.c_str());
        FableString fsDesc(description.c_str());
        AddFeatCard_API(m_pGameInterface, featID, fsName.get(), fsDesc.get());
    }
    else {
        Log("!!! ERROR: AddFeatCard - API not available!");
    }
}

void LuaQuestState::AddBoast(const std::string& questName, int boastID, int renown, int gold, bool bIsBoast, const std::string& boastText, int textID) {
    if (m_pGameInterface && AddBoast_API) {
        FableString fsQuest(questName.c_str());
        FableString fsBoast(boastText.c_str());
        AddBoast_API(m_pGameInterface, fsQuest.get(), boastID, renown, gold, bIsBoast, fsBoast.get(), textID);
    }
    else {
        Log("!!! ERROR: AddBoast - API not available!");
    }
}

void LuaQuestState::RemoveBoast(int boastID, const std::string& questName) {
    if (m_pGameInterface && RemoveBoast_API) {
        FableString fsQuest(questName.c_str());
        RemoveBoast_API(m_pGameInterface, boastID, fsQuest.get());
    }
    else {
        Log("!!! ERROR: RemoveBoast - API not available!");
    }
}

void LuaQuestState::SetBoastAsFailed(int boastID, const std::string& questName) {
    if (m_pGameInterface && SetBoastAsFailed_API) {
        FableString fsQuest(questName.c_str());
        SetBoastAsFailed_API(m_pGameInterface, boastID, fsQuest.get());
    }
    else {
        Log("!!! ERROR: SetBoastAsFailed - API not available!");
    }
}

void LuaQuestState::SetBoastAsCompleted(int boastID, const std::string& questName) {
    if (m_pGameInterface && SetBoastAsCompleted_API) {
        FableString fsQuest(questName.c_str());
        SetBoastAsCompleted_API(m_pGameInterface, boastID, fsQuest.get());
    }
    else {
        Log("!!! ERROR: SetBoastAsCompleted - API not available!");
    }
}

bool LuaQuestState::IsBoastTaken(int boastID, const std::string& questName) {
    if (m_pGameInterface && IsBoastTaken_API) {
        FableString fsQuest(questName.c_str());
        return IsBoastTaken_API(m_pGameInterface, boastID, fsQuest.get());
    }
    Log("!!! ERROR: IsBoastTaken - API not available!");
    return false;
}

void LuaQuestState::KickOffDeathScreen() {
    if (m_pGameInterface && KickOffDeathScreen_API) {
        KickOffDeathScreen_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: KickOffDeathScreen - API not available!");
    }
}

void LuaQuestState::KickOffCreditsScreen(const std::string& creditsName) {
    if (m_pGameInterface && KickOffCreditsScreen_API) {
        FableString fsCredits(creditsName.c_str());
        KickOffCreditsScreen_API(m_pGameInterface, fsCredits.get());
    }
    else {
        Log("!!! ERROR: KickOffCreditsScreen - API not available!");
    }
}

void LuaQuestState::SetPreferredQuickAccessItem(const std::string& itemName, int dPadDirection, int slot) {
    if (m_pGameInterface && SetPreferredQuickAccessItem_API) {
        FableString fsItem(itemName.c_str());
        SetPreferredQuickAccessItem_API(m_pGameInterface, fsItem.get(), dPadDirection, slot);
    }
    else {
        Log("!!! ERROR: SetPreferredQuickAccessItem - API not available!");
    }
}

std::string LuaQuestState::GetDeathRecoveryMarkerName() {
    if (m_pGameInterface && GetDeathRecoveryMarkerName_API) {
        CCharString resultName = { 0 };
        // Pass our stack-allocated CCharString as the result buffer
        GetDeathRecoveryMarkerName_API(m_pGameInterface, &resultName);

        if (resultName.pStringData) {
            const char* text = CCharString_ToConstChar_API(&resultName);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&resultName); // Must destroy the contents
                return finalString;
            }
            CCharString_Destroy(&resultName);
        }
    }
    Log("!!! ERROR: GetDeathRecoveryMarkerName - API not available or returned null!");
    return "";
}

void LuaQuestState::SetDeathRecoveryMarkerName(const std::string& markerName) {
    if (m_pGameInterface && SetDeathRecoveryMarkerName_API) {
        FableString fsMarker(markerName.c_str());
        SetDeathRecoveryMarkerName_API(m_pGameInterface, fsMarker.get());
    }
    else {
        Log("!!! ERROR: SetDeathRecoveryMarkerName - API not available!");
    }
}

void LuaQuestState::ResetDeathRecoveryMarkerNameToDefault() {
    if (m_pGameInterface && ResetDeathRecoveryMarkerNameToDefault_API) {
        ResetDeathRecoveryMarkerNameToDefault_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ResetDeathRecoveryMarkerNameToDefault - API not available!");
    }
}

bool LuaQuestState::IsToFailQuestOnDeath() {
    if (m_pGameInterface && IsToFailQuestOnDeath_API) {
        return IsToFailQuestOnDeath_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsToFailQuestOnDeath - API not available!");
    return false; // Default to not failing
}

void LuaQuestState::SetWhetherToFailQuestOnDeath(bool bShouldFail) {
    if (m_pGameInterface && SetWhetherToFailQuestOnDeath_API) {
        SetWhetherToFailQuestOnDeath_API(m_pGameInterface, bShouldFail);
    }
    else {
        Log("!!! ERROR: SetWhetherToFailQuestOnDeath - API not available!");
    }
}

void LuaQuestState::ResetWhetherToFailQuestOnDeathToDefault() {
    if (m_pGameInterface && ResetWhetherToFailQuestOnDeathToDefault_API) {
        ResetWhetherToFailQuestOnDeathToDefault_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ResetWhetherToFailQuestOnDeathToDefault - API not available!");
    }
}

std::shared_ptr<CScriptThing> LuaQuestState::GetMostRecentValidUsedTarget() {
    if (!m_pGameInterface || !GetMostRecentValidUsedTarget_API || !Game_malloc) {
        Log("!!! ERROR: GetMostRecentValidUsedTarget - API or memory allocator not available!");
        return nullptr;
    }

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: GetMostRecentValidUsedTarget - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    GetMostRecentValidUsedTarget_API(m_pGameInterface, pResult);

    // Wrap, validate, and return
    return WrapScriptThingOutput(pResult);
}

std::string LuaQuestState::GetMostRecentValidUsedTargetName() {
    if (m_pGameInterface && GetMostRecentValidUsedTargetName_API) {
        CCharString resultName = { 0 };
        GetMostRecentValidUsedTargetName_API(m_pGameInterface, &resultName);

        if (resultName.pStringData) {
            const char* text = CCharString_ToConstChar_API(&resultName);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&resultName);
                return finalString;
            }
            CCharString_Destroy(&resultName);
        }
    }
    Log("!!! ERROR: GetMostRecentValidUsedTargetName - API not available or returned null!");
    return "";
}

int LuaQuestState::AddQuestInfoTimer(int timerId, const std::string& text, float scale) {
    if (m_pGameInterface && AddQuestInfoTimer_API) {
        // Create the CTimer struct on the stack
        CTimer timer;
        timer.TimerIndex = timerId;

        FableString fsText(text.c_str());
        return AddQuestInfoTimer_API(m_pGameInterface, &timer, fsText.get(), scale);
    }
    Log("!!! ERROR: AddQuestInfoTimer - API not available!");
    return -1; // Return invalid ID
}

int LuaQuestState::AddQuestInfoCounterList(const std::string& text, int count, float scale) {
    if (m_pGameInterface && AddQuestInfoCounterList_API) {
        FableString fsText(text.c_str());
        return AddQuestInfoCounterList_API(m_pGameInterface, fsText.get(), count, scale);
    }
    Log("!!! ERROR: AddQuestInfoCounterList - API not available!");
    return -1; // Return invalid ID
}

int LuaQuestState::AddQuestInfoTickByAction(int gameAction, bool bState, float scale) {
    if (m_pGameInterface && AddQuestInfoTick_ByAction_API) {
        EGameAction eAction = static_cast<EGameAction>(gameAction);
        return AddQuestInfoTick_ByAction_API(m_pGameInterface, eAction, bState, scale);
    }
    Log("!!! ERROR: AddQuestInfoTickByAction - API not available!");
    return -1; // Return invalid ID
}

int LuaQuestState::AddQuestInfoTickByText(const std::string& text, bool bState, float scale) {
    if (m_pGameInterface && AddQuestInfoTick_ByText_API) {
        FableString fsText(text.c_str());
        return AddQuestInfoTick_ByText_API(m_pGameInterface, fsText.get(), bState, scale);
    }
    Log("!!! ERROR: AddQuestInfoTickByText - API not available!");
    return -1; // Return invalid ID
}

void LuaQuestState::UpdateQuestInfoBar(int id, float current, float max, float scale) {
    if (m_pGameInterface && UpdateQuestInfoBar_API) {
        UpdateQuestInfoBar_API(m_pGameInterface, id, current, max, scale);
    }
    else {
        Log("!!! ERROR: UpdateQuestInfoBar - API not available!");
    }
}

void LuaQuestState::ChangeQuestInfoBarColour(int id, sol::table color1, sol::table color2) {
    if (m_pGameInterface && ChangeQuestInfoBarColour_API) {
        // Parse the retail BGRA byte layout.
        CRGBColour c1 = ParseColorTable(color1);
        CRGBColour c2 = ParseColorTable(color2);
        ChangeQuestInfoBarColour_API(m_pGameInterface, id, &c1, &c2);
    }
    else {
        Log("!!! ERROR: ChangeQuestInfoBarColour - API not available!");
    }
}

void LuaQuestState::UpdateQuestInfoTimer(int id, float scale) {
    if (m_pGameInterface && UpdateQuestInfoTimer_API) {
        UpdateQuestInfoTimer_API(m_pGameInterface, id, scale);
    }
    else {
        Log("!!! ERROR: UpdateQuestInfoTimer - API not available!");
    }
}

void LuaQuestState::UpdateQuestInfoCounterList(int id, int count, int max) {
    if (m_pGameInterface && UpdateQuestInfoCounterList_API) {
        UpdateQuestInfoCounterList_API(m_pGameInterface, id, count, max);
    }
    else {
        Log("!!! ERROR: UpdateQuestInfoCounterList - API not available!");
    }
}

void LuaQuestState::UpdateQuestInfoTick(int id, bool bState) {
    if (m_pGameInterface && UpdateQuestInfoTick_API) {
        UpdateQuestInfoTick_API(m_pGameInterface, id, bState);
    }
    else {
        Log("!!! ERROR: UpdateQuestInfoTick - API not available!");
    }
}

void LuaQuestState::RemoveQuestInfoElement(int id) {
    if (m_pGameInterface && RemoveQuestInfoElement_API) {
        RemoveQuestInfoElement_API(m_pGameInterface, id);
    }
    else {
        Log("!!! ERROR: RemoveQuestInfoElement - API not available!");
    }
}

void LuaQuestState::RemoveAllQuestInfoElements() {
    if (m_pGameInterface && RemoveAllQuestInfoElements_API) {
        RemoveAllQuestInfoElements_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: RemoveAllQuestInfoElements - API not available!");
    }
}

void LuaQuestState::DisplayTime(bool bDisplay) {
    if (m_pGameInterface && DisplayTime_API) {
        DisplayTime_API(m_pGameInterface, bDisplay);
    }
    else {
        Log("!!! ERROR: DisplayTime - API not available!");
    }
}

void LuaQuestState::DisplayMoneyBag(bool bDisplay) {
    if (m_pGameInterface && DisplayMoneyBag_API) {
        DisplayMoneyBag_API(m_pGameInterface, bDisplay);
    }
    else {
        Log("!!! ERROR: DisplayMoneyBag - API not available!");
    }
}

void LuaQuestState::DisplayMiniGameInfo(bool bDisplay, int gameType) {
    if (m_pGameInterface && DisplayMiniGameInfo_API) {
        EMiniGameType eType = static_cast<EMiniGameType>(gameType);
        DisplayMiniGameInfo_API(m_pGameInterface, bDisplay, eType);
    }
    else {
        Log("!!! ERROR: DisplayMiniGameInfo - API not available!");
    }
}

void LuaQuestState::UpdateMiniGameInfoBar(float value) {
    if (m_pGameInterface && UpdateMiniGameInfoBar_API) {
        UpdateMiniGameInfoBar_API(m_pGameInterface, value);
    }
    else {
        Log("!!! ERROR: UpdateMiniGameInfoBar - API not available!");
    }
}

bool LuaQuestState::IsEntityPickPocketable(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityPickPocketable_API) {
        Log("!!! ERROR: IsEntityPickPocketable - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityPickPocketable - pThing is null!");
        return false;
    }
    return IsEntityPickPocketable_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsEntityPickLockable(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityPickLockable_API) {
        Log("!!! ERROR: IsEntityPickLockable - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityPickLockable - pThing is null!");
        return false;
    }
    return IsEntityPickLockable_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsEntityStealable(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityStealable_API) {
        Log("!!! ERROR: IsEntityStealable - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityStealable - pThing is null!");
        return false;
    }
    return IsEntityStealable_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetAsPickPocketed(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntitySetAsPickPocketed_API) {
        Log("!!! ERROR: EntitySetAsPickPocketed - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsPickPocketed - pThing is null!");
        return;
    }
    EntitySetAsPickPocketed_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetAsPickLocked(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntitySetAsPickLocked_API) {
        Log("!!! ERROR: EntitySetAsPickLocked - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsPickLocked - pThing is null!");
        return;
    }
    EntitySetAsPickLocked_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetAsStolen(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntitySetAsStolen_API) {
        Log("!!! ERROR: EntitySetAsStolen - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsStolen - pThing is null!");
        return;
    }
    EntitySetAsStolen_API(m_pGameInterface, pThing);
}

void LuaQuestState::MiniMapAddMarker(CScriptThing* pThing, const std::string& markerName) {
    if (!m_pGameInterface || !MiniMapAddMarker_API) {
        Log("!!! ERROR: MiniMapAddMarker - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: MiniMapAddMarker - pThing is null!");
        return;
    }
    FableString fsMarker(markerName.c_str());
    MiniMapAddMarker_API(m_pGameInterface, pThing, fsMarker.get());
}

void LuaQuestState::MiniMapSetMarkerGraphic(const std::shared_ptr<CScriptThing>& spThing, const std::string& graphicName) {
    if (!m_pGameInterface || !MiniMapSetMarkerGraphic_API) {
        Log("!!! ERROR: MiniMapSetMarkerGraphic - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: MiniMapSetMarkerGraphic - pThing is null!");
        return;
    }
    FableString fsGraphic(graphicName.c_str());
    MiniMapSetMarkerGraphic_API(m_pGameInterface, pThing, fsGraphic.get());
}

void LuaQuestState::MiniMapRemoveMarker(CScriptThing* pThing) {
    if (!m_pGameInterface || !MiniMapRemoveMarker_API) {
        Log("!!! ERROR: MiniMapRemoveMarker - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: MiniMapRemoveMarker - pThing is null!");
        return;
    }
    MiniMapRemoveMarker_API(m_pGameInterface, pThing);
}

void LuaQuestState::MiniMapRemoveAllMarkers() {
    if (m_pGameInterface && MiniMapRemoveAllMarkers_API) {
        MiniMapRemoveAllMarkers_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: MiniMapRemoveAllMarkers - API not available!");
    }
}

void LuaQuestState::MiniMapAllowRouteBetweenRegions(const std::string& region1, const std::string& region2, bool bAllow) {
    if (m_pGameInterface && MiniMapAllowRouteBetweenRegions_API) {
        FableString fsRegion1(region1.c_str());
        FableString fsRegion2(region2.c_str());
        MiniMapAllowRouteBetweenRegions_API(m_pGameInterface, fsRegion1.get(), fsRegion2.get(), bAllow);
    }
    else {
        Log("!!! ERROR: MiniMapAllowRouteBetweenRegions - API not available!");
    }
}

void LuaQuestState::MiniMapSetAsEnabled(bool bIsEnabled) {
    if (m_pGameInterface && MiniMapSetAsEnabled_API) {
        MiniMapSetAsEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: MiniMapSetAsEnabled - API not available!");
    }
}

void LuaQuestState::EntitySetAsHiddenOnMiniMap(const std::shared_ptr<CScriptThing>& spThing, bool bIsHidden) {
    if (!m_pGameInterface || !EntitySetAsHiddenOnMiniMap_API) {
        Log("!!! ERROR: EntitySetAsHiddenOnMiniMap - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsHiddenOnMiniMap - pThing is null!");
        return;
    }
    EntitySetAsHiddenOnMiniMap_API(m_pGameInterface, pThing, bIsHidden);
}

void LuaQuestState::SetHUDEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetHUDEnabled_API) {
        SetHUDEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetHUDEnabled - API not available!");
    }
}

void LuaQuestState::EntitySetWillBeUsingNarrator(const std::shared_ptr<CScriptThing>& spThing, const std::string& dialogueKey) {
    if (!m_pGameInterface || !EntitySetWillBeUsingNarrator_API) {
        Log("!!! ERROR: EntitySetWillBeUsingNarrator - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetWillBeUsingNarrator - pThing is null!");
        return;
    }
    FableString fsKey(dialogueKey.c_str());
    EntitySetWillBeUsingNarrator_API(m_pGameInterface, pThing, fsKey.get());
}

void LuaQuestState::EntityResetAsPureAINarrator(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityResetAsPureAINarrator_API) {
        Log("!!! ERROR: EntityResetAsPureAINarrator - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityResetAsPureAINarrator - pThing is null!");
        return;
    }
    EntityResetAsPureAINarrator_API(m_pGameInterface, pThing);
}

void LuaQuestState::PlayAVIMovie(const std::string& movieName) {
    if (m_pGameInterface && PlayAVIMovie_API) {
        FableString fsMovie(movieName.c_str());
        PlayAVIMovie_API(m_pGameInterface, fsMovie.get());
    }
    else {
        Log("!!! ERROR: PlayAVIMovie - API not available!");
    }
}

void LuaQuestState::FadeScreenOutUntilNextCallToFadeScreenIn(float fadeTime, float holdTime) {
    if (m_pGameInterface && FadeScreenOutUntilNextCallToFadeScreenIn_API) {
        FadeScreenOutUntilNextCallToFadeScreenIn_API(m_pGameInterface, fadeTime, holdTime);
    }
    else {
        Log("!!! ERROR: FadeScreenOutUntilNextCallToFadeScreenIn - API not available!");
    }
}

bool LuaQuestState::IsScreenFadingOut() {
    if (m_pGameInterface && IsScreenFadingOut_API) {
        return IsScreenFadingOut_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsScreenFadingOut - API not available!");
    return false;
}

void LuaQuestState::EndCutFade() {
    if (m_pGameInterface && EndCutFade_API) {
        EndCutFade_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: EndCutFade - API not available!");
    }
}

void LuaQuestState::EndLetterBox() {
    if (m_pGameInterface && EndLetterBox_API) {
        EndLetterBox_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: EndLetterBox - API not available!");
    }
}

void LuaQuestState::PauseAllEntities(bool bIsPaused) {
    if (m_pGameInterface && PauseAllEntities_API) {
        PauseAllEntities_API(m_pGameInterface, bIsPaused);
    }
    else {
        Log("!!! ERROR: PauseAllEntities - API not available!");
    }
}

void LuaQuestState::SetAllowScreenFadingOnNextRegionChange(bool bIsAllowed) {
    if (m_pGameInterface && SetAllowScreenFadingOnNextRegionChange_API) {
        SetAllowScreenFadingOnNextRegionChange_API(m_pGameInterface, bIsAllowed);
    }
    else {
        Log("!!! ERROR: SetAllowScreenFadingOnNextRegionChange - API not available!");
    }
}

void LuaQuestState::SetAllowScreenFadingIfAlreadyFaded(bool bIsAllowed) {
    if (m_pGameInterface && SetAllowScreenFadingIfAlreadyFaded_API) {
        SetAllowScreenFadingIfAlreadyFaded_API(m_pGameInterface, bIsAllowed);
    }
    else {
        Log("!!! ERROR: SetAllowScreenFadingIfAlreadyFaded - API not available!");
    }
}

void LuaQuestState::SetToKeepHeroAbilitiesDuringCutscenes(bool bDoKeep) {
    if (m_pGameInterface && SetToKeepHeroAbilitiesDuringCutscenes_API) {
        SetToKeepHeroAbilitiesDuringCutscenes_API(m_pGameInterface, bDoKeep);
    }
    else {
        Log("!!! ERROR: SetToKeepHeroAbilitiesDuringCutscenes - API not available!");
    }
}

void LuaQuestState::SetCutsceneSkippable(bool bIsSkippable) {
    if (m_pGameInterface && SetCutsceneSkippable_API) {
        SetCutsceneSkippable_API(m_pGameInterface, bIsSkippable);
    }
    else {
        Log("!!! ERROR: SetCutsceneSkippable - API not available!");
    }
}

void LuaQuestState::PreloadNewScene(float unknown) {
    if (m_pGameInterface && PreloadNewScene_API) {
        PreloadNewScene_API(m_pGameInterface, unknown);
    }
    else {
        Log("!!! ERROR: PreloadNewScene - API not available!");
    }
}

void LuaQuestState::StartProgressDisplay() {
    if (m_pGameInterface && StartProgressDisplay_API) {
        StartProgressDisplay_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: StartProgressDisplay - API not available!");
    }
}

void LuaQuestState::StopProgressDisplay() {
    if (m_pGameInterface && StopProgressDisplay_API) {
        StopProgressDisplay_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: StopProgressDisplay - API not available!");
    }
}

void LuaQuestState::SetScreenMessagesEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetScreenMessagesEnabled_API) {
        SetScreenMessagesEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetScreenMessagesEnabled - API not available!");
    }
}

void LuaQuestState::SetAbilityAvailability(int ability, bool bIsAvailable) {
    if (m_pGameInterface && SetAbilityAvailability_API) {
        // Assumes EHeroAbility enum is available
        SetAbilityAvailability_API(m_pGameInterface, static_cast<EHeroAbility>(ability), bIsAvailable);
    }
    else {
        Log("!!! ERROR: SetAbilityAvailability - API not available!");
    }
}

void LuaQuestState::SetEnvironmentalEffectsAlwaysUpdate(bool bAlwaysUpdate) {
    if (m_pGameInterface && SetEnvironmentalEffectsAlwaysUpdate_API) {
        SetEnvironmentalEffectsAlwaysUpdate_API(m_pGameInterface, bAlwaysUpdate);
    }
    else {
        Log("!!! ERROR: SetEnvironmentalEffectsAlwaysUpdate - API not available!");
    }
}

void LuaQuestState::SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden(bool bIsHidden) {
    if (m_pGameInterface && SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden_API) {
        SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden_API(m_pGameInterface, bIsHidden);
    }
    else {
        Log("!!! ERROR: SetDeadCreaturesAndExperienceOrbsAndDropBagsAsHidden - API not available!");
    }
}

void LuaQuestState::RemoveDeadCreature(const std::string& creatureName) {
    if (m_pGameInterface && RemoveDeadCreature_API) {
        FableString fsCreature(creatureName.c_str());
        RemoveDeadCreature_API(m_pGameInterface, fsCreature.get());
    }
    else {
        Log("!!! ERROR: RemoveDeadCreature - API not available!");
    }
}

void LuaQuestState::CameraSetCameraPreloadFlag(bool bPreload) {
    if (m_pGameInterface && CameraSetCameraPreloadFlag_API) {
        CameraSetCameraPreloadFlag_API(m_pGameInterface, bPreload);
    }
    else {
        Log("!!! ERROR: CameraSetCameraPreloadFlag - API not available!");
    }
}

void LuaQuestState::CameraCircleAroundThing(const std::shared_ptr<CScriptThing>& spThing, sol::table offset, float duration) {
    if (!m_pGameInterface || !CameraCircleAroundThing_API) {
        Log("!!! ERROR: CameraCircleAroundThing - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: CameraCircleAroundThing - pThing is null!");
        return;
    }
    C3DVector vecOffset = { offset["x"].get_or(0.0f), offset["y"].get_or(0.0f), offset["z"].get_or(0.0f) };
    CameraCircleAroundThing_API(m_pGameInterface, pThing, &vecOffset, duration);
}

void LuaQuestState::CameraCircleAroundPos(sol::table pos, sol::table offset, float duration) {
    if (m_pGameInterface && CameraCircleAroundPos_API) {
        C3DVector vecPos = { pos["x"].get_or(0.0f), pos["y"].get_or(0.0f), pos["z"].get_or(0.0f) };
        C3DVector vecOffset = { offset["x"].get_or(0.0f), offset["y"].get_or(0.0f), offset["z"].get_or(0.0f) };
        CameraCircleAroundPos_API(m_pGameInterface, &vecPos, &vecOffset, duration);
    }
    else {
        Log("!!! ERROR: CameraCircleAroundPos - API not available!");
    }
}

void LuaQuestState::CameraMoveToPosAndLookAtPos(sol::table camPos, sol::table targetPos, float duration) {
    if (m_pGameInterface && CameraMoveToPosAndLookAtPos_API) {
        C3DVector vecCamPos = { camPos["x"].get_or(0.0f), camPos["y"].get_or(0.0f), camPos["z"].get_or(0.0f) };
        C3DVector vecTargetPos = { targetPos["x"].get_or(0.0f), targetPos["y"].get_or(0.0f), targetPos["z"].get_or(0.0f) };
        CameraMoveToPosAndLookAtPos_API(m_pGameInterface, &vecCamPos, &vecTargetPos, duration);
    }
    else {
        Log("!!! ERROR: CameraMoveToPosAndLookAtPos - API not available!");
    }
}

void LuaQuestState::CameraMoveToPosAndLookAtThing(sol::table camPos, const std::shared_ptr<CScriptThing>& spTargetThing, float duration) {
    if (!m_pGameInterface || !CameraMoveToPosAndLookAtThing_API) {
        Log("!!! ERROR: CameraMoveToPosAndLookAtThing - API not available!");
        return;
    }
    CScriptThing* pTargetThing = spTargetThing.get();
    if (!pTargetThing) {
        Log("!!! ERROR: CameraMoveToPosAndLookAtThing - pTargetThing is null!");
        return;
    }
    C3DVector vecCamPos = { camPos["x"].get_or(0.0f), camPos["y"].get_or(0.0f), camPos["z"].get_or(0.0f) };
    CameraMoveToPosAndLookAtThing_API(m_pGameInterface, &vecCamPos, pTargetThing, duration);
}

void LuaQuestState::CameraMoveBetweenLookingAt(const std::shared_ptr<CScriptThing>& spTargetThing, sol::table startPos, sol::table endPos, float f1, float f2) {
    if (!m_pGameInterface || !CameraMoveBetweenLookingAt_ByThing_API) {
        Log("!!! ERROR: CameraMoveBetweenLookingAt (ByThing) - API not available!");
        return;
    }
    CScriptThing* pTargetThing = spTargetThing.get();
    if (!pTargetThing) {
        Log("!!! ERROR: CameraMoveBetweenLookingAt (ByThing) - pTargetThing is null!");
        return;
    }
    C3DVector vecStartPos = { startPos["x"].get_or(0.0f), startPos["y"].get_or(0.0f), startPos["z"].get_or(0.0f) };
    C3DVector vecEndPos = { endPos["x"].get_or(0.0f), endPos["y"].get_or(0.0f), endPos["z"].get_or(0.0f) };
    CameraMoveBetweenLookingAt_ByThing_API(m_pGameInterface, &vecStartPos, &vecEndPos, pTargetThing, f1, f2);
}

void LuaQuestState::CameraMoveBetweenLookingAt(sol::table targetPos, sol::table startPos, sol::table endPos, float f1, float f2) {
    if (m_pGameInterface && CameraMoveBetweenLookingAt_ByPos_API) {
        C3DVector vecTargetPos = { targetPos["x"].get_or(0.0f), targetPos["y"].get_or(0.0f), targetPos["z"].get_or(0.0f) };
        C3DVector vecStartPos = { startPos["x"].get_or(0.0f), startPos["y"].get_or(0.0f), startPos["z"].get_or(0.0f) };
        C3DVector vecEndPos = { endPos["x"].get_or(0.0f), endPos["y"].get_or(0.0f), endPos["z"].get_or(0.0f) };
        CameraMoveBetweenLookingAt_ByPos_API(m_pGameInterface, &vecStartPos, &vecEndPos, &vecTargetPos, f1, f2);
    }
    else {
        Log("!!! ERROR: CameraMoveBetweenLookingAt (ByPos) - API not available!");
    }
}

void LuaQuestState::CameraMoveBetweenLookFromAndLookTo(sol::table startFrom, sol::table startTo, sol::table endFrom, sol::table endTo, float duration) {
    if (m_pGameInterface && CameraMoveBetweenLookFromAndLookTo_API) {
        C3DVector vecStartFrom = { startFrom["x"].get_or(0.0f), startFrom["y"].get_or(0.0f), startFrom["z"].get_or(0.0f) };
        C3DVector vecStartTo = { startTo["x"].get_or(0.0f), startTo["y"].get_or(0.0f), startTo["z"].get_or(0.0f) };
        C3DVector vecEndFrom = { endFrom["x"].get_or(0.0f), endFrom["y"].get_or(0.0f), endFrom["z"].get_or(0.0f) };
        C3DVector vecEndTo = { endTo["x"].get_or(0.0f), endTo["y"].get_or(0.0f), endTo["z"].get_or(0.0f) };
        CameraMoveBetweenLookFromAndLookTo_API(m_pGameInterface, &vecStartFrom, &vecStartTo, &vecEndFrom, &vecEndTo, duration);
    }
    else {
        Log("!!! ERROR: CameraMoveBetweenLookFromAndLookTo - API not available!");
    }
}

void LuaQuestState::CameraUseCameraPoint(const std::shared_ptr<CScriptThing>& spCamPoint, const std::shared_ptr<CScriptThing>& spTarget, float duration, int i1, int i2) {
    if (!m_pGameInterface || !CameraUseCameraPoint_ThingAndTarget_API) {
        Log("!!! ERROR: CameraUseCameraPoint (ThingAndTarget) - API not available!");
        return;
    }
    CScriptThing* pCamPoint = spCamPoint.get();
    if (!pCamPoint) {
        Log("!!! ERROR: CameraUseCameraPoint (ThingAndTarget) - pCamPoint is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: CameraUseCameraPoint (ThingAndTarget) - pTarget is null!");
        return;
    }
    CameraUseCameraPoint_ThingAndTarget_API(m_pGameInterface, pCamPoint, pTarget, duration, i1, i2);
}

void LuaQuestState::CameraUseCameraPoint(const std::string& camPointName, const std::shared_ptr<CScriptThing>& spTarget, float duration, int i1, int i2) {
    if (!m_pGameInterface || !CameraUseCameraPoint_NameAndTarget_API) {
        Log("!!! ERROR: CameraUseCameraPoint (NameAndTarget) - API not available!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: CameraUseCameraPoint (NameAndTarget) - pTarget is null!");
        return;
    }
    FableString fsCamPoint(camPointName.c_str());
    CameraUseCameraPoint_NameAndTarget_API(m_pGameInterface, fsCamPoint.get(), pTarget, duration, i1, i2);
}

void LuaQuestState::CameraResetToViewBehindHero(float duration) {
    if (m_pGameInterface && CameraResetToViewBehindHero_API) {
        CameraResetToViewBehindHero_API(m_pGameInterface, duration);
    }
    else {
        Log("!!! ERROR: CameraResetToViewBehindHero - API not available!");
    }
}

bool LuaQuestState::IsCameraInScriptedMode() {
    if (m_pGameInterface && IsCameraInScriptedMode_API) {
        return IsCameraInScriptedMode_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsCameraInScriptedMode - API not available!");
    return false;
}

void LuaQuestState::CameraUseScreenEffect(float f1, float f2, float f3) {
    if (m_pGameInterface && CameraUseScreenEffect_API) {
        CameraUseScreenEffect_API(m_pGameInterface, f1, f2, f3);
    }
    else {
        Log("!!! ERROR: CameraUseScreenEffect - API not available!");
    }
}

void LuaQuestState::CameraCancelScreenEffect() {
    if (m_pGameInterface && CameraCancelScreenEffect_API) {
        CameraCancelScreenEffect_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: CameraCancelScreenEffect - API not available!");
    }
}

bool LuaQuestState::IsCameraPosOnScreen(sol::table pos) {
    if (m_pGameInterface && IsCameraPosOnScreen_API) {
        C3DVector vecPos = { pos["x"].get_or(0.0f), pos["y"].get_or(0.0f), pos["z"].get_or(0.0f) };
        return IsCameraPosOnScreen_API(m_pGameInterface, &vecPos);
    }
    Log("!!! ERROR: IsCameraPosOnScreen - API not available!");
    return false;
}

float LuaQuestState::GetGameAngleXY() {
    if (m_pGameInterface && GetGameAngleXY_API) {
        return GetGameAngleXY_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetGameAngleXY - API not available!");
    return 0.0f;
}

void LuaQuestState::CameraEarthquakeIntensityAtPos(sol::table pos, float intensity, float duration) {
    if (m_pGameInterface && CameraEarthquakeIntensityAtPos_API) {
        C3DVector vecPos = { pos["x"].get_or(0.0f), pos["y"].get_or(0.0f), pos["z"].get_or(0.0f) };
        CameraEarthquakeIntensityAtPos_API(m_pGameInterface, &vecPos, intensity, duration);
    }
    else {
        Log("!!! ERROR: CameraEarthquakeIntensityAtPos - API not available!");
    }
}

static CRightHandedSet ParseRotationTable(sol::table rotationTable) {
    CRightHandedSet result = {};
    sol::table up = rotationTable["Up"];
    sol::table forward = rotationTable["Forward"];

    if (up) {
        result.Up = { up["x"].get_or(0.0f), up["y"].get_or(0.0f), up["z"].get_or(0.0f) };
    }
    if (forward) {
        result.Forward = { forward["x"].get_or(0.0f), forward["y"].get_or(0.0f), forward["z"].get_or(0.0f) };
    }
    return result;
}

void LuaQuestState::CameraUseCameraPoint(const std::shared_ptr<CScriptThing>& spCamPoint, sol::table offset, sol::table rotation, float duration, int i1, int i2) {
    if (!m_pGameInterface || !CameraUseCameraPoint_ThingAndOffset_API) {
        Log("!!! ERROR: CameraUseCameraPoint (ThingAndOffset) - API not available!");
        return;
    }
    CScriptThing* pCamPoint = spCamPoint.get();
    if (!pCamPoint) {
        Log("!!! ERROR: CameraUseCameraPoint (ThingAndOffset) - pCamPoint is null!");
        return;
    }

    C3DVector vecOffset = { offset["x"].get_or(0.0f), offset["y"].get_or(0.0f), offset["z"].get_or(0.0f) };
    CRightHandedSet rotSet = ParseRotationTable(rotation);

    CameraUseCameraPoint_ThingAndOffset_API(m_pGameInterface, pCamPoint, &vecOffset, &rotSet, duration, i1, i2);
}

void LuaQuestState::CameraUseCameraPoint(const std::string& camPointName, sol::table offset, sol::table rotation, float duration, int i1, int i2) {
    if (!m_pGameInterface || !CameraUseCameraPoint_NameAndOffset_API) {
        Log("!!! ERROR: CameraUseCameraPoint (NameAndOffset) - API not available!");
        return;
    }

    FableString fsCamPoint(camPointName.c_str());
    C3DVector vecOffset = { offset["x"].get_or(0.0f), offset["y"].get_or(0.0f), offset["z"].get_or(0.0f) };
    CRightHandedSet rotSet = ParseRotationTable(rotation);

    CameraUseCameraPoint_NameAndOffset_API(m_pGameInterface, fsCamPoint.get(), &vecOffset, &rotSet, duration, i1, i2);
}

void LuaQuestState::CameraDoConversation(const std::shared_ptr<CScriptThing>& spSpeaker, const std::shared_ptr<CScriptThing>& spListener, int eOp, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !CameraDoConversation_API) {
        Log("!!! ERROR: CameraDoConversation - API not available!");
        return;
    }
    CScriptThing* pSpeaker = spSpeaker.get();
    if (!pSpeaker) {
        Log("!!! ERROR: CameraDoConversation - pSpeaker is null!");
        return;
    }
    CScriptThing* pListener = spListener.get();
    if (!pListener) {
        Log("!!! ERROR: CameraDoConversation - pListener is null!");
        return;
    }

    ECameraOp op = static_cast<ECameraOp>(eOp);
    bool bFinalUnknown = bUnknown.value_or(true); // Default to true

    CameraDoConversation_API(m_pGameInterface, pSpeaker, pListener, op, bFinalUnknown);
}

bool LuaQuestState::IsChestOpen(const std::shared_ptr<CScriptThing>& spChest) {
    if (!m_pGameInterface || !IsChestOpen_API) {
        Log("!!! ERROR: IsChestOpen - API not available!");
        return false;
    }
    CScriptThing* pChest = spChest.get();
    if (!pChest) {
        Log("!!! ERROR: IsChestOpen - pChest is null!");
        return false;
    }
    return IsChestOpen_API(m_pGameInterface, pChest);
}

bool LuaQuestState::OpenChest(const std::shared_ptr<CScriptThing>& spChest, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !OpenChest_API) {
        Log("!!! ERROR: OpenChest - API not available!");
        return false;
    }
    CScriptThing* pChest = spChest.get();
    if (!pChest) {
        Log("!!! ERROR: OpenChest - pChest is null!");
        return false;
    }
    // Default the unknown bool to 'true' as a safe guess
    bool bFinalUnknown = bUnknown.value_or(true);
    return OpenChest_API(m_pGameInterface, pChest, bFinalUnknown);
}

void LuaQuestState::CloseChest(const std::shared_ptr<CScriptThing>& spChest) {
    if (!m_pGameInterface || !CloseChest_API) {
        Log("!!! ERROR: CloseChest - API not available!");
        return;
    }
    CScriptThing* pChest = spChest.get();
    if (!pChest) {
        Log("!!! ERROR: CloseChest - pChest is null!");
        return;
    }
    CloseChest_API(m_pGameInterface, pChest);
}

sol::object LuaQuestState::GetNumberOfKeysNeededToUnlockChest(const std::shared_ptr<CScriptThing>& spChest, sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !GetNumberOfKeysNeededToUnlockChest_API) {
        Log("!!! ERROR: GetNumberOfKeysNeededToUnlockChest - API not available!");
        return sol::make_object(lua, sol::nil);
    }
    CScriptThing* pChest = spChest.get();
    if (!pChest) {
        Log("!!! ERROR: GetNumberOfKeysNeededToUnlockChest - pChest is null!");
        return sol::make_object(lua, sol::nil);
    }

    CCharString keyNameResult = { 0 };
    int keyCount = GetNumberOfKeysNeededToUnlockChest_API(m_pGameInterface, pChest, &keyNameResult);

    sol::table result = lua.create_table();
    result["count"] = keyCount;

    if (keyNameResult.pStringData) {
        const char* text = CCharString_ToConstChar_API(&keyNameResult);
        if (text) {
            result["keyName"] = std::string(text);
        }
        CCharString_Destroy(&keyNameResult);
    }

    return result;
}

void LuaQuestState::DisplayLockedChestMessage(const std::shared_ptr<CScriptThing>& spChest) {
    if (!m_pGameInterface || !DisplayLockedChestMessage_API) {
        Log("!!! ERROR: DisplayLockedChestMessage - API not available!");
        return;
    }
    CScriptThing* pChest = spChest.get();
    if (!pChest) {
        Log("!!! ERROR: DisplayLockedChestMessage - pChest is null!");
        return;
    }
    DisplayLockedChestMessage_API(m_pGameInterface, pChest);
}

void LuaQuestState::SetTrophyAsMountable(const std::shared_ptr<CScriptThing>& spTrophy, bool bIsMountable) {
    if (!m_pGameInterface || !SetTrophyAsMountable_API) {
        Log("!!! ERROR: SetTrophyAsMountable - API not available!");
        return;
    }
    CScriptThing* pTrophy = spTrophy.get();
    if (!pTrophy) {
        Log("!!! ERROR: SetTrophyAsMountable - pTrophy is null!");
        return;
    }
    SetTrophyAsMountable_API(m_pGameInterface, pTrophy, bIsMountable);
}

void LuaQuestState::SetVillageLimbo(const std::shared_ptr<CScriptThing>& spVillage, bool bIsInLimbo) {
    if (!m_pGameInterface || !SetVillageLimbo_API) {
        Log("!!! ERROR: SetVillageLimbo - API not available!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: SetVillageLimbo - pVillage is null!");
        return;
    }
    SetVillageLimbo_API(m_pGameInterface, pVillage, bIsInLimbo);
}

void LuaQuestState::SetCreatureNotReload(const std::shared_ptr<CScriptThing>& spCreature) {
    if (!m_pGameInterface || !SetCreatureNotReload_API) {
        Log("!!! ERROR: SetCreatureNotReload - API not available!");
        return;
    }
    CScriptThing* pCreature = spCreature.get();
    if (!pCreature) {
        Log("!!! ERROR: SetCreatureNotReload - pCreature is null!");
        return;
    }
    SetCreatureNotReload_API(m_pGameInterface, pCreature);
}

bool LuaQuestState::IsSleepingTime(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsSleepingTime_API) {
        Log("!!! ERROR: IsSleepingTime - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsSleepingTime - pThing is null!");
        return false;
    }
    return IsSleepingTime_API(m_pGameInterface, pThing);
}

void LuaQuestState::EnableGuards(const std::shared_ptr<CScriptThing>& spVillage, bool bIsEnabled) {
    if (!m_pGameInterface || !EnableGuards_API) {
        Log("!!! ERROR: EnableGuards - API not available!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: EnableGuards - pVillage is null!");
        return;
    }
    EnableGuards_API(m_pGameInterface, pVillage, bIsEnabled);
}

void LuaQuestState::EnableVillagerDefTypes(const std::shared_ptr<CScriptThing>& spVillage, bool bIsEnabled, const std::string& defName) {
    if (!m_pGameInterface || !EnableVillagerDefTypes_API) {
        Log("!!! ERROR: EnableVillagerDefTypes - API not available!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: EnableVillagerDefTypes - pVillage is null!");
        return;
    }
    FableString fsDefName(defName.c_str());
    EnableVillagerDefTypes_API(m_pGameInterface, pVillage, bIsEnabled, fsDefName.get());
}

std::shared_ptr<CScriptThing> LuaQuestState::TryToRespawnDefNamed(const std::shared_ptr<CScriptThing>& spThing, const std::string& defName, sol::table pos) {
    if (!m_pGameInterface || !TryToRespawnDefNamed_API || !Game_malloc) {
        Log("!!! ERROR: TryToRespawnDefNamed - API or memory allocator not available!");
        return nullptr;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: TryToRespawnDefNamed - pThing is null!");
        return nullptr;
    }

    FableString fsDefName(defName.c_str());
    const C3DVector vecPos = { pos["x"].get_or(0.0f), pos["y"].get_or(0.0f), pos["z"].get_or(0.0f) };

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: TryToRespawnDefNamed - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    TryToRespawnDefNamed_API(m_pGameInterface, pResult, pThing, fsDefName.get(), &vecPos);

    return WrapScriptThingOutput(pResult);
}

void LuaQuestState::ClearHeroEnemyOfGuards(const std::shared_ptr<CScriptThing>& spVillage) {
    if (!m_pGameInterface || !ClearHeroEnemyOfGuards_API) {
        Log("!!! ERROR: ClearHeroEnemyOfGuards - API not available!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: ClearHeroEnemyOfGuards - pVillage is null!");
        return;
    }
    ClearHeroEnemyOfGuards_API(m_pGameInterface, pVillage);
}

void LuaQuestState::SetThingAsUsable(const std::shared_ptr<CScriptThing>& spThing, bool bIsUsable) {
    if (!m_pGameInterface || !SetThingAsUsable_API) {
        Log("!!! ERROR: SetThingAsUsable - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetThingAsUsable - pThing is null!");
        return;
    }
    SetThingAsUsable_API(m_pGameInterface, pThing, bIsUsable);
}

void LuaQuestState::SetThingHomeBuilding(const std::shared_ptr<CScriptThing>& spThing, const std::shared_ptr<CScriptThing>& spHouse) {
    if (!m_pGameInterface || !SetThingHomeBuilding_API) {
        Log("!!! ERROR: SetThingHomeBuilding - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetThingHomeBuilding - pThing is null!");
        return;
    }
    CScriptThing* pHouse = spHouse.get();
    if (!pHouse) {
        Log("!!! ERROR: SetThingHomeBuilding - pHouse is null!");
        return;
    }
    SetThingHomeBuilding_API(m_pGameInterface, pThing, pHouse);
}

void LuaQuestState::GiveThingBestEnemyTarget(const std::shared_ptr<CScriptThing>& spThing, const std::shared_ptr<CScriptThing>& spTarget) {
    if (!m_pGameInterface || !GiveThingBestEnemyTarget_API) {
        Log("!!! ERROR: GiveThingBestEnemyTarget - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GiveThingBestEnemyTarget - pThing is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: GiveThingBestEnemyTarget - pTarget is null!");
        return;
    }
    GiveThingBestEnemyTarget_API(m_pGameInterface, pThing, pTarget);
}

void LuaQuestState::ClearThingBestEnemyTarget(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ClearThingBestEnemyTarget_API) {
        Log("!!! ERROR: ClearThingBestEnemyTarget - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ClearThingBestEnemyTarget - pThing is null!");
        return;
    }
    ClearThingBestEnemyTarget_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetInLimbo(const std::shared_ptr<CScriptThing>& spThing, bool bIsInLimbo, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntitySetInLimbo_API) {
        Log("!!! ERROR: EntitySetInLimbo - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetInLimbo - pThing is null!");
        return;
    }
    // Default the unknown bool to 'true' as a safe guess
    bool bFinalUnknown = bUnknown.value_or(true);
    EntitySetInLimbo_API(m_pGameInterface, pThing, bIsInLimbo, bFinalUnknown);
}

bool LuaQuestState::IsEntityInLimbo(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityInLimbo_API) {
        Log("!!! ERROR: IsEntityInLimbo - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityInLimbo - pThing is null!");
        return false;
    }
    return IsEntityInLimbo_API(m_pGameInterface, pThing);
}

void LuaQuestState::AddCrimeCommitted(const std::shared_ptr<CScriptThing>& spVillage, int crimeType, sol::optional<bool> bUnknown, const std::shared_ptr<CScriptThing>& spCriminal, const std::shared_ptr<CScriptThing>& spVictim, int deedType) {
    if (!m_pGameInterface || !AddCrimeCommitted_API) {
        Log("!!! ERROR: AddCrimeCommitted - API not available!");
        return;
    }

    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: AddCrimeCommitted - pVillage is null!");
        return;
    }

    // .get() safely returns nullptr if the shared_ptrs are nil
    CScriptThing* pCriminal = spCriminal.get();
    CScriptThing* pVictim = spVictim.get();

    ECrime eCrime = static_cast<ECrime>(crimeType);
    EOpinionPostDeedType eDeedType = static_cast<EOpinionPostDeedType>(deedType);
    // Default the unknown bool to 'true' as a safe guess
    bool bFinalUnknown = bUnknown.value_or(true);

    AddCrimeCommitted_API(m_pGameInterface, pVillage, eCrime, bFinalUnknown, pCriminal, pVictim, eDeedType);
}

void LuaQuestState::SetVillageAttitude(const std::shared_ptr<CScriptThing>& spVillage, int eAttitude) {
    if (!m_pGameInterface || !SetVillageAttitude_API) {
        Log("!!! ERROR: SetVillageAttitude - API not available!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: SetVillageAttitude - pVillage is null!");
        return;
    }

    EScriptVillageAttitude attitude = static_cast<EScriptVillageAttitude>(eAttitude);
    SetVillageAttitude_API(m_pGameInterface, pVillage, attitude);
}

void LuaQuestState::EntitySetNegateAllHits(const std::shared_ptr<CScriptThing>& spThing, bool bNegate) {
    if (!m_pGameInterface || !EntitySetNegateAllHits_API) {
        Log("!!! ERROR: EntitySetNegateAllHits - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetNegateAllHits - pThing is null!");
        return;
    }
    EntitySetNegateAllHits_API(m_pGameInterface, pThing, bNegate);
}

void LuaQuestState::EntitySetEvadeAllHits(const std::shared_ptr<CScriptThing>& spThing, bool bEvade) {
    if (!m_pGameInterface || !EntitySetEvadeAllHits_API) {
        Log("!!! ERROR: EntitySetEvadeAllHits - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetEvadeAllHits - pThing is null!");
        return;
    }
    EntitySetEvadeAllHits_API(m_pGameInterface, pThing, bEvade);
}

void LuaQuestState::EntitySetAbleToBeEngagedInCombat(const std::shared_ptr<CScriptThing>& spThing, bool bIsAble) {
    if (!m_pGameInterface || !EntitySetAbleToBeEngagedInCombat_API) {
        Log("!!! ERROR: EntitySetAbleToBeEngagedInCombat - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAbleToBeEngagedInCombat - pThing is null!");
        return;
    }
    EntitySetAbleToBeEngagedInCombat_API(m_pGameInterface, pThing, bIsAble);
}

void LuaQuestState::EntitySetAlwaysBlockAttacksFromThing(const std::shared_ptr<CScriptThing>& spBlocker, const std::shared_ptr<CScriptThing>& spAttacker, bool bBlock) {
    if (!m_pGameInterface || !EntitySetAlwaysBlockAttacksFromThing_API) {
        Log("!!! ERROR: EntitySetAlwaysBlockAttacksFromThing - API not available!");
        return;
    }
    CScriptThing* pBlocker = spBlocker.get();
    if (!pBlocker) {
        Log("!!! ERROR: EntitySetAlwaysBlockAttacksFromThing - pBlocker is null!");
        return;
    }
    CScriptThing* pAttacker = spAttacker.get();
    if (!pAttacker) {
        Log("!!! ERROR: EntitySetAlwaysBlockAttacksFromThing - pAttacker is null!");
        return;
    }
    EntitySetAlwaysBlockAttacksFromThing_API(m_pGameInterface, pBlocker, pAttacker, bBlock);
}

void LuaQuestState::EntitySetAttackThingImmediately(const std::shared_ptr<CScriptThing>& spAttacker, const std::shared_ptr<CScriptThing>& spTarget, sol::optional<bool> b1, sol::optional<bool> b2) {
    if (!m_pGameInterface || !EntitySetAttackThingImmediately_API) {
        Log("!!! ERROR: EntitySetAttackThingImmediately - API not available!");
        return;
    }
    CScriptThing* pAttacker = spAttacker.get();
    if (!pAttacker) {
        Log("!!! ERROR: EntitySetAttackThingImmediately - pAttacker is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: EntitySetAttackThingImmediately - pTarget is null!");
        return;
    }
    // Default both unknown bools to 'true' as a safe guess
    bool bFinal1 = b1.value_or(true);
    bool bFinal2 = b2.value_or(true);
    EntitySetAttackThingImmediately_API(m_pGameInterface, pAttacker, pTarget, bFinal1, bFinal2);
}

void LuaQuestState::EntitySetCombatType(const std::shared_ptr<CScriptThing>& spThing, const std::string& combatType) {
    if (!m_pGameInterface || !EntitySetCombatType_API) {
        Log("!!! ERROR: EntitySetCombatType - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetCombatType - pThing is null!");
        return;
    }
    FableString fsType(combatType.c_str());
    EntitySetCombatType_API(m_pGameInterface, pThing, fsType.get());
}

void LuaQuestState::EntityResetCombatTypeToDefault(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityResetCombatTypeToDefault_API) {
        Log("!!! ERROR: EntityResetCombatTypeToDefault - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityResetCombatTypeToDefault - pThing is null!");
        return;
    }
    EntityResetCombatTypeToDefault_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntityAttachToScript(const std::shared_ptr<CScriptThing>& spThing, const std::string& questName) {
    if (!m_pGameInterface || !EntityAttachToScript_API) {
        Log("!!! ERROR: EntityAttachToScript - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityAttachToScript - pThing is null!");
        return;
    }
    FableString fsQuest(questName.c_str());
    EntityAttachToScript_API(m_pGameInterface, pThing, fsQuest.get());
}

void LuaQuestState::EntitySetRangedTarget(const std::shared_ptr<CScriptThing>& spThing, const std::shared_ptr<CScriptThing>& spTarget) {
    if (!m_pGameInterface || !EntitySetRangedTarget_API) {
        Log("!!! ERROR: EntitySetRangedTarget - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetRangedTarget - pThing is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: EntitySetRangedTarget - pTarget is null!");
        return;
    }
    EntitySetRangedTarget_API(m_pGameInterface, pThing, pTarget);
}

void LuaQuestState::EntityClearRangedTarget(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityClearRangedTarget_API) {
        Log("!!! ERROR: EntityClearRangedTarget - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityClearRangedTarget - pThing is null!");
        return;
    }
    EntityClearRangedTarget_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetTargetingValidTargetWithoutLOS(const std::shared_ptr<CScriptThing>& spThing, bool bIsValid) {
    if (!m_pGameInterface || !EntitySetTargetingValidTargetWithoutLOS_API) {
        Log("!!! ERROR: EntitySetTargetingValidTargetWithoutLOS - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetTargetingValidTargetWithoutLOS - pThing is null!");
        return;
    }
    EntitySetTargetingValidTargetWithoutLOS_API(m_pGameInterface, pThing, bIsValid);
}

void LuaQuestState::EntitySetTargetingType(const std::shared_ptr<CScriptThing>& spThing, int eType) {
    if (!m_pGameInterface || !EntitySetTargetingType_API) {
        Log("!!! ERROR: EntitySetTargetingType - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetTargetingType - pThing is null!");
        return;
    }

    ETargetingType type = static_cast<ETargetingType>(eType);
    EntitySetTargetingType_API(m_pGameInterface, pThing, type);
}

sol::object LuaQuestState::EntityGetShotStrikePos(const std::shared_ptr<CScriptThing>& spThing, sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !EntityGetShotStrikePos_API) {
        Log("!!! ERROR: EntityGetShotStrikePos - API not available!");
        return sol::make_object(lua, sol::nil);
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetShotStrikePos - pThing is null!");
        return sol::make_object(lua, sol::nil);
    }

    C3DVector outPos = {}; // Initialize the output struct
    bool result = EntityGetShotStrikePos_API(m_pGameInterface, pThing, &outPos);

    if (result) {
        sol::table posTable = lua.create_table();
        posTable["x"] = outPos.x;
        posTable["y"] = outPos.y;
        posTable["z"] = outPos.z;
        return posTable;
    }

    return sol::make_object(lua, sol::nil);
}

void LuaQuestState::EntitySetMaxNumberOfAttackers(const std::shared_ptr<CScriptThing>& spThing, int maxAttackers) {
    if (!m_pGameInterface || !EntitySetMaxNumberOfAttackers_API) {
        Log("!!! ERROR: EntitySetMaxNumberOfAttackers - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetMaxNumberOfAttackers - pThing is null!");
        return;
    }
    EntitySetMaxNumberOfAttackers_API(m_pGameInterface, pThing, maxAttackers);
}

void LuaQuestState::EntityClearMaxNumberOfAttackers(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityClearMaxNumberOfAttackers_API) {
        Log("!!! ERROR: EntityClearMaxNumberOfAttackers - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityClearMaxNumberOfAttackers - pThing is null!");
        return;
    }
    EntityClearMaxNumberOfAttackers_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetCombatAbility(const std::shared_ptr<CScriptThing>& spThing, float ability) {
    if (!m_pGameInterface || !EntitySetCombatAbility_API) {
        Log("!!! ERROR: EntitySetCombatAbility - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetCombatAbility - pThing is null!");
        return;
    }
    EntitySetCombatAbility_API(m_pGameInterface, pThing, ability);
}

void LuaQuestState::EntityTeleportToPosition(const std::shared_ptr<CScriptThing>& spThing, sol::table pos, float f, sol::optional<bool> b1, sol::optional<bool> b2) {
    if (!m_pGameInterface || !EntityTeleportToPosition_API) {
        Log("!!! ERROR: EntityTeleportToPosition - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityTeleportToPosition - pThing is null!");
        return;
    }
    const C3DVector vecPos = { pos["x"].get_or(0.0f), pos["y"].get_or(0.0f), pos["z"].get_or(0.0f) };
    bool bFinal1 = b1.value_or(true);
    bool bFinal2 = b2.value_or(true);
    EntityTeleportToPosition_API(m_pGameInterface, pThing, &vecPos, f, bFinal1, bFinal2);
}

void LuaQuestState::EntitySetFacingAngle(CScriptThing* pThing, float angle, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntitySetFacingAngle_API) {
        Log("!!! ERROR: EntitySetFacingAngle - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: EntitySetFacingAngle - pThing is null!");
        return;
    }
    bool bFinalUnknown = bUnknown.value_or(true);
    EntitySetFacingAngle_API(m_pGameInterface, pThing, angle, bFinalUnknown);
}

void LuaQuestState::EntitySetPerceptionVariables(const std::shared_ptr<CScriptThing>& spThing, float f1, float f2, float f3) {
    if (!m_pGameInterface || !EntitySetPerceptionVariables_API) {
        Log("!!! ERROR: EntitySetPerceptionVariables - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetPerceptionVariables - pThing is null!");
        return;
    }
    EntitySetPerceptionVariables_API(m_pGameInterface, pThing, f1, f2, f3);
}

void LuaQuestState::EntitySetThingAsWantingMoney(const std::shared_ptr<CScriptThing>& spThing, int amount) {
    if (!m_pGameInterface || !EntitySetThingAsWantingMoney_API) {
        Log("!!! ERROR: EntitySetThingAsWantingMoney - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetThingAsWantingMoney - pThing is null!");
        return;
    }
    EntitySetThingAsWantingMoney_API(m_pGameInterface, pThing, amount);
}

void LuaQuestState::EntitySetAppearanceMorphSeed(const std::shared_ptr<CScriptThing>& spThing, int seed) {
    if (!m_pGameInterface || !EntitySetAppearanceMorphSeed_API) {
        Log("!!! ERROR: EntitySetAppearanceMorphSeed - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAppearanceMorphSeed - pThing is null!");
        return;
    }
    EntitySetAppearanceMorphSeed_API(m_pGameInterface, pThing, seed);
}

void LuaQuestState::SetEntityAsRegionFollowing(const std::shared_ptr<CScriptThing>& spFollower, const std::shared_ptr<CScriptThing>& spTarget, bool bFollow) {
    if (!m_pGameInterface || !SetEntityAsRegionFollowing_API) {
        Log("!!! ERROR: SetEntityAsRegionFollowing - API not available!");
        return;
    }
    CScriptThing* pFollower = spFollower.get();
    if (!pFollower) {
        Log("!!! ERROR: SetEntityAsRegionFollowing - pFollower is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: SetEntityAsRegionFollowing - pTarget is null!");
        return;
    }
    SetEntityAsRegionFollowing_API(m_pGameInterface, pFollower, pTarget, bFollow);
}

void LuaQuestState::SetEntityAsFollowingHeroThroughTeleporters(const std::shared_ptr<CScriptThing>& spThing, bool bFollow) {
    if (!m_pGameInterface || !SetEntityAsFollowingHeroThroughTeleporters_API) {
        Log("!!! ERROR: SetEntityAsFollowingHeroThroughTeleporters - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetEntityAsFollowingHeroThroughTeleporters - pThing is null!");
        return;
    }
    SetEntityAsFollowingHeroThroughTeleporters_API(m_pGameInterface, pThing, bFollow);
}

unsigned int LuaQuestState::EntityGetAppearanceSeed(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityGetAppearanceSeed_API) {
        Log("!!! ERROR: EntityGetAppearanceSeed - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetAppearanceSeed - pThing is null!");
        return 0;
    }

    unsigned int outSeed = 0;
    EntityGetAppearanceSeed_API(m_pGameInterface, pThing, &outSeed);
    return outSeed;
}

void LuaQuestState::EntityPlayObjectAnimation(const std::shared_ptr<CScriptThing>& spThing, const std::string& animName, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntityPlayObjectAnimation_API) {
        Log("!!! ERROR: EntityPlayObjectAnimation - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityPlayObjectAnimation - pThing is null!");
        return;
    }
    FableString fsAnim(animName.c_str());
    bool bFinalUnknown = bUnknown.value_or(true);
    EntityPlayObjectAnimation_API(m_pGameInterface, pThing, fsAnim.get(), bFinalUnknown);
}

void LuaQuestState::EntitySetMaxRunningSpeed(const std::shared_ptr<CScriptThing>& spThing, float speed) {
    if (!m_pGameInterface || !EntitySetMaxRunningSpeed_API) {
        Log("!!! ERROR: EntitySetMaxRunningSpeed - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetMaxRunningSpeed - pThing is null!");
        return;
    }
    EntitySetMaxRunningSpeed_API(m_pGameInterface, pThing, speed);
}

void LuaQuestState::EntitySetMaxWalkingSpeed(const std::shared_ptr<CScriptThing>& spThing, float speed) {
    if (!m_pGameInterface || !EntitySetMaxWalkingSpeed_API) {
        Log("!!! ERROR: EntitySetMaxWalkingSpeed - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetMaxWalkingSpeed - pThing is null!");
        return;
    }
    EntitySetMaxWalkingSpeed_API(m_pGameInterface, pThing, speed);
}

void LuaQuestState::EntityResetMaxRunningSpeed(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityResetMaxRunningSpeed_API) {
        Log("!!! ERROR: EntityResetMaxRunningSpeed - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityResetMaxRunningSpeed - pThing is null!");
        return;
    }
    EntityResetMaxRunningSpeed_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntityResetMaxWalkingSpeed(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityResetMaxWalkingSpeed_API) {
        Log("!!! ERROR: EntityResetMaxWalkingSpeed - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityResetMaxWalkingSpeed - pThing is null!");
        return;
    }
    EntityResetMaxWalkingSpeed_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntityAttachToVillage(const std::shared_ptr<CScriptThing>& spThing, const std::shared_ptr<CScriptThing>& spVillage) {
    if (!m_pGameInterface || !EntityAttachToVillage_API) {
        Log("!!! ERROR: EntityAttachToVillage - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityAttachToVillage - pThing is null!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: EntityAttachToVillage - pVillage is null!");
        return;
    }
    EntityAttachToVillage_API(m_pGameInterface, pThing, pVillage);
}

void LuaQuestState::EntitySetAsSittingOnFloor(const std::shared_ptr<CScriptThing>& spThing, bool bIsSitting) {
    if (!m_pGameInterface || !EntitySetAsSittingOnFloor_API) {
        Log("!!! ERROR: EntitySetAsSittingOnFloor - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsSittingOnFloor - pThing is null!");
        return;
    }
    EntitySetAsSittingOnFloor_API(m_pGameInterface, pThing, bIsSitting);
}

void LuaQuestState::EntitySetAsHavingBoundHands(const std::shared_ptr<CScriptThing>& spThing, bool bIsBound) {
    if (!m_pGameInterface || !EntitySetAsHavingBoundHands_API) {
        Log("!!! ERROR: EntitySetAsHavingBoundHands - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsHavingBoundHands - pThing is null!");
        return;
    }
    EntitySetAsHavingBoundHands_API(m_pGameInterface, pThing, bIsBound);
}

void LuaQuestState::EntitySetAsRemoveAllMovementBlockingModes(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntitySetAsRemoveAllMovementBlockingModes_API) {
        Log("!!! ERROR: EntitySetAsRemoveAllMovementBlockingModes - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsRemoveAllMovementBlockingModes - pThing is null!");
        return;
    }
    EntitySetAsRemoveAllMovementBlockingModes_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetAsScared(CScriptThing* pThing, bool bIsScared) {
    if (!m_pGameInterface || !EntitySetAsScared_API) {
        Log("!!! ERROR: EntitySetAsScared - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsScared - pThing is null!");
        return;
    }
    EntitySetAsScared_API(m_pGameInterface, pThing, bIsScared);
}

void LuaQuestState::EntitySetAsDrunk(const std::shared_ptr<CScriptThing>& spThing, bool bIsDrunk) {
    if (!m_pGameInterface || !EntitySetAsDrunk_API) {
        Log("!!! ERROR: EntitySetAsDrunk - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsDrunk - pThing is null!");
        return;
    }
    EntitySetAsDrunk_API(m_pGameInterface, pThing, bIsDrunk);
}

void LuaQuestState::EntityForceToLookAtThing(const std::shared_ptr<CScriptThing>& spLooker, const std::shared_ptr<CScriptThing>& spTarget) {
    if (!m_pGameInterface || !EntityForceToLookAtThing_API) {
        Log("!!! ERROR: EntityForceToLookAtThing - API not available!");
        return;
    }
    CScriptThing* pLooker = spLooker.get();
    if (!pLooker) {
        Log("!!! ERROR: EntityForceToLookAtThing - pLooker is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: EntityForceToLookAtThing - pTarget is null!");
        return;
    }
    EntityForceToLookAtThing_API(m_pGameInterface, pLooker, pTarget);
}

void LuaQuestState::EntityForceToLookAtCamera(const std::shared_ptr<CScriptThing>& spLooker, bool bLook) {
    if (!m_pGameInterface || !EntityForceToLookAtCamera_API) {
        Log("!!! ERROR: EntityForceToLookAtCamera - API not available!");
        return;
    }
    CScriptThing* pLooker = spLooker.get();
    if (!pLooker) {
        Log("!!! ERROR: EntityForceToLookAtCamera - pLooker is null!");
        return;
    }
    EntityForceToLookAtCamera_API(m_pGameInterface, pLooker, bLook);
}

void LuaQuestState::EntityForceToLookAtNothing(const std::shared_ptr<CScriptThing>& spLooker) {
    if (!m_pGameInterface || !EntityForceToLookAtNothing_API) {
        Log("!!! ERROR: EntityForceToLookAtNothing - API not available!");
        return;
    }
    CScriptThing* pLooker = spLooker.get();
    if (!pLooker) {
        Log("!!! ERROR: EntityForceToLookAtNothing - pLooker is null!");
        return;
    }
    EntityForceToLookAtNothing_API(m_pGameInterface, pLooker);
}

void LuaQuestState::EntityResetForceToLookAt(const std::shared_ptr<CScriptThing>& spLooker) {
    if (!m_pGameInterface || !EntityResetForceToLookAt_API) {
        Log("!!! ERROR: EntityResetForceToLookAt - API not available!");
        return;
    }
    CScriptThing* pLooker = spLooker.get();
    if (!pLooker) {
        Log("!!! ERROR: EntityResetForceToLookAt - pLooker is null!");
        return;
    }
    EntityResetForceToLookAt_API(m_pGameInterface, pLooker);
}

void LuaQuestState::EntitySetShotAccuracyPercentage(const std::shared_ptr<CScriptThing>& spThing, int accuracy) {
    if (!m_pGameInterface || !EntitySetShotAccuracyPercentage_API) {
        Log("!!! ERROR: EntitySetShotAccuracyPercentage - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetShotAccuracyPercentage - pThing is null!");
        return;
    }
    EntitySetShotAccuracyPercentage_API(m_pGameInterface, pThing, accuracy);
}

std::shared_ptr<CScriptThing> LuaQuestState::EntityGetStandingOnThing(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityGetStandingOnThing_API || !Game_malloc) {
        Log("!!! ERROR: EntityGetStandingOnThing - API or memory allocator not available!");
        return nullptr;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetStandingOnThing - pThing is null!");
        return nullptr;
    }

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: EntityGetStandingOnThing - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    EntityGetStandingOnThing_API(m_pGameInterface, pResult, pThing);

    return WrapScriptThingOutput(pResult);
}

std::shared_ptr<CScriptThing> LuaQuestState::EntityGetStandingInsideBuilding(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityGetStandingInsideBuilding_API || !Game_malloc) {
        Log("!!! ERROR: EntityGetStandingInsideBuilding - API or memory allocator not available!");
        return nullptr;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetStandingInsideBuilding - pThing is null!");
        return nullptr;
    }

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: EntityGetStandingInsideBuilding - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    EntityGetStandingInsideBuilding_API(m_pGameInterface, pResult, pThing);

    return WrapScriptThingOutput(pResult);
}

void LuaQuestState::EntityDropGenericBox(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityDropGenericBox_API) {
        Log("!!! ERROR: EntityDropGenericBox - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityDropGenericBox - pThing is null!");
        return;
    }
    EntityDropGenericBox_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySheatheWeapons(CScriptThing* pThing, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntitySheatheWeapons_API) {
        Log("!!! ERROR: EntitySheatheWeapons - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: EntitySheatheWeapons - pThing is null!");
        return;
    }
    bool bFinalUnknown = bUnknown.value_or(true);
    EntitySheatheWeapons_API(m_pGameInterface, pThing, bFinalUnknown);
}

void LuaQuestState::EntityUnsheatheWeapons(const std::shared_ptr<CScriptThing>& spThing, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntityUnsheatheWeapons_API) {
        Log("!!! ERROR: EntityUnsheatheWeapons - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityUnsheatheWeapons - pThing is null!");
        return;
    }
    bool bFinalUnknown = bUnknown.value_or(true);
    EntityUnsheatheWeapons_API(m_pGameInterface, pThing, bFinalUnknown);
}

void LuaQuestState::EntityUnsheatheMeleeWeapon(const std::shared_ptr<CScriptThing>& spThing, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntityUnsheatheMeleeWeapon_API) {
        Log("!!! ERROR: EntityUnsheatheMeleeWeapon - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityUnsheatheMeleeWeapon - pThing is null!");
        return;
    }
    bool bFinalUnknown = bUnknown.value_or(true);
    EntityUnsheatheMeleeWeapon_API(m_pGameInterface, pThing, bFinalUnknown);
}

void LuaQuestState::EntityUnsheatheRangedWeapon(const std::shared_ptr<CScriptThing>& spThing, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntityUnsheatheRangedWeapon_API) {
        Log("!!! ERROR: EntityUnsheatheRangedWeapon - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityUnsheatheRangedWeapon - pThing is null!");
        return;
    }
    bool bFinalUnknown = bUnknown.value_or(true);
    EntityUnsheatheRangedWeapon_API(m_pGameInterface, pThing, bFinalUnknown);
}

void LuaQuestState::EntitySetAlpha(const std::shared_ptr<CScriptThing>& spThing, float alpha, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntitySetAlpha_API) {
        Log("!!! ERROR: EntitySetAlpha - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAlpha - pThing is null!");
        return;
    }
    bool bFinalUnknown = bUnknown.value_or(true);
    EntitySetAlpha_API(m_pGameInterface, pThing, alpha, bFinalUnknown);
}

void LuaQuestState::EntitySetAsAbleToWalkThroughSolidObjects(const std::shared_ptr<CScriptThing>& spThing, bool bCanWalkThrough) {
    if (!m_pGameInterface || !EntitySetAsAbleToWalkThroughSolidObjects_API) {
        Log("!!! ERROR: EntitySetAsAbleToWalkThroughSolidObjects - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsAbleToWalkThroughSolidObjects - pThing is null!");
        return;
    }
    EntitySetAsAbleToWalkThroughSolidObjects_API(m_pGameInterface, pThing, bCanWalkThrough);
}

void LuaQuestState::EntitySetAsRespondToHit(const std::shared_ptr<CScriptThing>& spThing, bool bDoesRespond) {
    if (!m_pGameInterface || !EntitySetAsRespondToHit_API) {
        Log("!!! ERROR: EntitySetAsRespondToHit - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsRespondToHit - pThing is null!");
        return;
    }
    EntitySetAsRespondToHit_API(m_pGameInterface, pThing, bDoesRespond);
}

void LuaQuestState::EntitySetAsLocked(const std::shared_ptr<CScriptThing>& spThing, bool bIsLocked) {
    if (!m_pGameInterface || !EntitySetAsLocked_API) {
        Log("!!! ERROR: EntitySetAsLocked - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsLocked - pThing is null!");
        return;
    }
    EntitySetAsLocked_API(m_pGameInterface, pThing, bIsLocked);
}

void LuaQuestState::EntityDecapitate(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityDecapitate_API) {
        Log("!!! ERROR: EntityDecapitate - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityDecapitate - pThing is null!");
        return;
    }
    EntityDecapitate_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntityGiveGold(const std::shared_ptr<CScriptThing>& spThing, int amount) {
    if (!m_pGameInterface || !EntityGiveGold_API) {
        Log("!!! ERROR: EntityGiveGold - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGiveGold - pThing is null!");
        return;
    }
    EntityGiveGold_API(m_pGameInterface, pThing, amount);
}

int LuaQuestState::EntityGetSex(CScriptThing* pThing) {
    if (!m_pGameInterface || !EntityGetSex_API) {
        Log("!!! ERROR: EntityGetSex - API not available!");
        return 0; // Return SEX_NULL
    }
    if (!pThing) {
        Log("!!! ERROR: EntityGetSex - pThing is null!");
        return 0; // Return SEX_NULL
    }

    // Call the API and cast the ESex enum result to an int for Lua
    return static_cast<int>(EntityGetSex_API(m_pGameInterface, pThing));
}

void LuaQuestState::EntitySetAllowBossPhaseChanges(const std::shared_ptr<CScriptThing>& spThing, bool bAllow) {
    if (!m_pGameInterface || !EntitySetAllowBossPhaseChanges_API) {
        Log("!!! ERROR: EntitySetAllowBossPhaseChanges - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAllowBossPhaseChanges - pThing is null!");
        return;
    }
    EntitySetAllowBossPhaseChanges_API(m_pGameInterface, pThing, bAllow);
}

void LuaQuestState::EntitySetBossPhase(const std::shared_ptr<CScriptThing>& spThing, int phase) {
    if (!m_pGameInterface || !EntitySetBossPhase_API) {
        Log("!!! ERROR: EntitySetBossPhase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetBossPhase - pThing is null!");
        return;
    }
    EntitySetBossPhase_API(m_pGameInterface, pThing, phase);
}

void LuaQuestState::EntityResetCreatureMode(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityResetCreatureMode_API) {
        Log("!!! ERROR: EntityResetCreatureMode - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityResetCreatureMode - pThing is null!");
        return;
    }
    EntityResetCreatureMode_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetAsReceivingEvents(const std::shared_ptr<CScriptThing>& spThing, bool bReceivesEvents) {
    if (!m_pGameInterface || !EntitySetAsReceivingEvents_API) {
        Log("!!! ERROR: EntitySetAsReceivingEvents - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsReceivingEvents - pThing is null!");
        return;
    }
    EntitySetAsReceivingEvents_API(m_pGameInterface, pThing, bReceivesEvents);
}

void LuaQuestState::EntitySetAsToAddToStatChangesWhenHit(const std::shared_ptr<CScriptThing>& spThing, bool bAddsToStats) {
    if (!m_pGameInterface || !EntitySetAsToAddToStatChangesWhenHit_API) {
        Log("!!! ERROR: EntitySetAsToAddToStatChangesWhenHit - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsToAddToStatChangesWhenHit - pThing is null!");
        return;
    }
    EntitySetAsToAddToStatChangesWhenHit_API(m_pGameInterface, pThing, bAddsToStats);
}

void LuaQuestState::EntityLeaveCombatStance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityLeaveCombatStance_API) {
        Log("!!! ERROR: EntityLeaveCombatStance - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityLeaveCombatStance - pThing is null!");
        return;
    }
    EntityLeaveCombatStance_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetAsUseMovementInActions(CScriptThing* pThing, bool bUseMovement) {
    if (!m_pGameInterface || !EntitySetAsUseMovementInActions_API) {
        Log("!!! ERROR: EntitySetAsUseMovementInActions - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsUseMovementInActions - pThing is null!");
        return;
    }
    EntitySetAsUseMovementInActions_API(m_pGameInterface, pThing, bUseMovement);
}

void LuaQuestState::EntitySetAsDisplayingEmoteIcon(const std::shared_ptr<CScriptThing>& spThing, bool bDisplaysIcon) {
    if (!m_pGameInterface || !EntitySetAsDisplayingEmoteIcon_API) {
        Log("!!! ERROR: EntitySetAsDisplayingEmoteIcon - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsDisplayingEmoteIcon - pThing is null!");
        return;
    }
    EntitySetAsDisplayingEmoteIcon_API(m_pGameInterface, pThing, bDisplaysIcon);
}

void LuaQuestState::EntitySetAsCollidableToThings(const std::shared_ptr<CScriptThing>& spThing, bool bIsCollidable) {
    if (!m_pGameInterface || !EntitySetAsCollidableToThings_API) {
        Log("!!! ERROR: EntitySetAsCollidableToThings - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsCollidableToThings - pThing is null!");
        return;
    }
    EntitySetAsCollidableToThings_API(m_pGameInterface, pThing, bIsCollidable);
}

void LuaQuestState::EntityEnableGravity(const std::shared_ptr<CScriptThing>& spThing, bool bIsEnabled) {
    if (!m_pGameInterface || !EntityEnableGravity_API) {
        Log("!!! ERROR: EntityEnableGravity - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityEnableGravity - pThing is null!");
        return;
    }
    EntityEnableGravity_API(m_pGameInterface, pThing, bIsEnabled);
}

void LuaQuestState::EntitySetLightAsOn(const std::shared_ptr<CScriptThing>& spThing, bool bIsOn) {
    if (!m_pGameInterface || !EntitySetLightAsOn_API) {
        Log("!!! ERROR: EntitySetLightAsOn - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetLightAsOn - pThing is null!");
        return;
    }
    EntitySetLightAsOn_API(m_pGameInterface, pThing, bIsOn);
}

void LuaQuestState::EntityFadeIn(const std::shared_ptr<CScriptThing>& spThing, float duration) {
    if (!m_pGameInterface || !EntityFadeIn_API) {
        Log("!!! ERROR: EntityFadeIn - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityFadeIn - pThing is null!");
        return;
    }
    EntityFadeIn_API(m_pGameInterface, pThing, duration);
}

void LuaQuestState::EntityBeginLoadingAnimation(const std::shared_ptr<CScriptThing>& spThing, const std::string& pAnimName) {
    if (!m_pGameInterface || !EntityBeginLoadingAnimation_API) {
        Log("!!! ERROR: EntityBeginLoadingAnimation - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityBeginLoadingAnimation - pThing is null!");
        return;
    }
    FableString fsAnim(pAnimName.c_str());
    EntityBeginLoadingAnimation_API(m_pGameInterface, pThing, fsAnim.get());
}

void LuaQuestState::EntityBeginLoadingBasicAnimations(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityBeginLoadingBasicAnimations_API) {
        Log("!!! ERROR: EntityBeginLoadingBasicAnimations - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityBeginLoadingBasicAnimations - pThing is null!");
        return;
    }
    EntityBeginLoadingBasicAnimations_API(m_pGameInterface, pThing);
}

bool LuaQuestState::EntityCastForcePush(const std::shared_ptr<CScriptThing>& spCaster, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntityCastForcePush_API) {
        Log("!!! ERROR: EntityCastForcePush - API not available!");
        return false;
    }
    CScriptThing* pCaster = spCaster.get();
    if (!pCaster) {
        Log("!!! ERROR: EntityCastForcePush - pCaster is null!");
        return false;
    }
    bool bFinalUnknown = bUnknown.value_or(true); // Default to true
    return EntityCastForcePush_API(m_pGameInterface, pCaster, bFinalUnknown);
}

void LuaQuestState::EntityCastLightningAtTarget(const std::shared_ptr<CScriptThing>& spCaster, const std::shared_ptr<CScriptThing>& spTarget) {
    if (!m_pGameInterface || !EntityCastLightningAtTarget_API) {
        Log("!!! ERROR: EntityCastLightningAtTarget - API not available!");
        return;
    }
    CScriptThing* pCaster = spCaster.get();
    if (!pCaster) {
        Log("!!! ERROR: EntityCastLightningAtTarget - pCaster is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: EntityCastLightningAtTarget - pTarget is null!");
        return;
    }
    EntityCastLightningAtTarget_API(m_pGameInterface, pCaster, pTarget);
}

void LuaQuestState::BeginLoadingMesh(const std::string& pMeshName) {
    if (m_pGameInterface && BeginLoadingMesh_API) {
        FableString fsMesh(pMeshName.c_str());
        BeginLoadingMesh_API(m_pGameInterface, fsMesh.get());
    }
    else {
        Log("!!! ERROR: BeginLoadingMesh - API not available!");
    }
}

bool LuaQuestState::EntityWillTeleportToArea(const std::shared_ptr<CScriptThing>& spThing, sol::table pos, float f1, float f2) {
    if (!m_pGameInterface || !EntityWillTeleportToArea_API) {
        Log("!!! ERROR: EntityWillTeleportToArea - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityWillTeleportToArea - pThing is null!");
        return false;
    }
    C3DVector vecPos = {
        pos["x"].get_or(0.0f),
        pos["y"].get_or(0.0f),
        pos["z"].get_or(0.0f)
    };
    return EntityWillTeleportToArea_API(m_pGameInterface, pThing, vecPos, f1, f2);
}

void LuaQuestState::EntityStartScreamerSuperAttackThing(const std::shared_ptr<CScriptThing>& spScreamer, const std::shared_ptr<CScriptThing>& spTarget) {
    if (!m_pGameInterface || !EntityStartScreamerSuperAttackThing_API) {
        Log("!!! ERROR: EntityStartScreamerSuperAttackThing - API not available!");
        return;
    }
    CScriptThing* pScreamer = spScreamer.get();
    if (!pScreamer) {
        Log("!!! ERROR: EntityStartScreamerSuperAttackThing - pScreamer is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: EntityStartScreamerSuperAttackThing - pTarget is null!");
        return;
    }
    EntityStartScreamerSuperAttackThing_API(m_pGameInterface, pScreamer, pTarget);
}

void LuaQuestState::EntityEndScreamerSuperAttackThing(const std::shared_ptr<CScriptThing>& spScreamer) {
    if (!m_pGameInterface || !EntityEndScreamerSuperAttackThing_API) {
        Log("!!! ERROR: EntityEndScreamerSuperAttackThing - API not available!");
        return;
    }
    CScriptThing* pScreamer = spScreamer.get();
    if (!pScreamer) {
        Log("!!! ERROR: EntityEndScreamerSuperAttackThing - pScreamer is null!");
        return;
    }
    EntityEndScreamerSuperAttackThing_API(m_pGameInterface, pScreamer);
}

void LuaQuestState::SetLightColour(const std::shared_ptr<CScriptThing>& spLight, sol::table color) {
    if (!m_pGameInterface || !SetLightColour_API) {
        Log("!!! ERROR: SetLightColour - API not available!");
        return;
    }
    CScriptThing* pLight = spLight.get();
    if (!pLight) {
        Log("!!! ERROR: SetLightColour - pLight is null!");
        return;
    }
    // ParseColorTable (B,G,R,A) is used as it's the established pattern
    CRGBColour col = ParseColorTable(color);
    SetLightColour_API(m_pGameInterface, pLight, &col);
}

void LuaQuestState::CreatureGeneratorSetFamily(const std::shared_ptr<CScriptThing>& spGenerator, const std::string& familyName) {
    if (!m_pGameInterface || !CreatureGeneratorSetFamily_API) {
        Log("!!! ERROR: CreatureGeneratorSetFamily - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorSetFamily - pGenerator is null!");
        return;
    }
    FableString fsFamily(familyName.c_str());
    CreatureGeneratorSetFamily_API(m_pGameInterface, pGenerator, fsFamily.get());
}

void LuaQuestState::CreatureGeneratorTrigger(const std::shared_ptr<CScriptThing>& spGenerator) {
    if (!m_pGameInterface || !CreatureGeneratorTrigger_API) {
        Log("!!! ERROR: CreatureGeneratorTrigger - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorTrigger - pGenerator is null!");
        return;
    }
    CreatureGeneratorTrigger_API(m_pGameInterface, pGenerator);
}

void LuaQuestState::CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger(const std::shared_ptr<CScriptThing>& spGenerator, bool bAlwaysCreate) {
    if (!m_pGameInterface || !CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger_API) {
        Log("!!! ERROR: CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger - pGenerator is null!");
        return;
    }
    CreatureGeneratorSetAlwaysCreateCreaturesOnTrigger_API(m_pGameInterface, pGenerator, bAlwaysCreate);
}

bool LuaQuestState::CreatureGeneratorIsDepleted(const std::shared_ptr<CScriptThing>& spGenerator) {
    if (!m_pGameInterface || !CreatureGeneratorIsDepleted_API) {
        Log("!!! ERROR: CreatureGeneratorIsDepleted - API not available!");
        return true; // Default to 'depleted' on error
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorIsDepleted - pGenerator is null!");
        return true;
    }
    return CreatureGeneratorIsDepleted_API(m_pGameInterface, pGenerator);
}

bool LuaQuestState::CreatureGeneratorIsDestroyed(const std::shared_ptr<CScriptThing>& spGenerator) {
    if (!m_pGameInterface || !CreatureGeneratorIsDestroyed_API) {
        Log("!!! ERROR: CreatureGeneratorIsDestroyed - API not available!");
        return true; // Default to 'destroyed' on error
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorIsDestroyed - pGenerator is null!");
        return true;
    }
    return CreatureGeneratorIsDestroyed_API(m_pGameInterface, pGenerator);
}

void LuaQuestState::CreatureGeneratorSetGeneratedCreatureScriptName(const std::shared_ptr<CScriptThing>& spGenerator, const std::string& scriptName) {
    if (!m_pGameInterface || !CreatureGeneratorSetGeneratedCreatureScriptName_API) {
        Log("!!! ERROR: CreatureGeneratorSetGeneratedCreatureScriptName - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorSetGeneratedCreatureScriptName - pGenerator is null!");
        return;
    }
    FableString fsScript(scriptName.c_str());
    CreatureGeneratorSetGeneratedCreatureScriptName_API(m_pGameInterface, pGenerator, fsScript.get());
}

void LuaQuestState::CreatureGeneratorSetNumTriggers(const std::shared_ptr<CScriptThing>& spGenerator, int numTriggers) {
    if (!m_pGameInterface || !CreatureGeneratorSetNumTriggers_API) {
        Log("!!! ERROR: CreatureGeneratorSetNumTriggers - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorSetNumTriggers - pGenerator is null!");
        return;
    }
    CreatureGeneratorSetNumTriggers_API(m_pGameInterface, pGenerator, numTriggers);
}

int LuaQuestState::CreatureGeneratorGetNumGeneratedCreatures(const std::shared_ptr<CScriptThing>& spGenerator) {
    if (!m_pGameInterface || !CreatureGeneratorGetNumGeneratedCreatures_API) {
        Log("!!! ERROR: CreatureGeneratorGetNumGeneratedCreatures - API not available!");
        return 0;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorGetNumGeneratedCreatures - pGenerator is null!");
        return 0;
    }
    return CreatureGeneratorGetNumGeneratedCreatures_API(m_pGameInterface, pGenerator);
}

bool LuaQuestState::CreatureGeneratorAreAllCreaturesAlive(const std::shared_ptr<CScriptThing>& spGenerator) {
    if (!m_pGameInterface || !CreatureGeneratorAreAllCreaturesAlive_API) {
        Log("!!! ERROR: CreatureGeneratorAreAllCreaturesAlive - API not available!");
        return false;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorAreAllCreaturesAlive - pGenerator is null!");
        return false;
    }
    return CreatureGeneratorAreAllCreaturesAlive_API(m_pGameInterface, pGenerator);
}

void LuaQuestState::CreatureGeneratorAddTriggerer(const std::shared_ptr<CScriptThing>& spGenerator, const std::shared_ptr<CScriptThing>& spTriggerer) {
    if (!m_pGameInterface || !CreatureGeneratorAddTriggerer_API) {
        Log("!!! ERROR: CreatureGeneratorAddTriggerer - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorAddTriggerer - pGenerator is null!");
        return;
    }
    CScriptThing* pTriggerer = spTriggerer.get();
    if (!pTriggerer) {
        Log("!!! ERROR: CreatureGeneratorAddTriggerer - pTriggerer is null!");
        return;
    }
    CreatureGeneratorAddTriggerer_API(m_pGameInterface, pGenerator, pTriggerer);
}

void LuaQuestState::CreatureGeneratorRemoveTriggerer(const std::shared_ptr<CScriptThing>& spGenerator, const std::shared_ptr<CScriptThing>& spTriggerer) {
    if (!m_pGameInterface || !CreatureGeneratorRemoveTriggerer_API) {
        Log("!!! ERROR: CreatureGeneratorRemoveTriggerer - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: CreatureGeneratorRemoveTriggerer - pGenerator is null!");
        return;
    }
    CScriptThing* pTriggerer = spTriggerer.get();
    if (!pTriggerer) {
        Log("!!! ERROR: CreatureGeneratorRemoveTriggerer - pTriggerer is null!");
        return;
    }
    CreatureGeneratorRemoveTriggerer_API(m_pGameInterface, pGenerator, pTriggerer);
}

void LuaQuestState::SetCreatureGeneratorEnabled(const std::shared_ptr<CScriptThing>& spGenerator, bool bIsEnabled) {
    if (!m_pGameInterface || !SetCreatureGeneratorEnabled_API) {
        Log("!!! ERROR: SetCreatureGeneratorEnabled - API not available!");
        return;
    }
    CScriptThing* pGenerator = spGenerator.get();
    if (!pGenerator) {
        Log("!!! ERROR: SetCreatureGeneratorEnabled - pGenerator is null!");
        return;
    }
    SetCreatureGeneratorEnabled_API(m_pGameInterface, pGenerator, bIsEnabled);
}

void LuaQuestState::SetCreatureGeneratorsEnabledDuringScript(const std::string& regionName, bool bIsEnabled) {
    if (m_pGameInterface && SetCreatureGeneratorsEnabledDuringScript_API) {
        FableString fsRegion(regionName.c_str());
        SetCreatureGeneratorsEnabledDuringScript_API(m_pGameInterface, fsRegion.get(), bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetCreatureGeneratorsEnabledDuringScript - API not available!");
    }
}

void LuaQuestState::SetCreatureGeneratorsCreatureGroupAsEnabled(int eGroup, bool bIsEnabled) {
    if (m_pGameInterface && SetCreatureGeneratorsCreatureGroupAsEnabled_API) {
        // We assume ECreatureGroup is an enum that can be cast from an int
        SetCreatureGeneratorsCreatureGroupAsEnabled_API(m_pGameInterface, static_cast<ECreatureGroup>(eGroup), bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetCreatureGeneratorsCreatureGroupAsEnabled - API not available!");
    }
}

bool LuaQuestState::IsCreatureGenerationEnabledForRegion(const std::string& regionName) {
    if (m_pGameInterface && IsCreatureGenerationEnabledForRegion_API) {
        FableString fsRegion(regionName.c_str());
        return IsCreatureGenerationEnabledForRegion_API(m_pGameInterface, fsRegion.get());
    }
    Log("!!! ERROR: IsCreatureGenerationEnabledForRegion - API not available!");
    return false;
}

bool LuaQuestState::IsCreatureFlying(const std::shared_ptr<CScriptThing>& spCreature) {
    if (!m_pGameInterface || !IsCreatureFlying_API) {
        Log("!!! ERROR: IsCreatureFlying - API not available!");
        return false;
    }
    CScriptThing* pCreature = spCreature.get();
    if (!pCreature) {
        Log("!!! ERROR: IsCreatureFlying - pCreature is null!");
        return false;
    }
    return IsCreatureFlying_API(m_pGameInterface, pCreature);
}

void LuaQuestState::SetTeleporterAsActive(const std::shared_ptr<CScriptThing>& spTeleporter, bool bIsActive) {
    if (!m_pGameInterface || !SetTeleporterAsActive_API) {
        Log("!!! ERROR: SetTeleporterAsActive - API not available!");
        return;
    }
    CScriptThing* pTeleporter = spTeleporter.get();
    if (!pTeleporter) {
        Log("!!! ERROR: SetTeleporterAsActive - pTeleporter is null!");
        return;
    }
    SetTeleporterAsActive_API(m_pGameInterface, pTeleporter, bIsActive);
}

bool LuaQuestState::IsTeleporterActive(const std::shared_ptr<CScriptThing>& spTeleporter) {
    if (!m_pGameInterface || !IsTeleporterActive_API) {
        Log("!!! ERROR: IsTeleporterActive - API not available!");
        return false;
    }
    CScriptThing* pTeleporter = spTeleporter.get();
    if (!pTeleporter) {
        Log("!!! ERROR: IsTeleporterActive - pTeleporter is null!");
        return false;
    }
    return IsTeleporterActive_API(m_pGameInterface, pTeleporter);
}

void LuaQuestState::SetTeleportingAsActive(bool bIsActive) {
    if (m_pGameInterface && SetTeleportingAsActive_API) {
        SetTeleportingAsActive_API(m_pGameInterface, bIsActive);
    }
    else {
        Log("!!! ERROR: SetTeleportingAsActive - API not available!");
    }
}

bool LuaQuestState::IsTeleportingActive() {
    if (m_pGameInterface && IsTeleportingActive_API) {
        return IsTeleportingActive_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsTeleportingActive - API not available!");
    return false;
}

void LuaQuestState::SetRegionEntranceAsActive(const std::shared_ptr<CScriptThing>& spEntrance, bool bIsActive) {
    if (!m_pGameInterface || !SetRegionEntranceAsActive_API) {
        Log("!!! ERROR: SetRegionEntranceAsActive - API not available!");
        return;
    }
    CScriptThing* pEntrance = spEntrance.get();
    if (!pEntrance) {
        Log("!!! ERROR: SetRegionEntranceAsActive - pEntrance is null!");
        return;
    }
    SetRegionEntranceAsActive_API(m_pGameInterface, pEntrance, bIsActive);
}

void LuaQuestState::SetRegionTextDisplayAsActive(bool bIsActive) {
    if (m_pGameInterface && SetRegionTextDisplayAsActive_API) {
        SetRegionTextDisplayAsActive_API(m_pGameInterface, bIsActive);
    }
    else {
        Log("!!! ERROR: SetRegionTextDisplayAsActive - API not available!");
    }
}

void LuaQuestState::SetHeroSleepingAsEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetHeroSleepingAsEnabled_API) {
        SetHeroSleepingAsEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetHeroSleepingAsEnabled - API not available!");
    }
}

bool LuaQuestState::IsHeroSleepingEnabled() {
    if (m_pGameInterface && IsHeroSleepingEnabled_API) {
        return IsHeroSleepingEnabled_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsHeroSleepingEnabled - API not available!");
    return false;
}

void LuaQuestState::SetExperienceSpendingAsEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetExperienceSpendingAsEnabled_API) {
        SetExperienceSpendingAsEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetExperienceSpendingAsEnabled - API not available!");
    }
}

void LuaQuestState::SetMoralityChangingAsEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetMoralityChangingAsEnabled_API) {
        SetMoralityChangingAsEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetMoralityChangingAsEnabled - API not available!");
    }
}

void LuaQuestState::SetSummonerDeathExplosionAffectsHero(bool bDoesAffect) {
    if (m_pGameInterface && SetSummonerDeathExplosionAffectsHero_API) {
        SetSummonerDeathExplosionAffectsHero_API(m_pGameInterface, bDoesAffect);
    }
    else {
        Log("!!! ERROR: SetSummonerDeathExplosionAffectsHero - API not available!");
    }
}

std::shared_ptr<CScriptThing> LuaQuestState::GetNearestEnabledDiggingSpot(const std::shared_ptr<CScriptThing>& spRelativeTo) {
    if (!m_pGameInterface || !GetNearestEnabledDiggingSpot_API || !Game_malloc) {
        Log("!!! ERROR: GetNearestEnabledDiggingSpot - API or memory allocator not available!");
        return nullptr;
    }
    CScriptThing* pRelativeTo = spRelativeTo.get();
    if (!pRelativeTo) {
        Log("!!! ERROR: GetNearestEnabledDiggingSpot - pRelativeTo is null!");
        return nullptr;
    }

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: GetNearestEnabledDiggingSpot - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    GetNearestEnabledDiggingSpot_API(m_pGameInterface, pResult, pRelativeTo);

    return WrapScriptThingOutput(pResult);
}

bool LuaQuestState::IsDiggingSpotEnabled(const std::shared_ptr<CScriptThing>& spSpot) {
    if (!m_pGameInterface || !IsDiggingSpotEnabled_API) {
        Log("!!! ERROR: IsDiggingSpotEnabled - API not available!");
        return false;
    }
    CScriptThing* pSpot = spSpot.get();
    if (!pSpot) {
        Log("!!! ERROR: IsDiggingSpotEnabled - pSpot is null!");
        return false;
    }
    return IsDiggingSpotEnabled_API(m_pGameInterface, pSpot);
}

bool LuaQuestState::IsDiggingSpotHidden(const std::shared_ptr<CScriptThing>& spSpot) {
    if (!m_pGameInterface || !IsDiggingSpotHidden_API) {
        Log("!!! ERROR: IsDiggingSpotHidden - API not available!");
        return false;
    }
    CScriptThing* pSpot = spSpot.get();
    if (!pSpot) {
        Log("!!! ERROR: IsDiggingSpotHidden - pSpot is null!");
        return false;
    }
    return IsDiggingSpotHidden_API(m_pGameInterface, pSpot);
}

void LuaQuestState::SetDiggingSpotAsHidden(const std::shared_ptr<CScriptThing>& spSpot, bool bIsHidden) {
    if (!m_pGameInterface || !SetDiggingSpotAsHidden_API) {
        Log("!!! ERROR: SetDiggingSpotAsHidden - API not available!");
        return;
    }
    CScriptThing* pSpot = spSpot.get();
    if (!pSpot) {
        Log("!!! ERROR: SetDiggingSpotAsHidden - pSpot is null!");
        return;
    }
    SetDiggingSpotAsHidden_API(m_pGameInterface, pSpot, bIsHidden);
}

bool LuaQuestState::CheckForCameraMessage(const std::string& message) {
    if (m_pGameInterface && CheckForCameraMessage_API) {
        FableString fsMessage(message.c_str());
        return CheckForCameraMessage_API(m_pGameInterface, fsMessage.get());
    }
    Log("!!! ERROR: CheckForCameraMessage - API not available!");
    return false;
}

void LuaQuestState::WaitForCameraMessage(const std::string& message) {
    Log("--- WaitForCameraMessage_Blocking START: '" + message + "' ---");

    if (!m_pGameInterface || !CheckForCameraMessage_API || !NewScriptFrame_API || !IsActiveThreadTerminating_API) {
        Log("!!! ERROR: WaitForCameraMessage_Blocking - Prerequisites not met!");
        Log("--- WaitForCameraMessage_Blocking END (Failure) ---");
        return;
    }

    FableString fsMessage(message.c_str());
    int waitFrames = 0;

    // Loop *while* the message has NOT been received
    while (!CheckForCameraMessage_API(m_pGameInterface, fsMessage.get())) {
        NewScriptFrame(); // Yield
        waitFrames++;

        // Use the standard termination check
        if (IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(m_pGameInterface)) {
            Log("!!! WARNING: WaitForCameraMessage_Blocking - Thread terminated after " + std::to_string(waitFrames) + " frames.");
            break;
        }
    }

    if (waitFrames > 0) {
        Log("--- WaitForCameraMessage_Blocking END (Message received after " + std::to_string(waitFrames) + " frames) ---");
    }
    else {
        Log("--- WaitForCameraMessage_Blocking END (Message was already present) ---");
    }
}

void LuaQuestState::SetThingAsConscious(const std::shared_ptr<CScriptThing>& spThing, bool bIsConscious, sol::optional<std::string> animName) {
    if (!m_pGameInterface || !SetThingAsConscious_API) {
        Log("!!! ERROR: SetThingAsConscious - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetThingAsConscious - pThing is null!");
        return;
    }

    // Use the optional string, defaulting to an empty string if not provided
    FableString fsAnim(animName.value_or("").c_str());
    SetThingAsConscious_API(m_pGameInterface, pThing, bIsConscious, fsAnim.get());
}

void LuaQuestState::SetFireToThing(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !SetFireToThing_API) {
        Log("!!! ERROR: SetFireToThing - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetFireToThing - pThing is null!");
        return;
    }
    SetFireToThing_API(m_pGameInterface, pThing);
}

void LuaQuestState::ExtinguishFiresOnThing(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ExtinguishFiresOnThing_API) {
        Log("!!! ERROR: ExtinguishFiresOnThing - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ExtinguishFiresOnThing - pThing is null!");
        return;
    }
    ExtinguishFiresOnThing_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsThingOnFire(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsThingOnFire_API) {
        Log("!!! ERROR: IsThingOnFire - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsThingOnFire - pThing is null!");
        return false;
    }
    return IsThingOnFire_API(m_pGameInterface, pThing);
}

void LuaQuestState::AddItemToContainer(const std::shared_ptr<CScriptThing>& spContainer, const std::string& itemDefName) {
    LogToFile("===== AddItemToContainer START: item='" + itemDefName + "' =====");
    if (!m_pGameInterface || !AddItemToContainer_API) {
        Log("!!! ERROR: AddItemToContainer - API not available!");
        return;
    }
    CScriptThing* pContainer = spContainer.get();
    if (!pContainer) {
        Log("!!! ERROR: AddItemToContainer - pContainer is null!");
        return;
    }
    LogCScriptThingDetails("    Container", pContainer);
    FableString fsItem(itemDefName.c_str());
    AddItemToContainer_API(m_pGameInterface, pContainer, fsItem.get());
    LogToFile("===== AddItemToContainer END =====");
}

void LuaQuestState::RemoveItemFromContainer(const std::shared_ptr<CScriptThing>& spContainer, const std::string& itemDefName) {
    if (!m_pGameInterface || !RemoveItemFromContainer_API) {
        Log("!!! ERROR: RemoveItemFromContainer - API not available!");
        return;
    }
    CScriptThing* pContainer = spContainer.get();
    if (!pContainer) {
        Log("!!! ERROR: RemoveItemFromContainer - pContainer is null!");
        return;
    }
    FableString fsItem(itemDefName.c_str());
    RemoveItemFromContainer_API(m_pGameInterface, pContainer, fsItem.get());
}

void LuaQuestState::EntitySetDeathContainerAsEnabled(const std::shared_ptr<CScriptThing>& spThing, bool bIsEnabled) {
    if (!m_pGameInterface || !EntitySetDeathContainerAsEnabled_API) {
        Log("!!! ERROR: EntitySetDeathContainerAsEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetDeathContainerAsEnabled - pThing is null!");
        return;
    }
    EntitySetDeathContainerAsEnabled_API(m_pGameInterface, pThing, bIsEnabled);
}

sol::table LuaQuestState::GetItemDefNamesFromContainer(const std::shared_ptr<CScriptThing>& spContainer, sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetItemDefNamesFromContainer_API) {
        Log("!!! ERROR: GetItemDefNamesFromContainer - API not available!");
        return resultTable;
    }
    CScriptThing* pContainer = spContainer.get();
    if (!pContainer) {
        Log("!!! ERROR: GetItemDefNamesFromContainer - pContainer is null!");
        return resultTable;
    }

    std::vector<CCharString> outNames;
    GetItemDefNamesFromContainer_API(m_pGameInterface, pContainer, &outNames);

    int luaIndex = 1;
    for (auto& cStr : outNames) {
        const char* text = CCharString_ToConstChar_API(&cStr);
        if (text) {
            resultTable[luaIndex++] = std::string(text);
        }
        CCharString_Destroy(&cStr); // Must destroy the strings given by the game
    }

    return resultTable;
}

void LuaQuestState::EntitySetStategroupEnabled(const std::shared_ptr<CScriptThing>& spThing, const std::string& stateGroupName, bool bEnabled) {
    if (!m_pGameInterface || !EntitySetStategroupEnabled_API) {
        Log("!!! ERROR: EntitySetStategroupEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetStategroupEnabled - pThing is null!");
        return;
    }
    FableString fsGroup(stateGroupName.c_str());
    EntitySetStategroupEnabled_API(m_pGameInterface, pThing, fsGroup.get(), bEnabled);
}

void LuaQuestState::EntitySetCombatEnabled(const std::shared_ptr<CScriptThing>& spThing, bool bIsEnabled) {
    if (!m_pGameInterface || !EntitySetCombatEnabled_API) {
        Log("!!! ERROR: EntitySetCombatEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetCombatEnabled - pThing is null!");
        return;
    }
    EntitySetCombatEnabled_API(m_pGameInterface, pThing, bIsEnabled);
}

void LuaQuestState::EntitySetSleepEnabled(const std::shared_ptr<CScriptThing>& spThing, bool bIsEnabled) {
    if (!m_pGameInterface || !EntitySetSleepEnabled_API) {
        Log("!!! ERROR: EntitySetSleepEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetSleepEnabled - pThing is null!");
        return;
    }
    EntitySetSleepEnabled_API(m_pGameInterface, pThing, bIsEnabled);
}

void LuaQuestState::EntitySetOpinionReactionsEnabled(const std::shared_ptr<CScriptThing>& spThing, bool bEnabled) {
    if (!m_pGameInterface || !EntitySetOpinionReactionsEnabled_API) {
        Log("!!! ERROR: EntitySetOpinionReactionsEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetOpinionReactionsEnabled - pThing is null!");
        return;
    }
    EntitySetOpinionReactionsEnabled_API(m_pGameInterface, pThing, bEnabled);
}

void LuaQuestState::EntitySetDeedReactionsEnabled(CScriptThing* pThing, bool bEnabled) {
    if (!m_pGameInterface || !EntitySetDeedReactionsEnabled_API) {
        Log("!!! ERROR: EntitySetDeedReactionsEnabled - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: EntitySetDeedReactionsEnabled - pThing is null!");
        return;
    }
    EntitySetDeedReactionsEnabled_API(m_pGameInterface, pThing, bEnabled);
}

sol::table LuaQuestState::DebugGetAllTextEntriesForTargetedThing(sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !DebugGetAllTextEntriesForTargetedThing_API) {
        Log("!!! ERROR: DebugGetAllTextEntriesForTargetedThing - API not available!");
        return resultTable;
    }

    std::set<unsigned long> outEntries;
    DebugGetAllTextEntriesForTargetedThing_API(m_pGameInterface, &outEntries);

    int luaIndex = 1;
    for (unsigned long entry : outEntries) {
        resultTable[luaIndex++] = entry;
    }

    return resultTable;
}

void LuaQuestState::EntityUnsetThingAsEnemyOfThing(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    CScriptThing* pThing1 = spThing1.get();
    CScriptThing* pThing2 = spThing2.get();
    if (!m_pGameInterface || !EntityUnsetThingAsEnemyOfThing_API) {
        Log("!!! ERROR: EntityUnsetThingAsEnemyOfThing - API not available!");
        return;
    }
    if (!pThing1) {
        Log("!!! ERROR: EntityUnsetThingAsEnemyOfThing - pThing1 is null!");
        return;
    }
    if (!pThing2) {
        Log("!!! ERROR: EntityUnsetThingAsEnemyOfThing - pThing2 is null!");
        return;
    }
    EntityUnsetThingAsEnemyOfThing_API(m_pGameInterface, pThing1, pThing2);
}

void LuaQuestState::EntitySetThingAsAllyOfThing(CScriptThing* pThing1, CScriptThing* pThing2) {
    if (!m_pGameInterface || !EntitySetThingAsAllyOfThing_API) {
        Log("!!! ERROR: EntitySetThingAsAllyOfThing - API not available!");
        return;
    }
    if (!pThing1) {
        Log("!!! ERROR: EntitySetThingAsAllyOfThing - pThing1 is null!");
        return;
    }
    if (!pThing2) {
        Log("!!! ERROR: EntitySetThingAsAllyOfThing - pThing2 is null!");
        return;
    }
    EntitySetThingAsAllyOfThing_API(m_pGameInterface, pThing1, pThing2);
}

void LuaQuestState::EntityUnsetThingAsAllyOfThing(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    if (!m_pGameInterface || !EntityUnsetThingAsAllyOfThing_API) {
        Log("!!! ERROR: EntityUnsetThingAsAllyOfThing - API not available!");
        return;
    }
    CScriptThing* pThing1 = spThing1.get();
    if (!pThing1) {
        Log("!!! ERROR: EntityUnsetThingAsAllyOfThing - pThing1 is null!");
        return;
    }
    CScriptThing* pThing2 = spThing2.get();
    if (!pThing2) {
        Log("!!! ERROR: EntityUnsetThingAsAllyOfThing - pThing2 is null!");
        return;
    }
    EntityUnsetThingAsAllyOfThing_API(m_pGameInterface, pThing1, pThing2);
}

void LuaQuestState::SetFactionAsAlliedToFaction(const std::string& faction1, const std::string& faction2) {
    if (m_pGameInterface && SetFactionAsAlliedToFaction_API) {
        FableString fs1(faction1.c_str());
        FableString fs2(faction2.c_str());
        SetFactionAsAlliedToFaction_API(m_pGameInterface, fs1.get(), fs2.get());
    }
    else {
        Log("!!! ERROR: SetFactionAsAlliedToFaction - API not available!");
    }
}

void LuaQuestState::SetFactionAsNeutralToFaction(const std::string& faction1, const std::string& faction2) {
    if (m_pGameInterface && SetFactionAsNeutralToFaction_API) {
        FableString fs1(faction1.c_str());
        FableString fs2(faction2.c_str());
        SetFactionAsNeutralToFaction_API(m_pGameInterface, fs1.get(), fs2.get());
    }
    else {
        Log("!!! ERROR: SetFactionAsNeutralToFaction - API not available!");
    }
}

void LuaQuestState::SetFactionAsEnemyToFaction(const std::string& faction1, const std::string& faction2) {
    if (m_pGameInterface && SetFactionAsEnemyToFaction_API) {
        FableString fs1(faction1.c_str());
        FableString fs2(faction2.c_str());
        SetFactionAsEnemyToFaction_API(m_pGameInterface, fs1.get(), fs2.get());
    }
    else {
        Log("!!! ERROR: SetFactionAsEnemyToFaction - API not available!");
    }
}

bool LuaQuestState::AreEntitiesEnemies(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    if (!m_pGameInterface || !AreEntitiesEnemies_API) {
        Log("!!! ERROR: AreEntitiesEnemies - API not available!");
        return false;
    }
    CScriptThing* pThing1 = spThing1.get();
    if (!pThing1) {
        Log("!!! ERROR: AreEntitiesEnemies - pThing1 is null!");
        return false;
    }
    CScriptThing* pThing2 = spThing2.get();
    if (!pThing2) {
        Log("!!! ERROR: AreEntitiesEnemies - pThing2 is null!");
        return false;
    }
    return AreEntitiesEnemies_API(m_pGameInterface, pThing1, pThing2);
}

int LuaQuestState::GetNextInOpinionAttitudeGraph(int eCurrent) {
    if (m_pGameInterface && GetNextInOpinionAttitudeGraph_API) {
        // Cast int from Lua to enum, call API, cast enum result back to int
        return static_cast<int>(GetNextInOpinionAttitudeGraph_API(m_pGameInterface, static_cast<EOpinionAttitudeType>(eCurrent)));
    }
    Log("!!! ERROR: GetNextInOpinionAttitudeGraph - API not available!");
    return eCurrent; // Return the original value on error
}

std::string LuaQuestState::GetOpinionAttitudeAsString(int eAttitude) {
    if (m_pGameInterface && GetOpinionAttitudeAsString_API) {
        CCharString result = { 0 };
        GetOpinionAttitudeAsString_API(m_pGameInterface, static_cast<EOpinionAttitudeType>(eAttitude), &result);

        if (result.pStringData) {
            const char* text = CCharString_ToConstChar_API(&result);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&result);
                return finalString;
            }
            CCharString_Destroy(&result);
        }
    }
    Log("!!! ERROR: GetOpinionAttitudeAsString - API not available!");
    return "";
}

int LuaQuestState::EntityGetOpinionAttitudeToPlayer(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityGetOpinionAttitudeToPlayer_API) {
        Log("!!! ERROR: EntityGetOpinionAttitudeToPlayer - API not available!");
        return 0; // Default to a neutral/null enum value
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetOpinionAttitudeToPlayer - pThing is null!");
        return 0;
    }
    return static_cast<int>(EntityGetOpinionAttitudeToPlayer_API(m_pGameInterface, pThing));
}

std::string LuaQuestState::EntityGetOpinionAttitudeToPlayerAsString(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityGetOpinionAttitudeToPlayerAsString_API) {
        Log("!!! ERROR: EntityGetOpinionAttitudeToPlayerAsString - API not available!");
        return "";
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetOpinionAttitudeToPlayerAsString - pThing is null!");
        return "";
    }

    CCharString result = { 0 };
    EntityGetOpinionAttitudeToPlayerAsString_API(m_pGameInterface, pThing, &result);

    if (result.pStringData) {
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&result);
            return finalString;
        }
        CCharString_Destroy(&result);
    }
    return "";
}

float LuaQuestState::EntityGetOpinionOfPlayer(const std::shared_ptr<CScriptThing>& spThing, int eOpinion) {
    if (!m_pGameInterface || !EntityGetOpinionOfPlayer_API) {
        Log("!!! ERROR: EntityGetOpinionOfPlayer - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetOpinionOfPlayer - pThing is null!");
        return 0.0f;
    }
    return EntityGetOpinionOfPlayer_API(m_pGameInterface, pThing, static_cast<EOpinion>(eOpinion));
}

void LuaQuestState::EntitySetOpinionReactionMaskByInt(const std::shared_ptr<CScriptThing>& spThing, int mask) {
    if (!m_pGameInterface || !EntitySetOpinionReactionMaskByInt_API) {
        Log("!!! ERROR: EntitySetOpinionReactionMaskByInt - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetOpinionReactionMaskByInt - pThing is null!");
        return;
    }
    EntitySetOpinionReactionMaskByInt_API(m_pGameInterface, pThing, mask);
}

void LuaQuestState::EntitySetOpinionDeedMaskByString(const std::shared_ptr<CScriptThing>& spThing, const std::string& maskName) {
    if (!m_pGameInterface || !EntitySetOpinionDeedMaskByString_API) {
        Log("!!! ERROR: EntitySetOpinionDeedMaskByString - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetOpinionDeedMaskByString - pThing is null!");
        return;
    }
    FableString fsMask(maskName.c_str());
    EntitySetOpinionDeedMaskByString_API(m_pGameInterface, pThing, fsMask.get());
}

void LuaQuestState::EntitySetOpinionDeedMaskByInt(const std::shared_ptr<CScriptThing>& spThing, int mask) {
    if (!m_pGameInterface || !EntitySetOpinionDeedMaskByInt_API) {
        Log("!!! ERROR: EntitySetOpinionDeedMaskByInt - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetOpinionDeedMaskByInt - pThing is null!");
        return;
    }
    EntitySetOpinionDeedMaskByInt_API(m_pGameInterface, pThing, mask);
}

void LuaQuestState::EntitySetOpinionDeedTypeEnabled(const std::shared_ptr<CScriptThing>& spThing, int eDeed, bool bIsEnabled) {
    if (!m_pGameInterface || !EntitySetOpinionDeedTypeEnabled_API) {
        Log("!!! ERROR: EntitySetOpinionDeedTypeEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetOpinionDeedTypeEnabled - pThing is null!");
        return;
    }
    EntitySetOpinionDeedTypeEnabled_API(m_pGameInterface, pThing, static_cast<EOpinionDeedType>(eDeed), bIsEnabled);
}

void LuaQuestState::EntitySetOpinionAttitudeEnabled(const std::shared_ptr<CScriptThing>& spThing, int eAttitude, bool bIsEnabled) {
    if (!m_pGameInterface || !EntitySetOpinionAttitudeEnabled_API) {
        Log("!!! ERROR: EntitySetOpinionAttitudeEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetOpinionAttitudeEnabled - pThing is null!");
        return;
    }
    EntitySetOpinionAttitudeEnabled_API(m_pGameInterface, pThing, static_cast<EOpinionAttitudeType>(eAttitude), bIsEnabled);
}

void LuaQuestState::EntitySetOpinionReactionEnabled(const std::shared_ptr<CScriptThing>& spThing, int eReaction, bool bIsEnabled) {
    if (!m_pGameInterface || !EntitySetOpinionReactionEnabled_API) {
        Log("!!! ERROR: EntitySetOpinionReactionEnabled - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetOpinionReactionEnabled - pThing is null!");
        return;
    }
    EntitySetOpinionReactionEnabled_API(m_pGameInterface, pThing, static_cast<EOpinionReactionType>(eReaction), bIsEnabled);
}

void LuaQuestState::EntitySetPersonalityOverrideByInt(const std::shared_ptr<CScriptThing>& spThing, int personality) {
    if (!m_pGameInterface || !EntitySetPersonalityOverrideByInt_API) {
        Log("!!! ERROR: EntitySetPersonalityOverrideByInt - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetPersonalityOverrideByInt - pThing is null!");
        return;
    }
    EntitySetPersonalityOverrideByInt_API(m_pGameInterface, pThing, personality);
}

void LuaQuestState::EntitySetPersonalityOverrideByString(const std::shared_ptr<CScriptThing>& spThing, const std::string& personalityName) {
    if (!m_pGameInterface || !EntitySetPersonalityOverrideByString_API) {
        Log("!!! ERROR: EntitySetPersonalityOverrideByString - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetPersonalityOverrideByString - pThing is null!");
        return;
    }
    FableString fsName(personalityName.c_str());
    EntitySetPersonalityOverrideByString_API(m_pGameInterface, pThing, fsName.get());
}

void LuaQuestState::EntityClearPersonalityOverride(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityClearPersonalityOverride_API) {
        Log("!!! ERROR: EntityClearPersonalityOverride - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityClearPersonalityOverride - pThing is null!");
        return;
    }
    EntityClearPersonalityOverride_API(m_pGameInterface, pThing);
}

void LuaQuestState::EntitySetAsOpinionSourceByInt(const std::shared_ptr<CScriptThing>& spThing, int sourceID) {
    if (!m_pGameInterface || !EntitySetAsOpinionSourceByInt_API) {
        Log("!!! ERROR: EntitySetAsOpinionSourceByInt - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsOpinionSourceByInt - pThing is null!");
        return;
    }
    EntitySetAsOpinionSourceByInt_API(m_pGameInterface, pThing, sourceID);
}

void LuaQuestState::EntitySetAsOpinionSourceByString(const std::shared_ptr<CScriptThing>& spThing, const std::string& sourceName) {
    if (!m_pGameInterface || !EntitySetAsOpinionSourceByString_API) {
        Log("!!! ERROR: EntitySetAsOpinionSourceByString - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetAsOpinionSourceByString - pThing is null!");
        return;
    }
    FableString fsName(sourceName.c_str());
    EntitySetAsOpinionSourceByString_API(m_pGameInterface, pThing, fsName.get());
}

void LuaQuestState::EntityUnsetAsOpinionSource(const std::shared_ptr<CScriptThing>& spThing, sol::optional<bool> bUnknown) {
    if (!m_pGameInterface || !EntityUnsetAsOpinionSource_API) {
        Log("!!! ERROR: EntityUnsetAsOpinionSource - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityUnsetAsOpinionSource - pThing is null!");
        return;
    }
    // Default the unknown bool to 'true' as a safe guess
    bool bFinalUnknown = bUnknown.value_or(true);
    EntityUnsetAsOpinionSource_API(m_pGameInterface, pThing, bFinalUnknown);
}

void LuaQuestState::OpinionSourceSetAsExclusive(const std::shared_ptr<CScriptThing>& spSource, bool bIsExclusive) {
    if (!m_pGameInterface || !OpinionSourceSetAsExclusive_API) {
        Log("!!! ERROR: OpinionSourceSetAsExclusive - API not available!");
        return;
    }
    CScriptThing* pSource = spSource.get();
    if (!pSource) {
        Log("!!! ERROR: OpinionSourceSetAsExclusive - pSource is null!");
        return;
    }
    OpinionSourceSetAsExclusive_API(m_pGameInterface, pSource, bIsExclusive);
}

void LuaQuestState::OpinionSourceSetAsAttentionGrabbing(const std::shared_ptr<CScriptThing>& spSource, bool bIsGrabbing) {
    if (!m_pGameInterface || !OpinionSourceSetAsAttentionGrabbing_API) {
        Log("!!! ERROR: OpinionSourceSetAsAttentionGrabbing - API not available!");
        return;
    }
    CScriptThing* pSource = spSource.get();
    if (!pSource) {
        Log("!!! ERROR: OpinionSourceSetAsAttentionGrabbing - pSource is null!");
        return;
    }
    OpinionSourceSetAsAttentionGrabbing_API(m_pGameInterface, pSource, bIsGrabbing);
}

void LuaQuestState::EntityPostOpinionDeedToAll(const std::shared_ptr<CScriptThing>& spActor, int eDeed) {
    if (!m_pGameInterface || !EntityPostOpinionDeedToAll_API) {
        Log("!!! ERROR: EntityPostOpinionDeedToAll - API not available!");
        return;
    }
    CScriptThing* pActor = spActor.get();
    if (!pActor) {
        Log("!!! ERROR: EntityPostOpinionDeedToAll - pActor is null!");
        return;
    }
    EntityPostOpinionDeedToAll_API(m_pGameInterface, pActor, static_cast<EOpinionDeedType>(eDeed));
}

void LuaQuestState::EntityPostOpinionDeedToRecipient(const std::shared_ptr<CScriptThing>& spActor, int eDeed, const std::shared_ptr<CScriptThing>& spRecipient) {
    if (!m_pGameInterface || !EntityPostOpinionDeedToRecipient_API) {
        Log("!!! ERROR: EntityPostOpinionDeedToRecipient - API not available!");
        return;
    }
    CScriptThing* pActor = spActor.get();
    if (!pActor) {
        Log("!!! ERROR: EntityPostOpinionDeedToRecipient - pActor is null!");
        return;
    }
    CScriptThing* pRecipient = spRecipient.get();
    if (!pRecipient) {
        Log("!!! ERROR: EntityPostOpinionDeedToRecipient - pRecipient is null!");
        return;
    }
    EntityPostOpinionDeedToRecipient_API(m_pGameInterface, pActor, static_cast<EOpinionDeedType>(eDeed), pRecipient);
}

void LuaQuestState::EntityPostOpinionDeedToRecipientVillage(const std::shared_ptr<CScriptThing>& spActor, int eDeed, const std::shared_ptr<CScriptThing>& spVillage) {
    if (!m_pGameInterface || !EntityPostOpinionDeedToRecipientVillage_API) {
        Log("!!! ERROR: EntityPostOpinionDeedToRecipientVillage - API not available!");
        return;
    }
    CScriptThing* pActor = spActor.get();
    if (!pActor) {
        Log("!!! ERROR: EntityPostOpinionDeedToRecipientVillage - pActor is null!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: EntityPostOpinionDeedToRecipientVillage - pVillage is null!");
        return;
    }
    EntityPostOpinionDeedToRecipientVillage_API(m_pGameInterface, pActor, static_cast<EOpinionDeedType>(eDeed), pVillage);
}

int LuaQuestState::EntityPostOpinionDeedKeepSearchingForWitnesses(const std::shared_ptr<CScriptThing>& spActor, int eDeed, const std::shared_ptr<CScriptThing>& spRecipient) {
    if (!m_pGameInterface || !EntityPostOpinionDeedKeepSearchingForWitnesses_API) {
        Log("!!! ERROR: EntityPostOpinionDeedKeepSearchingForWitnesses - API not available!");
        return -1;
    }
    CScriptThing* pActor = spActor.get();
    if (!pActor) {
        Log("!!! ERROR: EntityPostOpinionDeedKeepSearchingForWitnesses - pActor is null!");
        return -1;
    }
    CScriptThing* pRecipient = spRecipient.get();
    // Note: Recipient can be null for this API call
    return EntityPostOpinionDeedKeepSearchingForWitnesses_API(m_pGameInterface, pActor, static_cast<EOpinionDeedType>(eDeed), pRecipient);
}

void LuaQuestState::RemoveOpinionDeedStillSearchingForWitnesses(const std::shared_ptr<CScriptThing>& spActor, int deedID) {
    if (!m_pGameInterface || !RemoveOpinionDeedStillSearchingForWitnesses_API) {
        Log("!!! ERROR: RemoveOpinionDeedStillSearchingForWitnesses - API not available!");
        return;
    }
    CScriptThing* pActor = spActor.get();
    if (!pActor) {
        Log("!!! ERROR: RemoveOpinionDeedStillSearchingForWitnesses - pActor is null!");
        return;
    }
    RemoveOpinionDeedStillSearchingForWitnesses_API(m_pGameInterface, pActor, deedID);
}

bool LuaQuestState::IsDeedWitnessed(int deedID) {
    if (m_pGameInterface && IsDeedWitnessed_API) {
        return IsDeedWitnessed_API(m_pGameInterface, deedID);
    }
    Log("!!! ERROR: IsDeedWitnessed - API not available!");
    return false;
}

bool LuaQuestState::CanThingBe_Seen_ByOtherThing(sol::object thing1, sol::object thing2) {
    if (!m_pGameInterface || !CanThingBe_Seen_ByOtherThing_API) {
        Log("!!! ERROR: CanThingBe_Seen_ByOtherThing - API not available!");
        return false;
    }
    // Entity scripts mix their raw `me` userdata with shared handles returned by GetHero.
    // Normalize both shapes before crossing the retail ABI, as the adjacent facing binding does.
    CScriptThing* pThing1 = GetScriptThingFromSolObject(thing1);
    CScriptThing* pThing2 = GetScriptThingFromSolObject(thing2);
    if (!pThing1) { Log("!!! ERROR: CanThingBe_Seen_ByOtherThing - pThing1 is null!"); return false; }
    if (!pThing2) { Log("!!! ERROR: CanThingBe_Seen_ByOtherThing - pThing2 is null!"); return false; }
    return CanThingBe_Seen_ByOtherThing_API(m_pGameInterface, pThing1, pThing2);
}

bool LuaQuestState::CanThingBe_NearlySeen_ByOtherThing(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    if (!m_pGameInterface || !CanThingBe_NearlySeen_ByOtherThing_API) {
        Log("!!! ERROR: CanThingBe_NearlySeen_ByOtherThing - API not available!");
        return false;
    }
    CScriptThing* pThing1 = spThing1.get();
    if (!pThing1) { Log("!!! ERROR: CanThingBe_NearlySeen_ByOtherThing - pThing1 is null!"); return false; }
    CScriptThing* pThing2 = spThing2.get();
    if (!pThing2) { Log("!!! ERROR: CanThingBe_NearlySeen_ByOtherThing - pThing2 is null!"); return false; }
    return CanThingBe_NearlySeen_ByOtherThing_API(m_pGameInterface, pThing1, pThing2);
}

bool LuaQuestState::CanThingBe_Smelled_ByOtherThing(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    if (!m_pGameInterface || !CanThingBe_Smelled_ByOtherThing_API) {
        Log("!!! ERROR: CanThingBe_Smelled_ByOtherThing - API not available!");
        return false;
    }
    CScriptThing* pThing1 = spThing1.get();
    if (!pThing1) { Log("!!! ERROR: CanThingBe_Smelled_ByOtherThing - pThing1 is null!"); return false; }
    CScriptThing* pThing2 = spThing2.get();
    if (!pThing2) { Log("!!! ERROR: CanThingBe_Smelled_ByOtherThing - pThing2 is null!"); return false; }
    return CanThingBe_Smelled_ByOtherThing_API(m_pGameInterface, pThing1, pThing2);
}

bool LuaQuestState::CanThingBe_Heard_ByOtherThing(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    if (!m_pGameInterface || !CanThingBe_Heard_ByOtherThing_API) {
        Log("!!! ERROR: CanThingBe_Heard_ByOtherThing - API not available!");
        return false;
    }
    CScriptThing* pThing1 = spThing1.get();
    if (!pThing1) { Log("!!! ERROR: CanThingBe_Heard_ByOtherThing - pThing1 is null!"); return false; }
    CScriptThing* pThing2 = spThing2.get();
    if (!pThing2) { Log("!!! ERROR: CanThingBe_Heard_ByOtherThing - pThing2 is null!"); return false; }
    return CanThingBe_Heard_ByOtherThing_API(m_pGameInterface, pThing1, pThing2);
}

bool LuaQuestState::IsThingAwareOfOtherThingInAnyWay(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    if (!m_pGameInterface || !IsThingAwareOfOtherThingInAnyWay_API) {
        Log("!!! ERROR: IsThingAwareOfOtherThingInAnyWay - API not available!");
        return false;
    }
    CScriptThing* pThing1 = spThing1.get();
    if (!pThing1) { Log("!!! ERROR: IsThingAwareOfOtherThingInAnyWay - pThing1 is null!"); return false; }
    CScriptThing* pThing2 = spThing2.get();
    if (!pThing2) { Log("!!! ERROR: IsThingAwareOfOtherThingInAnyWay - pThing2 is null!"); return false; }
    return IsThingAwareOfOtherThingInAnyWay_API(m_pGameInterface, pThing1, pThing2);
}

void LuaQuestState::EntitySetAsAwareOfThing(const std::shared_ptr<CScriptThing>& spThing1, const std::shared_ptr<CScriptThing>& spThing2) {
    if (!m_pGameInterface || !EntitySetAsAwareOfThing_API) {
        Log("!!! ERROR: EntitySetAsAwareOfThing - API not available!");
        return;
    }
    CScriptThing* pThing1 = spThing1.get();
    if (!pThing1) { Log("!!! ERROR: EntitySetAsAwareOfThing - pThing1 is null!"); return; }
    CScriptThing* pThing2 = spThing2.get();
    if (!pThing2) { Log("!!! ERROR: EntitySetAsAwareOfThing - pThing2 is null!"); return; }
    EntitySetAsAwareOfThing_API(m_pGameInterface, pThing1, pThing2);
}

void LuaQuestState::EntitySetSoundRadius(const std::shared_ptr<CScriptThing>& spThing, float radius) {
    if (!m_pGameInterface || !EntitySetSoundRadius_API) {
        Log("!!! ERROR: EntitySetSoundRadius - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetSoundRadius - pThing is null!");
        return;
    }
    EntitySetSoundRadius_API(m_pGameInterface, pThing, radius);
}

void LuaQuestState::EntitySetSmellRadius(const std::shared_ptr<CScriptThing>& spThing, float radius) {
    if (!m_pGameInterface || !EntitySetSmellRadius_API) {
        Log("!!! ERROR: EntitySetSmellRadius - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetSmellRadius - pThing is null!");
        return;
    }
    EntitySetSmellRadius_API(m_pGameInterface, pThing, radius);
}

void LuaQuestState::EntitySetSightRadius(CScriptThing* pThing, float radius) {
    if (!m_pGameInterface || !EntitySetSightRadius_API) {
        Log("!!! ERROR: EntitySetSightRadius - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: EntitySetSightRadius - pThing is null!");
        return;
    }
    EntitySetSightRadius_API(m_pGameInterface, pThing, radius);
}

void LuaQuestState::EntitySetExtendedSightRadius(const std::shared_ptr<CScriptThing>& spThing, float radius) {
    if (!m_pGameInterface || !EntitySetExtendedSightRadius_API) {
        Log("!!! ERROR: EntitySetExtendedSightRadius - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetExtendedSightRadius - pThing is null!");
        return;
    }
    EntitySetExtendedSightRadius_API(m_pGameInterface, pThing, radius);
}

void LuaQuestState::EntitySetGiveUpChaseRadius(const std::shared_ptr<CScriptThing>& spThing, float radius) {
    if (!m_pGameInterface || !EntitySetGiveUpChaseRadius_API) {
        Log("!!! ERROR: EntitySetGiveUpChaseRadius - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntitySetGiveUpChaseRadius - pThing is null!");
        return;
    }
    EntitySetGiveUpChaseRadius_API(m_pGameInterface, pThing, radius);
}

float LuaQuestState::EntityGetHearingRadius(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityGetHearingRadius_API) {
        Log("!!! ERROR: EntityGetHearingRadius - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetHearingRadius - pThing is null!");
        return 0.0f;
    }
    return EntityGetHearingRadius_API(m_pGameInterface, pThing);
}

bool LuaQuestState::ManuallyTriggerTrap(const std::shared_ptr<CScriptThing>& spTrap) {
    if (!m_pGameInterface || !ManuallyTriggerTrap_API) {
        Log("!!! ERROR: ManuallyTriggerTrap - API not available!");
        return false;
    }
    CScriptThing* pTrap = spTrap.get();
    if (!pTrap) {
        Log("!!! ERROR: ManuallyTriggerTrap - pTrap is null!");
        return false;
    }
    return ManuallyTriggerTrap_API(m_pGameInterface, pTrap);
}

bool LuaQuestState::ManuallyResetTrap(const std::shared_ptr<CScriptThing>& spTrap) {
    if (!m_pGameInterface || !ManuallyResetTrap_API) {
        Log("!!! ERROR: ManuallyResetTrap - API not available!");
        return false;
    }
    CScriptThing* pTrap = spTrap.get();
    if (!pTrap) {
        Log("!!! ERROR: ManuallyResetTrap - pTrap is null!");
        return false;
    }
    return ManuallyResetTrap_API(m_pGameInterface, pTrap);
}

void LuaQuestState::SetTimeOfDay(float time) {
    if (m_pGameInterface && SetTimeOfDay_API) {
        SetTimeOfDay_API(m_pGameInterface, time);
    }
    else {
        Log("!!! ERROR: SetTimeOfDay - API not available!");
    }
}

int LuaQuestState::GetTimeOfDay() {
    if (m_pGameInterface && GetTimeOfDay_API) {
        return GetTimeOfDay_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetTimeOfDay - API not available!");
    return -1; // Return error
}

void LuaQuestState::SetTimeAsStopped(bool bIsStopped) {
    if (m_pGameInterface && SetTimeAsStopped_API) {
        SetTimeAsStopped_API(m_pGameInterface, bIsStopped, &m_stopTimeIndex);
    }
    else {
        Log("!!! ERROR: SetTimeAsStopped - API not available!");
    }
}

void LuaQuestState::FastForwardTimeTo(float time, float duration) {
    if (m_pGameInterface && FastForwardTimeTo_API) {
        FastForwardTimeTo_API(m_pGameInterface, time, duration);
    }
    else {
        Log("!!! ERROR: FastForwardTimeTo - API not available!");
    }
}

bool LuaQuestState::IsTimeOfDayBetween(int startTime, int endTime) {
    if (m_pGameInterface && IsTimeOfDayBetween_API) {
        return IsTimeOfDayBetween_API(m_pGameInterface, startTime, endTime);
    }
    Log("!!! ERROR: IsTimeOfDayBetween - API not available!");
    return false;
}

int LuaQuestState::GetDayOfWeek() {
    if (m_pGameInterface && GetDayOfWeek_API) {
        return GetDayOfWeek_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetDayOfWeek - API not available!");
    return -1; // Return error
}

int LuaQuestState::GetDayCount() {
    if (m_pGameInterface && GetDayCount_API) {
        return GetDayCount_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetDayCount - API not available!");
    return -1; // Return error
}

int LuaQuestState::GetConstantFPS() {
    if (m_pGameInterface && GetConstantFPS_API) {
        return GetConstantFPS_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetConstantFPS - API not available!");
    return -1; // Return error
}

void LuaQuestState::TransitionToTheme(const std::string& themeName, float duration) {
    if (m_pGameInterface && TransitionToTheme_API) {
        FableString fsTheme(themeName.c_str());
        TransitionToTheme_API(m_pGameInterface, fsTheme.get(), duration);
    }
    else {
        Log("!!! ERROR: TransitionToTheme - API not available!");
    }
}

void LuaQuestState::ResetToDefaultTheme(float duration) {
    if (m_pGameInterface && ResetToDefaultTheme_API) {
        ResetToDefaultTheme_API(m_pGameInterface, duration);
    }
    else {
        Log("!!! ERROR: ResetToDefaultTheme - API not available!");
    }
}

void LuaQuestState::TransitionToThemeAllInternals(const std::string& themeName, float duration) {
    if (m_pGameInterface && TransitionToThemeAllInternals_API) {
        FableString fsTheme(themeName.c_str());
        TransitionToThemeAllInternals_API(m_pGameInterface, fsTheme.get(), duration);
    }
    else {
        Log("!!! ERROR: TransitionToThemeAllInternals - API not available!");
    }
}

void LuaQuestState::ResetToDefaultThemeAllInternals(float duration) {
    if (m_pGameInterface && ResetToDefaultThemeAllInternals_API) {
        ResetToDefaultThemeAllInternals_API(m_pGameInterface, duration);
    }
    else {
        Log("!!! ERROR: ResetToDefaultThemeAllInternals - API not available!");
    }
}

void LuaQuestState::TransitionToThemeExternals(const std::string& themeName, float duration) {
    if (m_pGameInterface && TransitionToThemeExternals_API) {
        FableString fsTheme(themeName.c_str());
        TransitionToThemeExternals_API(m_pGameInterface, fsTheme.get(), duration);
    }
    else {
        Log("!!! ERROR: TransitionToThemeExternals - API not available!");
    }
}

void LuaQuestState::ResetToDefaultThemeExternals(float duration) {
    if (m_pGameInterface && ResetToDefaultThemeExternals_API) {
        ResetToDefaultThemeExternals_API(m_pGameInterface, duration); 
    }
    else {
        Log("!!! ERROR: ResetToDefaultThemeExternals - API not available!");
    }
}

void LuaQuestState::SetEnvironmentThemeWeightAllChannels(const std::string& themeName, float weight) {
    if (m_pGameInterface && SetEnvironmentThemeWeightAllChannels_API) {
        FableString fsTheme(themeName.c_str());
        SetEnvironmentThemeWeightAllChannels_API(m_pGameInterface, fsTheme.get(), weight);
    }
    else {
        Log("!!! ERROR: SetEnvironmentThemeWeightAllChannels - API not available!");
    }
}

void LuaQuestState::SetEnvironmentThemeWeightAllInternals(const std::string& themeName, float weight) {
    if (m_pGameInterface && SetEnvironmentThemeWeightAllInternals_API) {
        FableString fsTheme(themeName.c_str());
        SetEnvironmentThemeWeightAllInternals_API(m_pGameInterface, fsTheme.get(), weight);
    }
    else {
        Log("!!! ERROR: SetEnvironmentThemeWeightAllInternals - API not available!");
    }
}

void LuaQuestState::SetEnvironmentThemeWeightExternals(const std::string& themeName, float weight) {
    if (m_pGameInterface && SetEnvironmentThemeWeightExternals_API) {
        FableString fsTheme(themeName.c_str());
        SetEnvironmentThemeWeightExternals_API(m_pGameInterface, fsTheme.get(), weight);
    }
    else {
        Log("!!! ERROR: SetEnvironmentThemeWeightExternals - API not available!");
    }
}

void LuaQuestState::SetSoundThemesAsEnabledForRegion(const std::string& regionName, bool bIsEnabled) {
    if (m_pGameInterface && SetSoundThemesAsEnabledForRegion_API) {
        FableString fsRegion(regionName.c_str());
        SetSoundThemesAsEnabledForRegion_API(m_pGameInterface, fsRegion.get(), bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetSoundThemesAsEnabledForRegion - API not available!");
    }
}

void LuaQuestState::SetAllSoundsAsMuted(bool bIsMuted) {
    if (m_pGameInterface && SetAllSoundsAsMuted_API) {
        SetAllSoundsAsMuted_API(m_pGameInterface, bIsMuted);
    }
    else {
        Log("!!! ERROR: SetAllSoundsAsMuted - API not available!");
    }
}

void* LuaQuestState::RadialBlurFadeTo_NoPos(float f1, float f2, float f3, float f4, float f5, float f6, float f7) {
    if (m_pGameInterface && RadialBlurFadeTo_NoPos_API) {
        return RadialBlurFadeTo_NoPos_API(m_pGameInterface, f1, f2, f3, f4, f5, f6, f7);
    }
    Log("!!! ERROR: RadialBlurFadeTo_NoPos - API not available!");
    return nullptr;
}

void* LuaQuestState::RadialBlurFadeTo_WithPos(float f1, sol::table v1, float f2, sol::table v2, float f3, float f4, float f5) {
    if (m_pGameInterface && RadialBlurFadeTo_WithPos_API) {
        C3DVector vec1 = { v1["x"].get_or(0.0f), v1["y"].get_or(0.0f), v1["z"].get_or(0.0f) };
        C3DVector vec2 = { v2["x"].get_or(0.0f), v2["y"].get_or(0.0f), v2["z"].get_or(0.0f) };
        return RadialBlurFadeTo_WithPos_API(m_pGameInterface, f1, vec1, f2, vec2, f3, f4, f5);
    }
    Log("!!! ERROR: RadialBlurFadeTo_WithPos - API not available!");
    return nullptr;
}

void LuaQuestState::RadialBlurFadeOut(float duration, void* pBlur) {
    if (m_pGameInterface && RadialBlurFadeOut_API) {
        if (!pBlur) {
            Log("!!! ERROR: RadialBlurFadeOut - pBlur handle is null!");
            return;
        }
        RadialBlurFadeOut_API(m_pGameInterface, duration, pBlur);
    }
    else {
        Log("!!! ERROR: RadialBlurFadeOut - API not available!");
    }
}

bool LuaQuestState::IsRadialBlurFadeActive() {
    if (m_pGameInterface && IsRadialBlurFadeActive_API) {
        return IsRadialBlurFadeActive_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsRadialBlurFadeActive - API not available!");
    return false;
}

void LuaQuestState::CancelRadialBlurFade() {
    if (m_pGameInterface && CancelRadialBlurFade_API) {
        CancelRadialBlurFade_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: CancelRadialBlurFade - API not available!");
    }
}

void LuaQuestState::RadialBlurSetCenterWorldPos(void* pBlur, sol::table pos) {
    if (m_pGameInterface && RadialBlurSetCenterWorldPos_API) {
        if (!pBlur) {
            Log("!!! ERROR: RadialBlurSetCenterWorldPos - pBlur handle is null!");
            return;
        }
        C3DVector vecPos = { pos["x"].get_or(0.0f), pos["y"].get_or(0.0f), pos["z"].get_or(0.0f) };
        RadialBlurSetCenterWorldPos_API(m_pGameInterface, pBlur, &vecPos);
    }
    else {
        Log("!!! ERROR: RadialBlurSetCenterWorldPos - API not available!");
    }
}

void* LuaQuestState::DisplacementMonochromeEffectColourFadeTo(float duration, sol::table color) {
    if (m_pGameInterface && DisplacementMonochromeEffectColourFadeTo_API) {
        CRGBFloatColour col = ParseFloatColorTable(color);
        return DisplacementMonochromeEffectColourFadeTo_API(m_pGameInterface, duration, &col);
    }
    Log("!!! ERROR: DisplacementMonochromeEffectColourFadeTo - API not available!");
    return nullptr;
}

void LuaQuestState::DisplacementMonochromeEffectColourFadeOut(float duration, void* pEffect) {
    if (m_pGameInterface && DisplacementMonochromeEffectColourFadeOut_API) {
        if (!pEffect) {
            Log("!!! ERROR: DisplacementMonochromeEffectColourFadeOut - pEffect handle is null!");
            return;
        }
        DisplacementMonochromeEffectColourFadeOut_API(m_pGameInterface, duration, pEffect);
    }
    else {
        Log("!!! ERROR: DisplacementMonochromeEffectColourFadeOut - API not available!");
    }
}

void* LuaQuestState::ScreenFilterFadeTo(float f1, float f2, float f3, float f4, float f5, sol::table color) {
    if (m_pGameInterface && ScreenFilterFadeTo_API) {
        CRGBFloatColour col = ParseFloatColorTable(color);
        // Pass nullptr for the bypass vector, as it's complex and likely unused
        return ScreenFilterFadeTo_API(m_pGameInterface, f1, f2, f3, f4, f5, &col, nullptr);
    }
    Log("!!! ERROR: ScreenFilterFadeTo - API not available!");
    return nullptr;
}

void LuaQuestState::ScreenFilterFadeOut(float duration, void* pFilter) {
    if (m_pGameInterface && ScreenFilterFadeOut_API) {
        if (!pFilter) {
            Log("!!! ERROR: ScreenFilterFadeOut - pFilter handle is null!");
            return;
        }
        ScreenFilterFadeOut_API(m_pGameInterface, duration, pFilter);
    }
    else {
        Log("!!! ERROR: ScreenFilterFadeOut - API not available!");
    }
}

void LuaQuestState::SetThingAndCarriedItemsNotAffectedByScreenFilter(const std::shared_ptr<CScriptThing>& spThing, void* pFilter) {
    if (!m_pGameInterface || !SetThingAndCarriedItemsNotAffectedByScreenFilter_API) {
        Log("!!! ERROR: SetThingAndCarriedItemsNotAffectedByScreenFilter - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetThingAndCarriedItemsNotAffectedByScreenFilter - pThing is null!");
        return;
    }
    if (!pFilter) {
        Log("!!! ERROR: SetThingAndCarriedItemsNotAffectedByScreenFilter - pFilter handle is null!");
        return;
    }
    SetThingAndCarriedItemsNotAffectedByScreenFilter_API(m_pGameInterface, pThing, pFilter);
}

void LuaQuestState::UnSetThingAndCarriedItemsNotAffectedByScreenFilter(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !UnSetThingAndCarriedItemsNotAffectedByScreenFilter_API) {
        Log("!!! ERROR: UnSetThingAndCarriedItemsNotAffectedByScreenFilter - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: UnSetThingAndCarriedItemsNotAffectedByScreenFilter - pThing is null!");
        return;
    }
    UnSetThingAndCarriedItemsNotAffectedByScreenFilter_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsGiftRomantic(const std::string& giftName) {
    if (m_pGameInterface && IsGiftRomantic_API) {
        FableString fsGift(giftName.c_str());
        return IsGiftRomantic_API(m_pGameInterface, fsGift.get());
    }
    Log("!!! ERROR: IsGiftRomantic - API not available!");
    return false;
}

bool LuaQuestState::IsGiftFriendly(const std::string& giftName) {
    if (m_pGameInterface && IsGiftFriendly_API) {
        FableString fsGift(giftName.c_str());
        return IsGiftFriendly_API(m_pGameInterface, fsGift.get());
    }
    Log("!!! ERROR: IsGiftFriendly - API not available!");
    return false;
}

bool LuaQuestState::IsGiftOffensive(const std::string& giftName) {
    if (m_pGameInterface && IsGiftOffensive_API) {
        FableString fsGift(giftName.c_str());
        return IsGiftOffensive_API(m_pGameInterface, fsGift.get());
    }
    Log("!!! ERROR: IsGiftOffensive - API not available!");
    return false;
}

bool LuaQuestState::IsThingABed(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsThingABed_API) {
        Log("!!! ERROR: IsThingABed - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsThingABed - pThing is null!");
        return false;
    }
    return IsThingABed_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsThingAChest(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsThingAChest_API) {
        Log("!!! ERROR: IsThingAChest - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsThingAChest - pThing is null!");
        return false;
    }
    return IsThingAChest_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsThingADoor(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsThingADoor_API) {
        Log("!!! ERROR: IsThingADoor - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsThingADoor - pThing is null!");
        return false;
    }
    return IsThingADoor_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsThingSmashable(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsThingSmashable_API) {
        Log("!!! ERROR: IsThingSmashable - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsThingSmashable - pThing is null!");
        return false;
    }
    return IsThingSmashable_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsThingSearchable(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsThingSearchable_API) {
        Log("!!! ERROR: IsThingSearchable - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsThingSearchable - pThing is null!");
        return false;
    }
    return IsThingSearchable_API(m_pGameInterface, pThing);
}

void LuaQuestState::ApplyScriptBrush(const std::string& brushName) {
    if (m_pGameInterface && ApplyScriptBrush_API) {
        FableString fsBrush(brushName.c_str());
        ApplyScriptBrush_API(m_pGameInterface, fsBrush.get());
    }
    else {
        Log("!!! ERROR: ApplyScriptBrush - API not available!");
    }
}

void LuaQuestState::EnableDecals(bool bIsEnabled) {
    if (m_pGameInterface && EnableDecals_API) {
        EnableDecals_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: EnableDecals - API not available!");
    }
}

unsigned int LuaQuestState::PlayCriteriaSoundOnThing(const std::shared_ptr<CScriptThing>& spThing, const std::string& soundName) {
    if (!m_pGameInterface || !PlayCriteriaSoundOnThing_API) {
        Log("!!! ERROR: PlayCriteriaSoundOnThing - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: PlayCriteriaSoundOnThing - pThing is null!");
        return 0;
    }
    FableString fsSound(soundName.c_str());
    return PlayCriteriaSoundOnThing_API(m_pGameInterface, pThing, fsSound.get());
}

bool LuaQuestState::IsSoundPlaying(unsigned int soundID) {
    if (m_pGameInterface && IsSoundPlaying_API) {
        return IsSoundPlaying_API(m_pGameInterface, soundID);
    }
    Log("!!! ERROR: IsSoundPlaying - API not available!");
    return false;
}

void LuaQuestState::EnableSounds(bool bIsEnabled) {
    if (m_pGameInterface && EnableSounds_API) {
        EnableSounds_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: EnableSounds - API not available!");
    }
}

void LuaQuestState::StopSound(unsigned int soundID) {
    if (m_pGameInterface && StopSound_API) {
        StopSound_API(m_pGameInterface, soundID);
    }
    else {
        Log("!!! ERROR: StopSound - API not available!");
    }
}

void LuaQuestState::CacheMusicSet(int eType) {
    if (m_pGameInterface && CacheMusicSet_API) {
        // We assume EMusicSetType is an enum that can be cast from an int
        CacheMusicSet_API(m_pGameInterface, static_cast<EMusicSetType>(eType));
    }
    else {
        Log("!!! ERROR: CacheMusicSet - API not available!");
    }
}

void LuaQuestState::EnableDangerMusic(bool bIsEnabled) {
    if (m_pGameInterface && EnableDangerMusic_API) {
        EnableDangerMusic_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: EnableDangerMusic - API not available!");
    }
}

bool LuaQuestState::IsDangerMusicEnabled() {
    if (m_pGameInterface && IsDangerMusicEnabled_API) {
        return IsDangerMusicEnabled_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsDangerMusicEnabled - API not available!");
    return false; // Default to false
}

void LuaQuestState::StartCountdownTimer(float time) {
    if (m_pGameInterface && StartCountdownTimer_API) {
        StartCountdownTimer_API(m_pGameInterface, time);
    }
    else {
        Log("!!! ERROR: StartCountdownTimer - API not available!");
    }
}

float LuaQuestState::GetCountdownTimer() {
    if (m_pGameInterface && GetCountdownTimer_API) {
        return GetCountdownTimer_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetCountdownTimer - API not available!");
    return 0.0f;
}

void LuaQuestState::AutoSaveCheckPoint() {
    if (m_pGameInterface && AutoSaveCheckPoint_API) {
        AutoSaveCheckPoint_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: AutoSaveCheckPoint - API not available!");
    }
}

void LuaQuestState::AutoSaveQuestStart() {
    if (m_pGameInterface && AutoSaveQuestStart_API) {
        AutoSaveQuestStart_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: AutoSaveQuestStart - API not available!");
    }
}

void LuaQuestState::AutoSave() {
    if (m_pGameInterface && AutoSave_API) {
        AutoSave_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: AutoSave - API not available!");
    }
}

void LuaQuestState::SetSavingAsEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetSavingAsEnabled_API) {
        SetSavingAsEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetSavingAsEnabled - API not available!");
    }
}

bool LuaQuestState::IsSavingEnabled() {
    if (m_pGameInterface && IsSavingEnabled_API) {
        return IsSavingEnabled_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsSavingEnabled - API not available!");
    return false; // Default to false
}

void LuaQuestState::SetSaveGameMarkerPos(sol::table pos) {
    if (m_pGameInterface && SetSaveGameMarkerPos_API) {
        C3DVector vecPos = {
            pos["x"].get_or(0.0f),
            pos["y"].get_or(0.0f),
            pos["z"].get_or(0.0f)
        };
        SetSaveGameMarkerPos_API(m_pGameInterface, &vecPos);
    }
    else {
        Log("!!! ERROR: SetSaveGameMarkerPos - API not available!");
    }
}

void LuaQuestState::ResetToFrontEnd() {
    if (m_pGameInterface && ResetToFrontEnd_API) {
        ResetToFrontEnd_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ResetToFrontEnd - API not available!");
    }
}

void LuaQuestState::SetGuildSealRecallLocation(sol::table pos, float angle) {
    if (m_pGameInterface && SetGuildSealRecallLocation_API) {
        C3DVector vecPos = {
            pos["x"].get_or(0.0f),
            pos["y"].get_or(0.0f),
            pos["z"].get_or(0.0f)
        };
        SetGuildSealRecallLocation_API(m_pGameInterface, &vecPos, angle);
    }
    else {
        Log("!!! ERROR: SetGuildSealRecallLocation - API not available!");
    }
}

sol::table LuaQuestState::GetGuildSealRecallPos(sol::this_state s) {
    sol::state_view lua(s);
    sol::table posTable = lua.create_table();

    if (m_pGameInterface && GetGuildSealRecallPos_API) {
        C3DVector outPos = {}; // Initialize
        GetGuildSealRecallPos_API(m_pGameInterface, &outPos);
        posTable["x"] = outPos.x;
        posTable["y"] = outPos.y;
        posTable["z"] = outPos.z;
    }
    else {
        Log("!!! ERROR: GetGuildSealRecallPos - API not available!");
    }
    return posTable;
}

float LuaQuestState::GetGuildSealRecallAngleXY() {
    if (m_pGameInterface && GetGuildSealRecallAngleXY_API) {
        return GetGuildSealRecallAngleXY_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetGuildSealRecallAngleXY - API not available!");
    return 0.0f;
}

void LuaQuestState::SetReadableObjectText(const std::shared_ptr<CScriptThing>& spThing, const std::string& text) {
    if (!m_pGameInterface || !SetReadableObjectText_API || !g_fableBase) {
        Log("!!! ERROR: SetReadableObjectText - API or game base not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetReadableObjectText - pThing is null!");
        return;
    }

    std::wstring converted;
    if (!Utf8ToWide(text, converted)) {
        Log("!!! ERROR: SetReadableObjectText - argument is not valid UTF-8.");
        return;
    }
    RetailWideString retailText;
    if (!retailText.Construct(converted, ASLR<tRetailWideStringCtor>(0x0099B6B0),
                              ASLR<tRetailWideStringDtor>(0x0099B510))) {
        Log("!!! ERROR: SetReadableObjectText - retail CWideString construction failed.");
        return;
    }
    SetReadableObjectText_API(m_pGameInterface, pThing, retailText.Get());
}

void LuaQuestState::SetReadableObjectTextTag(const std::shared_ptr<CScriptThing>& spThing, const std::string& textTag) {
    if (!m_pGameInterface || !SetReadableObjectTextTag_API) {
        Log("!!! ERROR: SetReadableObjectTextTag - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetReadableObjectTextTag - pThing is null!");
        return;
    }
    FableString fsTag(textTag.c_str());
    SetReadableObjectTextTag_API(m_pGameInterface, pThing, fsTag.get());
}

void LuaQuestState::RemoveRumourCategory(const std::string& categoryName) {
    if (m_pGameInterface && RemoveRumourCategory_API) {
        FableString fsCat(categoryName.c_str());
        RemoveRumourCategory_API(m_pGameInterface, fsCat.get());
    }
    else {
        Log("!!! ERROR: RemoveRumourCategory - API not available!");
    }
}

void LuaQuestState::SetCategoryActivity(const std::string& categoryName, bool bIsActive) {
    if (m_pGameInterface && SetCategoryActivity_API) {
        FableString fsCat(categoryName.c_str());
        SetCategoryActivity_API(m_pGameInterface, fsCat.get(), bIsActive);
    }
    else {
        Log("!!! ERROR: SetCategoryActivity - API not available!");
    }
}

void LuaQuestState::AddGossipVillage(const std::string& categoryName, const std::string& villageName) {
    if (m_pGameInterface && AddGossipVillage_API) {
        FableString fsCat(categoryName.c_str());
        FableString fsVill(villageName.c_str());
        AddGossipVillage_API(m_pGameInterface, fsCat.get(), fsVill.get());
    }
    else {
        Log("!!! ERROR: AddGossipVillage - API not available!");
    }
}

void LuaQuestState::SetIsGossipForPlayer_ByObject(const std::string& categoryName, bool bIsForPlayer) {
    if (m_pGameInterface && SetIsGossipForPlayer_ByObject_API) {
        FableString fsCat(categoryName.c_str());
        // This API (tSetIsGossipForPlayer1) takes CCharString by value
        SetIsGossipForPlayer_ByObject_API(m_pGameInterface, fsCat.m_charString, bIsForPlayer);
    }
    else {
        Log("!!! ERROR: SetIsGossipForPlayer_ByObject - API not available!");
    }
}

void LuaQuestState::SetIsGossipForPlayer_ByName(const std::string& categoryName, bool bIsForPlayer) {
    if (m_pGameInterface && SetIsGossipForPlayer_ByName_API) {
        FableString fsCat(categoryName.c_str());
        // This API (tSetIsGossipForPlayer2) takes CCharString by pointer
        SetIsGossipForPlayer_ByName_API(m_pGameInterface, fsCat.get(), bIsForPlayer);
    }
    else {
        Log("!!! ERROR: SetIsGossipForPlayer_ByName - API not available!");
    }
}

float LuaQuestState::GetBestTimePairs() {
    if (m_pGameInterface && GetBestTimePairs_API) {
        return GetBestTimePairs_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBestTimePairs - API not available!");
    return -1.0f; // Return error value
}

float LuaQuestState::GetBestTimeSorting() {
    if (m_pGameInterface && GetBestTimeSorting_API) {
        return GetBestTimeSorting_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBestTimeSorting - API not available!");
    return -1.0f; // Return error value
}

int LuaQuestState::GetBestScoreBlackjack() {
    if (m_pGameInterface && GetBestScoreBlackjack_API) {
        return GetBestScoreBlackjack_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBestScoreBlackjack - API not available!");
    return -1; // Return error value
}

int LuaQuestState::GetBestScoreCoinGolfOakVale() {
    if (m_pGameInterface && GetBestScoreCoinGolfOakVale_API) {
        return GetBestScoreCoinGolfOakVale_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBestScoreCoinGolfOakVale - API not available!");
    return -1; // Return error value
}

int LuaQuestState::GetBestScoreCoinGolfSnowSpire() {
    if (m_pGameInterface && GetBestScoreCoinGolfSnowSpire_API) {
        return GetBestScoreCoinGolfSnowSpire_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBestScoreCoinGolfSnowSpire - API not available!");
    return -1; // Return error value
}

int LuaQuestState::GetBestScoreShoveHaPenny() {
    if (m_pGameInterface && GetBestScoreShoveHaPenny_API) {
        return GetBestScoreShoveHaPenny_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBestScoreShoveHaPenny - API not available!");
    return -1; // Return error value
}

float LuaQuestState::GetBestTimeGuessTheAddition() {
    if (m_pGameInterface && GetBestTimeGuessTheAddition_API) {
        return GetBestTimeGuessTheAddition_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBestTimeGuessTheAddition - API not available!");
    return -1.0f; // Return error value
}

bool LuaQuestState::IsHeroInTavernGame() {
    if (m_pGameInterface && IsHeroInTavernGame_API) {
        return IsHeroInTavernGame_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsHeroInTavernGame - API not available!");
    return false;
}

int LuaQuestState::GetNumHousesOwned() {
    if (m_pGameInterface && GetNumHousesOwned_API) {
        return GetNumHousesOwned_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetNumHousesOwned - API not available!");
    return 0;
}

void LuaQuestState::StartSneaking() {
    if (m_pGameInterface && StartSneaking_API) {
        StartSneaking_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: StartSneaking - API not available!");
    }
}

int LuaQuestState::GetStealDuration(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetStealDuration_API) {
        Log("!!! ERROR: GetStealDuration - API not available!");
        return -1;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetStealDuration - pThing is null!");
        return -1;
    }
    return GetStealDuration_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetUseableByHero(const std::shared_ptr<CScriptThing>& spThing, bool bIsUseable) {
    if (!m_pGameInterface || !SetUseableByHero_API) {
        Log("!!! ERROR: SetUseableByHero - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetUseableByHero - pThing is null!");
        return;
    }
    SetUseableByHero_API(m_pGameInterface, pThing, bIsUseable);
}

void LuaQuestState::SetOwnedByHero(const std::shared_ptr<CScriptThing>& spThing, bool bIsOwned) {
    if (!m_pGameInterface || !SetOwnedByHero_API) {
        Log("!!! ERROR: SetOwnedByHero - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetOwnedByHero - pThing is null!");
        return;
    }
    SetOwnedByHero_API(m_pGameInterface, pThing, bIsOwned);
}

void LuaQuestState::SetTavernTableAvailableForUse(const std::shared_ptr<CScriptThing>& spTable, bool bIsAvailable) {
    if (!m_pGameInterface || !SetTavernTableAvailableForUse_API) {
        Log("!!! ERROR: SetTavernTableAvailableForUse - API not available!");
        return;
    }
    CScriptThing* pTable = spTable.get();
    if (!pTable) {
        Log("!!! ERROR: SetTavernTableAvailableForUse - pTable is null!");
        return;
    }
    SetTavernTableAvailableForUse_API(m_pGameInterface, pTable, bIsAvailable);
}

void LuaQuestState::SetIsThingTurncoatable(const std::shared_ptr<CScriptThing>& spThing, bool bIsTurncoatable) {
    if (!m_pGameInterface || !SetIsThingTurncoatable_API) {
        Log("!!! ERROR: SetIsThingTurncoatable - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetIsThingTurncoatable - pThing is null!");
        return;
    }
    SetIsThingTurncoatable_API(m_pGameInterface, pThing, bIsTurncoatable);
}

void LuaQuestState::SetIsThingForcePushable(const std::shared_ptr<CScriptThing>& spThing, bool bIsPushable) {
    if (!m_pGameInterface || !SetIsThingForcePushable_API) {
        Log("!!! ERROR: SetIsThingForcePushable - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetIsThingForcePushable - pThing is null!");
        return;
    }
    SetIsThingForcePushable_API(m_pGameInterface, pThing, bIsPushable);
}

void LuaQuestState::SetIsThingLightningable(const std::shared_ptr<CScriptThing>& spThing, bool bIsLightningable) {
    if (!m_pGameInterface || !SetIsThingLightningable_API) {
        Log("!!! ERROR: SetIsThingLightningable - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetIsThingLightningable - pThing is null!");
        return;
    }
    SetIsThingLightningable_API(m_pGameInterface, pThing, bIsLightningable);
}

void LuaQuestState::SetIsThingEpicSpellable(const std::shared_ptr<CScriptThing>& spThing, bool bIsSpellable) {
    if (!m_pGameInterface || !SetIsThingEpicSpellable_API) {
        Log("!!! ERROR: SetIsThingEpicSpellable - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetIsThingEpicSpellable - pThing is null!");
        return;
    }
    SetIsThingEpicSpellable_API(m_pGameInterface, pThing, bIsSpellable);
}

bool LuaQuestState::IsThingTurncoated(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsThingTurncoated_API) {
        Log("!!! ERROR: IsThingTurncoated - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsThingTurncoated - pThing is null!");
        return false;
    }
    return IsThingTurncoated_API(m_pGameInterface, pThing);
}

void LuaQuestState::AddCreatureScriptedMode(const std::shared_ptr<CScriptThing>& spCreature, const std::string& mode) {
    CScriptThing* pCreature = spCreature.get();
    if (!m_pGameInterface || !AddCreatureScriptedMode_API) {
        Log("!!! ERROR: AddCreatureScriptedMode - API not available!");
        return;
    }
    if (!pCreature) {
        Log("!!! ERROR: AddCreatureScriptedMode - pCreature is null!");
        return;
    }
    FableString fsMode(mode.c_str());
    AddCreatureScriptedMode_API(m_pGameInterface, pCreature, fsMode.get());
}

void LuaQuestState::RemoveCreatureScriptedMode(const std::shared_ptr<CScriptThing>& spCreature) {
    if (!m_pGameInterface || !RemoveCreatureScriptedMode_API) {
        Log("!!! ERROR: RemoveCreatureScriptedMode - API not available!");
        return;
    }
    CScriptThing* pCreature = spCreature.get();
    if (!pCreature) {
        Log("!!! ERROR: RemoveCreatureScriptedMode - pCreature is null!");
        return;
    }
    RemoveCreatureScriptedMode_API(m_pGameInterface, pCreature);
}

void LuaQuestState::ForceShipsVisible() {
    if (m_pGameInterface && ForceShipsVisible_API) {
        ForceShipsVisible_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ForceShipsVisible - API not available!");
    }
}

sol::table LuaQuestState::GetSleepingPositionAndOrientationFromBed(const std::shared_ptr<CScriptThing>& spBed, const std::shared_ptr<CScriptThing>& spSleeper, sol::this_state s) {
    sol::state_view lua(s);
    if (!m_pGameInterface || !GetSleepingPositionAndOrientationFromBed_API) {
        Log("!!! ERROR: GetSleepingPositionAndOrientationFromBed - API not available!");
        return sol::make_object(lua, sol::nil);
    }
    CScriptThing* pBed = spBed.get();
    if (!pBed) {
        Log("!!! ERROR: GetSleepingPositionAndOrientationFromBed - pBed is null!");
        return sol::make_object(lua, sol::nil);
    }
    CScriptThing* pSleeper = spSleeper.get();
    if (!pSleeper) {
        Log("!!! ERROR: GetSleepingPositionAndOrientationFromBed - pSleeper is null!");
        return sol::make_object(lua, sol::nil);
    }

    C3DVector outPos = {};
    C3DVector outOrient = {};
    bool result = GetSleepingPositionAndOrientationFromBed_API(m_pGameInterface, pBed, pSleeper, &outPos, &outOrient);

    if (result) {
        sol::table retTable = lua.create_table();
        sol::table posTable = lua.create_table_with("x", outPos.x, "y", outPos.y, "z", outPos.z);
        sol::table orientTable = lua.create_table_with("x", outOrient.x, "y", outOrient.y, "z", outOrient.z);
        retTable["pos"] = posTable;
        retTable["orient"] = orientTable;
        return retTable;
    }

    return sol::make_object(lua, sol::nil); // API returned false
}

void LuaQuestState::SetBedAvailability(const std::shared_ptr<CScriptThing>& spBed, bool bIsAvailable) {
    if (!m_pGameInterface || !SetBedAvailability_API) {
        Log("!!! ERROR: SetBedAvailability - API not available!");
        return;
    }
    CScriptThing* pBed = spBed.get();
    if (!pBed) {
        Log("!!! ERROR: SetBedAvailability - pBed is null!");
        return;
    }
    SetBedAvailability_API(m_pGameInterface, pBed, bIsAvailable);
}

void LuaQuestState::RepopulateVillage(const std::shared_ptr<CScriptThing>& spVillage) {
    if (!m_pGameInterface || !RepopulateVillage_API) {
        Log("!!! ERROR: RepopulateVillage - API not available!");
        return;
    }
    CScriptThing* pVillage = spVillage.get();
    if (!pVillage) {
        Log("!!! ERROR: RepopulateVillage - pVillage is null!");
        return;
    }
    RepopulateVillage_API(m_pGameInterface, pVillage);
}

void LuaQuestState::SmashAllWindowsWithinRadiusOfPoint(sol::table pos, float radius) {
    if (m_pGameInterface && SmashAllWindowsWithinRadiusOfPoint_API) {
        C3DVector vecPos = {
            pos["x"].get_or(0.0f),
            pos["y"].get_or(0.0f),
            pos["z"].get_or(0.0f)
        };
        SmashAllWindowsWithinRadiusOfPoint_API(m_pGameInterface, &vecPos, radius);
    }
    else {
        Log("!!! ERROR: SmashAllWindowsWithinRadiusOfPoint - API not available!");
    }
}

std::shared_ptr<CScriptThing> LuaQuestState::SetResidency(const std::shared_ptr<CScriptThing>& spThing, bool bIsResident) {
    if (!m_pGameInterface || !SetResidency_API || !Game_malloc) {
        Log("!!! ERROR: SetResidency - API or memory allocator not available!");
        return nullptr;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetResidency - pThing is null!");
        return nullptr;
    }

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: SetResidency - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    SetResidency_API(m_pGameInterface, pResult, pThing, bIsResident);

    return WrapScriptThingOutput(pResult);
}

void LuaQuestState::SetThankingPhrase(const std::shared_ptr<CScriptThing>& spThing, unsigned int phraseID) {
    if (!m_pGameInterface || !SetThankingPhrase_API) {
        Log("!!! ERROR: SetThankingPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetThankingPhrase - pThing is null!");
        return;
    }
    SetThankingPhrase_API(m_pGameInterface, pThing, phraseID);
}

unsigned int LuaQuestState::GetThankingPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetThankingPhrase_API) {
        Log("!!! ERROR: GetThankingPhrase - API not available!");
        return 0; // Return 0 as error/default
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetThankingPhrase - pThing is null!");
        return 0;
    }
    return GetThankingPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetThankingPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetThankingPhrase_API) {
        Log("!!! ERROR: ResetThankingPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetThankingPhrase - pThing is null!");
        return;
    }
    ResetThankingPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetIgnoringPhrase(const std::shared_ptr<CScriptThing>& spThing, unsigned int phraseID) {
    if (!m_pGameInterface || !SetIgnoringPhrase_API) {
        Log("!!! ERROR: SetIgnoringPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetIgnoringPhrase - pThing is null!");
        return;
    }
    SetIgnoringPhrase_API(m_pGameInterface, pThing, phraseID);
}

unsigned int LuaQuestState::GetIgnoringPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetIgnoringPhrase_API) {
        Log("!!! ERROR: GetIgnoringPhrase - API not available!");
        return 0; // Return 0 as error/default
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetIgnoringPhrase - pThing is null!");
        return 0;
    }
    return GetIgnoringPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetIgnoringPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetIgnoringPhrase_API) {
        Log("!!! ERROR: ResetIgnoringPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetIgnoringPhrase - pThing is null!");
        return;
    }
    ResetIgnoringPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetWanderCentrePoint(CScriptThing* pThing, sol::table pos) {
    if (!m_pGameInterface || !SetWanderCentrePoint_API) {
        Log("!!! ERROR: SetWanderCentrePoint - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: SetWanderCentrePoint - pThing is null!");
        return;
    }
    C3DVector vecPos = {
        pos["x"].get_or(0.0f),
        pos["y"].get_or(0.0f),
        pos["z"].get_or(0.0f)
    };
    CScriptThing retainedCopy = *pThing;
    if (retainedCopy.pImp.Info) retainedCopy.pImp.Info->RefCount++;
    SetWanderCentrePoint_API(m_pGameInterface, retainedCopy, vecPos);
}

sol::table LuaQuestState::GetWanderCentrePoint(const std::shared_ptr<CScriptThing>& spThing, sol::this_state s) {
    sol::state_view lua(s);
    sol::table posTable = lua.create_table();
    if (!m_pGameInterface || !GetWanderCentrePoint_API) {
        Log("!!! ERROR: GetWanderCentrePoint - API not available!");
        return posTable;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetWanderCentrePoint - pThing is null!");
        return posTable;
    }

    C3DVector outPos = {};
    GetWanderCentrePoint_API(m_pGameInterface, &outPos, pThing);
    posTable["x"] = outPos.x;
    posTable["y"] = outPos.y;
    posTable["z"] = outPos.z;
    return posTable;
}

void LuaQuestState::ResetWanderCentrePoint(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetWanderCentrePoint_API) {
        Log("!!! ERROR: ResetWanderCentrePoint - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetWanderCentrePoint - pThing is null!");
        return;
    }
    ResetWanderCentrePoint_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetWanderMinDistance(CScriptThing* pThing, float distance) {
    if (!m_pGameInterface || !SetWanderMinDistance_API) {
        Log("!!! ERROR: SetWanderMinDistance - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: SetWanderMinDistance - pThing is null!");
        return;
    }
    CScriptThing retainedCopy = *pThing;
    if (retainedCopy.pImp.Info) retainedCopy.pImp.Info->RefCount++;
    SetWanderMinDistance_API(m_pGameInterface, retainedCopy, distance);
}

float LuaQuestState::GetWanderMinDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetWanderMinDistance_API) {
        Log("!!! ERROR: GetWanderMinDistance - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetWanderMinDistance - pThing is null!");
        return 0.0f;
    }
    return GetWanderMinDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetWanderMinDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetWanderMinDistance_API) {
        Log("!!! ERROR: ResetWanderMinDistance - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetWanderMinDistance - pThing is null!");
        return;
    }
    ResetWanderMinDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetWanderMaxDistance(CScriptThing* pThing, float distance) {
    if (!m_pGameInterface || !SetWanderMaxDistance_API) {
        Log("!!! ERROR: SetWanderMaxDistance - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: SetWanderMaxDistance - pThing is null!");
        return;
    }
    CScriptThing retainedCopy = *pThing;
    if (retainedCopy.pImp.Info) retainedCopy.pImp.Info->RefCount++;
    SetWanderMaxDistance_API(m_pGameInterface, retainedCopy, distance);
}

float LuaQuestState::GetWanderMaxDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetWanderMaxDistance_API) {
        Log("!!! ERROR: GetWanderMaxDistance - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetWanderMaxDistance - pThing is null!");
        return 0.0f;
    }
    return GetWanderMaxDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetWanderMaxDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetWanderMaxDistance_API) {
        Log("!!! ERROR: ResetWanderMaxDistance - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetWanderMaxDistance - pThing is null!");
        return;
    }
    ResetWanderMaxDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetGossipCounter(const std::shared_ptr<CScriptThing>& spThing, int count) {
    if (!m_pGameInterface || !SetGossipCounter_API) {
        Log("!!! ERROR: SetGossipCounter - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetGossipCounter - pThing is null!");
        return;
    }
    SetGossipCounter_API(m_pGameInterface, pThing, count);
}

int LuaQuestState::GetGossipCounter(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetGossipCounter_API) {
        Log("!!! ERROR: GetGossipCounter - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetGossipCounter - pThing is null!");
        return 0;
    }
    return GetGossipCounter_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetGossipCounter(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetGossipCounter_API) {
        Log("!!! ERROR: ResetGossipCounter - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetGossipCounter - pThing is null!");
        return;
    }
    ResetGossipCounter_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetMaxGossipPhrase(const std::shared_ptr<CScriptThing>& spThing, int max) {
    if (!m_pGameInterface || !SetMaxGossipPhrase_API) {
        Log("!!! ERROR: SetMaxGossipPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetMaxGossipPhrase - pThing is null!");
        return;
    }
    SetMaxGossipPhrase_API(m_pGameInterface, pThing, max);
}

int LuaQuestState::GetMaxGossipPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetMaxGossipPhrase_API) {
        Log("!!! ERROR: GetMaxGossipPhrase - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetMaxGossipPhrase - pThing is null!");
        return 0;
    }
    return GetMaxGossipPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetMaxGossipPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetMaxGossipPhrase_API) {
        Log("!!! ERROR: ResetMaxGossipPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetMaxGossipPhrase - pThing is null!");
        return;
    }
    ResetMaxGossipPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetWarningPhrase(const std::shared_ptr<CScriptThing>& spThing, unsigned int phraseID) {
    if (!m_pGameInterface || !SetWarningPhrase_API) {
        Log("!!! ERROR: SetWarningPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetWarningPhrase - pThing is null!");
        return;
    }
    SetWarningPhrase_API(m_pGameInterface, pThing, phraseID);
}

unsigned int LuaQuestState::GetWarningPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetWarningPhrase_API) {
        Log("!!! ERROR: GetWarningPhrase - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetWarningPhrase - pThing is null!");
        return 0;
    }
    return GetWarningPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetWarningPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetWarningPhrase_API) {
        Log("!!! ERROR: ResetWarningPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetWarningPhrase - pThing is null!");
        return;
    }
    ResetWarningPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetBeerRequestPhrase(const std::shared_ptr<CScriptThing>& spThing, unsigned int phraseID) {
    if (!m_pGameInterface || !SetBeerRequestPhrase_API) {
        Log("!!! ERROR: SetBeerRequestPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetBeerRequestPhrase - pThing is null!");
        return;
    }
    SetBeerRequestPhrase_API(m_pGameInterface, pThing, phraseID);
}

unsigned int LuaQuestState::GetBeerRequestPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetBeerRequestPhrase_API) {
        Log("!!! ERROR: GetBeerRequestPhrase - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetBeerRequestPhrase - pThing is null!");
        return 0;
    }
    return GetBeerRequestPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetBeerRequestPhrase(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetBeerRequestPhrase_API) {
        Log("!!! ERROR: ResetBeerRequestPhrase - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetBeerRequestPhrase - pThing is null!");
        return;
    }
    ResetBeerRequestPhrase_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetScriptingStateGroup(CScriptThing* pThing, int eGroup) {
    if (!m_pGameInterface || !SetScriptingStateGroup_API) {
        Log("!!! ERROR: SetScriptingStateGroup - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: SetScriptingStateGroup - pThing is null!");
        return;
    }
    // Cast the int from Lua to the enum
    CScriptThing retainedCopy = *pThing;
    if (retainedCopy.pImp.Info) retainedCopy.pImp.Info->RefCount++;
    SetScriptingStateGroup_API(m_pGameInterface, retainedCopy, static_cast<EScriptingStateGroups>(eGroup));
}

int LuaQuestState::GetScriptingStateGroup(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetScriptingStateGroup_API) {
        Log("!!! ERROR: GetScriptingStateGroup - API not available!");
        return 0; // Return a default/null enum value
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetScriptingStateGroup - pThing is null!");
        return 0;
    }
    // Cast the enum result to an int for Lua
    return static_cast<int>(GetScriptingStateGroup_API(m_pGameInterface, pThing));
}

void LuaQuestState::ResetScriptingStateGroup(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetScriptingStateGroup_API) {
        Log("!!! ERROR: ResetScriptingStateGroup - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetScriptingStateGroup - pThing is null!");
        return;
    }
    ResetScriptingStateGroup_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetMaxHeroReactionDistance(const std::shared_ptr<CScriptThing>& spThing, float distance) {
    if (!m_pGameInterface || !SetMaxHeroReactionDistance_API) {
        Log("!!! ERROR: SetMaxHeroReactionDistance - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetMaxHeroReactionDistance - pThing is null!");
        return;
    }
    SetMaxHeroReactionDistance_API(m_pGameInterface, pThing, distance);
}

float LuaQuestState::GetMaxHeroReactionDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetMaxHeroReactionDistance_API) {
        Log("!!! ERROR: GetMaxHeroReactionDistance - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetMaxHeroReactionDistance - pThing is null!");
        return 0.0f;
    }
    return GetMaxHeroReactionDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetMaxHeroReactionDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetMaxHeroReactionDistance_API) {
        Log("!!! ERROR: ResetMaxHeroReactionDistance - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetMaxHeroReactionDistance - pThing is null!");
        return;
    }
    ResetMaxHeroReactionDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetActionFrequency(const std::shared_ptr<CScriptThing>& spThing, int frequency) {
    if (!m_pGameInterface || !SetActionFrequency_API) {
        Log("!!! ERROR: SetActionFrequency - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetActionFrequency - pThing is null!");
        return;
    }
    SetActionFrequency_API(m_pGameInterface, pThing, frequency);
}

int LuaQuestState::GetActionFrequency(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetActionFrequency_API) {
        Log("!!! ERROR: GetActionFrequency - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetActionFrequency - pThing is null!");
        return 0;
    }
    return GetActionFrequency_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetActionFrequency(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetActionFrequency_API) {
        Log("!!! ERROR: ResetActionFrequency - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetActionFrequency - pThing is null!");
        return;
    }
    ResetActionFrequency_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetActionFrequencyVariation(const std::shared_ptr<CScriptThing>& spThing, float variation) {
    if (!m_pGameInterface || !SetActionFrequencyVariation_API) {
        Log("!!! ERROR: SetActionFrequencyVariation - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetActionFrequencyVariation - pThing is null!");
        return;
    }
    SetActionFrequencyVariation_API(m_pGameInterface, pThing, variation);
}

float LuaQuestState::GetActionFrequencyVariation(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetActionFrequencyVariation_API) {
        Log("!!! ERROR: GetActionFrequencyVariation - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetActionFrequencyVariation - pThing is null!");
        return 0.0f;
    }
    return GetActionFrequencyVariation_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetActionFrequencyVariation(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetActionFrequencyVariation_API) {
        Log("!!! ERROR: ResetActionFrequencyVariation - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetActionFrequencyVariation - pThing is null!");
        return;
    }
    ResetActionFrequencyVariation_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetAction(const std::shared_ptr<CScriptThing>& spThing, const std::string& action) {
    if (!m_pGameInterface || !SetAction_API) {
        Log("!!! ERROR: SetAction - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetAction - pThing is null!");
        return;
    }
    // The API takes CCharString by value
    FableString fsAction(action.c_str());
    SetAction_API(m_pGameInterface, pThing, fsAction.m_charString);
}

std::string LuaQuestState::GetAction(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetAction_API) {
        Log("!!! ERROR: GetAction - API not available!");
        return "";
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetAction - pThing is null!");
        return "";
    }

    CCharString result = { 0 };
    GetAction_API(m_pGameInterface, &result, pThing);

    if (result.pStringData) {
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&result);
            return finalString;
        }
        CCharString_Destroy(&result);
    }
    return "";
}

void LuaQuestState::ResetAction(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetAction_API) {
        Log("!!! ERROR: ResetAction - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetAction - pThing is null!");
        return;
    }
    ResetAction_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetFaceHeroForAction(const std::shared_ptr<CScriptThing>& spThing, bool bFaceHero) {
    if (!m_pGameInterface || !SetFaceHeroForAction_API) {
        Log("!!! ERROR: SetFaceHeroForAction - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetFaceHeroForAction - pThing is null!");
        return;
    }
    SetFaceHeroForAction_API(m_pGameInterface, pThing, bFaceHero);
}

bool LuaQuestState::GetFaceHeroForAction(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetFaceHeroForAction_API) {
        Log("!!! ERROR: GetFaceHeroForAction - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetFaceHeroForAction - pThing is null!");
        return false;
    }
    return GetFaceHeroForAction_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetFaceHeroForAction(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetFaceHeroForAction_API) {
        Log("!!! ERROR: ResetFaceHeroForAction - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetFaceHeroForAction - pThing is null!");
        return;
    }
    ResetFaceHeroForAction_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetTargetName(const std::shared_ptr<CScriptThing>& spThing, const std::string& targetName) {
    if (!m_pGameInterface || !SetTargetName_API) {
        Log("!!! ERROR: SetTargetName - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetTargetName - pThing is null!");
        return;
    }
    // API takes CCharString by value
    FableString fsTarget(targetName.c_str());
    SetTargetName_API(m_pGameInterface, pThing, fsTarget.m_charString);
}

std::string LuaQuestState::GetTargetName(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetTargetName_API) {
        Log("!!! ERROR: GetTargetName - API not available!");
        return "";
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetTargetName - pThing is null!");
        return "";
    }

    CCharString result = { 0 };
    GetTargetName_API(m_pGameInterface, &result, pThing);

    if (result.pStringData) {
        const char* text = CCharString_ToConstChar_API(&result);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&result);
            return finalString;
        }
        CCharString_Destroy(&result);
    }
    return "";
}

void LuaQuestState::ResetTargetName(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetTargetName_API) {
        Log("!!! ERROR: ResetTargetName - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetTargetName - pThing is null!");
        return;
    }
    ResetTargetName_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetFollowDistance(const std::shared_ptr<CScriptThing>& spThing, float distance) {
    if (!m_pGameInterface || !SetFollowDistance_API) {
        Log("!!! ERROR: SetFollowDistance - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetFollowDistance - pThing is null!");
        return;
    }
    SetFollowDistance_API(m_pGameInterface, pThing, distance);
}

float LuaQuestState::GetFollowDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetFollowDistance_API) {
        Log("!!! ERROR: GetFollowDistance - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetFollowDistance - pThing is null!");
        return 0.0f;
    }
    return GetFollowDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetFollowDistance(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetFollowDistance_API) {
        Log("!!! ERROR: ResetFollowDistance - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetFollowDistance - pThing is null!");
        return;
    }
    ResetFollowDistance_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetAttackHeroOnSight(const std::shared_ptr<CScriptThing>& spThing, bool bAttack) {
    if (!m_pGameInterface || !SetAttackHeroOnSight_API) {
        Log("!!! ERROR: SetAttackHeroOnSight - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetAttackHeroOnSight - pThing is null!");
        return;
    }
    SetAttackHeroOnSight_API(m_pGameInterface, pThing, bAttack);
}

bool LuaQuestState::GetAttackHeroOnSight(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetAttackHeroOnSight_API) {
        Log("!!! ERROR: GetAttackHeroOnSight - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetAttackHeroOnSight - pThing is null!");
        return false;
    }
    return GetAttackHeroOnSight_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetAttackHeroOnSight(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetAttackHeroOnSight_API) {
        Log("!!! ERROR: ResetAttackHeroOnSight - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetAttackHeroOnSight - pThing is null!");
        return;
    }
    ResetAttackHeroOnSight_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetTimeToSpendHarassingHero(const std::shared_ptr<CScriptThing>& spThing, int time) {
    if (!m_pGameInterface || !SetTimeToSpendHarassingHero_API) {
        Log("!!! ERROR: SetTimeToSpendHarassingHero - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetTimeToSpendHarassingHero - pThing is null!");
        return;
    }
    SetTimeToSpendHarassingHero_API(m_pGameInterface, pThing, time);
}

int LuaQuestState::GetTimeToSpendHarassingHero(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetTimeToSpendHarassingHero_API) {
        Log("!!! ERROR: GetTimeToSpendHarassingHero - API not available!");
        return 0;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetTimeToSpendHarassingHero - pThing is null!");
        return 0;
    }
    return GetTimeToSpendHarassingHero_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetTimeToSpendHarassingHero(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetTimeToSpendHarassingHero_API) {
        Log("!!! ERROR: ResetTimeToSpendHarassingHero - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetTimeToSpendHarassingHero - pThing is null!");
        return;
    }
    ResetTimeToSpendHarassingHero_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetCombatNearbyEnemyFleeingBreakOffRange(const std::shared_ptr<CScriptThing>& spThing, float range) {
    if (!m_pGameInterface || !SetCombatNearbyEnemyFleeingBreakOffRange_API) {
        Log("!!! ERROR: SetCombatNearbyEnemyFleeingBreakOffRange - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetCombatNearbyEnemyFleeingBreakOffRange - pThing is null!");
        return;
    }
    SetCombatNearbyEnemyFleeingBreakOffRange_API(m_pGameInterface, pThing, range);
}

float LuaQuestState::GetCombatNearbyEnemyFleeingBreakOffRange(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetCombatNearbyEnemyFleeingBreakOffRange_API) {
        Log("!!! ERROR: GetCombatNearbyEnemyFleeingBreakOffRange - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetCombatNearbyEnemyFleeingBreakOffRange - pThing is null!");
        return 0.0f;
    }
    return GetCombatNearbyEnemyFleeingBreakOffRange_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetCombatNearbyEnemyFleeingBreakOffRange(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetCombatNearbyEnemyFleeingBreakOffRange_API) {
        Log("!!! ERROR: ResetCombatNearbyEnemyFleeingBreakOffRange - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetCombatNearbyEnemyFleeingBreakOffRange - pThing is null!");
        return;
    }
    ResetCombatNearbyEnemyFleeingBreakOffRange_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetCombatNearbyBreakOffRange(const std::shared_ptr<CScriptThing>& spThing, float range) {
    if (!m_pGameInterface || !SetCombatNearbyBreakOffRange_API) {
        Log("!!! ERROR: SetCombatNearbyBreakOffRange - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetCombatNearbyBreakOffRange - pThing is null!");
        return;
    }
    SetCombatNearbyBreakOffRange_API(m_pGameInterface, pThing, range);
}

float LuaQuestState::GetCombatNearbyBreakOffRange(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetCombatNearbyBreakOffRange_API) {
        Log("!!! ERROR: GetCombatNearbyBreakOffRange - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetCombatNearbyBreakOffRange - pThing is null!");
        return 0.0f;
    }
    return GetCombatNearbyBreakOffRange_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetCombatNearbyBreakOffRange(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetCombatNearbyBreakOffRange_API) {
        Log("!!! ERROR: ResetCombatNearbyBreakOffRange - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetCombatNearbyBreakOffRange - pThing is null!");
        return;
    }
    ResetCombatNearbyBreakOffRange_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetStealStealableItems(const std::shared_ptr<CScriptThing>& spThing, bool bSteal) {
    if (!m_pGameInterface || !SetStealStealableItems_API) {
        Log("!!! ERROR: SetStealStealableItems - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetStealStealableItems - pThing is null!");
        return;
    }
    SetStealStealableItems_API(m_pGameInterface, pThing, bSteal);
}

bool LuaQuestState::GetStealStealableItems(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetStealStealableItems_API) {
        Log("!!! ERROR: GetStealStealableItems - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetStealStealableItems - pThing is null!");
        return false;
    }
    return GetStealStealableItems_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetStealStealableItems(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetStealStealableItems_API) {
        Log("!!! ERROR: ResetStealStealableItems - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetStealStealableItems - pThing is null!");
        return;
    }
    ResetStealStealableItems_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetRecoverStealableItems(const std::shared_ptr<CScriptThing>& spThing, bool bRecover) {
    if (!m_pGameInterface || !SetRecoverStealableItems_API) {
        Log("!!! ERROR: SetRecoverStealableItems - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetRecoverStealableItems - pThing is null!");
        return;
    }
    SetRecoverStealableItems_API(m_pGameInterface, pThing, bRecover);
}

bool LuaQuestState::GetRecoverStealableItems(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetRecoverStealableItems_API) {
        Log("!!! ERROR: GetRecoverStealableItems - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetRecoverStealableItems - pThing is null!");
        return false;
    }
    return GetRecoverStealableItems_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetRecoverStealableItems(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetRecoverStealableItems_API) {
        Log("!!! ERROR: ResetRecoverStealableItems - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetRecoverStealableItems - pThing is null!");
        return;
    }
    ResetRecoverStealableItems_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetTakeStealableItemToRandomDestination(const std::shared_ptr<CScriptThing>& spThing, bool bRandom) {
    if (!m_pGameInterface || !SetTakeStealableItemToRandomDestination_API) {
        Log("!!! ERROR: SetTakeStealableItemToRandomDestination - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetTakeStealableItemToRandomDestination - pThing is null!");
        return;
    }
    SetTakeStealableItemToRandomDestination_API(m_pGameInterface, pThing, bRandom);
}

bool LuaQuestState::GetTakeStealableItemToRandomDestination(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetTakeStealableItemToRandomDestination_API) {
        Log("!!! ERROR: GetTakeStealableItemToRandomDestination - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetTakeStealableItemToRandomDestination - pThing is null!");
        return false;
    }
    return GetTakeStealableItemToRandomDestination_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetTakeStealableItemToRandomDestination(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetTakeStealableItemToRandomDestination_API) {
        Log("!!! ERROR: ResetTakeStealableItemToRandomDestination - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetTakeStealableItemToRandomDestination - pThing is null!");
        return;
    }
    ResetTakeStealableItemToRandomDestination_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetKillSelfAndStealableItemAfterReachingDestination(const std::shared_ptr<CScriptThing>& spThing, bool bKill) {
    if (!m_pGameInterface || !SetKillSelfAndStealableItemAfterReachingDestination_API) {
        Log("!!! ERROR: SetKillSelfAndStealableItemAfterReachingDestination - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetKillSelfAndStealableItemAfterReachingDestination - pThing is null!");
        return;
    }
    SetKillSelfAndStealableItemAfterReachingDestination_API(m_pGameInterface, pThing, bKill);
}

bool LuaQuestState::GetKillSelfAndStealableItemAfterReachingDestination(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetKillSelfAndStealableItemAfterReachingDestination_API) {
        Log("!!! ERROR: GetKillSelfAndStealableItemAfterReachingDestination - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetKillSelfAndStealableItemAfterReachingDestination - pThing is null!");
        return false;
    }
    return GetKillSelfAndStealableItemAfterReachingDestination_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetKillSelfAndStealableItemAfterReachingDestination(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetKillSelfAndStealableItemAfterReachingDestination_API) {
        Log("!!! ERROR: ResetKillSelfAndStealableItemAfterReachingDestination - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetKillSelfAndStealableItemAfterReachingDestination - pThing is null!");
        return;
    }
    ResetKillSelfAndStealableItemAfterReachingDestination_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetAllowedToFollow(const std::shared_ptr<CScriptThing>& spThing, bool bAllowed) {
    if (!m_pGameInterface || !SetAllowedToFollow_API) {
        Log("!!! ERROR: SetAllowedToFollow - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetAllowedToFollow - pThing is null!");
        return;
    }
    SetAllowedToFollow_API(m_pGameInterface, pThing, bAllowed);
}

bool LuaQuestState::GetAllowedToFollow(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetAllowedToFollow_API) {
        Log("!!! ERROR: GetAllowedToFollow - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetAllowedToFollow - pThing is null!");
        return false;
    }
    return GetAllowedToFollow_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetAllowedToFollow(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetAllowedToFollow_API) {
        Log("!!! ERROR: ResetAllowedToFollow - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetAllowedToFollow - pThing is null!");
        return;
    }
    ResetAllowedToFollow_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetTableName(const std::shared_ptr<CScriptThing>& spThing, const std::string& tableName) {
    if (!m_pGameInterface || !SetTableName_API) {
        Log("!!! ERROR: SetTableName - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetTableName - pThing is null!");
        return;
    }
    FableString fsName(tableName.c_str());
    // The API signature is CCharString by value, so we pass the inner struct
    SetTableName_API(m_pGameInterface, pThing, fsName.m_charString);
}

std::string LuaQuestState::GetTableName(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetTableName_API) {
        Log("!!! ERROR: GetTableName - API not available!");
        return "";
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetTableName - pThing is null!");
        return "";
    }
    CCharString resultName = { 0 };
    GetTableName_API(m_pGameInterface, &resultName, pThing);
    if (resultName.pStringData) {
        const char* text = CCharString_ToConstChar_API(&resultName);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&resultName);
            return finalString;
        }
        CCharString_Destroy(&resultName);
    }
    return "";
}

void LuaQuestState::ResetTableName(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetTableName_API) {
        Log("!!! ERROR: ResetTableName - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetTableName - pThing is null!");
        return;
    }
    ResetTableName_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetSeatName(const std::shared_ptr<CScriptThing>& spThing, const std::string& seatName) {
    if (!m_pGameInterface || !SetSeatName_API) {
        Log("!!! ERROR: SetSeatName - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetSeatName - pThing is null!");
        return;
    }
    FableString fsName(seatName.c_str());
    // The API signature is CCharString by value
    SetSeatName_API(m_pGameInterface, pThing, fsName.m_charString);
}

std::string LuaQuestState::GetSeatName(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetSeatName_API) {
        Log("!!! ERROR: GetSeatName - API not available!");
        return "";
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetSeatName - pThing is null!");
        return "";
    }
    CCharString resultName = { 0 };
    GetSeatName_API(m_pGameInterface, &resultName, pThing);
    if (resultName.pStringData) {
        const char* text = CCharString_ToConstChar_API(&resultName);
        if (text) {
            std::string finalString(text);
            CCharString_Destroy(&resultName);
            return finalString;
        }
        CCharString_Destroy(&resultName);
    }
    return "";
}

void LuaQuestState::ResetSeatName(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetSeatName_API) {
        Log("!!! ERROR: ResetSeatName - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetSeatName - pThing is null!");
        return;
    }
    ResetSeatName_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetDisableHeadLooking(const std::shared_ptr<CScriptThing>& spThing, bool bDisabled) {
    if (!m_pGameInterface || !SetDisableHeadLooking_API) {
        Log("!!! ERROR: SetDisableHeadLooking - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetDisableHeadLooking - pThing is null!");
        return;
    }
    SetDisableHeadLooking_API(m_pGameInterface, pThing, bDisabled);
}

bool LuaQuestState::GetDisableHeadLooking(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetDisableHeadLooking_API) {
        Log("!!! ERROR: GetDisableHeadLooking - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetDisableHeadLooking - pThing is null!");
        return false;
    }
    return GetDisableHeadLooking_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetDisableHeadLooking(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetDisableHeadLooking_API) {
        Log("!!! ERROR: ResetDisableHeadLooking - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetDisableHeadLooking - pThing is null!");
        return;
    }
    ResetDisableHeadLooking_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetIsPushableByHero(CScriptThing* pThing, bool bIsPushable) {
    if (!m_pGameInterface || !SetIsPushableByHero_API) {
        Log("!!! ERROR: SetIsPushableByHero - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: SetIsPushableByHero - pThing is null!");
        return;
    }
    CScriptThing retainedCopy = *pThing;
    {
        std::ostringstream ss;
        ss << "    SetIsPushableByHero ABI probe: api=0x" << std::hex
           << reinterpret_cast<uintptr_t>(SetIsPushableByHero_API)
           << " thing=0x" << reinterpret_cast<uintptr_t>(pThing)
           << " data=0x" << reinterpret_cast<uintptr_t>(retainedCopy.pImp.Data)
           << " info=0x" << reinterpret_cast<uintptr_t>(retainedCopy.pImp.Info);
        LogToFile(ss.str());
    }
    if (retainedCopy.pImp.Info) {
        retainedCopy.pImp.Info->RefCount++;
    }
    SetIsPushableByHero_API(m_pGameInterface, retainedCopy, bIsPushable);
}

bool LuaQuestState::GetIsPushableByHero(CScriptThing* pThing) {
    if (!m_pGameInterface || !GetIsPushableByHero_API) {
        Log("!!! ERROR: GetIsPushableByHero - API not available!");
        return false;
    }
    if (!pThing) {
        Log("!!! ERROR: GetIsPushableByHero - pThing is null!");
        return false;
    }
    CScriptThing retainedCopy = *pThing;
    if (retainedCopy.pImp.Info) {
        retainedCopy.pImp.Info->RefCount++;
    }
    return GetIsPushableByHero_API(m_pGameInterface, retainedCopy);
}

void LuaQuestState::ResetIsPushableByHero(CScriptThing* pThing) {
    if (!m_pGameInterface || !ResetIsPushableByHero_API) {
        Log("!!! ERROR: ResetIsPushableByHero - API not available!");
        return;
    }
    if (!pThing) {
        Log("!!! ERROR: ResetIsPushableByHero - pThing is null!");
        return;
    }
    CScriptThing retainedCopy = *pThing;
    if (retainedCopy.pImp.Info) {
        retainedCopy.pImp.Info->RefCount++;
    }
    ResetIsPushableByHero_API(m_pGameInterface, retainedCopy);
}

void LuaQuestState::AddLogbookTutorialEntry(const std::string& textKey) {
    // Retail 0x00CBE9EE accepts the key in ECX, derives <key>_TITLE, submits
    // category 2 through CGSI +0x4D0, and performs the helper's trailing yield.
    if (AddLogbookTutorialEntry_Func) {
        FableString fsKey(textKey.c_str());
        AddLogbookTutorialEntry_Func(fsKey.get());
    } else {
        Log("!!! ERROR: AddLogbookTutorialEntry - API not available!");
    }
}

void LuaQuestState::SetLookForFiniteTime(const std::shared_ptr<CScriptThing>& spThing, bool bFinite) {
    if (!m_pGameInterface || !SetLookForFiniteTime_API) {
        Log("!!! ERROR: SetLookForFiniteTime - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetLookForFiniteTime - pThing is null!");
        return;
    }
    SetLookForFiniteTime_API(m_pGameInterface, pThing, bFinite);
}

bool LuaQuestState::GetLookForFiniteTime(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetLookForFiniteTime_API) {
        Log("!!! ERROR: GetLookForFiniteTime - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetLookForFiniteTime - pThing is null!");
        return false;
    }
    return GetLookForFiniteTime_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetLookForFiniteTime(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetLookForFiniteTime_API) {
        Log("!!! ERROR: ResetLookForFiniteTime - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetLookForFiniteTime - pThing is null!");
        return;
    }
    ResetLookForFiniteTime_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetAvoidRegionExits(const std::shared_ptr<CScriptThing>& spThing, bool bAvoid) {
    if (!m_pGameInterface || !SetAvoidRegionExits_API) {
        Log("!!! ERROR: SetAvoidRegionExits - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetAvoidRegionExits - pThing is null!");
        return;
    }
    SetAvoidRegionExits_API(m_pGameInterface, pThing, bAvoid);
}

bool LuaQuestState::GetAvoidRegionExits(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetAvoidRegionExits_API) {
        Log("!!! ERROR: GetAvoidRegionExits - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetAvoidRegionExits - pThing is null!");
        return false;
    }
    return GetAvoidRegionExits_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetAvoidRegionExits(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetAvoidRegionExits_API) {
        Log("!!! ERROR: ResetAvoidRegionExits - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetAvoidRegionExits - pThing is null!");
        return;
    }
    ResetAvoidRegionExits_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetTargetingDistanceOffset(const std::shared_ptr<CScriptThing>& spThing, float offset) {
    if (!m_pGameInterface || !SetTargetingDistanceOffset_API) {
        Log("!!! ERROR: SetTargetingDistanceOffset - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: SetTargetingDistanceOffset - pThing is null!");
        return;
    }
    SetTargetingDistanceOffset_API(m_pGameInterface, pThing, offset);
}

float LuaQuestState::GetTargetingDistanceOffset(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !GetTargetingDistanceOffset_API) {
        Log("!!! ERROR: GetTargetingDistanceOffset - API not available!");
        return 0.0f;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: GetTargetingDistanceOffset - pThing is null!");
        return 0.0f;
    }
    return GetTargetingDistanceOffset_API(m_pGameInterface, pThing);
}

void LuaQuestState::ResetTargetingDistanceOffset(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !ResetTargetingDistanceOffset_API) {
        Log("!!! ERROR: ResetTargetingDistanceOffset - API not available!");
        return;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: ResetTargetingDistanceOffset - pThing is null!");
        return;
    }
    ResetTargetingDistanceOffset_API(m_pGameInterface, pThing);
}

void LuaQuestState::SetPlayerUsingMeleeDummies(bool bIsUsing) {
    if (m_pGameInterface && SetPlayerUsingMeleeDummies_API) {
        SetPlayerUsingMeleeDummies_API(m_pGameInterface, bIsUsing);
    }
    else {
        Log("!!! ERROR: SetPlayerUsingMeleeDummies - API not available!");
    }
}

bool LuaQuestState::GetPlayerUsingMeleeDummies() {
    if (m_pGameInterface && GetPlayerUsingMeleeDummies_API) {
        return GetPlayerUsingMeleeDummies_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetPlayerUsingMeleeDummies - API not available!");
    return false;
}

void LuaQuestState::SetPlayerUsingRangedDummies(bool bIsUsing) {
    if (m_pGameInterface && SetPlayerUsingRangedDummies_API) {
        SetPlayerUsingRangedDummies_API(m_pGameInterface, bIsUsing);
    }
    else {
        Log("!!! ERROR: SetPlayerUsingRangedDummies - API not available!");
    }
}

bool LuaQuestState::GetPlayerUsingRangedDummies() {
    if (m_pGameInterface && GetPlayerUsingRangedDummies_API) {
        return GetPlayerUsingRangedDummies_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetPlayerUsingRangedDummies - API not available!");
    return false;
}

void LuaQuestState::SetPlayerUsingWillDummies(bool bIsUsing) {
    if (m_pGameInterface && SetPlayerUsingWillDummies_API) {
        SetPlayerUsingWillDummies_API(m_pGameInterface, bIsUsing);
    }
    else {
        Log("!!! ERROR: SetPlayerUsingWillDummies - API not available!");
    }
}

bool LuaQuestState::GetPlayerUsingWillDummies() {
    if (m_pGameInterface && GetPlayerUsingWillDummies_API) {
        return GetPlayerUsingWillDummies_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetPlayerUsingWillDummies - API not available!");
    return false;
}

void LuaQuestState::SetCheapHeadLooking(bool bIsCheap) {
    if (m_pGameInterface && SetCheapHeadLooking_API) {
        SetCheapHeadLooking_API(m_pGameInterface, bIsCheap);
    }
    else {
        Log("!!! ERROR: SetCheapHeadLooking - API not available!");
    }
}

bool LuaQuestState::GetCheapHeadLooking() {
    if (m_pGameInterface && GetCheapHeadLooking_API) {
        return GetCheapHeadLooking_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetCheapHeadLooking - API not available!");
    return false;
}

void LuaQuestState::SetQuitTavernGame(bool bQuit) {
    if (m_pGameInterface && SetQuitTavernGame_API) {
        SetQuitTavernGame_API(m_pGameInterface, bQuit);
    }
    else {
        Log("!!! ERROR: SetQuitTavernGame - API not available!");
    }
}

bool LuaQuestState::GetQuitTavernGame() {
    if (m_pGameInterface && GetQuitTavernGame_API) {
        return GetQuitTavernGame_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetQuitTavernGame - API not available!");
    return false;
}

void LuaQuestState::SetPrizeTavernTable(bool bIsPrize) {
    if (m_pGameInterface && SetPrizeTavernTable_API) {
        SetPrizeTavernTable_API(m_pGameInterface, bIsPrize);
    }
    else {
        Log("!!! ERROR: SetPrizeTavernTable - API not available!");
    }
}

bool LuaQuestState::GetPrizeTavernTable() {
    if (m_pGameInterface && GetPrizeTavernTable_API) {
        return GetPrizeTavernTable_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetPrizeTavernTable - API not available!");
    return false;
}

void LuaQuestState::SetBettingActive(bool bIsActive) {
    if (m_pGameInterface && SetBettingActive_API) {
        SetBettingActive_API(m_pGameInterface, bIsActive);
    }
    else {
        Log("!!! ERROR: SetBettingActive - API not available!");
    }
}

bool LuaQuestState::GetBettingActive() {
    if (m_pGameInterface && GetBettingActive_API) {
        return GetBettingActive_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBettingActive - API not available!");
    return false;
}

void LuaQuestState::SetBettingAccept(bool bAccept) {
    if (m_pGameInterface && SetBettingAccept_API) {
        SetBettingAccept_API(m_pGameInterface, bAccept);
    }
    else {
        Log("!!! ERROR: SetBettingAccept - API not available!");
    }
}

bool LuaQuestState::GetBettingAccept() {
    if (m_pGameInterface && GetBettingAccept_API) {
        return GetBettingAccept_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBettingAccept - API not available!");
    return false;
}

void LuaQuestState::SetBettingAmount(int amount) {
    if (m_pGameInterface && SetBettingAmount_API) {
        SetBettingAmount_API(m_pGameInterface, amount);
    }
    else {
        Log("!!! ERROR: SetBettingAmount - API not available!");
    }
}

int LuaQuestState::GetBettingAmount() {
    if (m_pGameInterface && GetBettingAmount_API) {
        return GetBettingAmount_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBettingAmount - API not available!");
    return 0;
}

void LuaQuestState::SetCountBetMoneyDown(bool bCount) {
    if (m_pGameInterface && SetCountBetMoneyDown_API) {
        SetCountBetMoneyDown_API(m_pGameInterface, bCount);
    }
    else {
        Log("!!! ERROR: SetCountBetMoneyDown - API not available!");
    }
}

bool LuaQuestState::GetCountBetMoneyDown() {
    if (m_pGameInterface && GetCountBetMoneyDown_API) {
        return GetCountBetMoneyDown_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetCountBetMoneyDown - API not available!");
    return false;
}

void LuaQuestState::SetSpotTheAdditionBeaten(bool bBeaten) {
    if (m_pGameInterface && SetSpotTheAdditionBeaten_API) {
        SetSpotTheAdditionBeaten_API(m_pGameInterface, bBeaten);
    }
    else {
        Log("!!! ERROR: SetSpotTheAdditionBeaten - API not available!");
    }
}

bool LuaQuestState::GetSpotTheAdditionBeaten() {
    if (m_pGameInterface && GetSpotTheAdditionBeaten_API) {
        return GetSpotTheAdditionBeaten_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetSpotTheAdditionBeaten - API not available!");
    return false;
}

void LuaQuestState::SetGlobalTargetingDistanceOffset(float offset) {
    if (m_pGameInterface && SetGlobalTargetingDistanceOffset_API) {
        SetGlobalTargetingDistanceOffset_API(m_pGameInterface, offset);
    }
    else {
        Log("!!! ERROR: SetGlobalTargetingDistanceOffset - API not available!");
    }
}

float LuaQuestState::GetGlobalTargetingDistanceOffset() {
    if (m_pGameInterface && GetGlobalTargetingDistanceOffset_API) {
        return GetGlobalTargetingDistanceOffset_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetGlobalTargetingDistanceOffset - API not available!");
    return 0.0f;
}

void LuaQuestState::SetTradingPriceMult(float mult) {
    if (m_pGameInterface && SetTradingPriceMult_API) {
        SetTradingPriceMult_API(m_pGameInterface, mult);
    }
    else {
        Log("!!! ERROR: SetTradingPriceMult - API not available!");
    }
}

float LuaQuestState::GetTradingPriceMult() {
    if (m_pGameInterface && GetTradingPriceMult_API) {
        return GetTradingPriceMult_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetTradingPriceMult - API not available!");
    return 1.0f; // Default multiplier
}

void LuaQuestState::SetBoastingEnabled(bool bIsEnabled) {
    if (m_pGameInterface && SetBoastingEnabled_API) {
        SetBoastingEnabled_API(m_pGameInterface, bIsEnabled);
    }
    else {
        Log("!!! ERROR: SetBoastingEnabled - API not available!");
    }
}

bool LuaQuestState::GetBoastingEnabled() {
    if (m_pGameInterface && GetBoastingEnabled_API) {
        return GetBoastingEnabled_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetBoastingEnabled - API not available!");
    return false;
}

void LuaQuestState::SetActiveGossipCategories(const std::string& category, bool isActive) {
    if (m_pGameInterface && SetActiveGossipCategories_API) {
        FableString fsCategory(category.c_str());
        // API takes CCharString by value
        SetActiveGossipCategories_API(m_pGameInterface, fsCategory.m_charString, isActive);
    }
    else {
        Log("!!! ERROR: SetActiveGossipCategories - API not available!");
    }
}

sol::object LuaQuestState::GetActiveGossipCategories(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetActiveGossipCategories_WholeMap_API) {
        const std::map<CCharString, bool>* pMap = GetActiveGossipCategories_WholeMap_API(m_pGameInterface);
        return ConvertGossipMapToLua(pMap, lua);
    }
    Log("!!! ERROR: GetActiveGossipCategories (WholeMap) - API not available!");
    return sol::make_object(lua, lua.create_table());
}

sol::object LuaQuestState::GetActiveGossipCategories(const std::string& category, sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetActiveGossipCategories_ByCategory_API) {
        FableString fsCategory(category.c_str());
        bool* pResult = GetActiveGossipCategories_ByCategory_API(m_pGameInterface, fsCategory.m_charString);
        if (pResult) {
            return sol::make_object(lua, *pResult);
        }
        // pResult is null if category doesn't exist
        return sol::make_object(lua, sol::nil);
    }
    Log("!!! ERROR: GetActiveGossipCategories (ByCategory) - API not available!");
    return sol::make_object(lua, sol::nil);
}

int LuaQuestState::GetActiveGossipCategoriesSize() {
    if (m_pGameInterface && GetActiveGossipCategoriesSize_API) {
        return GetActiveGossipCategoriesSize_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetActiveGossipCategoriesSize - API not available!");
    return 0;
}

void LuaQuestState::ClearActiveGossipCategories() {
    if (m_pGameInterface && ClearActiveGossipCategories_API) {
        ClearActiveGossipCategories_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ClearActiveGossipCategories - API not available!");
    }
}

sol::object LuaQuestState::GetIsGossipForPlayer(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetIsGossipForPlayer_WholeMap_API) {
        const std::map<CCharString, bool>* pMap = GetIsGossipForPlayer_WholeMap_API(m_pGameInterface);
        return ConvertGossipMapToLua(pMap, lua);
    }
    Log("!!! ERROR: GetIsGossipForPlayer (WholeMap) - API not available!");
    return sol::make_object(lua, lua.create_table());
}

sol::object LuaQuestState::GetIsGossipForPlayer(const std::string& category, sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetIsGossipForPlayer_ByCategory_API) {
        FableString fsCategory(category.c_str());
        bool* pResult = GetIsGossipForPlayer_ByCategory_API(m_pGameInterface, fsCategory.m_charString);
        if (pResult) {
            return sol::make_object(lua, *pResult);
        }
        return sol::make_object(lua, sol::nil);
    }
    Log("!!! ERROR: GetIsGossipForPlayer (ByCategory) - API not available!");
    return sol::make_object(lua, sol::nil);
}

int LuaQuestState::GetIsGossipForPlayerSize() {
    if (m_pGameInterface && GetIsGossipForPlayerSize_API) {
        return GetIsGossipForPlayerSize_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetIsGossipForPlayerSize - API not available!");
    return 0;
}

void LuaQuestState::ClearIsGossipForPlayer() {
    if (m_pGameInterface && ClearIsGossipForPlayer_API) {
        ClearIsGossipForPlayer_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ClearIsGossipForPlayer - API not available!");
    }
}

void LuaQuestState::SetGossip(const std::string& category, const std::string& rumour, int index) {
    if (m_pGameInterface && SetGossip_API) {
        FableString fsCategory(category.c_str());
        FableString fsRumour(rumour.c_str());
        SetGossip_API(m_pGameInterface, fsCategory.m_charString, fsRumour.m_charString, index);
    }
    else {
        Log("!!! ERROR: SetGGossip - API not available!");
    }
}

sol::object LuaQuestState::GetGossip(const std::string& category, sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetGossip_All_API) {
        FableString fsCategory(category.c_str());
        const std::vector<CCharString>* pVec = GetGossip_All_API(m_pGameInterface, fsCategory.m_charString);
        return ConvertGossipVectorToLua(pVec, lua);
    }
    Log("!!! ERROR: GetGossip (All) - API not available!");
    return sol::make_object(lua, lua.create_table());
}

std::string LuaQuestState::GetGossip(const std::string& category, int index) {
    if (m_pGameInterface && GetGossip_ByIndex_API) {
        CCharString resultStr = { 0 };
        FableString fsCategory(category.c_str());
        GetGossip_ByIndex_API(m_pGameInterface, &resultStr, fsCategory.m_charString, index);
        if (resultStr.pStringData) {
            const char* text = CCharString_ToConstChar_API(&resultStr);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&resultStr);
                return finalString;
            }
            CCharString_Destroy(&resultStr);
        }
    }
    Log("!!! ERROR: GetGossip (ByIndex) - API not available or index invalid!");
    return "";
}

int LuaQuestState::GetGossipSize(const std::string& category) {
    if (m_pGameInterface && GetGossipSize_API) {
        FableString fsCategory(category.c_str());
        return GetGossipSize_API(m_pGameInterface, fsCategory.m_charString);
    }
    Log("!!! ERROR: GetGossipSize - API not available!");
    return 0;
}

void LuaQuestState::ClearGossip(const std::string& category) {
    if (m_pGameInterface && ClearGossip_API) {
        FableString fsCategory(category.c_str());
        ClearGossip_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: ClearGossip - API not available!");
    }
}

void LuaQuestState::RemoveGossip(const std::string& category) {
    if (m_pGameInterface && RemoveGossip_API) {
        FableString fsCategory(category.c_str());
        RemoveGossip_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: RemoveGossip - API not available!");
    }
}

void LuaQuestState::AddGossip(const std::string& category) {
    if (m_pGameInterface && AddGossip_NoRumour_API) {
        FableString fsCategory(category.c_str());
        AddGossip_NoRumour_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: AddGossip (NoRumour) - API not available!");
    }
}

void LuaQuestState::AddGossip(const std::string& category, const std::string& rumour) {
    if (m_pGameInterface && AddGossip_WithRumour_API) {
        FableString fsCategory(category.c_str());
        FableString fsRumour(rumour.c_str());
        AddGossip_WithRumour_API(m_pGameInterface, fsCategory.m_charString, fsRumour.m_charString);
    }
    else {
        Log("!!! ERROR: AddGossip (WithRumour) - API not available!");
    }
}

void LuaQuestState::SetGossipVillages(const std::string& category, const std::string& village, int index) {
    if (m_pGameInterface && SetGossipVillages_API) {
        FableString fsCategory(category.c_str());
        FableString fsVillage(village.c_str());
        SetGossipVillages_API(m_pGameInterface, fsCategory.m_charString, fsVillage.m_charString, index);
    }
    else {
        Log("!!! ERROR: SetGossipVillages - API not available!");
    }
}

sol::object LuaQuestState::GetGossipVillages(const std::string& category, sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetGossipVillages_All_API && CCharString_ToConstChar_API) {
        FableString fsCategory(category.c_str());
        const std::vector<CCharString>* pVec = GetGossipVillages_All_API(m_pGameInterface, fsCategory.m_charString);
        // We reuse the ConvertGossipVectorToLua helper from the previous step
        return ConvertGossipVectorToLua(pVec, lua);
    }
    Log("!!! ERROR: GetGossipVillages (All) - API not available!");
    return sol::make_object(lua, lua.create_table());
}

std::string LuaQuestState::GetGossipVillages(const std::string& category, int index) {
    if (m_pGameInterface && GetGossipVillages_ByIndex_API) {
        CCharString resultStr = { 0 };
        FableString fsCategory(category.c_str());
        GetGossipVillages_ByIndex_API(m_pGameInterface, &resultStr, fsCategory.m_charString, index);
        if (resultStr.pStringData) {
            const char* text = CCharString_ToConstChar_API(&resultStr);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&resultStr);
                return finalString;
            }
            CCharString_Destroy(&resultStr);
        }
    }
    Log("!!! ERROR: GetGossipVillages (ByIndex) - API not available or index invalid!");
    return "";
}

int LuaQuestState::GetGossipVillagesSize(const std::string& category) {
    if (m_pGameInterface && GetGossipVillagesSize_API) {
        FableString fsCategory(category.c_str());
        return GetGossipVillagesSize_API(m_pGameInterface, fsCategory.m_charString);
    }
    Log("!!! ERROR: GetGossipVillagesSize - API not available!");
    return 0;
}

void LuaQuestState::ClearGossipVillages(const std::string& category) {
    if (m_pGameInterface && ClearGossipVillages_API) {
        FableString fsCategory(category.c_str());
        ClearGossipVillages_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: ClearGossipVillages - API not available!");
    }
}

void LuaQuestState::RemoveGossipVillages(const std::string& category) {
    if (m_pGameInterface && RemoveGossipVillages_API) {
        FableString fsCategory(category.c_str());
        RemoveGossipVillages_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: RemoveGossipVillages - API not available!");
    }
}

void LuaQuestState::AddGossipVillages(const std::string& category) {
    if (m_pGameInterface && AddGossipVillages_NoVillage_API) {
        FableString fsCategory(category.c_str());
        AddGossipVillages_NoVillage_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: AddGossipVillages (NoVillage) - API not available!");
    }
}

void LuaQuestState::AddGossipVillages(const std::string& category, const std::string& village) {
    if (m_pGameInterface && AddGossipVillages_WithVillage_API) {
        FableString fsCategory(category.c_str());
        FableString fsVillage(village.c_str());
        AddGossipVillages_WithVillage_API(m_pGameInterface, fsCategory.m_charString, fsVillage.m_charString);
    }
    else {
        Log("!!! ERROR: AddGossipVillages (WithVillage) - API not available!");
    }
}

void LuaQuestState::SetGossipFactions(const std::string& category, const std::string& faction, int index) {
    if (m_pGameInterface && SetGossipFactions_API) {
        FableString fsCategory(category.c_str());
        FableString fsFaction(faction.c_str());
        SetGossipFactions_API(m_pGameInterface, fsCategory.m_charString, fsFaction.m_charString, index);
    }
    else {
        Log("!!! ERROR: SetGossipFactions - API not available!");
    }
}

sol::object LuaQuestState::GetGossipFactions(const std::string& category, sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && GetGossipFactions_All_API && CCharString_ToConstChar_API) {
        FableString fsCategory(category.c_str());
        const std::vector<CCharString>* pVec = GetGossipFactions_All_API(m_pGameInterface, fsCategory.m_charString);
        return ConvertGossipVectorToLua(pVec, lua);
    }
    Log("!!! ERROR: GetGossipFactions (All) - API not available!");
    return sol::make_object(lua, lua.create_table());
}

std::string LuaQuestState::GetGossipFactions(const std::string& category, int index) {
    if (m_pGameInterface && GetGossipFactions_ByIndex_API) {
        CCharString resultStr = { 0 };
        FableString fsCategory(category.c_str());
        GetGossipFactions_ByIndex_API(m_pGameInterface, &resultStr, fsCategory.m_charString, index);
        if (resultStr.pStringData) {
            const char* text = CCharString_ToConstChar_API(&resultStr);
            if (text) {
                std::string finalString(text);
                CCharString_Destroy(&resultStr);
                return finalString;
            }
            CCharString_Destroy(&resultStr);
        }
    }
    Log("!!! ERROR: GetGossipFactions (ByIndex) - API not available or index invalid!");
    return "";
}

int LuaQuestState::GetGossipFactionsSize(const std::string& category) {
    if (m_pGameInterface && GetGossipFactionsSize_API) {
        FableString fsCategory(category.c_str());
        return GetGossipFactionsSize_API(m_pGameInterface, fsCategory.m_charString);
    }
    Log("!!! ERROR: GetGossipFactionsSize - API not available!");
    return 0;
}

void LuaQuestState::ClearGossipFactions(const std::string& category) {
    if (m_pGameInterface && ClearGossipFactions_API) {
        FableString fsCategory(category.c_str());
        ClearGossipFactions_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: ClearGossipFactions - API not available!");
    }
}

void LuaQuestState::RemoveGossipFactions(const std::string& category) {
    if (m_pGameInterface && RemoveGossipFactions_API) {
        FableString fsCategory(category.c_str());
        RemoveGossipFactions_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: RemoveGossipFactions - API not available!");
    }
}

void LuaQuestState::AddGossipFactions(const std::string& category) {
    if (m_pGameInterface && AddGossipFactions_NoFaction_API) {
        FableString fsCategory(category.c_str());
        AddGossipFactions_NoFaction_API(m_pGameInterface, fsCategory.m_charString);
    }
    else {
        Log("!!! ERROR: AddGossipFactions (NoFaction) - API not available!");
    }
}

void LuaQuestState::AddGossipFactions(const std::string& category, const std::string& faction) {
    if (m_pGameInterface && AddGossipFactions_WithFaction_API) {
        FableString fsCategory(category.c_str());
        FableString fsFaction(faction.c_str());
        AddGossipFactions_WithFaction_API(m_pGameInterface, fsCategory.m_charString, fsFaction.m_charString);
    }
    else {
        Log("!!! ERROR: AddGossipFactions (WithFaction) - API not available!");
    }
}

void LuaQuestState::SetTrapAsActive(const std::shared_ptr<CScriptThing>& spTrap, bool bIsActive) {
    if (!m_pGameInterface || !SetTrapAsActive_API) {
        Log("!!! ERROR: SetTrapAsActive - API not available!");
        return;
    }
    CScriptThing* pTrap = spTrap.get();
    if (!pTrap) {
        Log("!!! ERROR: SetTrapAsActive - pTrap is null!");
        return;
    }
    SetTrapAsActive_API(m_pGameInterface, pTrap, bIsActive);
}

std::shared_ptr<CScriptThing> LuaQuestState::GetRandomThingWithScriptName(const std::string& scriptName) {
    if (!m_pGameInterface || !GetRandomThingWithScriptName_API || !Game_malloc) {
        Log("!!! ERROR: GetRandomThingWithScriptName - API or memory allocator not available!");
        return nullptr;
    }

    FableString fsName(scriptName.c_str());

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: GetRandomThingWithScriptName - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    GetRandomThingWithScriptName_API(m_pGameInterface, pResult, fsName.get());

    return WrapScriptThingOutput(pResult);
}

sol::table LuaQuestState::GetAllCreaturesExcludingHero(sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetAllCreaturesExcludingHero_API || !Game_malloc) {
        Log("!!! ERROR: GetAllCreaturesExcludingHero - API or memory allocator not available!");
        return resultTable;
    }

    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(20); // Reserve space

    int count = GetAllCreaturesExcludingHero_API(m_pGameInterface, &thingsVectorCopies);

    if (count > 0) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: GetAllCreaturesExcludingHero - Failed to allocate heap buffer for vector item!");
                continue;
            }
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing));
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
        }
    }
    return resultTable;
}

sol::table LuaQuestState::GetAllThingsWithDefNameByDistanceFrom(const std::shared_ptr<CScriptThing>& spRelativeTo, const std::string& defName, sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetAllThingsWithDefNameByDistanceFrom_API || !Game_malloc) {
        Log("!!! ERROR: GetAllThingsWithDefNameByDistanceFrom - API or memory allocator not available!");
        return resultTable;
    }

    CScriptThing* pRelativeTo = spRelativeTo.get();
    if (!pRelativeTo) {
        Log("!!! ERROR: GetAllThingsWithDefNameByDistanceFrom - pRelativeTo is null!");
        return resultTable;
    }

    FableString fsDefName(defName.c_str());
    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(20);

    int count = GetAllThingsWithDefNameByDistanceFrom_API(m_pGameInterface, pRelativeTo, fsDefName.get(), &thingsVectorCopies);

    if (count > 0) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: GetAllThingsWithDefNameByDistanceFrom - Failed to allocate heap buffer for vector item!");
                continue;
            }
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing));
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
        }
    }
    return resultTable;
}

sol::table LuaQuestState::GetAllCreaturesInAreaWithScriptName(const std::string& scriptName, sol::table position, float radius, sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetAllCreaturesInAreaWithScriptName_API || !Game_malloc) {
        Log("!!! ERROR: GetAllCreaturesInAreaWithScriptName - API or memory allocator not available!");
        return resultTable;
    }

    FableString fsName(scriptName.c_str());
    C3DVector pos = {
        position["x"].get_or(0.0f),
        position["y"].get_or(0.0f),
        position["z"].get_or(0.0f)
    };
    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(20);

    int count = GetAllCreaturesInAreaWithScriptName_API(m_pGameInterface, fsName.get(), &pos, radius, &thingsVectorCopies);

    if (count > 0) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: GetAllCreaturesInAreaWithScriptName - Failed to allocate heap buffer for vector item!");
                continue;
            }
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing));
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
        }
    }
    return resultTable;
}

void LuaQuestState::SetCreatureCreationDelayFrames(int frames) {
    if (m_pGameInterface && SetCreatureCreationDelayFrames_API) {
        SetCreatureCreationDelayFrames_API(m_pGameInterface, frames);
    }
    else {
        Log("!!! ERROR: SetCreatureCreationDelayFrames - API not available!");
    }
}

void LuaQuestState::ResetCreatureCreationDelayFrames() {
    if (m_pGameInterface && ResetCreatureCreationDelayFrames_API) {
        ResetCreatureCreationDelayFrames_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: ResetCreatureCreationDelayFrames - API not available!");
    }
}

sol::object LuaQuestState::MsgOnFeatAccepted(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && MsgOnFeatAccepted_API) {
        int featID = 0; // Output parameter
        bool result = MsgOnFeatAccepted_API(m_pGameInterface, &featID);
        if (result) {
            return sol::make_object(lua, featID);
        }
    }
    return sol::make_object(lua, sol::nil);
}

bool LuaQuestState::MsgIsBoastMade(int boastID) {
    if (m_pGameInterface && MsgIsBoastMade_API) {
        return MsgIsBoastMade_API(m_pGameInterface, boastID);
    }
    Log("!!! ERROR: MsgIsBoastMade - API not available!");
    return false;
}

sol::table LuaQuestState::MsgOnBoastsMade(sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (m_pGameInterface && MsgOnBoastsMade_API && CCharString_ToConstChar_API) {
        std::vector<std::pair<long, CCharString>> boastsVector;
        bool result = MsgOnBoastsMade_API(m_pGameInterface, &boastsVector);

        if (result && !boastsVector.empty()) {
            int luaTableIndex = 1;
            for (auto& pair : boastsVector) {
                sol::table boastInfo = lua.create_table();
                boastInfo["id"] = pair.first;

                const char* text = CCharString_ToConstChar_API(&pair.second);
                if (text) {
                    boastInfo["name"] = std::string(text);
                }

                resultTable[luaTableIndex++] = boastInfo;

                // We must destroy the CCharString copies the game gave us
                CCharString_Destroy(&pair.second);
            }
        }
    }
    else {
        Log("!!! ERROR: MsgOnBoastsMade - API not available!");
    }
    return resultTable;
}

void LuaQuestState::RemoveBoastMessage() {
    if (m_pGameInterface && RemoveBoastMessage_API) {
        RemoveBoastMessage_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: RemoveBoastMessage - API not available!");
    }
}

sol::object LuaQuestState::IsQuestStartScreenActive(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && IsQuestStartScreenActive_API) {
        CCharString resultName = { 0 };
        bool isActive = IsQuestStartScreenActive_API(m_pGameInterface, &resultName);
        if (isActive) {
            if (resultName.pStringData) {
                const char* text = CCharString_ToConstChar_API(&resultName);
                if (text) {
                    std::string finalString(text);
                    CCharString_Destroy(&resultName);
                    return sol::make_object(lua, finalString);
                }
                CCharString_Destroy(&resultName);
            }
            return sol::make_object(lua, true); // Active but string was bad
        }
    }
    return sol::make_object(lua, sol::nil); // Not active
}

sol::object LuaQuestState::MsgOnLeavingQuestStartScreen(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && MsgOnLeavingQuestStartScreen_API) {
        CCharString resultName = { 0 };
        bool hasLeft = MsgOnLeavingQuestStartScreen_API(m_pGameInterface, &resultName);
        if (hasLeft) {
            if (resultName.pStringData) {
                const char* text = CCharString_ToConstChar_API(&resultName);
                if (text) {
                    std::string finalString(text);
                    CCharString_Destroy(&resultName);
                    return sol::make_object(lua, finalString);
                }
                CCharString_Destroy(&resultName);
            }
            return sol::make_object(lua, true); // Left but string was bad
        }
    }
    return sol::make_object(lua, sol::nil); // Has not left
}

bool LuaQuestState::MsgOnLeavingExperienceSpendingScreen() {
    if (m_pGameInterface && MsgOnLeavingExperienceSpendingScreen_API) {
        return MsgOnLeavingExperienceSpendingScreen_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgOnLeavingExperienceSpendingScreen - API not available!");
    return false;
}

int LuaQuestState::MsgIsQuestionAnsweredYesOrNo() {
    if (m_pGameInterface && MsgIsQuestionAnsweredYesOrNo_API) {
        return MsgIsQuestionAnsweredYesOrNo_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgIsQuestionAnsweredYesOrNo - API not available!");
    return -1; // Default to "no answer"
}

bool LuaQuestState::MsgIsGameInfoClickedPast() {
    if (m_pGameInterface && MsgIsGameInfoClickedPast_API) {
        return MsgIsGameInfoClickedPast_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgIsGameInfoClickedPast - API not available!");
    return false;
}

bool LuaQuestState::MsgIsTutorialClickedPast() {
    if (m_pGameInterface && MsgIsTutorialClickedPast_API) {
        return MsgIsTutorialClickedPast_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgIsTutorialClickedPast - API not available!");
    return false;
}

bool LuaQuestState::MsgIsActionModeButtonPressed() {
    if (m_pGameInterface && MsgIsActionModeButtonPressed_API) {
        return MsgIsActionModeButtonPressed_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgIsActionModeButtonPressed - API not available!");
    return false;
}

sol::object LuaQuestState::MsgOnExpressionPerformed(sol::this_state s) {
    sol::state_view lua(s);
    if (m_pGameInterface && MsgOnExpressionPerformed_API) {
        CCharString resultName = { 0 };
        bool performed = MsgOnExpressionPerformed_API(m_pGameInterface, &resultName);
        if (performed) {
            if (resultName.pStringData) {
                const char* text = CCharString_ToConstChar_API(&resultName);
                if (text) {
                    std::string finalString(text);
                    CCharString_Destroy(&resultName);
                    return sol::make_object(lua, finalString);
                }
                CCharString_Destroy(&resultName);
            }
            return sol::make_object(lua, true); // Performed but string was bad
        }
    }
    return sol::make_object(lua, sol::nil); // Not performed
}

bool LuaQuestState::MsgIsCutSceneSkipped() {
    if (m_pGameInterface && MsgIsCutSceneSkipped_API) {
        return MsgIsCutSceneSkipped_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgIsCutSceneSkipped - API not available!");
    return false;
}

void LuaQuestState::RemoveAllCutSceneSkippedMessages() {
    if (m_pGameInterface && RemoveAllCutSceneSkippedMessages_API) {
        RemoveAllCutSceneSkippedMessages_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: RemoveAllCutSceneSkippedMessages - API not available!");
    }
}

bool LuaQuestState::MsgOnChestOpeningCancelled() {
    if (m_pGameInterface && MsgOnChestOpeningCancelled_API) {
        return MsgOnChestOpeningCancelled_API(m_pGameInterface);
    }
    Log("!!! ERROR: MsgOnChestOpeningCancelled - API not available!");
    return false;
}

float LuaQuestState::GetHeroHealth() {
    if (m_pGameInterface && GetHeroHealth_API) {
        return GetHeroHealth_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHealth - API not available!");
    return 0.0f;
}

void LuaQuestState::RespawnHero(bool bUnknown) {
    if (m_pGameInterface && RespawnHero_API) {
        RespawnHero_API(m_pGameInterface, bUnknown);
    }
    else {
        Log("!!! ERROR: RespawnHero - API not available!");
    }
}

bool LuaQuestState::GetHeroHasCurrentMarriage() {
    if (m_pGameInterface && GetHeroHasCurrentMarriage_API) {
        return GetHeroHasCurrentMarriage_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHasCurrentMarriage - API not available!");
    return false;
}

bool LuaQuestState::GetHeroHasDivorcedMarriage() {
    if (m_pGameInterface && GetHeroHasDivorcedMarriage_API) {
        return GetHeroHasDivorcedMarriage_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHasDivorcedMarriage - API not available!");
    return false;
}

bool LuaQuestState::GetHeroHasChildren() {
    if (m_pGameInterface && GetHeroHasChildren_API) {
        return GetHeroHasChildren_API(m_pGameInterface);
    }
    Log("!!! ERROR: GetHeroHasChildren - API not available!");
    return false;
}

bool LuaQuestState::IsHeroChild() {
    if (m_pGameInterface && IsHeroChild_API) {
        return IsHeroChild_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsHeroChild - API not available!");
    return false;
}

void LuaQuestState::CancelHeroTeleportEffects() {
    if (m_pGameInterface && CancelHeroTeleportEffects_API) {
        CancelHeroTeleportEffects_API(m_pGameInterface);
    }
    else {
        Log("!!! ERROR: CancelHeroTeleportEffects - API not available!");
    }
}

void LuaQuestState::EntityFollowThing(const std::shared_ptr<CScriptThing>& spFollower, const std::shared_ptr<CScriptThing>& spTarget, float distance, bool bUnknown) {
    if (!m_pGameInterface || !EntityFollowThing_API) {
        Log("!!! ERROR: EntityFollowThing - API not available!");
        return;
    }
    CScriptThing* pFollower = spFollower.get();
    if (!pFollower) {
        Log("!!! ERROR: EntityFollowThing - pFollower is null!");
        return;
    }
    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: EntityFollowThing - pTarget is null!");
        return;
    }
    EntityFollowThing_API(m_pGameInterface, pFollower, pTarget, distance, bUnknown);
}

void LuaQuestState::EntityStopFollowing(const std::shared_ptr<CScriptThing>& spFollower) {
    if (!m_pGameInterface || !EntityStopFollowing_API) {
        Log("!!! ERROR: EntityStopFollowing - API not available!");
        return;
    }
    CScriptThing* pFollower = spFollower.get();
    if (!pFollower) {
        Log("!!! ERROR: EntityStopFollowing - pFollower is null!");
        return;
    }
    EntityStopFollowing_API(m_pGameInterface, pFollower);
}

sol::table LuaQuestState::GetFollowingEntityList(const std::shared_ptr<CScriptThing>& spTarget, sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetFollowingEntityList_API || !Game_malloc) {
        Log("!!! ERROR: GetFollowingEntityList - API or memory allocator not available!");
        return resultTable;
    }

    CScriptThing* pTarget = spTarget.get();
    if (!pTarget) {
        Log("!!! ERROR: GetFollowingEntityList - pTarget is null!");
        return resultTable;
    }

    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(10);

    bool result = GetFollowingEntityList_API(m_pGameInterface, pTarget, &thingsVectorCopies);

    if (result && !thingsVectorCopies.empty()) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: GetFollowingEntityList - Failed to allocate heap buffer for vector item!");
                continue;
            }
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing));
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
        }
    }
    return resultTable;
}

sol::table LuaQuestState::GetHeroSummonedCreaturesList(sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetHeroSummonedCreaturesList_API || !Game_malloc) {
        Log("!!! ERROR: GetHeroSummonedCreaturesList - API or memory allocator not available!");
        return resultTable;
    }

    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(10);

    bool result = GetHeroSummonedCreaturesList_API(m_pGameInterface, &thingsVectorCopies);

    if (result && !thingsVectorCopies.empty()) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: GetHeroSummonedCreaturesList - Failed to allocate heap buffer for vector item!");
                continue;
            }
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing));
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
        }
    }
    return resultTable;
}

sol::table LuaQuestState::GetPerceivingHeroEntityList(sol::this_state s) {
    sol::state_view lua(s);
    sol::table resultTable = lua.create_table();

    if (!m_pGameInterface || !GetPerceivingHeroEntityList_API || !Game_malloc) {
        Log("!!! ERROR: GetPerceivingHeroEntityList - API or memory allocator not available!");
        return resultTable;
    }

    std::vector<CScriptThing> thingsVectorCopies;
    thingsVectorCopies.reserve(20);

    bool result = GetPerceivingHeroEntityList_API(m_pGameInterface, &thingsVectorCopies);

    if (result && !thingsVectorCopies.empty()) {
        int luaTableIndex = 1;
        for (const auto& thingCopy : thingsVectorCopies) {
            CScriptThing* pHeapCopy = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
            if (!pHeapCopy) {
                Log("!!! ERROR: GetPerceivingHeroEntityList - Failed to allocate heap buffer for vector item!");
                continue;
            }
            memcpy(pHeapCopy, &thingCopy, sizeof(CScriptThing));
            std::shared_ptr<CScriptThing> spThing = WrapScriptThingOutput(pHeapCopy, true);
            if (spThing) {
                resultTable[luaTableIndex++] = spThing;
            }
        }
    }
    return resultTable;
}

bool LuaQuestState::IsEntityFollowingHero(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityFollowingHero_API) {
        Log("!!! ERROR: IsEntityFollowingHero - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityFollowingHero - pThing is null!");
        return false;
    }
    return IsEntityFollowingHero_API(m_pGameInterface, pThing);
}

bool LuaQuestState::IsEntityAbleToAttack(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !IsEntityAbleToAttack_API) {
        Log("!!! ERROR: IsEntityAbleToAttack - API not available!");
        return false;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: IsEntityAbleToAttack - pThing is null!");
        return false;
    }
    return IsEntityAbleToAttack_API(m_pGameInterface, pThing);
}

std::shared_ptr<CScriptThing> LuaQuestState::EntityGetThingInPrimarySlot(const std::shared_ptr<CScriptThing>& spThing) {
    if (!m_pGameInterface || !EntityGetThingInPrimarySlot_API || !Game_malloc) {
        Log("!!! ERROR: EntityGetThingInPrimarySlot - API or memory allocator not available!");
        return nullptr;
    }
    CScriptThing* pThing = spThing.get();
    if (!pThing) {
        Log("!!! ERROR: EntityGetThingInPrimarySlot - pThing is null!");
        return nullptr;
    }

    CScriptThing* pResult = static_cast<CScriptThing*>(Game_malloc(sizeof(CScriptThing)));
    if (!pResult) {
        Log("!!! ERROR: EntityGetThingInPrimarySlot - Failed to allocate memory for result!");
        return nullptr;
    }
    memset(pResult, 0, sizeof(CScriptThing));

    EntityGetThingInPrimarySlot_API(m_pGameInterface, pResult, pThing);

    return WrapScriptThingOutput(pResult);
}

bool LuaQuestState::IsInCutscene() {
    if (m_pGameInterface && IsInCutscene_API) {
        return IsInCutscene_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsInCutscene - API not available!");
    return false;
}

bool LuaQuestState::IsInMovieSequence() {
    if (m_pGameInterface && IsInMovieSequence_API) {
        return IsInMovieSequence_API(m_pGameInterface);
    }
    Log("!!! ERROR: IsInMovieSequence - API not available!");
    return false;
}

void LuaQuestState::SetCreatureBrain(CScriptThing* pCreature, const std::string& brainName) {
    if (!m_pGameInterface || !SetCreatureBrain_API) {
        Log("!!! ERROR: SetCreatureBrain - API not available!");
        return;
    }
    if (!pCreature) {
        Log("!!! ERROR: SetCreatureBrain - pCreature is null!");
        return;
    }
    FableString fsBrain(brainName.c_str());
    SetCreatureBrain_API(m_pGameInterface, pCreature, fsBrain.get());
}

// ===== Ported from upstream FSE (eeeeeAeoN/FableScriptExtender @ master,
// 2026-09-02). =====
void LuaQuestState::AddLogbookStoryEntryString(const std::string& textKey) {
    if (AddLogbookStoryEntryString_Func) {
        FableString fsKey(textKey.c_str());
        AddLogbookStoryEntryString_Func(fsKey.get());
    } else {
        Log("!!! ERROR: AddLogbookStoryEntryString - API not available!");
    }
}

float LuaQuestState::GetDistanceBetweenThings(sol::object obj1, sol::object obj2) {
    CScriptThing* pThing1 = GetScriptThingFromSolObject(obj1);
    CScriptThing* pThing2 = GetScriptThingFromSolObject(obj2);
    if (!pThing1 || !pThing2 || !pThing1->pVTable || !pThing2->pVTable) {
        return 0.0f;
    }
    CScriptThingVTable* pVTable1 = reinterpret_cast<CScriptThingVTable*>(pThing1->pVTable);
    CScriptThingVTable* pVTable2 = reinterpret_cast<CScriptThingVTable*>(pThing2->pVTable);
    if (!pVTable1->GetPos || !pVTable2->GetPos) {
        return 0.0f;
    }
    const C3DVector* pos1 = pVTable1->GetPos(pThing1);
    const C3DVector* pos2 = pVTable2->GetPos(pThing2);
    if (!pos1 || !pos2) {
        return 0.0f;
    }
    float dx = pos1->x - pos2->x;
    float dy = pos1->y - pos2->y;
    float dz = pos1->z - pos2->z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

void LuaQuestState::SetMasterGameState(const std::string& flagName, sol::object value) {
    if (!m_pParentHost) { return; }
    CQ_SunnyvaleMasterData* pMaster = m_pParentHost->GetMasterData();
    if (!pMaster) { return; }

    if (value.is<bool>()) {
        bool bVal = value.as<bool>();
        if (flagName == "HenchmanSacrificedBodge") { pMaster->HenchmanSacrificedBodge = bVal; return; }
        if (flagName == "HauntedBarrowFieldsCompleted") { pMaster->HauntedBarrowFieldsCompleted = bVal; return; }
        if (flagName == "RandomEntitySheriffAlive") { pMaster->RandomEntitySheriffAlive = bVal; return; }
        if (flagName == "RandomEntityLieutenantAlive") { pMaster->RandomEntityLieutenantAlive = bVal; return; }
        if (flagName == "BanditCampTwinbladeKilled") { pMaster->BanditCampTwinbladeKilled = bVal; return; }
        if (flagName == "FriendOfForeman") { pMaster->FriendOfForeman = bVal; return; }
        if (flagName == "BridgeOpened") { pMaster->BridgeOpened = bVal; return; }
        if (flagName == "GrannyMemoryReturned") { pMaster->GrannyMemoryReturned = bVal; return; }
        if (flagName == "IsLunaHuman") { pMaster->IsLunaHuman = bVal; return; }
        if (flagName == "CondemnedManDead") { pMaster->CondemnedManDead = bVal; return; }
        if (flagName == "CondemnedManForgiven") { pMaster->CondemnedManForgiven = bVal; return; }
        if (flagName == "CondemnedManMeetsBodyGuard") { pMaster->CondemnedManMeetsBodyGuard = bVal; return; }
        if (flagName == "CondemnedManMeetsBodyGuardCutSceneStart") { pMaster->CondemnedManMeetsBodyGuardCutSceneStart = bVal; return; }
        if (flagName == "CondemnedManMeetsBodyGuardCutSceneFinished") { pMaster->CondemnedManMeetsBodyGuardCutSceneFinished = bVal; return; }
        if (flagName == "SeenAbbeyMotherAtGuild") { pMaster->SeenAbbeyMotherAtGuild = bVal; return; }
        if (flagName == "DefeatedThunder") { pMaster->DefeatedThunder = bVal; return; }
        if (flagName == "LostToThunder") { pMaster->LostToThunder = bVal; return; }
        if (flagName == "KilledThunder") { pMaster->KilledThunder = bVal; return; }
        if (flagName == "CollectedSoulFromArena") { pMaster->CollectedSoulFromArena = bVal; return; }
        if (flagName == "KilledBriar") { pMaster->KilledBriar = bVal; return; }
        if (flagName == "CollectedSoulFromMother") { pMaster->CollectedSoulFromMother = bVal; return; }
        if (flagName == "KilledGM") { pMaster->KilledGM = bVal; return; }
        if (flagName == "CollectedSoulFromNostro") { pMaster->CollectedSoulFromNostro = bVal; return; }
        if (flagName == "WhisperKilledByHero") { pMaster->WhisperKilledByHero = bVal; return; }
        if (flagName == "ArenaFinished") { pMaster->ArenaFinished = bVal; return; }
        if (flagName == "GatesRequireClosing") { pMaster->GatesRequireClosing = bVal; return; }
        if (flagName == "GatesRequireOpening") { pMaster->GatesRequireOpening = bVal; return; }
        if (flagName == "TrophyDealerHeroSpokenToDemonDoors") { pMaster->TrophyDealerHeroSpokenToDemonDoors = bVal; return; }
        if (flagName == "MotherDodgingKraken") { pMaster->MotherDodgingKraken = bVal; return; }
        if (flagName == "JackBossBattleHeroGoodAtEnd") { pMaster->JackBossBattleHeroGoodAtEnd = bVal; return; }
        if (flagName == "WifeLeavingYou") { pMaster->WifeLeavingYou = bVal; return; }
        if (flagName == "WifeLeftYou") { pMaster->WifeLeftYou = bVal; return; }
        if (flagName == "StartNextScene") { pMaster->StartNextScene = bVal; return; }
        if (flagName == "ScorpionsDestroyed") { pMaster->ScorpionsDestroyed = bVal; return; }
        if (flagName == "ScorpionsDestroyedCutscenePlayed") { pMaster->ScorpionsDestroyedCutscenePlayed = bVal; return; }
        if (flagName == "SkillTrainingStarted") { pMaster->SkillTrainingStarted = bVal; return; }
        if (flagName == "WillTrainingStarted") { pMaster->WillTrainingStarted = bVal; return; }
        if (flagName == "MovingDummiesNeeded") { pMaster->MovingDummiesNeeded = bVal; return; }
        if (flagName == "MeleeApprenticeNeededForCutscene") { pMaster->MeleeApprenticeNeededForCutscene = bVal; return; }
        if (flagName == "GuildWarningOccuring") { pMaster->GuildWarningOccuring = bVal; return; }
        if (flagName == "SkillTestOccuring") { pMaster->SkillTestOccuring = bVal; return; }
        if (flagName == "WillTestOccuring") { pMaster->WillTestOccuring = bVal; return; }
        if (flagName == "SkillRepeating") { pMaster->SkillRepeating = bVal; return; }
        if (flagName == "SkillRepeatKnown") { pMaster->SkillRepeatKnown = bVal; return; }
        if (flagName == "SkillDummyReset") { pMaster->SkillDummyReset = bVal; return; }
        if (flagName == "HeroTakingGuildTest") { pMaster->HeroTakingGuildTest = bVal; return; }
        if (flagName == "SwordInTheStoneComplete") { pMaster->SwordInTheStoneComplete = bVal; return; }
        if (flagName == "AmbushTradersAllGuardsDead") { pMaster->AmbushTradersAllGuardsDead = bVal; return; }
        if (flagName == "AmbushTradersAllTradersDead") { pMaster->AmbushTradersAllTradersDead = bVal; return; }
        if (flagName == "AmbushTradersSpyDead") { pMaster->AmbushTradersSpyDead = bVal; return; }
        if (flagName == "StruckDealWithLadyGrey") { pMaster->StruckDealWithLadyGrey = bVal; return; }
        if (flagName == "HeroExposedLadyGrey") { pMaster->HeroExposedLadyGrey = bVal; return; }
        if (flagName == "HeroMarriedLadyGrey") { pMaster->HeroMarriedLadyGrey = bVal; return; }
        if (flagName == "BountyHuntWithinTimeLimit") { pMaster->BountyHuntWithinTimeLimit = bVal; return; }
        if (flagName == "BountyHuntDecapitation") { pMaster->BountyHuntDecapitation = bVal; return; }
        if (flagName == "BountyHuntTimeLimitExceeded") { pMaster->BountyHuntTimeLimitExceeded = bVal; return; }
        if (flagName == "BreakSiegeFinished") { pMaster->BreakSiegeFinished = bVal; return; }
        if (flagName == "WhiteBalverineFinished") { pMaster->WhiteBalverineFinished = bVal; return; }
        if (flagName == "MadBomberFinished") { pMaster->MadBomberFinished = bVal; return; }
        if (flagName == "BodyGuardsMustStandAndWait") { pMaster->BodyGuardsMustStandAndWait = bVal; return; }
        if (flagName == "BodyGuardsInLimbo") { pMaster->BodyGuardsInLimbo = bVal; return; }
        if (flagName == "WifeHeroFlirting") { pMaster->WifeHeroFlirting = bVal; return; }
        if (flagName == "PrisonRaceWonByHero") { pMaster->PrisonRaceWonByHero = bVal; return; }
        if (flagName == "WasPrisonRaceEverWonByHero") { pMaster->WasPrisonRaceEverWonByHero = bVal; return; }
        if (flagName == "PrisonKeyStolenByHero") { pMaster->PrisonKeyStolenByHero = bVal; return; }
        if (flagName == "PrisonTorturedBefore") { pMaster->PrisonTorturedBefore = bVal; return; }
        if (flagName == "PrisonGameDoneBefore") { pMaster->PrisonGameDoneBefore = bVal; return; }
        if (flagName == "HeroDollsScriptUsingTeacher") { pMaster->HeroDollsScriptUsingTeacher = bVal; return; }
        if (flagName == "TimeAdvancePointTriggered") { pMaster->TimeAdvancePointTriggered = bVal; return; }
        if (flagName == "OFBRCratesStolen") { pMaster->OFBRCratesStolen = bVal; return; }
        if (flagName == "OFBR_NoCratesWereStolen") { pMaster->OFBR_NoCratesWereStolen = bVal; return; }
        if (flagName == "DarkwoodAllTradersAlive") { pMaster->DarkwoodAllTradersAlive = bVal; return; }
        if (flagName == "MadBomberNoBombsExplode") { pMaster->MadBomberNoBombsExplode = bVal; return; }
        if (flagName == "HobbeCavePerpertratorKilled") { pMaster->HobbeCavePerpertratorKilled = bVal; return; }
        if (flagName == "HobbeCaveBoyUnharmed") { pMaster->HobbeCaveBoyUnharmed = bVal; return; }
        if (flagName == "BanditCampKillNoBandits") { pMaster->BanditCampKillNoBandits = bVal; return; }
        if (flagName == "BreakSiegeNoAssistance") { pMaster->BreakSiegeNoAssistance = bVal; return; }
        if (flagName == "BreakSiegeKilledLeader") { pMaster->BreakSiegeKilledLeader = bVal; return; }
        if (flagName == "OrchardFarmBanditKilled") { pMaster->OrchardFarmBanditKilled = bVal; return; }
        if (flagName == "OrchardFarmGuardKilled") { pMaster->OrchardFarmGuardKilled = bVal; return; }
        if (flagName == "BanditCampKillManyBandits") { pMaster->BanditCampKillManyBandits = bVal; return; }
        if (flagName == "HobbeContestKillMoreHobbes") { pMaster->HobbeContestKillMoreHobbes = bVal; return; }
        if (flagName == "GraveyardKillAllInnocents") { pMaster->GraveyardKillAllInnocents = bVal; return; }
        if (flagName == "HangingTreeBanditKilled") { pMaster->HangingTreeBanditKilled = bVal; return; }
        if (flagName == "HangingTreeGuardKilled") { pMaster->HangingTreeGuardKilled = bVal; return; }
        if (flagName == "TCGKillNoBandits") { pMaster->TCGKillNoBandits = bVal; return; }
        if (flagName == "TCGMadeTimeLimit") { pMaster->TCGMadeTimeLimit = bVal; return; }
        if (flagName == "TCEKeepBanditFollowerAlive") { pMaster->TCEKeepBanditFollowerAlive = bVal; return; }
        if (flagName == "TCEMadeTimeLimit") { pMaster->TCEMadeTimeLimit = bVal; return; }
        if (flagName == "RansomVictimVictimNoDamage") { pMaster->RansomVictimVictimNoDamage = bVal; return; }
        if (flagName == "RansomVictimKidnappersKilled") { pMaster->RansomVictimKidnappersKilled = bVal; return; }
        if (flagName == "RansomVictimSaveVictim") { pMaster->RansomVictimSaveVictim = bVal; return; }
        if (flagName == "RansomVictimHaveVictimKilled") { pMaster->RansomVictimHaveVictimKilled = bVal; return; }
        if (flagName == "MinionCampBriarNoDamage") { pMaster->MinionCampBriarNoDamage = bVal; return; }
        if (flagName == "FireHeartFreeAllPrisoners") { pMaster->FireHeartFreeAllPrisoners = bVal; return; }
        if (flagName == "FireHeartKillAllPrisoners") { pMaster->FireHeartKillAllPrisoners = bVal; return; }
        if (flagName == "TCETimeLimitBoastTaken") { pMaster->TCETimeLimitBoastTaken = bVal; return; }
        if (flagName == "HeroWoreMask") { pMaster->HeroWoreMask = bVal; return; }
    }
    else if (value.is<int>()) {
        int iVal = value.as<int>();
        if (flagName == "PostSavePosition") { pMaster->PostSavePosition = static_cast<EGameflowPosition>(iVal); return; }
        if (flagName == "RandomEntityVillagerMax") { pMaster->RandomEntityVillagerMax = iVal; return; }
        if (flagName == "RandomEntityGuardMax") { pMaster->RandomEntityGuardMax = iVal; return; }
        if (flagName == "RandomEntityBanditMax") { pMaster->RandomEntityBanditMax = iVal; return; }
        if (flagName == "RandomEntityVillagerCount") { pMaster->RandomEntityVillagerCount = iVal; return; }
        if (flagName == "RandomEntityGuardCount") { pMaster->RandomEntityGuardCount = iVal; return; }
        if (flagName == "RandomEntityBanditCount") { pMaster->RandomEntityBanditCount = iVal; return; }
        if (flagName == "ArcheryStateCurrent") { pMaster->ArcheryStateCurrent = iVal; return; }
        if (flagName == "ArcheryStateRequired") { pMaster->ArcheryStateRequired = iVal; return; }
        if (flagName == "ArcheryHighScore") { pMaster->ArcheryHighScore = iVal; return; }
        if (flagName == "OrchardFarmRaidLastCompleted") { pMaster->OrchardFarmRaidLastCompleted = iVal; return; }
        if (flagName == "OrchardFarmTraderEscortCounter") { pMaster->OrchardFarmTraderEscortCounter = iVal; return; }
        if (flagName == "DeliveredSoul") { pMaster->DeliveredSoul = iVal; return; }
        if (flagName == "HeroDrunkness") { pMaster->HeroDrunkness = iVal; return; }
        if (flagName == "JackBossBattleResult") { pMaster->JackBossBattleResult = iVal; return; }
        if (flagName == "CurrentScene") { pMaster->CurrentScene = iVal; return; }
        if (flagName == "CurrentChapter") { pMaster->CurrentChapter = iVal; return; }
        if (flagName == "SkillScore") { pMaster->SkillScore = iVal; return; }
        if (flagName == "HighestSkillScore") { pMaster->HighestSkillScore = iVal; return; }
        if (flagName == "WillScore") { pMaster->WillScore = iVal; return; }
        if (flagName == "GlobalMeleeGrade") { pMaster->GlobalMeleeGrade = iVal; return; }
        if (flagName == "GlobalSkillGrade") { pMaster->GlobalSkillGrade = iVal; return; }
        if (flagName == "GlobalWillGrade") { pMaster->GlobalWillGrade = iVal; return; }
        if (flagName == "AmbushTradersKillCount") { pMaster->AmbushTradersKillCount = iVal; return; }
        if (flagName == "AmbushTradersBanditHireCount") { pMaster->AmbushTradersBanditHireCount = iVal; return; }
        if (flagName == "AchievementsWorthyOfSong") { pMaster->AchievementsWorthyOfSong = iVal; return; }
        if (flagName == "PrisonRaceNumber") { pMaster->PrisonRaceNumber = iVal; return; }
        if (flagName == "MaxChickenKickingScore") { pMaster->MaxChickenKickingScore = iVal; return; }
        if (flagName == "StoryTellerSpecialStories") { pMaster->StoryTellerSpecialStories = iVal; return; }
        if (flagName == "StoryTellerToldSpecialStories") { pMaster->StoryTellerToldSpecialStories = iVal; return; }
        if (flagName == "HobbeCaveInnocentSacrificed") { pMaster->HobbeCaveInnocentSacrificed = iVal; return; }
        if (flagName == "DarkwoodPickpocketedAllTraders") { pMaster->DarkwoodPickpocketedAllTraders = iVal; return; }
    }
    else if (value.is<float>()) {
        float fVal = value.as<float>();
        if (flagName == "RandomEntityVillagerPercentage") { pMaster->RandomEntityVillagerPercentage = fVal; return; }
        if (flagName == "RandomEntityGuardPercentage") { pMaster->RandomEntityGuardPercentage = fVal; return; }
        if (flagName == "RandomEntityBanditPercentage") { pMaster->RandomEntityBanditPercentage = fVal; return; }
        if (flagName == "VillagerAngryRating") { pMaster->VillagerAngryRating = fVal; return; }
    }
    else if (value.is<std::string>()) {
        if (flagName == "TeddySolution" && CCharString_AssignLiteral_API) {
            const std::string stringValue = value.as<std::string>();
            CCharString_AssignLiteral_API(&pMaster->TeddySolution, stringValue.c_str());
            return;
        }
    }
    Log("! SetMasterGameState WARNING: Unknown flag or unsupported type: '" + flagName + "'");
}
