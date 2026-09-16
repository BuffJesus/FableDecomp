#include "teddy_girl_specific_harness.cpp"
static std::string mode;
static std::map<CScriptThing*,std::string> ownedThings;
static CCharString* presentedOutput=nullptr;
static int pollCalls=0,talkCalls=0,ownerAcquires=0;
static void ownerEvent(const std::string& value){trace.push_back(value);if(value==mode)throw std::runtime_error("OWNER_FAULT "+mode);}
static CCharString* __fastcall ownerOutputCtor(CCharString* self,void*){check(!presentedOutput);presentedOutput=self;strings[self]="";self->pStringData=nullptr;ownerEvent("output.new");return self;}
static bool __fastcall ownerNotEqual(CCharString* self,void*,const char* value){check(self==presentedOutput);return strings.at(self)!=value;}
static void __fastcall ownerTextDtor(CCharString* self,void*){if(self==presentedOutput){check(!movies&&!paused);presentedOutput=nullptr;trace.push_back("output.destroy");stringDtor(self,nullptr);}else textDestroy(self,nullptr);}
static bool __fastcall ownerPoll(CScriptThing* actor,void*,CCharString* output){check(actor==&girl&&output==presentedOutput);++pollCalls;ownerEvent("poll");
    if(pollCalls==1){strings[output]="OBJECT_TEDDY_BEAR_UNGIVEABLE";output->pStringData=reinterpret_cast<void*>(1);return mode=="teddy";}
    if(mode=="poll_error")throw std::runtime_error("OWNER_FAULT poll_error");
    if(mode=="wrong"){strings[output]="";output->pStringData=nullptr;return true;}return false;
}
static bool __fastcall ownerTalk(CScriptThing* actor,void*,const CCharString* who){check(actor==&girl&&strings.at(const_cast<CCharString*>(who))=="SCRIPT_NAME_HERO");++talkCalls;return mode=="talk"||(talkCalls==1&&(mode=="question"||mode=="failed_populated"||mode=="movie_error"||mode=="health_error"||mode=="getter_error"||mode=="speak_error"));}
static bool __fastcall ownerPossession(CGameScriptInterfaceBase*,void*,const CCharString* key,const CScriptThing* actor){check(actor==&hero&&strings.at(const_cast<CCharString*>(key))=="OBJECT_TEDDY_BEAR_UNGIVEABLE");return mode!="talk";}
static bool __fastcall ownerHit(CScriptThing*,void*,const CCharString*){return mode=="hit";}
static bool __fastcall ownerNoAbility(CScriptThing*,void*,const CCharString*){return false;}
static bool __fastcall ownerNoExcluded(CScriptThing*,void*,EHeroAbility,const CCharString*){return false;}
static CScriptThing* __fastcall ownerLookup(CGameScriptInterfaceBase*,void*,CScriptThing* output,const CCharString* key){auto name=strings.at(const_cast<CCharString*>(key));check(name=="NOVI_Bully"||name=="NOVI_AffairWife");check(ownedThings.emplace(output,name).second);output->pImp.Data=nullptr;trace.push_back("thing.new:"+name);return output;}
static void __fastcall ownerThingDtor(CScriptThing* self,void*){auto found=ownedThings.find(self);check(found!=ownedThings.end());trace.push_back("thing.destroy:"+found->second);ownedThings.erase(found);}
static CScriptThing* __fastcall ownerControlledThing(CScriptGameResourceObjectScriptedThingBase* self,void*,CScriptThing* output){check(self==expectedExpert);check(ownedThings.emplace(output,"health").second);output->pImp.Data=nullptr;trace.push_back("thing.new:health");if(mode=="getter_error")ownerEvent("getter_error");return output;}
static float __fastcall ownerHealth(CGameScriptInterfaceBase*,void*,const CScriptThing* thing){check(ownedThings.at(const_cast<CScriptThing*>(thing))=="health");ownerEvent(mode=="health_error"?"health_error":"health");return mode=="speak_error"?1.0f:0.0f;}
static bool __fastcall ownerAcquire(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,CScriptGameResourceObjectScriptedThingBase* resource,EScriptAIPriority priority){check(actor==&girl&&static_cast<int>(priority)==4);++ownerAcquires;if(!resource->pImp.Data){resource->pImp.Data=expectedExpert;++refs[expectedExpert];locals.insert(resource);}trace.push_back(mode=="failed_populated"?"acquire.false.populated":"acquire.true");return mode!="failed_populated";}
static bool __fastcall ownerPrepare(CScriptGameResourceObjectScriptedThingBase* resource,void*){return resource->pImp.Data!=nullptr;}
static void __fastcall ownerPrepareRelease(CScriptGameResourceObjectScriptedThingBase* resource,void*){release(resource,nullptr);resource->pImp.Data=nullptr;resource->pImp.Info=nullptr;trace.push_back("prepare.release");}
static void __fastcall ownerRelease(void* resource,void*){trace.push_back("control.destroy");release(resource,nullptr);}
static void __fastcall ownerMovieStart(CGameScriptInterfaceBase* game,void*,const CCharString* key,CScriptGameResourceObjectMovieBase* movie){movieStart(game,nullptr,key,movie);ownerEvent(mode=="movie_error"?"movie_error":"movie.start");}
static void __fastcall ownerMovieDestroy(CScriptGameResourceObjectMovieBase* movie,void*){check(!paused);movieDestroy(movie,nullptr);trace.push_back("movie.destroy");}
static void __fastcall ownerPause(CGameScriptInterfaceBase*,void*,bool value){paused=value;trace.push_back(value?"pause.true":"pause.false");}
static void __fastcall ownerSpeak(CScriptGameResourceObjectScriptedThingBase* self,void*,const CScriptThing* target,const char*,ETextGroupSelectionMethod selection,bool listen,bool sound,bool fade){check(self==expectedExpert&&target==&hero&&static_cast<int>(selection)==0&&!listen&&sound&&!fade);ownerEvent("speak_error");}
static bool __fastcall ownerBusy(CScriptGameResourceObjectScriptedThingBase*,void*){return false;}
static void __fastcall ownerMove(void* self,void*,const CScriptThing* target,float radius,EScriptEntityMoveType kind,CTCScriptedControl* wait,bool a,bool b,bool c){check(self==expectedExpert&&ownedThings.at(const_cast<CScriptThing*>(target))=="NOVI_AffairWife"&&radius==3.0f&&static_cast<int>(kind)==1&&!wait&&!a&&!b&&c);ownerEvent(mode=="move_error"?"move_error":"move");}
static int __fastcall ownerConversation(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool a,bool b){check(actor==&girl&&!a&&!b);return -7;}
static void __fastcall ownerPerson(CGameScriptInterfaceBase*,void*,int id,const CScriptThing* actor){check(id==-7&&actor==&hero);}
static void __fastcall ownerAlly(CGameScriptInterfaceBase*,void*,const CScriptThing* first,const CScriptThing* second){check((first==&girl&&second==&hero)||(first==&hero&&second==&girl));trace.push_back("ally");}
tAddNewConversation AddNewConversation_API=reinterpret_cast<tAddNewConversation>(&ownerConversation);
tAddPersonToConversation AddPersonToConversation_API=reinterpret_cast<tAddPersonToConversation>(&ownerPerson);
tEntitySetThingAsAllyOfThing EntitySetThingAsAllyOfThing_API=reinterpret_cast<tEntitySetThingAsAllyOfThing>(&ownerAlly);
static void ownerJump(unsigned char* address,void* target){address[0]=0xe9;*reinterpret_cast<int*>(address+1)=reinterpret_cast<unsigned char*>(target)-(address+5);}
static size_t indexOf(const std::string& event){auto found=std::find(trace.begin(),trace.end(),event);check(found!=trace.end());return found-trace.begin();}
int main(){try{
    auto* arena=static_cast<unsigned char*>(VirtualAlloc(nullptr,0x321000,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE));check(arena!=nullptr);g_fableBase=reinterpret_cast<DWORD>(arena)-(0x99e000-0x400000);ownerJump(arena+0x4b0,reinterpret_cast<void*>(&ownerOutputCtor));ownerJump(arena+0x960,reinterpret_cast<void*>(&ownerNotEqual));memcpy(arena+0x3202ff,underBytes,sizeof underBytes);memcpy(arena+0x3203ea,overBytes,sizeof overBytes);FlushInstructionCache(GetCurrentProcess(),arena,0x321000);
    CScriptThingVTable table{};table.IsAlive=reinterpret_cast<decltype(table.IsAlive)>(&alive);table.GetPos=reinterpret_cast<decltype(table.GetPos)>(&distancePosition);table.MsgIsTalkedToBy=reinterpret_cast<decltype(table.MsgIsTalkedToBy)>(&ownerTalk);table.MsgIsPresentedWithItem=reinterpret_cast<decltype(table.MsgIsPresentedWithItem)>(&ownerPoll);table.MsgIsHitBy=reinterpret_cast<decltype(table.MsgIsHitBy)>(&ownerHit);table.MsgIsHitByAnySpecialAbilityFrom=reinterpret_cast<decltype(table.MsgIsHitByAnySpecialAbilityFrom)>(&ownerNoAbility);table.MsgIsHitBySpecialAbilityFrom=reinterpret_cast<decltype(table.MsgIsHitBySpecialAbilityFrom)>(&ownerNoExcluded);girl.pVTable=reinterpret_cast<void**>(&table);hero.pVTable=girl.pVTable;g_pCScriptThingVTable=girl.pVTable;heroResult=&hero;positionResult=&origin;screenResult=false;
    CScriptGameResourceObjectScriptedThingBaseVTable expertTable{};expertTable.GetScriptThing=reinterpret_cast<decltype(expertTable.GetScriptThing)>(&ownerControlledThing);expertTable.Speak=reinterpret_cast<decltype(expertTable.Speak)>(&ownerSpeak);expertTable.IsPerformingScriptTask=reinterpret_cast<decltype(expertTable.IsPerformingScriptTask)>(&ownerBusy);expertTable.MoveToThing=reinterpret_cast<decltype(expertTable.MoveToThing)>(&ownerMove);CScriptGameResourceObjectScriptedThingBase expert{};expert.pVTable=reinterpret_cast<void**>(&expertTable);expectedExpert=&expert;
    CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&textCtor);CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&ownerTextDtor);RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&ownerThingDtor);GetThingWithScriptName_ByName_API=reinterpret_cast<tGetThingWithScriptName1>(&ownerLookup);GetHealth_API=reinterpret_cast<tGetHealth>(&ownerHealth);IsObjectInThingsPossession_API=reinterpret_cast<tIsObjectInThingsPossession>(&ownerPossession);StartScriptingEntity_API=reinterpret_cast<tStartScriptingEntity>(&ownerAcquire);InitScriptObjectHelper1_API=reinterpret_cast<decltype(InitScriptObjectHelper1_API)>(&ownerPrepare);InitScriptObjectHelper2_API=reinterpret_cast<decltype(InitScriptObjectHelper2_API)>(&ownerPrepareRelease);CSGROSTB_Destroy_API=reinterpret_cast<decltype(CSGROSTB_Destroy_API)>(&ownerRelease);StartMovieSequence_API=reinterpret_cast<tStartMovieSequence>(&ownerMovieStart);MovieResource_Destroy_API=reinterpret_cast<decltype(MovieResource_Destroy_API)>(&ownerMovieDestroy);PauseAllNonScriptedEntities_API=reinterpret_cast<decltype(PauseAllNonScriptedEntities_API)>(&ownerPause);
    for(const char* selected:{"none","question","teddy","wrong","talk","hit","departure","failed_populated","poll_error","movie_error","health_error","getter_error","speak_error","move_error"}){
        mode=selected;fault.clear();trace.clear();pollCalls=talkCalls=ownerAcquires=0;sol::state lua;lua.open_libraries(sol::lib::base,sol::lib::package);RegisterRetailResources(lua);lua["girl"]=&girl;lua["hero"]=&hero;lua["mode"]=mode;
        lua["ownerScope"]=[](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};lua["ownerPause"]=[](bool value){ownerPause(nullptr,nullptr,value);};lua["record"]=[](const std::string& text){trace.push_back(text);};
        check(lua.safe_script_file("candidate.lua",sol::script_pass_on_error).valid());auto result=lua.safe_script(R"(
            package.preload['NewOakValeIntro.native_quest_helpers']=function() return {AddGoodDeed=function() record('goodDeed') end,AddBadDeed=function(_,_,amount) assert(amount==2);record('badDeed') end} end
            local q={frames=0}
            function q:WithRetailResources(body) return ownerScope(body) end
            function q:RegisterBoundConsciousCondition() record('condition') end
            function q:NewScriptFrame() self.frames=self.frames+1;record('frame') end
            function q:IsActiveThreadTerminating() return self.frames>=2 end
            function q:GetHero() return hero end
            function q:GetStateBool(key) return key=='SpokeAboutFindingTeddy' and (mode=='departure' or mode=='move_error') end
            function q:PauseAllNonScriptedEntities(value) ownerPause(value) end
            function q:MsgIsQuestionAnsweredYesOrNo() return 1 end
            function q:SetMasterGameState(key,value) assert(key=='TeddySolution');record('master:'..value) end
            function q:ClearThingHasInformation() record('clear') end
            Init(q,girl); Main(q,girl)
        )",sol::script_pass_on_error);
        const bool error=mode.find("error")!=std::string::npos;check(result.valid()!=error);if(error){sol::error e=result;check(std::string(e.what()).find("OWNER_FAULT "+mode)!=std::string::npos);}
        check(ownedThings.empty()&&!presentedOutput&&strings.empty()&&locals.empty()&&!movies&&!paused);for(const auto& ref:refs)check(ref.second==0);
        check(indexOf("output.destroy")<indexOf("thing.destroy:NOVI_Bully")&&indexOf("thing.destroy:NOVI_Bully")<indexOf("control.destroy"));
        if(mode=="health_error"||mode=="getter_error")check(indexOf("thing.destroy:health")<indexOf("movie.destroy"));
        if(mode=="move_error"||mode=="departure")check(indexOf("thing.destroy:NOVI_AffairWife")<indexOf("output.destroy"));
        if(mode=="failed_populated")check(ownerAcquires==1&&std::count(trace.begin(),trace.end(),"acquire.false.populated")==1);
        if(mode=="question"||mode=="teddy")check(indexOf("destroy:OBJECT_TEDDY_BEAR_UNGIVEABLE")<indexOf("goodDeed")&&indexOf("goodDeed")<indexOf("master:B"));
    }
    VirtualFree(arena,0,MEM_RELEASE);std::cout<<"TeddyGirl merged owner: 14 full-candidate real-Lua/x86 normal/cancel/error scenarios passed\n";return 0;
}catch(const std::exception& error){std::cerr<<"mode="<<mode<<" "<<error.what()<<'\n';for(auto& event:trace)std::cerr<<event<<'\n';return 1;}}
