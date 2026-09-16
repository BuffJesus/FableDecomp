"""Attach the isolated Victim recovery to converter entity entry points."""
import hashlib
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.victim_candidate import generate as compose
from tools.script_recovery.native_new_oakvale_conditions import verify as condition

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Victim.lua'
DRAFT_SHA='84ae057af7f25b7901ff24a3fbdbc08092d2841a10c5c816ca1d303dcd15ab5a'


def generate(*,draft_path=DRAFT):
    draft=Path(draft_path).read_bytes()
    if hashlib.sha256(draft).hexdigest()!=DRAFT_SHA:raise ValueError('Victim converter draft changed')
    source,report=compose();entry=condition('NOVI_Victim')
    if entry['method']!='RegisterBoundConsciousCondition':raise ValueError('Victim condition changed')
    call='quest:RegisterBoundConsciousCondition(me)'
    if source.count(call)!=1:raise ValueError('Victim condition registration count changed')
    source=source.replace(call,'quest:RegisterBoundConsciousCondition()')
    state='''local entityFields = {}
local entityState = {
    GetStateBool = function(_, name) return entityFields[name] end,
    SetStateBool = function(_, name, value) entityFields[name] = value end,
}
'''
    entrypoints='''
function Init(quest, me)
    VictimInit(quest, me, entityState)
end

function Main(quest, me)
    VictimMain(quest, me, entityState, function(deed)
        require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, deed)
    end)
end
'''
    result=state+source+entrypoints
    LuaRuntime().execute('assert(load(...))',result)
    report=dict(report,enabled=False,entryCondition=entry,draftSha256=DRAFT_SHA,
                candidateSha256=hashlib.sha256(result.encode()).hexdigest(),
                integration='work/victim_converter/INTEGRATION.md')
    return result,report
