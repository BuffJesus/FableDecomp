// Checked host method snapshot, actual x86 API types, and deterministic engine doubles.
#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static std::map<void*, CScriptGameResourceObjectScriptedThingBase*> g_controlHandlesByEntityData;
static unsigned controlFrames, failAttempts, cancelFrame;
static bool populateFailure;
static std::vector<void*> outputAddresses;
std::string GetLogFilePath() { return ""; }
tIsActiveThreadTerminating_Entity IsActiveThreadTerminating_Entity_API = reinterpret_cast<tIsActiveThreadTerminating_Entity>(1);
tIsActiveThreadTerminating_Quest IsActiveThreadTerminating_Quest_API = nullptr;
class LuaEntityAPI {
public:
    CScriptGameResourceObjectScriptedThingBase* m_pControlHandle = nullptr;
    void* m_pControlledEntityData = nullptr;
    void* m_pEntityHost = reinterpret_cast<void*>(1);
    void* m_pQuestHost = nullptr;
    CGameScriptInterfaceBase* m_pGameInterface = reinterpret_cast<CGameScriptInterfaceBase*>(1);
    std::map<void*, CScriptGameResourceObjectScriptedThingBase*> m_ownedControlHandles, m_borrowedControlHandles;
    std::map<void*, int> m_ownedControlPriorities, m_ownedControlDepth;
    void SelectControlHandle(CScriptThing*);
    bool AcquireControl(CScriptThing*, sol::optional<int>);
    void ReleaseControl(CScriptThing*);
    bool IsThreadTerminating() { return cancelFrame && controlFrames >= cancelFrame; }
};
#include "scythe_control_host_snapshot.inc"
static bool __fastcall controlledAcquire(CGameScriptInterfaceBase* game, void*, const CScriptThing* thing,
        CScriptGameResourceObjectScriptedThingBase* resource, EScriptAIPriority priority) {
    outputAddresses.push_back(resource);
    bool result = outputAddresses.size() > failAttempts;
    acquireOK = result || populateFailure;
    acquire(game, nullptr, thing, resource, priority);
    return result;
}
static void __fastcall controlFrame(CGameScriptInterfaceBase*, void*) { ++controlFrames; }
int main() {
    try {
        static_assert(sizeof(void*) == 4, "Retail ABI requires x86");
        StartScriptingEntity_API = reinterpret_cast<tStartScriptingEntity>(&controlledAcquire);
        NewScriptFrame_API = reinterpret_cast<tNewScriptFrame>(&controlFrame);
        CScriptThing thing{};
        thing.pImp.Data = reinterpret_cast<decltype(thing.pImp.Data)>(&thing);
        for (bool populated : {false, true}) for (unsigned cancel : {0u, 1u, 2u}) {
            LuaEntityAPI api;
            outputAddresses.clear(); controlFrames = 0; failAttempts = 2;
            cancelFrame = cancel; populateFailure = populated;
            auto beforeDestroy = destroys;
            bool result = api.AcquireControl(&thing, 4);
            check(result == !cancel);
            check(outputAddresses.size() == (cancel ? cancel : 3));
            check(controlFrames == (cancel ? cancel : 2));
            for (auto* p : outputAddresses) check(p == outputAddresses.front());
            if (result) {
                check(destroys == beforeDestroy);
                check(g_controlHandlesByEntityData.at(thing.pImp.Data) == api.m_pControlHandle);
                api.ReleaseControl(&thing);
            }
            check(destroys == beforeDestroy + 1);
            check(!api.m_pControlHandle && api.m_ownedControlHandles.empty());
            check(g_controlHandlesByEntityData.empty() && locals.empty());
            for (auto ref : refs) check(ref.second == 0);
        }
        // Borrowed ownership must remain with the other VM on release.
        CScriptGameResourceObjectScriptedThingBase borrowed{};
        borrowed.pImp.Data = &borrowed;
        g_controlHandlesByEntityData[thing.pImp.Data] = &borrowed;
        LuaEntityAPI borrower;
        auto beforeAttempts = attempts, beforeDestroy = destroys;
        check(borrower.AcquireControl(&thing, 4));
        borrower.ReleaseControl(&thing);
        check(attempts == beforeAttempts && destroys == beforeDestroy);
        check(g_controlHandlesByEntityData.at(thing.pImp.Data) == &borrowed);
        g_controlHandlesByEntityData.clear();
        // An existing owned resource is retained until the outer release.
        LuaEntityAPI nested;
        cancelFrame = 0; failAttempts = 0; outputAddresses.clear();
        check(nested.AcquireControl(&thing, 4));
        auto* outer = nested.m_pControlHandle;
        beforeAttempts = attempts; beforeDestroy = destroys;
        check(nested.AcquireControl(&thing, 4));
        nested.ReleaseControl(&thing);
        check(attempts == beforeAttempts && destroys == beforeDestroy && nested.m_pControlHandle == outer);
        nested.ReleaseControl(&thing);
        check(destroys == beforeDestroy + 1 && g_controlHandlesByEntityData.empty());
        std::cout << "PASS: actual host retry/cancellation, same output across retries, empty/populated failure lifetime, borrowed and nested owner retention\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
