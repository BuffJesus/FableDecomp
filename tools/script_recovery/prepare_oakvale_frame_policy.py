"""Keep native query placement explicit in the emitted Lua control flow."""
import re
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose

OUTPUT=ROOT/'work/oakvale_frame_integration'


def edit(bodies):
    package=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/readable/FSE'
    assigned=0
    for path in package.rglob('*.lua'):
        lines=path.read_text().splitlines()
        for i,line in enumerate(lines):
            if re.search(r'\b\w+\s*=\s*quest:NewScriptFrame\(',line):
                assigned+=1
                if i+1==len(lines) or not re.match(r'\s*alive\s*=\s*not quest:IsActiveThreadTerminating\(\)',lines[i+1]):
                    raise ValueError('Emitted frame result is consumed before explicit termination check: '+str(path))
    header=bodies['LuaQuestHost.h'].decode();anchor='    const std::string& GetScriptName() const;'
    if header.count(anchor)!=1:raise ValueError('Host lifetime accessor insertion changed')
    bodies['LuaQuestHost.h']=header.replace(anchor,anchor+'\n    NativeQuestLifetime GetNativeLifetime() const { return m_nativeLifetime; }').encode()
    state=bodies['LuaQuestState.cpp'].decode()
    for signature in ('bool LuaQuestState::NewScriptFrame() {','bool LuaQuestState::NewScriptFrame(CScriptThing* pMe) {'):
        if state.count(signature)!=1:raise ValueError('Quest frame overload changed')
        state=state.replace(signature,signature+'''
    if(m_pParentHost && m_pParentHost->GetNativeLifetime()==NativeQuestLifetime::NewOakValeIntro) {
        if(!m_pGameInterface || !NewScriptFrame_API)throw std::runtime_error("Oakvale frame API unavailable");
        NewScriptFrame_API(m_pGameInterface);
        // Native callers issue their termination query separately, immediately
        // after the frame. The converted Lua already contains that exact query.
        return true;
    }
''')
    bodies['LuaQuestState.cpp']=state.encode()
    host=bodies['LuaQuestHost.cpp'].decode()
    for before in ('if (IsActiveThreadTerminating_Quest_API && IsActiveThreadTerminating_Quest_API(&this->base)) {',
                   'if (this->pInterface && IsActiveThreadTerminating_API && IsActiveThreadTerminating_API(this->pInterface)) {'):
        if host.count(before)!=2:raise ValueError('Main/thread entry guard changed')
        host=host.replace(before,'if (m_nativeLifetime!=NativeQuestLifetime::NewOakValeIntro && '+before[4:])
    bodies['LuaQuestHost.cpp']=host.encode()
    return dict(status='explicit native quest Main/thread/frame query placement',assignedFrameResults=assigned,
        limits='Emitted native callers own their checks. Legacy guards remain. Entity-host entry guard and engine scheduler quiescence remain separate review gates.')


def prepare():return compose(ROOT/'work/oakvale_thread_registration_integration',OUTPUT,edit,'oakvale-frame-policy.patch')


if __name__=='__main__':print(prepare()['candidateSha256'])
