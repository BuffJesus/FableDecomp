#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
static std::vector<std::string> events;
static CScriptThing speaker{}, hero{};
static bool throwLookup, throwLine;
static bool setupMode, soundIn2D, playDuringCutscene;
static int setupFailure, conversationResult = 73;
static bool __fastcall unexpectedValidityQuery(CScriptThing*, void*) {
    throw std::runtime_error("extra host validity query");
}
class LuaQuestState {
public:
    CGameScriptInterfaceBase* m_pGameInterface = reinterpret_cast<CGameScriptInterfaceBase*>(1);
    void AddConversationLineToHero(int, const std::string&, CScriptThing*, bool);
    int StartConversationWithHero(CScriptThing*, bool, bool);
};
static void __fastcall keyCtor(CCharString* key, void* unused, const char* value, int length) {
    events.push_back("key"); stringCtor(key, unused, value, length);
}
static void __fastcall keyDtor(CCharString* key, void* unused) {
    events.push_back("destroy-key"); stringDtor(key, unused);
}
static CScriptThing* __fastcall getHero(CGameScriptInterfaceBase* game, void*) {
    check(game == reinterpret_cast<CGameScriptInterfaceBase*>(1) && strings.size() == (setupMode ? 0u : 1u));
    events.push_back("hero");
    if (setupMode ? setupFailure == 2 : throwLookup) throw std::runtime_error("lookup failure");
    return &hero;
}
static void __fastcall addLine(CGameScriptInterfaceBase* game, void*, int conversation,
    const CCharString* key, bool subtitle, const CScriptThing* actor, const CScriptThing* listener) {
    check(game == reinterpret_cast<CGameScriptInterfaceBase*>(1));
    check(conversation == 73 && !subtitle && actor == &speaker && listener == &hero);
    check(strings.at(const_cast<CCharString*>(key)) == "TEXT_QST_048_TRADER_ROLL_UP");
    events.push_back("line");
    if (throwLine) throw std::runtime_error("line failure");
}
tGetHero GetHero_API = reinterpret_cast<tGetHero>(&getHero);
tAddLineToConversation AddLineToConversation_API = reinterpret_cast<tAddLineToConversation>(&addLine);
static int __fastcall newConversation(CGameScriptInterfaceBase* game, void*, const CScriptThing* actor, bool a, bool b) {
    check(game == reinterpret_cast<CGameScriptInterfaceBase*>(1) && actor == &speaker);
    check(a == soundIn2D && b == playDuringCutscene && strings.empty());
    events.push_back("create");
    if (setupFailure == 1) throw std::runtime_error("create failure");
    return conversationResult;
}
static void __fastcall addPerson(CGameScriptInterfaceBase* game, void*, int conversation, const CScriptThing* person) {
    check(game == reinterpret_cast<CGameScriptInterfaceBase*>(1));
    check(conversation == conversationResult && person == &hero && strings.empty());
    events.push_back("person");
    if (setupFailure == 3) throw std::runtime_error("person failure");
}
tAddNewConversation AddNewConversation_API = reinterpret_cast<tAddNewConversation>(&newConversation);
tAddPersonToConversation AddPersonToConversation_API = reinterpret_cast<tAddPersonToConversation>(&addPerson);
#include "book_trader_conversation_adapter.inc"
int main() {
    try {
        static_assert(sizeof(void*) == 4, "retail x86 ABI required");
        CScriptThingVTable table{};
        table.IsNull = reinterpret_cast<decltype(table.IsNull)>(&unexpectedValidityQuery);
        speaker.pVTable = hero.pVTable = reinterpret_cast<void**>(&table);
        CCharString_Construct_Literal = reinterpret_cast<tCCharString_Constructor_Literal>(&keyCtor);
        CCharString_Destroy = reinterpret_cast<tCCharString_Destructor>(&keyDtor);
        LuaQuestState host;
        sol::state lua; lua.open_libraries(sol::lib::base);
        lua.new_usertype<LuaQuestState>("Quest", "AddConversationLineToHero", &LuaQuestState::AddConversationLineToHero,
            "StartConversationWithHero", &LuaQuestState::StartConversationWithHero);
        lua["quest"] = &host; lua["speaker"] = &speaker;
        for (bool populated : {false, true}) for (int failure = 0; failure < 3; ++failure) {
            hero.pImp.Data = populated ? reinterpret_cast<decltype(hero.pImp.Data)>(7) : nullptr;
            throwLookup = failure == 1; throwLine = failure == 2; events.clear();
            auto result = lua.safe_script("quest:AddConversationLineToHero(73, 'TEXT_QST_048_TRADER_ROLL_UP', speaker, false)", sol::script_pass_on_error);
            check(result.valid() == (failure == 0) && strings.empty());
            check(events == (throwLookup ? std::vector<std::string>{"key", "hero", "destroy-key"} :
                std::vector<std::string>{"key", "hero", "line", "destroy-key"}));
            check(hero.pImp.Data == (populated ? reinterpret_cast<decltype(hero.pImp.Data)>(7) : nullptr));
        }
        setupMode = true;
        for (bool populated : {false, true}) for (int id : {-1, 0, 73})
        for (bool a : {false, true}) for (bool b : {false, true}) for (int failure = 0; failure < 4; ++failure) {
            hero.pImp.Data = populated ? reinterpret_cast<decltype(hero.pImp.Data)>(7) : nullptr;
            setupFailure = failure; conversationResult = id; soundIn2D = a; playDuringCutscene = b;
            events.clear(); lua["soundIn2D"] = a; lua["playDuringCutscene"] = b;
            auto result = lua.safe_script("return quest:StartConversationWithHero(speaker, soundIn2D, playDuringCutscene)", sol::script_pass_on_error);
            check(result.valid() == (failure == 0));
            if (failure == 0) check(result.get<int>() == id);
            std::vector<std::string> expected{"create"};
            if (failure != 1) expected.push_back("hero");
            if (failure == 0 || failure == 3) expected.push_back("person");
            check(events == expected && strings.empty());
            check(hero.pImp.Data == (populated ? reinterpret_cast<decltype(hero.pImp.Data)>(7) : nullptr));
        }
        std::cout << "PASS: x86 binding preserves key/hero/line/destruction order, borrowed identity, empty value, exception cleanup\n";
        std::cout << "PASS: create/hero/person order, both flags, negative IDs, empty participants, staged failures\n";
        return 0;
    } catch (const std::exception& error) { std::cerr << error.what() << '\n'; return 1; }
}
