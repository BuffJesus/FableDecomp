"""Stage opt-in native quest base destruction after VM and member cleanup."""
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose
from tools.script_recovery.native_oakvale_lifecycle import prove

OUTPUT=ROOT/'work/oakvale_lifecycle_integration'


def once(source,old,new):
    if source.count(old)!=1:raise ValueError('Oakvale lifecycle insertion changed: '+old[:70])
    return source.replace(old,new)


def edit(bodies):
    evidence=prove()
    header=bodies['LuaQuestHost.h'].decode()
    header=once(header,'    void Destructor(bool bDelete);','    void* Destructor(unsigned flags);')
    header=once(header,'    std::string m_scriptName;',
                '    NativeQuestLifetime m_nativeLifetime;\n    std::string m_scriptName;')
    bodies['LuaQuestHost.h']=header.encode()
    cpp=bodies['LuaQuestHost.cpp'].decode()
    newline='\r\n' if '\r\n' in cpp else '\n'
    cpp=cpp.replace('\r\n','\n')
    cpp=once(cpp,': m_scriptName(scriptName),',': m_nativeLifetime(nativeLifetime),\n    m_scriptName(scriptName),')
    old='''    m_pQuestState = std::make_unique<LuaQuestState>(this, interface);
    m_pQuestState->ConfigureNativeLifetime(nativeLifetime);'''
    new='''    try {
        m_pQuestState = std::make_unique<LuaQuestState>(this, interface);
        m_pQuestState->ConfigureNativeLifetime(nativeLifetime);
    } catch (...) {
        if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro) {
            m_pQuestState.reset();
            using NativeBaseDestructor=void(__thiscall*)(CScriptBase_Retail*);
            try { ASLR<NativeBaseDestructor>(0x00CBD510)(&this->base); } catch(...) {}
        }
        throw;
    }'''
    cpp=once(cpp,old,new)
    cpp=once(cpp,'''LuaQuestHost::~LuaQuestHost() {
    CleanupThreads();
}''','''LuaQuestHost::~LuaQuestHost() {
    CleanupThreads();
    if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro) {
        // VM finalizers may borrow quest state: release the VM while it is live.
        m_env=sol::environment();
        m_pLuaState.reset();
        // State closes watch, ambient, then its speech vectors.
        m_pQuestState.reset();
        using NativeBaseDestructor=void(__thiscall*)(CScriptBase_Retail*);
        ASLR<NativeBaseDestructor>(0x00CBD510)(&this->base);
    }
}''')
    cpp=once(cpp,'void LuaQuestHost::Destructor(bool bDelete) {',
        '''void* LuaQuestHost::Destructor(unsigned flags) {
    if(m_nativeLifetime==NativeQuestLifetime::NewOakValeIntro) {
        // DBEFA0: destruction always runs; only bit zero frees the allocation.
        this->~LuaQuestHost();
        if(flags&1u)operator delete(this);
        return this;
    }
    const bool bDelete=(flags&0xffu)!=0;''')
    start=cpp.index('void* LuaQuestHost::Destructor(unsigned flags) {')
    end=cpp.index('\n}\n',start)
    cpp=cpp[:end]+'\n    return this;'+cpp[end:]
    bodies['LuaQuestHost.cpp']=cpp.replace('\n',newline).encode()
    return dict(nativeLifecycle=evidence,
        policy='NewOakValeIntro only; default lifetime retains existing behavior',
        order=['unref threads','release Lua environment and VM','destroy quest state timers/vectors','native CScriptBase destructor','free iff flags bit zero'],
        limits='Requires full actual-host lifecycle tests; engine scheduler quiescence remains external.')


def prepare():
    return compose(ROOT/'work/oakvale_quest_integration',OUTPUT,edit,'oakvale-lifecycle-integration.patch')


if __name__=='__main__':print(prepare()['composition'])
