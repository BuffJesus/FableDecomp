"""Match native entity callback query placement under the quest's opt-in policy."""
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.compose_runtime_files import compose

OUTPUT=ROOT/'work/oakvale_entity_frame_integration'


def edit(bodies):
    source=bodies['LuaEntityHost.cpp'].decode()
    before='if (IsActiveThreadTerminating_Entity_API && IsActiveThreadTerminating_Entity_API(this)) {'
    if source.count(before)!=1:raise ValueError('Entity Main entry guard changed')
    after='''if ((!m_pParentHost || m_pParentHost->GetNativeLifetime()!=NativeQuestLifetime::NewOakValeIntro) &&
        IsActiveThreadTerminating_Entity_API && IsActiveThreadTerminating_Entity_API(this)) {'''
    bodies['LuaEntityHost.cpp']=source.replace(before,after).encode()
    return dict(status='native entity callbacks retain explicit Lua termination queries',
        policy='NewOakValeIntro parent only',
        limits='Engine scheduler liveness and native predicate dispatch are separate gates; legacy host entry guard remains.')


def prepare():
    return compose(ROOT/'work/oakvale_frame_integration',OUTPUT,edit,'oakvale-entity-frame-policy.patch',
        extra_sources={'LuaEntityHost.cpp':Path('D:/Code/ForgeFSE-retail-shadow/FableScriptExtender/LuaEntityHost.cpp')})


if __name__=='__main__':print(prepare()['candidateSha256'])
