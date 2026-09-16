#define main resource_smoke_main
#include "runtime_checks/retail_resources_smoke.cpp"
#undef main
#include <cstdarg>
static std::vector<std::string> literalEvents;
static bool literalFailure=false;
static void __fastcall tracedLiteralCtor(CCharString* self,void*,const char* value,int length){
    stringCtor(self,nullptr,value,length);if(!*value)self->pStringData=nullptr;literalEvents.push_back("+"+std::string(value));
}
static void __fastcall tracedLiteralDestroy(CCharString* self,void*){literalEvents.push_back("-"+strings.at(self));stringDtor(self,nullptr);}
static void __fastcall questionCall(CGameScriptInterfaceBase*,void*,const CCharString* q,const CCharString* y,const CCharString* n,const CCharString* empty,bool flag){
    check(flag && strings.at(const_cast<CCharString*>(q))=="TEXT_QST_048_GIVE_TEDDY_TO_BULLY" && strings.at(const_cast<CCharString*>(y))=="TEXT_OBJECT_HERO_ANSWER_YES" && strings.at(const_cast<CCharString*>(n))=="TEXT_OBJECT_HERO_ANSWER_NO" && strings.at(const_cast<CCharString*>(empty)).empty());
    if(literalFailure)throw std::runtime_error("LITERAL_BODY");
}
static int __fastcall hudCall(CGameScriptInterfaceBase*,void*,float current,float maximum,const CRGBColour* green,const CRGBColour* red,const CCharString* icon,const CCharString* text,float scale){
    check(current==4.0f && maximum==0.0f && scale==1.0f && green->B==0 && green->G==255 && green->R==0 && green->A==255 && red->B==0 && red->G==0 && red->R==255 && red->A==255);
    check(strings.at(const_cast<CCharString*>(icon))=="HUD_QUEST_ICON_GRANDSON" && strings.at(const_cast<CCharString*>(text)).empty());
    if(literalFailure)throw std::runtime_error("LITERAL_BODY");return -73;
}
tGiveHeroYesNoQuestion GiveHeroYesNoQuestion_API=reinterpret_cast<tGiveHeroYesNoQuestion>(&questionCall);
tAddQuestInfoBar AddQuestInfoBar_API=reinterpret_cast<tAddQuestInfoBar>(&hudCall);
static CScriptThing initActor{},initHeroActor{};
static CScriptThing* initHeroResult=nullptr;
static std::vector<std::string> initEvents;
static unsigned initNullQueries=0;
static void initRecord(const char* name,const CScriptThing* actor){check(actor==&initActor);initEvents.push_back(name);}
static void __fastcall initDamage(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool flag){check(!flag);initRecord("damage",actor);}
static void __fastcall initKill(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool a,bool b){check(!a && !b);initRecord("kill",actor);}
static void __fastcall initCombo(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool flag){check(!flag);initRecord("combo",actor);}
static void __fastcall initInformation(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool a,bool b,bool c){check(!a && !b && !c);initRecord("information",actor);}
static CScriptThing* __fastcall initHero(CGameScriptInterfaceBase*,void*){initEvents.push_back("hero");return initHeroResult;}
static void __fastcall initAlly(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,const CScriptThing* hero){check(hero==initHeroResult);initRecord("ally",actor);}
static void __fastcall initPushable(CGameScriptInterfaceBase*,void*,CScriptThing argument,bool flag){
    check(!flag && argument.pVTable==g_pCScriptThingVTable && argument.pImp.Data==initActor.pImp.Data && argument.pImp.Info==initActor.pImp.Info);
    if(argument.pImp.Info){check(argument.pImp.Info->RefCount==8);--argument.pImp.Info->RefCount;}
    initEvents.push_back("pushable");
}
static bool __fastcall initIsNull(CScriptThing*,void*){++initNullQueries;return true;}
tEntitySetAsDamageable EntitySetAsDamageable_API=reinterpret_cast<tEntitySetAsDamageable>(&initDamage);
tEntitySetAsKillable EntitySetAsKillable_API=reinterpret_cast<tEntitySetAsKillable>(&initKill);
tEntitySetAsToAddToComboMultiplierWhenHit EntitySetAsToAddToComboMultiplierWhenHit_API=reinterpret_cast<tEntitySetAsToAddToComboMultiplierWhenHit>(&initCombo);
tSetThingHasInformation SetThingHasInformation_API=reinterpret_cast<tSetThingHasInformation>(&initInformation);
tGetHero GetHero_API=reinterpret_cast<tGetHero>(&initHero);
tSetIsPushableByHero SetIsPushableByHero_API=reinterpret_cast<tSetIsPushableByHero>(&initPushable);
DWORD g_fableBase=0;
static CCharString* activeText=nullptr;
static unsigned textsClosed=0;
static CCharString* __fastcall textCtor(CCharString* self,void*) {check(!activeText);activeText=self;self->pStringData=nullptr;strings[self]="";return self;}
static void __cdecl formatText(CCharString* self,const char* format,...) {
    check(self==activeText && strings.at(self).empty());va_list args;va_start(args,format);const auto index=va_arg(args,int);va_end(args);
    check(std::string(format)=="TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_%d");strings[self]="TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_"+std::to_string(index);
}
static void __fastcall ownedStringDtor(CCharString* self,void*) {if(self==activeText){activeText=nullptr;++textsClosed;}stringDtor(self,nullptr);}
static void jump(unsigned char* at,void* target){at[0]=0xe9;*reinterpret_cast<int*>(at+1)=reinterpret_cast<unsigned char*>(target)-(at+5);}
static unsigned char* animationByte=nullptr;
static unsigned char desiredAnimationByte=0;
static unsigned animationConstructs=0,animationCalls=0;
static void __fastcall animationStringCtor(CCharString* self,void*,const char* value,int length){
    stringCtor(self,nullptr,value,length);
    if(std::string(value)=="ANIMATION") {check(activeText && !strings.at(activeText).empty());++animationConstructs;*animationByte=desiredAnimationByte;}
}
static void __fastcall rawAnimation(void*,void*,const CCharString* key,
    bool b1,bool b2,bool b3,bool b4,unsigned char b5,bool b6,bool b7){
    check(strings.at(const_cast<CCharString*>(key))=="ANIMATION");
    check(!b1 && !b2 && !b3 && b4 && b5==desiredAnimationByte && !b6 && !b7);++animationCalls;
}
static std::map<void*,std::map<std::string,CCharString>> inputMaps;
static unsigned infoCalls=0,thingDestroys=0;
static unsigned allyCalls=0;
static CScriptThing conversationActor{};
static unsigned conversationLines=0;
static int __fastcall newConversation(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,bool a,bool b){check(actor==&conversationActor && !a && !b);return -73;}
static void __fastcall addPerson(CGameScriptInterfaceBase*,void*,int id,const CScriptThing* actor){check(id==-73 && actor && !actor->pImp.Data);}
static void __fastcall addLine(CGameScriptInterfaceBase*,void*,int id,const CCharString* key,bool flag,const CScriptThing* speaker,const CScriptThing* listener){
    auto text=strings.at(const_cast<CCharString*>(key));check(id==-73 && (text=="TEXT_DYNAMIC" || text=="TEXT_QST_048_BULLY_SCRMSG_INTIMIDATING_-5") && !flag && speaker==&conversationActor && listener && !listener->pImp.Data);++conversationLines;
}
tAddNewConversation AddNewConversation_API=reinterpret_cast<tAddNewConversation>(&newConversation);
tAddPersonToConversation AddPersonToConversation_API=reinterpret_cast<tAddPersonToConversation>(&addPerson);
tAddLineToConversation AddLineToConversation_API=reinterpret_cast<tAddLineToConversation>(&addLine);
static void __fastcall face(CGameScriptInterfaceBase*,void*,const CScriptThing* actor,const CScriptThing* target,bool snap){check(actor==&conversationActor && target && !target->pImp.Data && snap);}
tEntitySetFacingAngleTowardsThing EntitySetFacingAngleTowardsThing_API=reinterpret_cast<tEntitySetFacingAngleTowardsThing>(&face);
static bool __fastcall distance(CScriptThing* first,CScriptThing* second,float range){check(first==&conversationActor && !second && range==7.25f);return true;}
static int policy=0;
static void __fastcall inputCtor(void* self,void*) {check(inputMaps.emplace(self,decltype(inputMaps)::mapped_type{}).second);}
static CCharString* __fastcall inputBracket(void* self,void*,const CCharString* key) {return &inputMaps.at(self)[strings.at(const_cast<CCharString*>(key))];}
static CCharString* __fastcall literalAssign(CCharString* self,void*,const char* value) {strings[self]=value;return self;}
static void __fastcall inputDestroy(void* self,void*) {
    check(!movies && !paused && !maps.empty());
    for(auto& pair:inputMaps.at(self))strings.erase(&pair.second);
    check(inputMaps.erase(self)==1);
}
static void __fastcall thingDestroy(CScriptThing* self,void*) {check(!self->pImp.Data);++thingDestroys;}
static void __fastcall clearInfo(CGameScriptInterfaceBase*,void*,const CScriptThing* thing){check(thing && !thing->pImp.Data);++infoCalls;}
static void __fastcall runoffMacro(const CCharString* key,void* actors,void* flags,void* inputs,bool setup,bool skippable){
    check(!flags && !setup && skippable && movies==1 && paused && maps.at(actors).size()==3);
    for(const auto& pair:maps.at(actors))check(!pair.second.pImp.Data); // Failed acquisitions stay representable.
    const auto name=strings.at(const_cast<CCharString*>(key));
    if(name=="FIRST") {
        check(inputs && inputMaps.count(inputs)==1);
        if(policy==0)check(inputMaps.at(inputs).empty());
        else check(strings.at(&inputMaps.at(inputs).at("$BRATLINE"))=="TEXT_DYNAMIC");
        if(policy==2)throw std::runtime_error("MACRO");
    } else {check(name=="SECOND" && !inputs && inputMaps.size()==1);}
}
tStdMap_String_Construct StdMap_String_Construct_API=reinterpret_cast<tStdMap_String_Construct>(&inputCtor);
tStdMap_String_OperatorBracket StdMap_String_OperatorBracket_API=reinterpret_cast<tStdMap_String_OperatorBracket>(&inputBracket);
tStdMap_String_Destructor StdMap_String_Destroy_API=reinterpret_cast<tStdMap_String_Destructor>(&inputDestroy);
tCCharString_AssignmentLiteral CCharString_AssignLiteral_API=reinterpret_cast<tCCharString_AssignmentLiteral>(&literalAssign);
tClearThingHasInformation ClearThingHasInformation_API=reinterpret_cast<tClearThingHasInformation>(&clearInfo);
static void __fastcall ally(CGameScriptInterfaceBase*,void*,const CScriptThing* first,const CScriptThing* second){check(!first && !second);++allyCalls;}
tEntitySetThingAsAllyOfThing EntitySetThingAsAllyOfThing_API=reinterpret_cast<tEntitySetThingAsAllyOfThing>(&ally);
int main(){try {
    acquireOK=false;RunCutsceneMacro_Func=&runoffMacro;RetailThing_Destroy_API=reinterpret_cast<tRetailThingDestroy>(&thingDestroy);
    for(policy=0;policy<4;++policy){
        const auto before=thingDestroys;sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};lua["policy"]=policy;
        lua["me"]=&conversationActor;
        auto result=lua.safe_script(R"(
          scope(function(r)
            r:SetThingAsAlly(nil,nil)
            local hero,victim,bully=r:NewResource(),r:NewResource(),r:NewResource()
            local retained=r:NewThingFromResource(victim)
            assert(not r:TryAcquireThing(victim,retained,4))
            local conversation=r:NewConversation(me,false,false)
            r:AddConversationPerson(conversation,retained)
            r:AddConversationLine(conversation,'TEXT_DYNAMIC',me,retained,false)
            r:ClearThingHasInformation(retained)
            local actors=r:NewActorMap()
            r:SetActor(actors,'HERO',hero);r:SetActor(actors,'BRAT',victim);r:SetActor(actors,'BULLY',bully)
            local inputs=r:NewStringMap()
            if policy~=0 then r:SetString(inputs,'$BRATLINE','TEXT_DYNAMIC') end
            if policy==3 then return end
            local movie=r:StartMovie('');r:Pause(true)
            r:RunMacroWithStrings('FIRST',actors,inputs,false,true)
            r:RunMacro('SECOND',actors,false,true)
            r:Pause(false);r:DestroyMovie(movie)
            r:DestroyStringMap(inputs);r:DestroyActorMap(actors)
            assert(not pcall(function()r:SetString(inputs,'K','V')end))
          end)
        )",sol::script_pass_on_error);
        check(result.valid()==(policy!=2));if(policy==2){sol::error e=result;check(std::string(e.what()).find("MACRO")!=std::string::npos);}
        check(thingDestroys==before+1 && strings.empty() && inputMaps.empty() && maps.empty() && !movies && !paused && locals.empty());
    }
    check(infoCalls==4 && allyCalls==4 && conversationLines==4);
    auto* arena=static_cast<unsigned char*>(VirtualAlloc(nullptr,0xb00000,MEM_COMMIT|MEM_RESERVE,PAGE_EXECUTE_READWRITE));check(arena!=nullptr);
    g_fableBase=reinterpret_cast<DWORD>(arena)-(0x99e000-0x400000);animationByte=arena+(0x1375748-0x99e000);
    jump(arena+0x4b0,reinterpret_cast<void*>(&textCtor));jump(arena+0x11f0,reinterpret_cast<void*>(&formatText));jump(arena+(0xcbe2ff-0x99e000),reinterpret_cast<void*>(&distance));
    *reinterpret_cast<int*>(arena+(0x13ac860-0x99e000))=17;*reinterpret_cast<float*>(arena+(0x13ac85c-0x99e000))=7.25f;
    CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&ownedStringDtor);
    CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&animationStringCtor);
    CScriptGameResourceObjectScriptedThingBase expert{};
    CScriptGameResourceObjectScriptedThingBaseVTable expertTable{};
    expertTable.PlayAnimation=reinterpret_cast<decltype(expertTable.PlayAnimation)>(&rawAnimation);
    expert.pVTable=reinterpret_cast<void**>(&expertTable);acquireExpert=&expert;
    for(unsigned value: {0u,1u,2u,255u})for(bool empty: {false,true}){
        desiredAnimationByte=static_cast<unsigned char>(value);*animationByte=37;acquireOK=!empty;
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
        lua["me"]=&conversationActor;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        auto result=lua.safe_script(R"(scope(function(r)
            local empty=r:NewResource();local victim=r:NewThingFromResource(empty)
            r:FaceRetainedThing(me,victim,true)
            assert(r:ReadBullyRandomModulus()==17 and r:ReadBullyProximityRange()==7.25)
            assert(r:IsDistanceBetweenThingsUnder(me,nil,r:ReadBullyProximityRange()))
            local text=r:NewText();r:FormatBullyIntimidationText(text,-5)
            r:AddConversationText(-73,text,me,victim,false)
            local control=r:NewResource();r:TryAcquire(control,me,4)
            r:PlayAnimationWithNativeArgument5(control,'ANIMATION',false,false,false,true,false,false)
            r:DestroyText(text)
            local emptyText=r:NewText();r:DestroyText(emptyText)
        end))",sol::script_pass_on_error);check(result.valid());check(strings.empty() && locals.empty());
    }
    check(animationConstructs==8 && animationCalls==4 && textsClosed==16 && !activeText);
    for(bool formatted:{false,true}){
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};lua["formatted"]=formatted;
        auto result=lua.safe_script(R"(scope(function(r)
            local key=r:NewText();if formatted then r:FormatBullyIntimidationText(key,-5) end
            error('TEXT_BODY')
        end))",sol::script_pass_on_error);check(!result.valid());sol::error e=result;check(std::string(e.what()).find("TEXT_BODY")!=std::string::npos);
        check(!activeText && strings.empty());
    }
    check(textsClosed==18);VirtualFree(arena,0,MEM_RELEASE);
    CCharString_Construct_Literal=reinterpret_cast<tCCharString_Constructor_Literal>(&tracedLiteralCtor);
    CCharString_Destroy=reinterpret_cast<tCCharString_Destructor>(&tracedLiteralDestroy);
    for(bool hud:{false,true})for(bool fail:{false,true}){
        literalEvents.clear();literalFailure=fail;
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["hud"]=hud;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        auto result=lua.safe_script(R"(scope(function(r)
            if hud then assert(r:AddBullyHealthBar(4)==-73) else r:GiveBullyTeddyQuestion() end
        end))",sol::script_pass_on_error);check(result.valid()!=fail);
        if(fail){sol::error e=result;check(std::string(e.what()).find("LITERAL_BODY")!=std::string::npos);}
        std::vector<std::string> expected=hud?std::vector<std::string>{"+","+HUD_QUEST_ICON_GRANDSON","-HUD_QUEST_ICON_GRANDSON","-"}:
            std::vector<std::string>{"+","+TEXT_OBJECT_HERO_ANSWER_NO","+TEXT_OBJECT_HERO_ANSWER_YES","+TEXT_QST_048_GIVE_TEDDY_TO_BULLY","-TEXT_QST_048_GIVE_TEDDY_TO_BULLY","-TEXT_OBJECT_HERO_ANSWER_YES","-TEXT_OBJECT_HERO_ANSWER_NO","-"};
        check(literalEvents==expected && strings.empty());
    }
    EntitySetThingAsAllyOfThing_API=reinterpret_cast<tEntitySetThingAsAllyOfThing>(&initAlly);
    CScriptThingVTable initTable{};initTable.IsNull=reinterpret_cast<decltype(initTable.IsNull)>(&initIsNull);
    initActor.pVTable=reinterpret_cast<void**>(&initTable);
    std::remove_pointer_t<decltype(initActor.pImp.Info)> initInfo{};
    for(bool data:{false,true})for(bool info:{false,true})for(bool hero:{false,true}){
        initEvents.clear();initInfo.RefCount=7;
        initActor.pImp.Data=data?reinterpret_cast<decltype(initActor.pImp.Data)>(0x4242):nullptr;
        initActor.pImp.Info=info?&initInfo:nullptr;initHeroResult=hero?&initHeroActor:nullptr;
        sol::state lua;lua.open_libraries(sol::lib::base);RegisterRetailResources(lua);lua["me"]=&initActor;
        lua["scope"]=[&](sol::protected_function body){WithRetailResources(reinterpret_cast<CGameScriptInterfaceBase*>(1),body);};
        auto result=lua.safe_script("scope(function(r) r:InitializeBullyActor(me) end)",sol::script_pass_on_error);
        check(result.valid() && initInfo.RefCount==7 && !initNullQueries);
        check(initActor.pVTable==reinterpret_cast<void**>(&initTable));
        check(initEvents==std::vector<std::string>{"damage","kill","combo","information","hero","ally","pushable"});
    }
    std::cout<<"Bully runoff x86: 4 string-map/empty-Thing/movie/error Lua policies passed; 8 raw animation byte policies passed; 4 HUD/question literal policies passed; 8 Init copy policies passed\n";return 0;
}catch(const std::exception& e){std::cerr<<e.what()<<'\n';return 1;}}
