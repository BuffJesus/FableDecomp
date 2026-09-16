"""Attach the isolated LiveFather recovery to converter entity entry points."""
import hashlib
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.live_father_candidate import generate as compose
from tools.script_recovery.native_new_oakvale_conditions import verify as condition

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_LiveFather.lua'
DRAFT_SHA='0477aabb0be9ea2cb847b2cecd6cd84f0e2be35b4f99086aa562a331a52d2fd1'


def generate(*,draft_path=DRAFT):
    draft=Path(draft_path).read_bytes()
    if hashlib.sha256(draft).hexdigest()!=DRAFT_SHA:raise ValueError('LiveFather converter draft changed')
    source,report=compose();entry=condition('NOVI_LiveFather')
    if entry['method']!='RegisterBoundConsciousCondition':raise ValueError('LiveFather condition changed')
    call='quest:RegisterBoundConsciousCondition(me)'
    if source.count(call)!=1:raise ValueError('LiveFather condition registration count changed')
    source=source.replace(call,'quest:RegisterBoundConsciousCondition()')
    state='''local entityFields = {}
local entityState = {
    GetStateInt = function(_, name) return entityFields[name] end,
    SetStateInt = function(_, name, value) entityFields[name] = value end,
}
'''
    entrypoints='''
function Init(quest, me)
    LiveFatherInit(quest, me, entityState)
end

function Main(quest, me)
    LiveFatherMain(quest, me, entityState, function(deed)
        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, deed)
    end)
end
'''
    result=state+source+entrypoints
    LuaRuntime().execute('assert(load(...))',result)
    report=dict(report,enabled=False,entryCondition=entry,draftSha256=DRAFT_SHA,
                candidateSha256=hashlib.sha256(result.encode()).hexdigest(),
                integration='work/live_father_converter/INTEGRATION.md')
    return result,report
