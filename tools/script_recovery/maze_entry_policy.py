"""Stage an opt-in native helper entry policy over checked current host source."""
from pathlib import Path
import hashlib
import difflib
from tools.script_recovery.maze_lifecycle_audit import audit


def stage(output, runtime):
    audit(runtime=runtime)
    header=(runtime/'LuaQuestHost.h').read_text()
    source=(runtime/'LuaQuestHost.cpp').read_text()
    original_header=header
    def replace(text,old,new):
        if text.count(old)!=1:raise ValueError('Maze entry host correspondence changed: '+old[:60])
        return text.replace(old,new)
    header=replace(header,'    int registryRef = LUA_NOREF;',
        '    int registryRef = LUA_NOREF;\n    bool nativeEntry = false;')
    signature='    void CreateThread(const std::string& luaFunctionName, const std::string& regionName, const std::vector<sol::object>& args);'
    header=replace(header,signature,signature+'''
    void CreateNativeEntryThread(const std::string&, const std::string&, const std::vector<sol::object>&);
    void CreateThreadWithEntryPolicy(const std::string&, const std::string&, const std::vector<sol::object>&, bool);
    void BeginNativeEntryTeardown() { m_nativeEntryClosing=true; }
    bool NativeEntryCallbacksDrained() const { return m_nativeEntryClosing && m_activeNativeEntries==0; }
''')
    header=replace(header,'    int m_threadCount = 0;',
        '    bool m_nativeEntryClosing=false;\n    unsigned m_activeNativeEntries=0;\n    int m_threadCount = 0;')
    start=source.index('void LuaQuestHost::CreateThread(')
    end=source.index('template void LuaQuestHost::ThreadRunner<0>();',start)
    methods=source[start:end]
    original_methods=methods
    create='void LuaQuestHost::CreateThread(const std::string& luaFunctionName, const std::string& regionName, const std::vector<sol::object>& args) {'
    methods=replace(methods,create,create+'''
    CreateThreadWithEntryPolicy(luaFunctionName,regionName,args,false);
}
void LuaQuestHost::CreateNativeEntryThread(const std::string& name,const std::string& region,const std::vector<sol::object>& args) {
    CreateThreadWithEntryPolicy(name,region,args,true);
}
void LuaQuestHost::CreateThreadWithEntryPolicy(const std::string& luaFunctionName,const std::string& regionName,const std::vector<sol::object>& args,bool nativeEntry) {
    if(nativeEntry && m_nativeEntryClosing) return;
''')
    methods=replace(methods,'    info.functionName = luaFunctionName;',
        '    info.nativeEntry = nativeEntry;\n    info.functionName = luaFunctionName;')
    begin='template<int N> void LuaQuestHost::ThreadRunner() {\n'
    methods=replace(methods,begin,begin+'''
    auto entry=m_threads.find(N);
    const bool nativeEntry=entry!=m_threads.end() && entry->second.nativeEntry;
    if(nativeEntry && m_nativeEntryClosing) return;
    struct NativeEntryScope {
        unsigned* active;
        explicit NativeEntryScope(unsigned* value):active(value){if(active)++*active;}
        ~NativeEntryScope(){if(active)--*active;}
    } active(nativeEntry?&m_activeNativeEntries:nullptr);
    if(!nativeEntry) {
''')
    methods=replace(methods,'\n    try {\n        auto it = m_threads.find(N);',
        '\n    } // Existing guarded entry remains the default.\n    try {\n        auto it = m_threads.find(N);')
    output.mkdir(parents=True,exist_ok=True)
    (output/'maze_entry_host.h').write_text(header)
    (output/'maze_entry_methods.inc').write_text(methods)
    changes=''.join(difflib.unified_diff(original_header.splitlines(True),header.splitlines(True),
        fromfile='LuaQuestHost.h',tofile='LuaQuestHost.h (proposal)'))
    changes+=''.join(difflib.unified_diff(original_methods.splitlines(True),methods.splitlines(True),
        fromfile='LuaQuestHost.cpp CreateThread/ThreadRunner',tofile='LuaQuestHost.cpp (proposal)'))
    (output/'host_changes.diff').write_text(changes)
    return {'originalHeaderSha256':hashlib.sha256(original_header.encode()).hexdigest(),
        'originalMethodsSha256':hashlib.sha256(original_methods.encode()).hexdigest(),
        'headerSha256':hashlib.sha256(header.encode()).hexdigest(),
        'methodsSha256':hashlib.sha256(methods.encode()).hexdigest(),
        'scope':'Actual current host header and CreateThread/ThreadRunner bodies with explicit policy changes only.'}
