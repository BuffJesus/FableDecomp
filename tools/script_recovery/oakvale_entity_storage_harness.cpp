#define main TheresaReferenceMain
#include "theresa_scope_runtime_harness.cpp"
#undef main
#include <set>

static std::set<void*> entityMaps,entityMovies;
static std::map<CCharString*,std::string> entityTexts;
static std::vector<std::string> entityClosed;
static void* __cdecl entityAllocate(size_t size){return ::operator new(size);}
static void __cdecl entityFree(void* value){::operator delete(value);}
static void __fastcall entityMapNew(void* value,void*){check(entityMaps.insert(value).second);}
static void __fastcall entityMapClose(void* value,void*){check(entityMaps.erase(value)==1);entityClosed.emplace_back("map");}
static CCharString* __fastcall entityBracket(void*,void*,const CCharString*){throw std::runtime_error("unexpected bracket");}
static void __fastcall entityAssign(CCharString*,void*,const char*){throw std::runtime_error("unexpected assignment");}
decltype(StdMap_String_Construct_API) StdMap_String_Construct_API=reinterpret_cast<decltype(StdMap_String_Construct_API)>(&entityMapNew);
decltype(StdMap_String_Destroy_API) StdMap_String_Destroy_API=reinterpret_cast<decltype(StdMap_String_Destroy_API)>(&entityMapClose);
decltype(StdMap_String_OperatorBracket_API) StdMap_String_OperatorBracket_API=reinterpret_cast<decltype(StdMap_String_OperatorBracket_API)>(&entityBracket);
decltype(CCharString_AssignLiteral_API) CCharString_AssignLiteral_API=reinterpret_cast<decltype(CCharString_AssignLiteral_API)>(&entityAssign);
static void __fastcall entityTextNew(CCharString* value,void*,const char* text,int length){check(length==-1&&entityTexts.emplace(value,text).second);value->pStringData=reinterpret_cast<decltype(value->pStringData)>(1);}
static void __fastcall entityTextClose(CCharString* value,void*){check(entityTexts.erase(value)==1);entityClosed.emplace_back("text");}
static void __fastcall entityMovieNew(void* value,void*){check(entityMovies.insert(value).second);}
static void __fastcall entityMovieClose(void* value,void*){check(entityMovies.erase(value)==1);entityClosed.emplace_back("movie");}
template<class F> static void rejectsEntity(F call){bool rejected=false;try{call();}catch(const std::exception&){rejected=true;}check(rejected);}

int main(){try{
    Game_malloc=reinterpret_cast<decltype(Game_malloc)>(&entityAllocate);Game_free=reinterpret_cast<decltype(Game_free)>(&entityFree);
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&entityTextNew);
    CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&entityTextClose);
    CBaseObject_Construct_API=reinterpret_cast<decltype(CBaseObject_Construct_API)>(&entityMovieNew);
    MovieResource_Destroy_API=reinterpret_cast<decltype(MovieResource_Destroy_API)>(&entityMovieClose);
    sol::state lua;lua.open_libraries(sol::lib::base);
    auto type=lua.new_usertype<LuaRetailResources>("Scope",sol::no_constructor);
    type["NewLiteralText"]=&LuaRetailResources::NewLiteralText;type["DestroyText"]=&LuaRetailResources::DestroyText;
    type["NewStringMap"]=&LuaRetailResources::NewStringMap;type["DestroyStringMap"]=&LuaRetailResources::DestroyStringMap;
    type["NewMovie"]=&LuaRetailResources::NewMovie;type["DestroyMovie"]=&LuaRetailResources::DestroyMovie;
    type["Close"]=&LuaRetailResources::Close;
    {LuaRetailResources scope(&game);lua["scope"]=&scope;
        lua.script(R"(
            local movie=scope:NewMovie()
            assert(not pcall(function()scope:NewMovie()end))
            local previous=movie
            for i=1,1000 do
                local text=scope:NewLiteralText('test')
                local map=scope:NewStringMap()
                assert(text>previous and map>text)
                scope:DestroyText(text)
                scope:DestroyStringMap(map)
                assert(not pcall(function()scope:DestroyText(text)end))
                assert(not pcall(function()scope:DestroyStringMap(map)end))
                previous=map
            end
            scope:DestroyMovie(movie)
            local replacement=scope:NewMovie()
            assert(replacement>previous)
            assert(not pcall(function()scope:DestroyMovie(movie)end))
            scope:DestroyMovie(replacement)
        )");
        check(entityMaps.empty()&&entityTexts.empty()&&entityMovies.empty());
        entityClosed.clear();scope.NewLiteralText("last");scope.NewStringMap();scope.NewMovie();
        scope.Close();scope.Close();check(entityClosed==std::vector<std::string>({"movie","map","text"}));
        rejectsEntity([&]{scope.NewLiteralText("closed");});rejectsEntity([&]{scope.NewStringMap();});rejectsEntity([&]{scope.NewMovie();});
    }
    check(entityMaps.empty()&&entityTexts.empty()&&entityMovies.empty());lua["scope"]=sol::nil;
    std::cout<<"PASS: 1000 Lua text/map allocation-release cycles, movie exclusivity, stale handles, reverse close and closed-owner rejection\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
