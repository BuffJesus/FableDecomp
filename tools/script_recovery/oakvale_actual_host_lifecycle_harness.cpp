#include "LuaQuestHost.h"
#include "LuaQuestState.h"
#include "LuaEntityHost.h"
#include "LuaManager.h"
#include "EntityScriptingAPI.h"
#include <iostream>
#include <set>
#include <cstring>
#include <fstream>
#include <filesystem>

static CGameScriptInterfaceBase hostGame{};
static void* hostTable[512]{};
static std::set<int> liveTimers;
static std::vector<CSpawnedFunc*> spawned;
static std::vector<std::string> threadSections;
extern std::vector<std::string> g_entityScriptFileNames;
static std::vector<std::string> bindingNames,mainFlow;
static std::vector<CEntityScriptBindingBase*> ownedBindings;
static int emittedMainCases=0;
static void cleanupBindings();
static std::vector<std::string> lifecycle;
static std::shared_ptr<RetailVillagerSpeechLists> observedLists;
static LuaQuestHost* observedHost=nullptr;
static int registrationCalls=0,failRegistration=0;
static int expectedSpeechCount=0,initCases=0,ambientResets=0;
static std::set<void*> speechAllocations;
static bool nativeCase=true,constructorFailure=false;
static void ensureAt(bool value,int line){if(!value)throw std::runtime_error("Actual host lifecycle assertion failed at line "+std::to_string(line));}
#define ensure(...) ensureAt((__VA_ARGS__),__LINE__)
static int __fastcall registerTimer(CGameScriptInterfaceBase* game,void*){
    ensure(game==&hostGame);++registrationCalls;
    if(registrationCalls==failRegistration)throw std::runtime_error("REGISTER_FAILURE");
    int id=registrationCalls==1?-7:0;ensure(liveTimers.insert(id).second);return id;
}
static void __fastcall closeTimer(CGameScriptInterfaceBase* game,void*,int id){ensure(game==&hostGame&&liveTimers.erase(id)==1);lifecycle.push_back("timer:"+std::to_string(id));}
static void __fastcall resetAmbient(CGameScriptInterfaceBase* game,void*,int id,int value){ensure(game==&hostGame&&liveTimers.count(id)&&id==-7&&value==0);++ambientResets;}
static void __fastcall constructBase(CScriptBase_Retail* base,void*){std::memset(base,0,sizeof(*base));}
static void __fastcall destroyBase(CScriptBase_Retail*,void*){
    ensure(liveTimers.empty());
    if(!constructorFailure){
        ensure(lifecycle.size()>=3&&lifecycle[0]=="gc"&&lifecycle[1]=="timer:0"&&lifecycle[2]=="timer:-7");
        bool closed=false;try{observedLists->Count("good",true);}catch(...){closed=true;}ensure(closed);
    }
    cleanupBindings();for(auto* function:spawned)std::free(function);spawned.clear();lifecycle.emplace_back("base");
}
static bool registeringMain=false;
static int allocationFailure=0,keyFailure=0,keyCalls=0,mainCalls=0,threadCalls=0;
static bool terminationValue=false;
static int questQueries=0,gameQueries=0,frameCalls=0,frameCases=0;
static LuaEntityHost* expectedEntity=nullptr;
static int entityQueries=0,entityFrameCases=0,entityCallbacks=0;
static int childFinalizers=0;
static int nativeEntityCallbackCases=0,legacyEntityCallbackCases=0;
static int entityFileCases=0,emptyEntityInitCases=0,barrelCallbackCases=0,entityPositionQueries=0;
static const C3DVector* __fastcall callbackPosition(CGameScriptThing* thing,void*){++entityPositionQueries;return &thing->Pos;}
static void* schedulerFiber=nullptr;
static int fiberCases=0;
struct FiberProbe { CSpawnedFunc* function; bool finished=false; };
static FiberProbe shutdownProbes[2];
static void* shutdownFibers[2]{};
static CSpawnedFunc shutdownProcesses[2]{};
static void* shutdownProcessTable[2]{};
static int shutdownCallbacks=0,drainCalls=0;
static void WINAPI runFiberProbe(void* raw){
    auto* probe=static_cast<FiberProbe*>(raw);
    using Thunk=void(__thiscall*)(CScriptBase_Retail*);
    reinterpret_cast<Thunk>(probe->function->pThunkToMain)(probe->function->pOwnerScript);
    probe->finished=true;SwitchToFiber(schedulerFiber);
    std::abort();
}
static bool allocatingEntityReference=false;
static int entityReferenceFailure=0,entityAllocationCases=0;
static std::set<void*> entityReferenceBlocks;
static bool __fastcall entityTerminating(LuaEntityHost* entity,void*){ensure(entity==expectedEntity);++entityQueries;return terminationValue;}
static bool __fastcall questTerminating(CScriptBase_Retail* owner,void*){ensure(owner==&observedHost->base);++questQueries;return terminationValue;}
static bool __fastcall gameTerminating(CGameScriptInterfaceBase* game,void*){ensure(game==&hostGame);++gameQueries;return terminationValue;}
static void __fastcall frameCall(CGameScriptInterfaceBase* game,void*){ensure(game==&hostGame);++frameCalls;if(schedulerFiber)SwitchToFiber(schedulerFiber);}
static void __fastcall resumeShutdownProcess(CSpawnedFunc* process,void*){
    const int index=static_cast<int>(process-shutdownProcesses);ensure(index==0||index==1);
    ensure(process->unknown_base_padding[1]==1&&observedHost->IsClosing());
    SwitchToFiber(shutdownFibers[index]);
    ensure(shutdownProbes[index].finished);process->unknown_base_padding[0]=0;
}
static void __fastcall drainProcesses(CScriptBase_Retail* owner,void*){
    ensure(observedHost&&owner==&observedHost->base&&observedHost->IsClosing());
    ensure(observedHost->GetLuaState()&&observedHost->GetQuestState()&&liveTimers.size()==2);
    ensure(lifecycle.empty());++drainCalls;
    if(!schedulerFiber)return;
    terminationValue=true;
    for(int index:{0,1}){
        using Terminate=void(__thiscall*)(CSpawnedFunc*);
        ASLR<Terminate>(0xa4b200)(&shutdownProcesses[index]);
        ensure(shutdownProbes[index].finished);DeleteFiber(shutdownFibers[index]);shutdownFibers[index]=nullptr;
    }
    schedulerFiber=nullptr;ensure(ConvertFiberToThread()!=0);terminationValue=false;
}
static std::string expectedFunctionName="Main",expectedSectionName;
static bool persistenceReading=false,persistencePresent=false,persistenceSaved=false;
static int persistenceTransfers=0,persistenceCases=0;
static void* persistenceContext=reinterpret_cast<void*>(0x123456);
static void __fastcall transferOakvale(CPersistContext* context,void*,const char* key,bool* value,const bool* fallback){
    ensure(context==persistenceContext&&std::string(key)=="AttackOver"&&value&&fallback&&!*fallback);
    ++persistenceTransfers;
    if(persistenceReading)*value=persistencePresent?persistenceSaved:*fallback;
    else {persistenceSaved=*value;persistencePresent=true;}
}
static std::map<CCharString*,std::string> registrationKeys;
static std::set<void*> unregisteredAllocations;
static void* __cdecl hostMalloc(size_t size){
    if(allocatingEntityReference){ensure(size==12);if(entityReferenceFailure==1)return nullptr;if(entityReferenceFailure==2)throw std::bad_alloc();}
    if(registeringMain){ensure(size==sizeof(CSpawnedFunc));if(allocationFailure==1)return nullptr;if(allocationFailure==2)throw std::bad_alloc();}
    auto* result=std::malloc(size);if(registeringMain)unregisteredAllocations.insert(result);if(allocatingEntityReference)entityReferenceBlocks.insert(result);return result;
}
static void __cdecl freeUnregistered(void* pointer){ensure(unregisteredAllocations.erase(pointer)+entityReferenceBlocks.erase(pointer)==1);std::free(pointer);}
static void __fastcall destroyUnregistered(CSpawnedFunc* function,void*){ensure(unregisteredAllocations.count(function)==1);}
static void __fastcall constructThread(CSpawnedFunc* function,void*,const CCharString*,int zero){ensure(zero==0);std::memset(function,0,sizeof(*function));}
static void __fastcall addThread(CScriptBase_Retail* owner,void*,CSpawnedFunc* function,const CCharString* section){
    if(registeringMain){
        ensure(owner==&observedHost->base&&function->pOwnerScript==owner);
        auto* expectedThunk=expectedFunctionName=="Main"?ASLR<void*>(0xcdd440):GetMemberFunctionAddress(&LuaQuestHost::ThreadRunner<0>);
        ensure(function->pVTable==ASLR<void**>(0x12d7a3c)&&function->pThunkToMain==expectedThunk);
        ensure(registrationKeys.size()==2&&registrationKeys.at(const_cast<CCharString*>(section))==expectedSectionName);
        ensure(unregisteredAllocations.erase(function)==1);
    }
    spawned.push_back(function);
    threadSections.push_back(registrationKeys.at(const_cast<CCharString*>(section)));
}
static void __fastcall constructKey(CCharString* key,void*,const char* text,int length){
    ensure(length==-1);if(registeringMain){++keyCalls;if(keyCalls==keyFailure)throw std::runtime_error("KEY_FAILURE");ensure(std::string(text)==(keyCalls==1?expectedFunctionName:expectedSectionName));}
    registrationKeys[key]=text;key->pStringData=reinterpret_cast<decltype(key->pStringData)>(1);
}
static void __fastcall destroyKey(CCharString* key,void*){ensure(registrationKeys.erase(key)==1);key->pStringData=nullptr;}
static void cleanupBindings(){for(auto* binding:ownedBindings){destroyKey(&binding->EntityScriptName,nullptr);std::free(binding);}ownedBindings.clear();}
static void __fastcall addBinding(CScriptBase_Retail* parent,void*,CEntityScriptBindingBase* binding){
    const auto index=ownedBindings.size();ensure(parent==&observedHost->base&&index<bindingNames.size());
    ensure(registrationKeys.at(&binding->EntityScriptName)==bindingNames[index]&&binding->pParentScript==parent);
    ensure(binding->pVTable==g_pEntityScriptBindingVTable&&binding->bSomething&&binding->unknown_zero==0);
    ensure(binding->pAllocFunc==GetEntityAllocatorForScript("NewOakValeIntro/Entities/"+bindingNames[index]));
    ownedBindings.push_back(binding);
}
static void __fastcall finishBaseBindings(CScriptBase_Retail* owner,void*){ensure(owner==&observedHost->base&&ownedBindings.size()==16);mainFlow.emplace_back("base.finalize");}
static void __fastcall finishGameBindings(CGameScriptInterfaceBase* game,void*){ensure(game==&hostGame&&mainFlow==std::vector<std::string>{"base.finalize"});mainFlow.emplace_back("game.finalize");}
static CCharString* __fastcall activeQuest(CGameScriptInterfaceBase* game,void*,CCharString* out){ensure(game==&hostGame);constructKey(out,nullptr,"active",-1);return out;}
static void __fastcall initialObjective(CGameScriptInterfaceBase* game,void*,const CCharString* name,const CCharString* objective,const CCharString* r1,const CCharString* r2){
    ensure(game==&hostGame&&registrationKeys.at(const_cast<CCharString*>(name))=="active");
    ensure(registrationKeys.at(const_cast<CCharString*>(objective))=="TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01");
    ensure(registrationKeys.at(const_cast<CCharString*>(r1)).empty()&&registrationKeys.at(const_cast<CCharString*>(r2)).empty());mainFlow.emplace_back("objective");
}
static void __fastcall deactivatePostAttack(CGameScriptInterfaceBase* game,void*,const CCharString* key,unsigned delay){ensure(game==&hostGame&&delay==0&&registrationKeys.at(const_cast<CCharString*>(key))=="Q__OakValeIntro_PostAttack");mainFlow.emplace_back("deactivate");}
static CCharString* __fastcall copySpeechKey(CCharString* out,void*,const CCharString* source){
    ensure(!registrationKeys.count(out));registrationKeys[out]=registrationKeys.at(const_cast<CCharString*>(source));*out=*source;return out;
}
struct SpeechVector {CCharString* begin;CCharString* end;CCharString* capacity;};
static void __fastcall insertSpeech(SpeechVector* vector,void*,CCharString* position,const CCharString* source,void*,unsigned count,bool append){
    ensure(position==vector->end&&count==1&&append&&!vector->begin);
    vector->begin=static_cast<CCharString*>(std::malloc(16*sizeof(CCharString)));ensure(vector->begin!=nullptr);
    speechAllocations.insert(vector->begin);vector->end=vector->begin;vector->capacity=vector->begin+16;
    copySpeechKey(vector->end++,nullptr,source);
}
static void __cdecl freeSpeech(void* pointer){ensure(speechAllocations.erase(pointer)==1);std::free(pointer);}
static void unexpectedInitResource(){throw std::runtime_error("Unexpected resource API during Oakvale Init");}
static int anchoredThreads(lua_State* state){int count=0;lua_pushnil(state);while(lua_next(state,LUA_REGISTRYINDEX)){if(lua_isthread(state,-1))++count;lua_pop(state,1);}return count;}
static void initializeUnusedResourceGuards(){
#define UNUSED_API(name) name=reinterpret_cast<decltype(name)>(&unexpectedInitResource);
    UNUSED_API(Game_free);UNUSED_API(CBaseObject_Construct_API);UNUSED_API(g_pCScriptGameResourceObjectScriptedThingBaseVTable);
    UNUSED_API(CSGROSTB_Destroy_API);UNUSED_API(InitScriptObjectHelper1_API);UNUSED_API(InitScriptObjectHelper2_API);
    UNUSED_API(StartScriptingEntity_API);UNUSED_API(StdMap_Construct_API);UNUSED_API(StdMap_Destroy_API);
    UNUSED_API(StdMap_OperatorBracket_API);UNUSED_API(CBaseObject_Assign_API);UNUSED_API(RunCutsceneMacro_Func);
    UNUSED_API(g_pMovieObjectVTable);UNUSED_API(StartMovieSequence_API);UNUSED_API(MovieResource_Destroy_API);
    UNUSED_API(PauseAllNonScriptedEntities_API);UNUSED_API(NewScriptFrame_API);
#undef UNUSED_API
}
static void loadNativeHelpers(const char* path){
    std::ifstream file(path,std::ios::binary);ensure(bool(file));
    auto word=[&](){unsigned value=0;file.read(reinterpret_cast<char*>(&value),4);ensure(bool(file));return value;};
    const unsigned count=word();ensure(count==7);
    for(unsigned i=0;i<count;++i){
        const unsigned address=word(),size=word(),fixes=word();ensure(address>=0x430000&&address+size<=0x1450000&&size<512&&fixes<16);
        auto* destination=ASLR<unsigned char*>(address);file.read(reinterpret_cast<char*>(destination),size);ensure(bool(file));
        for(unsigned j=0;j<fixes;++j){const unsigned offset=word();ensure(offset+4<=size);*reinterpret_cast<DWORD*>(destination+offset)+=g_fableBase-0x400000;}
        FlushInstructionCache(GetCurrentProcess(),destination,size);
    }
}
static void finalizer(){
    ensure(observedHost&&observedHost->GetQuestState());
    ensure(observedLists->Count("good",true)==expectedSpeechCount);
    if(nativeCase){ensure(liveTimers.size()==2);ensure(observedHost->GetQuestState()->GetStateInt("WatchTimer")==0);}
    lifecycle.emplace_back("gc");
}
static void redirect(DWORD address,void* target){auto* code=ASLR<unsigned char*>(address);code[0]=0xe9;*reinterpret_cast<int*>(code+1)=static_cast<int>(reinterpret_cast<unsigned char*>(target)-code-5);FlushInstructionCache(GetCurrentProcess(),code,5);}
static void reset(){ensure(liveTimers.empty()&&spawned.empty()&&speechAllocations.empty());registrationCalls=0;failRegistration=0;lifecycle.clear();threadSections.clear();observedLists.reset();observedHost=nullptr;constructorFailure=false;expectedSpeechCount=0;}
int main(int argc,char** argv){try{
    auto* arena=static_cast<unsigned char*>(VirtualAlloc(nullptr,0x1020000,MEM_RESERVE|MEM_COMMIT,PAGE_EXECUTE_READWRITE));ensure(arena!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(arena)-(0x430000-0x400000);
    redirect(0xcbd510,reinterpret_cast<void*>(&destroyBase));
    redirect(0xcb7f60,reinterpret_cast<void*>(&drainProcesses));
    redirect(0xbfea1a,reinterpret_cast<void*>(&hostMalloc));redirect(0xbfe9bc,reinterpret_cast<void*>(&freeUnregistered));
    redirect(0xcdd4c0,reinterpret_cast<void*>(&destroyUnregistered));
    redirect(0x433530,reinterpret_cast<void*>(&insertSpeech));redirect(0x99ec30,reinterpret_cast<void*>(&copySpeechKey));
    redirect(0xbfea14,reinterpret_cast<void*>(&freeSpeech));
    // Exact original CDD440 virtual-Main thunk, now targeting the real host vtable.
    const unsigned char mainThunk[]={0x8b,0x01,0xff,0x60,0x08};std::memcpy(ASLR<void*>(0xcdd440),mainThunk,sizeof(mainThunk));
    if(argc>3)loadNativeHelpers(argv[3]);
    if(argc>2)g_fseBasePath=std::filesystem::path(argv[2]).parent_path().parent_path().string();
    *reinterpret_cast<void***>(&hostGame)=hostTable;
    hostTable[0x15c/4]=reinterpret_cast<void*>(&registerTimer);hostTable[0x160/4]=reinterpret_cast<void*>(&closeTimer);
    hostTable[0x164/4]=reinterpret_cast<void*>(&resetAmbient);
    *ASLR<CGameScriptInterfaceBase**>(0x143e8f8)=&hostGame;
    CScriptBase_Construct=reinterpret_cast<decltype(CScriptBase_Construct)>(&constructBase);
    Game_malloc=reinterpret_cast<decltype(Game_malloc)>(&hostMalloc);
    CSpawnedFunc_Construct=reinterpret_cast<decltype(CSpawnedFunc_Construct)>(&constructThread);
    AddSpawnedFunction_func=reinterpret_cast<decltype(AddSpawnedFunction_func)>(&addThread);
    CCharString_Construct_Literal=reinterpret_cast<decltype(CCharString_Construct_Literal)>(&constructKey);
    CCharString_Destroy=reinterpret_cast<decltype(CCharString_Destroy)>(&destroyKey);
    IsActiveThreadTerminating_Quest_API=reinterpret_cast<decltype(IsActiveThreadTerminating_Quest_API)>(&questTerminating);
    IsActiveThreadTerminating_API=reinterpret_cast<decltype(IsActiveThreadTerminating_API)>(&gameTerminating);
    unsigned cases=0;
    for(unsigned flags:{0u,1u,2u,3u}){
        reset();nativeCase=true;
        auto* host=new LuaQuestHost(nullptr,&hostGame,argc>2?"NewOakValeIntro/NewOakValeIntro":"actual-host-lifecycle",NativeQuestLifetime::NewOakValeIntro);observedHost=host;
        std::vector<std::unique_ptr<LuaEntityHost>> retainedChildren;
        ensure(registrationCalls==2);observedLists=host->GetQuestState()->GetVillagerSpeechLists();
        auto* lua=host->GetLuaState();lua->set_function("observe_gc",&finalizer);
        if(argc>1)lua->script_file(argv[1]);
        registeringMain=true;expectedFunctionName="Main";expectedSectionName="";
        for(int failure:{1,2,3,4}){
            allocationFailure=failure<=2?failure:0;keyFailure=failure>=3?failure-2:0;keyCalls=0;
            bool rejected=false;try{host->RegisterMain();}catch(const std::exception&){rejected=true;}
            ensure(rejected&&spawned.empty()&&registrationKeys.empty()&&unregisteredAllocations.empty());
        }
        allocationFailure=keyFailure=keyCalls=0;host->RegisterMain();ensure(spawned.size()==1&&registrationKeys.empty());
        host->GetEnvironment()["Main"]=[&](LuaQuestState* quest){ensure(quest==host->GetQuestState());++mainCalls;};
        using MainThunk=void(__thiscall*)(CScriptBase_Retail*);
        terminationValue=true;questQueries=gameQueries=0;
        auto* mainFunction=spawned.back();reinterpret_cast<MainThunk>(mainFunction->pThunkToMain)(mainFunction->pOwnerScript);
        ensure(questQueries==0&&gameQueries==0);terminationValue=false;
        expectedFunctionName="threadProbe";expectedSectionName="StartOakVale";
        host->GetEnvironment()["threadProbe"]=[&](LuaQuestState* state,int argument){ensure(state==host->GetQuestState()&&argument==17);++threadCalls;};
        std::vector<sol::object> probeArgs{sol::make_object(*lua,17)};
        const int originalAnchors=anchoredThreads(lua->lua_state());
        for(int failure:{1,2,3,4}){
            allocationFailure=failure<=2?failure:0;keyFailure=failure>=3?failure-2:0;keyCalls=0;
            bool rejected=false;try{host->CreateThread("threadProbe","StartOakVale",probeArgs);}catch(const std::exception&){rejected=true;}
            ensure(rejected&&spawned.size()==1&&registrationKeys.empty()&&unregisteredAllocations.empty());
            ensure(anchoredThreads(lua->lua_state())==originalAnchors);
        }
        allocationFailure=keyFailure=keyCalls=0;host->CreateThread("threadProbe","StartOakVale",probeArgs);probeArgs.clear();
        ensure(spawned.size()==2&&registrationKeys.empty()&&anchoredThreads(lua->lua_state())==originalAnchors+1);
        terminationValue=true;questQueries=gameQueries=0;
        auto* threadFunction=spawned.back();reinterpret_cast<MainThunk>(threadFunction->pThunkToMain)(threadFunction->pOwnerScript);
        ensure(questQueries==0&&gameQueries==0);terminationValue=false;
        registeringMain=false;
        if(argc>2){
            NewScriptFrame_API=reinterpret_cast<decltype(NewScriptFrame_API)>(&frameCall);
            schedulerFiber=ConvertThreadToFiber(nullptr);ensure(schedulerFiber!=nullptr);
            for(bool reverse:{false,true})for(bool cancel:{false,true}){
                int ticks[2]={0,0},completed=0;
                lua->set_function("fiber_tick",[&](int id,int tick){ensure(id>=0&&id<2&&tick==++ticks[id]);});
                lua->set_function("fiber_done",[&](int id,int value){ensure(value==(id==0?41:17));++completed;});
                lua->script(R"lua(
                    function fiber_body(quest,id,value)
                        local identity={id=id,value=value}
                        for tick=1,3 do
                            fiber_tick(identity.id,tick)
                            quest:NewScriptFrame()
                            if quest:IsActiveThreadTerminating() then break end
                        end
                        fiber_done(identity.id,identity.value)
                    end
                    function Main(quest) fiber_body(quest,0,41) end
                    function threadProbe(quest,value) fiber_body(quest,1,value) end
                )lua",host->GetEnvironment());
                FiberProbe probes[2]={{mainFunction},{threadFunction}};
                void* fibers[2]={CreateFiber(0,&runFiberProbe,&probes[0]),CreateFiber(0,&runFiberProbe,&probes[1])};
                ensure(fibers[0]&&fibers[1]);terminationValue=false;
                const int first=reverse?1:0,second=1-first;
                SwitchToFiber(fibers[first]);SwitchToFiber(fibers[second]);
                ensure(ticks[0]==1&&ticks[1]==1&&completed==0);
                lua_gc(lua->lua_state(),LUA_GCCOLLECT,0);
                terminationValue=cancel;
                for(int round=0;round<(cancel?1:3);++round){
                    ensure(!probes[first].finished&&!probes[second].finished);
                    SwitchToFiber(fibers[second]);SwitchToFiber(fibers[first]);
                }
                ensure(probes[0].finished&&probes[1].finished&&completed==2);
                ensure(ticks[0]==(cancel?1:3)&&ticks[1]==(cancel?1:3));
                DeleteFiber(fibers[0]);DeleteFiber(fibers[1]);++fiberCases;
                (*lua)["fiber_tick"]=sol::nil;(*lua)["fiber_done"]=sol::nil;
            }
            schedulerFiber=nullptr;ensure(ConvertFiberToThread()!=0);terminationValue=false;
        }
        (*lua)["registeredQuest"]=host->GetQuestState();
        lua->script("registeredQuest:CreateThread('unused'); registeredQuest:CreateThread('unused',{}); registeredQuest:CreateThread('unused',{region='Class'})");
        ensure(spawned.size()==5&&threadSections[2].empty()&&threadSections[3].empty()&&threadSections[4]=="Class");
        if(argc>2){
            initializeUnusedResourceGuards();
            for(int repeat:{1,2}){
                using Initialize=void(__thiscall*)(CScriptBase_Retail*);
                reinterpret_cast<Initialize>(host->base.pVTable[3])(&host->base);
                expectedSpeechCount=6*repeat;ensure(observedLists->Count("good",true)==expectedSpeechCount);
                for(bool male:{false,true}){
                    ensure(observedLists->Count("bad",male)==6*repeat&&observedLists->Count("good",male)==6*repeat);
                    ensure(observedLists->Count("both",male)==4*repeat&&observedLists->Count("none",male)==5*repeat);
                }
                ensure(registrationKeys.size()==42*repeat&&speechAllocations.size()==8);
                ensure(!host->GetQuestState()->GetStateBool("AttackOver")&&host->GetQuestState()->GetStateInt("GoodDeedsPerformed")==0);
                ++initCases;
            }
            CPersistContext_Transfer_bool_API=reinterpret_cast<decltype(CPersistContext_Transfer_bool_API)>(&transferOakvale);
            auto* state=host->GetQuestState();state->SetStateInt("GoodDeedsPerformed",7);state->SetStateBool("GivenSweets",true);
            for(bool initial:{false,true})for(bool reading:{false,true})for(int saved:{-1,0,1}){
                state->SetStateBool("AttackOver",initial);persistenceReading=reading;persistencePresent=saved!=-1;persistenceSaved=saved==1;persistenceTransfers=0;
                host->OnPersist(persistenceContext);
                ensure(persistenceTransfers==1&&state->GetStateBool("AttackOver")== (reading?saved==1:initial));
                ensure(persistenceSaved==(reading?saved==1:initial));
                ensure(state->GetStateInt("GoodDeedsPerformed")==7&&state->GetStateBool("GivenSweets"));
                ensure(state->GetStateInt("WatchTimer")==0&&liveTimers.size()==2);++persistenceCases;
            }
            bindingNames.clear();g_entityScriptFileNames.clear();mainFlow.clear();
            IsActiveThreadTerminating_Quest_API=reinterpret_cast<decltype(IsActiveThreadTerminating_Quest_API)>(&questTerminating);
            IsActiveThreadTerminating_API=reinterpret_cast<decltype(IsActiveThreadTerminating_API)>(&gameTerminating);
            NewScriptFrame_API=reinterpret_cast<decltype(NewScriptFrame_API)>(&frameCall);
            CScriptThing sampleActor{};(*lua)["sampleActor"]=&sampleActor;
            for(bool withActor:{false,true})for(bool stopping:{false,true}){
                terminationValue=stopping;questQueries=gameQueries=frameCalls=0;(*lua)["expectedTermination"]=stopping;
                lua->script(withActor?"assert(registeredQuest:NewScriptFrame(sampleActor)==true); assert(registeredQuest:IsActiveThreadTerminating()==expectedTermination)":"assert(registeredQuest:NewScriptFrame()==true); assert(registeredQuest:IsActiveThreadTerminating()==expectedTermination)");
                ensure(frameCalls==1&&questQueries==1&&gameQueries==0);++frameCases;
            }
            (*lua)["sampleActor"]=sol::nil;
            {
                LuaEntityHost entity(nullptr,host,&sampleActor,"oakvale-entity-frame-check");expectedEntity=&entity;
                auto* data=LuaManager::GetInstance().GetEntityScriptData(&entity);ensure(data&&data->pLuaState);
                IsActiveThreadTerminating_Entity_API=reinterpret_cast<decltype(IsActiveThreadTerminating_Entity_API)>(&entityTerminating);
                data->pLuaState->set_function("observe_frame",[](bool stopped){ensure(stopped==terminationValue);++entityCallbacks;});
                data->pLuaState->script("function Main(quest,me) quest:NewScriptFrame(me); observe_frame(quest:IsActiveThreadTerminating()) end",data->env);
                data->luaMain=data->env["Main"];
                for(bool stopping:{false,true}){
                    terminationValue=stopping;entityQueries=questQueries=gameQueries=frameCalls=0;const int previous=entityCallbacks;
                    entity.Main();ensure(!data->errorLogged&&entityCallbacks==previous+1);
                    ensure(entityQueries==1&&questQueries==0&&gameQueries==0&&frameCalls==1);++entityFrameCases;
                }
                expectedEntity=nullptr;terminationValue=false;
                int failed=0,interrupted=0;
                data->env["Main"]=[](LuaQuestState*,CScriptThing*){};data->luaMain=data->env["Main"];
                data->env["OnPredicateFail"]=[&](LuaQuestState* q,CScriptThing* me){ensure(q==host->GetQuestState()&&me==&entity.m_Me);++failed;};
                data->env["OnInterrupted"]=[&](LuaQuestState* q,CScriptThing* me){ensure(q==host->GetQuestState()&&me==&entity.m_Me);++interrupted;};
                entity.Main();ensure(failed==0&&interrupted==0);
                CSpawnedFunc process{};
                using Forward=void(__thiscall*)(CSpawnedFunc*);
                ASLR<Forward>(0xce1090)(&process);ASLR<Forward>(0xce10a0)(&process);ensure(failed==0&&interrupted==0);
                process.pThunkToMain=&entity; // Native active-process +34 is its entity-script pointer.
                for(int repeat:{1,2}){
                    ASLR<Forward>(0xce1090)(&process);ASLR<Forward>(0xce10a0)(&process);
                    ensure(failed==repeat&&interrupted==repeat);
                }
                ++nativeEntityCallbackCases;
            }
            sol::table expectedBindings=(*lua)["__oakvale_binding_names"];
            for(unsigned i=1;i<=expectedBindings.size();++i){std::string name=expectedBindings[i];bindingNames.push_back(name);g_entityScriptFileNames.push_back("NewOakValeIntro/Entities/"+name);}
            ensure(bindingNames.size()==16);
            g_pEntityScriptBindingVTable=ASLR<void**>(0x12d8370);
            AddEntityScriptBinding_API=reinterpret_cast<decltype(AddEntityScriptBinding_API)>(&addBinding);
            PostAddScriptedEntities_CScriptBase_API=reinterpret_cast<decltype(PostAddScriptedEntities_CScriptBase_API)>(&finishBaseBindings);
            PostAddScriptedEntities_API=reinterpret_cast<decltype(PostAddScriptedEntities_API)>(&finishGameBindings);
            GetActiveQuestName_API=reinterpret_cast<decltype(GetActiveQuestName_API)>(&activeQuest);
            SetQuestCardObjective_API=reinterpret_cast<decltype(SetQuestCardObjective_API)>(&initialObjective);
            DeactivateQuest_API=reinterpret_cast<decltype(DeactivateQuest_API)>(&deactivatePostAttack);
            const bool afterAttack=(flags&1)!=0;terminationValue=(flags&2)!=0;questQueries=gameQueries=0;state->SetStateBool("AttackOver",afterAttack);
            host->GetEnvironment()["DoMission"]=[&](LuaQuestState* quest){ensure(quest==host->GetQuestState());mainFlow.emplace_back("mission");};
            host->Main();
            const bool completes=!(afterAttack&&terminationValue);
            std::vector<std::string> wanted{"base.finalize","game.finalize"};
            if(completes){if(afterAttack)wanted.emplace_back("deactivate");wanted.emplace_back("objective");wanted.emplace_back("mission");}
            ensure(mainFlow==wanted&&spawned.size()==(completes?6:5));if(completes)ensure(threadSections.back().empty());
            ensure(questQueries==int(afterAttack)&&gameQueries==0);terminationValue=false;++emittedMainCases;
            for(auto* binding:ownedBindings)for(int failure:{0,1,2}){
                CCPPointerInfo actorInfo{};actorInfo.RefCount=7;
                CGameScriptThing implementation{};CGameScriptThingVTable thingTable{};
                thingTable.GetPos=reinterpret_cast<decltype(thingTable.GetPos)>(&callbackPosition);
                implementation.pVTable=reinterpret_cast<void**>(&thingTable);
                CScriptThing actor{};actor.pImp.Info=&actorInfo;actor.pImp.Data=reinterpret_cast<decltype(actor.pImp.Data)>(&implementation);
                CCountedPointer<LuaEntityHost> result{};allocatingEntityReference=true;entityReferenceFailure=failure;
                bool failed=false;try{ensure(binding->pAllocFunc(&result,&host->base,nullptr,&actor)==&result);}catch(const std::bad_alloc&){failed=true;}
                allocatingEntityReference=false;entityReferenceFailure=0;
                if(failure){ensure(failed&&!result.Data&&!result.Info&&actorInfo.RefCount==7&&entityReferenceBlocks.empty());}
                else{
                    ensure(!failed&&result.Data&&result.Info&&result.Info->RefCount==1&&actorInfo.RefCount==8);
                    auto* pointer=result.Data;ensure(LuaManager::GetInstance().GetEntityScriptData(pointer));
                    auto* data=LuaManager::GetInstance().GetEntityScriptData(pointer);
                    data->pLuaState->script_file(GetScriptPath(data->scriptName),data->env);
                    sol::protected_function main=data->env["Main"];ensure(main.valid());main=sol::protected_function();
                    sol::protected_function persist=data->env["OnPersist"],interrupt=data->env["OnInterrupted"];
                    ensure(!persist.valid()&&!interrupt.valid());persist=sol::protected_function();interrupt=sol::protected_function();
                    pointer->OnPersist(persistenceContext);pointer->OnInterrupted_Stub();
                    ensure(state->GetStateInt("GoodDeedsPerformed")==7&&!data->errorLogged);++entityFileCases;
                    if(data->scriptName=="NewOakValeIntro/Entities/NOVI_Barrel"||data->scriptName=="NewOakValeIntro/Entities/NOVI_CreatedBeetle"){
                        using Initialize=void(__thiscall*)(LuaEntityHost*);
                        reinterpret_cast<Initialize>(pointer->pVTable[2])(pointer);ensure(data->luaMain.valid()&&!data->errorLogged);++emptyEntityInitCases;
                    }
                    CSpawnedFunc process{};process.pThunkToMain=pointer;
                    using Forward=void(__thiscall*)(CSpawnedFunc*);
                    if(data->scriptName=="NewOakValeIntro/Entities/NOVI_Barrel"){
                        for(C3DVector position: {C3DVector{-1.25f,0,7.5f},C3DVector{12,-8.25f,0.125f}}){
                            implementation.Pos=position;state->SetStateBool("BarrelBrokenInstantaneous",false);state->SetStateBool("BarrelBrokenPersistent",false);
                            const int queries=entityPositionQueries;ASLR<Forward>(0xce1090)(&process);
                            ensure(!data->errorLogged&&entityPositionQueries==queries+1);
                            ensure(state->GetStateBool("BarrelBrokenInstantaneous")&&state->GetStateBool("BarrelBrokenPersistent"));
                            ensure(state->GetStateFloat("BarrelBrokenPos_x")==position.x&&state->GetStateFloat("BarrelBrokenPos_y")==position.y&&state->GetStateFloat("BarrelBrokenPos_z")==position.z);++barrelCallbackCases;
                        }
                    }else{const int queries=entityPositionQueries;ASLR<Forward>(0xce1090)(&process);ensure(!data->errorLogged&&entityPositionQueries==queries);}
                    const bool notified=(flags&1)!=0;
                    if(notified){
                        using Scalar=void*(__thiscall*)(LuaEntityHost*,unsigned);
                        auto notify=reinterpret_cast<Scalar>(pointer->pVTable[0]);
                        for(unsigned scalarFlags:{0u,1u,2u,3u})ensure(notify(pointer,scalarFlags)==pointer);
                        ensure(actorInfo.RefCount==7&&!LuaManager::GetInstance().GetEntityScriptData(pointer));
                    }
                    auto shared=result;++result.Info->RefCount;
                    using Release=void(__thiscall*)(CCountedPointer<LuaEntityHost>*);
                    ASLR<Release>(0xce1000)(&result);ensure(!result.Data&&!result.Info&&actorInfo.RefCount==(notified?7u:8u));
                    ASLR<Release>(0xce1000)(&shared);ensure(!shared.Data&&!shared.Info&&actorInfo.RefCount==7&&entityReferenceBlocks.empty());
                    ensure(!LuaManager::GetInstance().GetEntityScriptData(pointer));
                }
                ++entityAllocationCases;
            }
            for(int childIndex:{0,1}){
                auto child=std::make_unique<LuaEntityHost>(nullptr,host,&sampleActor,"retained-oakvale-child-"+std::to_string(childIndex));
                auto* pointer=child.get();auto* childData=LuaManager::GetInstance().GetEntityScriptData(pointer);ensure(childData&&childData->pLuaState);
                (*childData->pLuaState)["borrowedQuest"]=state;
                childData->pLuaState->set_function("observe_child_close",[host,pointer](){
                    ensure(host->IsClosing()&&host->GetQuestState()&&liveTimers.size()==2);
                    ensure(host->GetQuestState()->GetStateInt("WatchTimer")==0&&observedLists->Count("good",true)==12);
                    ensure(!LuaManager::GetInstance().GetEntityScriptData(pointer)&&pointer->GetParentScript_Stub()==nullptr);
                    LuaManager::GetInstance().UnregisterEntityScriptData(pointer); // Reentrant removal is inert.
                    bool rejected=false;CScriptThing actor{};
                    try{LuaEntityHost late(nullptr,host,&actor,"late-child");}catch(const std::exception&){rejected=true;}
                    ensure(rejected);++childFinalizers;
                });
                childData->pLuaState->script("sentinel=setmetatable({}, {__gc=function() assert(not pcall(function() borrowedQuest:CreateThread('late') end)); assert(not pcall(function() borrowedQuest:NewScriptFrame() end)); observe_child_close() end})");
                retainedChildren.push_back(std::move(child));
            }
        }
        lua->script("sentinel=setmetatable({}, {__gc=function() observe_gc() end})");
        std::vector<sol::object> args{sol::make_object(*lua,17)};
        const int usedSlots=static_cast<int>(spawned.size())-1;
        host->CreateThread("unused","",args);args.clear();
        for(int slot=usedSlots+1;slot<20;++slot)host->CreateThread("unused","",{});
        ensure(spawned.size()==21&&anchoredThreads(lua->lua_state())==originalAnchors+20);
        bool full=false;try{host->CreateThread("unused","",{});}catch(const std::exception&){full=true;}
        ensure(full&&spawned.size()==21&&anchoredThreads(lua->lua_state())==originalAnchors+20);
        if(argc>2){
            lua->set_function("observe_shutdown_resume",[&](){
                ensure(host->IsClosing()&&liveTimers.size()==2&&host->GetQuestState());
                ensure(anchoredThreads(lua->lua_state())==originalAnchors+20);
                ++shutdownCallbacks;
            });
            lua->script(R"lua(
                function shutdown_body(quest)
                    quest:NewScriptFrame()
                    assert(quest:IsActiveThreadTerminating())
                    observe_shutdown_resume()
                    assert(not pcall(function() quest:CreateThread('late') end))
                end
                function Main(quest) shutdown_body(quest) end
                function threadProbe(quest,value) assert(value==17); shutdown_body(quest) end
            )lua",host->GetEnvironment());
            schedulerFiber=ConvertThreadToFiber(nullptr);ensure(schedulerFiber);
            NewScriptFrame_API=reinterpret_cast<decltype(NewScriptFrame_API)>(&frameCall);
            shutdownProcessTable[1]=reinterpret_cast<void*>(&resumeShutdownProcess);
            for(int index:{0,1}){
                shutdownProbes[index]={index==0?mainFunction:threadFunction,false};
                shutdownProcesses[index]={};shutdownProcesses[index].pVTable=shutdownProcessTable;
                shutdownProcesses[index].unknown_base_padding[0]=1;
                shutdownFibers[index]=CreateFiber(0,&runFiberProbe,&shutdownProbes[index]);ensure(shutdownFibers[index]);
                SwitchToFiber(shutdownFibers[index]);ensure(!shutdownProbes[index].finished);
            }
        }
        using ScalarDestructor=void*(__thiscall*)(LuaQuestHost*,unsigned);
        auto destroy=reinterpret_cast<ScalarDestructor>(GetMemberFunctionAddress(&LuaQuestHost::Destructor));
        ensure(destroy(host,flags)==host);
        ensure(lifecycle==std::vector<std::string>({"gc","timer:0","timer:-7","base"}));
        for(auto& child:retainedChildren){
            ensure(child->m_pParentHost==nullptr&&child->pInterface==nullptr&&!LuaManager::GetInstance().GetEntityScriptData(child.get()));
            const int previousQueries=entityQueries;child->Main();child->OnPersist(persistenceContext);child->Destructor(false);child->Destructor(true);
            ensure(entityQueries==previousQueries&&child->GetParentScript_Stub()==nullptr);
        }
        retainedChildren.clear();
        ensure(spawned.empty()&&liveTimers.empty());if(!(flags&1u))::operator delete(host);observedHost=nullptr;++cases;
    }
    for(int failure:{1,2}){
        reset();constructorFailure=true;failRegistration=failure;bool failed=false;
        try{auto* host=new LuaQuestHost(nullptr,&hostGame,"failed-host",NativeQuestLifetime::NewOakValeIntro);delete host;}
        catch(const std::exception& e){failed=std::string(e.what()).find("REGISTER_FAILURE")!=std::string::npos;}
        ensure(failed&&liveTimers.empty());ensure(lifecycle==(failure==1?std::vector<std::string>{"base"}:std::vector<std::string>{"timer:-7","base"}));++cases;
    }
    reset();nativeCase=false;
    auto* legacy=new LuaQuestHost(nullptr,&hostGame,"legacy-host");observedHost=legacy;observedLists=legacy->GetQuestState()->GetVillagerSpeechLists();
    ensure(std::string(legacy->DefaultThreadRegion())=="Class");
    if(argc>2){
        CScriptThing actor{};LuaEntityHost entity(nullptr,legacy,&actor,"legacy-entity-entry-guard");expectedEntity=&entity;
        auto* data=LuaManager::GetInstance().GetEntityScriptData(&entity);ensure(data&&data->pLuaState);
        data->env["Main"]=[](LuaQuestState*,CScriptThing*){++entityCallbacks;};data->luaMain=data->env["Main"];
        terminationValue=true;entityQueries=0;const int previous=entityCallbacks;entity.Main();
        ensure(entityQueries==1&&entityCallbacks==previous);terminationValue=false;expectedEntity=nullptr;
        int failed=0;
        data->env["Main"]=[](LuaQuestState*,CScriptThing*){};data->luaMain=data->env["Main"];
        data->env["OnPredicateFail"]=[&](LuaQuestState*,CScriptThing*){++failed;};
        expectedEntity=&entity;entity.Main();expectedEntity=nullptr;ensure(failed==1);
        CSpawnedFunc process{};process.pThunkToMain=&entity;
        using Forward=void(__thiscall*)(CSpawnedFunc*);
        ASLR<Forward>(0xce1090)(&process);ASLR<Forward>(0xce1090)(&process);ensure(failed==1);++legacyEntityCallbackCases;
    }
    legacy->GetEnvironment()["Main"]=[](LuaQuestState*){throw std::runtime_error("Legacy stopped Main must not run");};
    terminationValue=true;questQueries=gameQueries=0;legacy->Main();ensure(questQueries==1&&gameQueries==0);terminationValue=false;
    legacy->GetLuaState()->set_function("observe_gc",&finalizer);legacy->GetLuaState()->script("sentinel=setmetatable({}, {__gc=function() observe_gc() end})");
    legacy->Destructor(0);ensure(lifecycle.empty()&&registrationCalls==0&&legacy->GetQuestState()!=nullptr);
    legacy->Destructor(1);observedHost=nullptr;ensure(lifecycle==std::vector<std::string>{"gc"});++cases;
    ensure(mainCalls==4&&threadCalls==4&&registrationKeys.empty()&&unregisteredAllocations.empty());
    observedLists.reset();VirtualFree(arena,0,MEM_RELEASE);
    std::cout<<"PASS: 20 actual-host Main registration policies and 4 native virtual-Main dispatches\n";
    std::cout<<"PASS: 20 spawned-thread registration policies, 4 dispatches and 4 slot-exhaustion cases\n";
    std::cout<<"PASS: 12 Lua CreateThread default/explicit region cases and legacy default\n";
    if(argc>2){ensure(persistenceCases==48&&initCases==8&&ambientResets==8);std::cout<<"PASS: 48 actual-host emitted OnPersist save/restore/default cases and 8 Init calls with owned speech lists\n";}
    if(argc>2){ensure(emittedMainCases==4);std::cout<<"PASS: 4 emitted Main flows with 16 real allocator bindings each, objective and timer-thread registration; mission body is a boundary\n";}
    if(argc>2){ensure(frameCases==16);std::cout<<"PASS: 16 actual Lua frame/query order cases; Main query placement matches native\n";}
    if(argc>2){ensure(entityFrameCases==8&&entityCallbacks==8);std::cout<<"PASS: 8 actual LuaEntityHost frame/query cases with entity-specific termination\n";}
    if(argc>2){ensure(childFinalizers==8);std::cout<<"PASS: 8 child VM finalizers with live parent state, rejected late work and safe post-parent callbacks\n";}
    if(argc>2){ensure(entityAllocationCases==192);std::cout<<"PASS: 192 actual entity allocator ownership/failure cases using native Thing copy and counted release instructions\n";}
    if(argc>2){ensure(fiberCases==16);std::cout<<"PASS: 16 Windows fiber interleavings with separate Main/thread Lua stacks, suspended garbage collection and graceful cancellation\n";}
    if(argc>2){ensure(drainCalls==4&&shutdownCallbacks==8);std::cout<<"PASS: 8 suspended Lua callbacks drained by original native TerminateProcess before anchors, VMs and quest state close\n";}
    if(argc>2){ensure(nativeEntityCallbackCases==4&&legacyEntityCallbackCases==1);std::cout<<"PASS: 4 native entity callback timing/forwarding cases and preserved legacy inference/deduplication\n";}
    if(argc>2){ensure(entityFileCases==64&&emptyEntityInitCases==8&&barrelCallbackCases==8);std::cout<<"PASS: 64 real entity VM file loads/default callbacks, 8 virtual empty Init calls and 8 emitted Barrel callbacks through native forwarding\n";}
    std::cout<<"PASS: "<<cases<<" actual LuaQuestHost/LuaQuestState lifecycle cases with full runtime objects and real Lua\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
