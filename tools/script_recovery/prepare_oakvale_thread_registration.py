"""Share native registration and roll back unregistered Lua thread slots."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose

OUTPUT=ROOT/'work/oakvale_thread_registration_integration'


def edit(bodies):
    source=bodies['LuaQuestHost.cpp'].decode()
    start=source.index('void LuaQuestHost::RegisterMain() {')
    original=source[start:]
    marker='    if(!CCharString_Construct_Literal'
    body=original[original.index(marker):]
    substitutions={
        'bool nameLive=false,sectionLive=false,constructed=false,handedOff=false;':'bool nameLive=false,sectionLive=false,constructed=false,handedOff=false;',
        'CCharString_Construct_Literal(&name,"Main",-1)':'CCharString_Construct_Literal(&name,functionName.c_str(),-1)',
        'function->pThunkToMain=ASLR<void*>(0x00CDD440);':'function->pThunkToMain=thunk;',
        'function->pOwnerScript=&this->base;':'function->pOwnerScript=owner;',
        'CCharString_Construct_Literal(&section,"",-1)':'CCharString_Construct_Literal(&section,sectionName.c_str(),-1)',
        'handedOff=true;AddSpawnedFunction_func(&this->base,function,&section);':
            'handedOff=true;if(ownershipTransferred)*ownershipTransferred=true;AddSpawnedFunction_func(owner,function,&section);',
    }
    for before,after in substitutions.items():
        if body.count(before)!=1:raise ValueError('Main native registration helper changed: '+before)
        body=body.replace(before,after)
    helper='''static void RegisterOakvaleSpawnedFunction(CScriptBase_Retail* owner,
    const std::string& functionName,const std::string& sectionName,void* thunk,bool* ownershipTransferred=nullptr) {
    if(ownershipTransferred)*ownershipTransferred=false;
'''+body
    replacement='''void LuaQuestHost::RegisterMain() {
    if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro)
        RegisterOakvaleSpawnedFunction(&this->base,"Main","",ASLR<void*>(0x00CDD440));
    else AutoRegisterMain(&this->base,GetMemberFunctionAddress(&LuaQuestHost::Main));
}
'''
    source=source[:start]+replacement
    anchor='void LuaQuestHost::CreateThread(const std::string& luaFunctionName, const std::string& regionName, const std::vector<sol::object>& args) {'
    if source.count(anchor)!=1:raise ValueError('Host CreateThread insertion changed')
    native='''    if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro) {
        if(m_threadCount>=MAX_QUEST_THREADS)throw std::runtime_error("Oakvale Lua thread slots exhausted");
        const int threadIndex=m_threadCount;
        QuestThreadInfo info;info.functionName=luaFunctionName;info.args=args;
        lua_State* mainL=m_pLuaState->lua_state();
        info.threadState=lua_newthread(mainL);
        const int reference=luaL_ref(mainL,LUA_REGISTRYINDEX);info.registryRef=reference;
        bool tracked=false,handedOff=false;
        try {
            auto inserted=m_threads.emplace(threadIndex,std::move(info));
            if(!inserted.second)throw std::runtime_error("Oakvale Lua thread slot collision");
            tracked=true;++m_threadCount;
            RegisterOakvaleSpawnedFunction(&this->base,luaFunctionName,regionName,
                GetMemberFunctionAddress(g_threadRunnerPool[threadIndex]),&handedOff);
        }catch(...) {
            // Once registration starts the engine may retain this callback.
            // Keep its Lua anchor until host teardown in that case.
            if(!handedOff){
                if(tracked){m_threads.erase(threadIndex);--m_threadCount;}
                luaL_unref(mainL,LUA_REGISTRYINDEX,reference);
            }
            throw;
        }
        return;
    }
'''
    source=source.replace(anchor,helper+'\n'+anchor+'\n'+native)
    bodies['LuaQuestHost.cpp']=source.encode()
    header=bodies['LuaQuestHost.h'].decode()
    anchor='    const std::string& GetScriptName() const;'
    if header.count(anchor)!=1:raise ValueError('Host thread-region accessor insertion changed')
    header=header.replace(anchor,anchor+'\n    const char* DefaultThreadRegion() const { return m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro?"":"Class"; }')
    bodies['LuaQuestHost.h']=header.encode()
    state=bodies['LuaQuestState.cpp'].decode()
    for before,after in (('std::string regionName = "Class";',
                         'std::string regionName = m_pParentHost->DefaultThreadRegion();'),
                        ('args.get_or("region", std::string("Class"))',
                         'args.get_or("region", std::string(m_pParentHost->DefaultThreadRegion()))')):
        if state.count(before)!=1:raise ValueError('Quest thread default region changed')
        state=state.replace(before,after)
    bodies['LuaQuestState.cpp']=state.encode()
    return dict(status='opt-in native thread registration with pre-handoff slot rollback',
        nativeScope='DoMission thread callers use operator new, 0x3c objects, native spawned vtable and empty sections',
        limits='Retains Lua anchors after entering engine registration. Engine scheduler, registration failure after acceptance, and Lua allocator longjmp behavior remain separate gates.')


def prepare():return compose(ROOT/'work/oakvale_main_registration_integration',OUTPUT,edit,'oakvale-thread-registration.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
