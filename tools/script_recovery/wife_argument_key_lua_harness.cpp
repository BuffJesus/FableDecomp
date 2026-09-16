#define main argument_key_cpp_main
#include "wife_argument_key_harness.cpp"
#undef main
#include "retail_wife_argument_key_lua.h"

int main() {
    argument_key_cpp_main();
    WifeArgumentKeyAPIs api{Number,reinterpret_cast<tCCharString_Constructor_Literal>(Literal),Concat,
        reinterpret_cast<tCCharString_Destructor>(Destroy),reinterpret_cast<tCCharString_AssignmentLiteral>(Assign),
        reinterpret_cast<tTextEntryExists>(Exists),reinterpret_cast<tAddLineToConversation>(Line)};
    sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::string);
    RegisterRetailWifeArgumentKey(lua);
    lua.set_function("withKey",[&](int number,sol::protected_function callback){return WithRetailWifeArgumentKey(game,number,callback,api);});
    lua.set_function("addLine",[](RetailWifeArgumentKey& key){key.AddLine(-7,wife,husband);});
    for(bool exists:{false,true}) {
        textExists=exists;
        lua.script(R"(
            assert(withKey(50,function(key)
                escaped=key
                if not key:Exists() then key:ResetToFirst() end
                addLine(key)
                return true
            end))
            assert(not pcall(function() escaped:Exists() end))
            assert(not pcall(function() escaped:ResetToFirst() end))
            assert(not pcall(function() addLine(escaped) end))
        )");
        assert(live.empty());
    }
    lua.script(R"(
        assert(withKey(10,function(key) escaped=key;return false end)==false)
        assert(not pcall(function() escaped:Exists() end))
        local ok,err=pcall(function()
            withKey(20,function(key) escaped=key;error('original callback failure') end)
        end)
        assert(not ok and string.find(err,'original callback failure',1,true))
        assert(not pcall(function() escaped:Exists() end))
        assert(not pcall(function() withKey(30,function(key) escaped=key;return nil end) end))
        assert(not pcall(function() escaped:Exists() end))
    )");
    assert(live.empty());
    std::cout<<"PASS: real Lua key scope, continuation boolean, text lookup/reset, shared native identity, escaped-key rejection and callback-error cleanup\n";
}
