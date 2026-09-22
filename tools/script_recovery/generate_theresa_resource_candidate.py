"""Assemble a disabled Theresa candidate; integration limits remain explicit."""
import hashlib
import json
import re
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT, RData
from tools.script_recovery.native_new_oakvale_conditions import verify as condition
from tools.script_recovery.native_theresa_init import verify as init
from tools.script_recovery.native_theresa_control import verify as control
from tools.script_recovery.native_theresa_movies import verify as movies
from tools.script_recovery.native_theresa_health import verify as health
from tools.script_recovery.native_theresa_actor_maps import verify as actors
from tools.script_recovery.native_theresa_guard_vectors import verify as guards

DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Theresa.lua'
DRAFT_SHA='de70fb7e04dd331e33cf76bbebae5707163067a4c4df38ce05231346fcda5119'   # 2026-09-22: the const-bool nil guard reached MsgIsPresentedWithItem
PARTS=('theresa_init_body.lua','theresa_approach_body.lua','theresa_cutscene_actors.lua',
       'theresa_chocolate_question.lua','theresa_gift_commit.lua','theresa_accept_chocolates.lua',
       'theresa_meeting_body.lua','theresa_offer_choice.lua','theresa_speech_body.lua',
       'theresa_offer_body.lua','theresa_presented_gift.lua','theresa_talk_body.lua',
       'theresa_hit_movie.lua','theresa_hit_body.lua','theresa_outro_body.lua',
       'theresa_presented_choice.lua','theresa_main_body.lua')


def generate(output=ROOT/'work/theresa_converter/candidate', *, draft_path=DRAFT):
    if hashlib.sha256(Path(draft_path).read_bytes()).hexdigest()!=DRAFT_SHA:raise ValueError('Theresa draft changed')
    data=RData();entry=condition('NOVI_Theresa',data)
    if entry['method']!='RegisterBoundConsciousCondition':raise ValueError('Theresa entry condition changed')
    evidence={name:check(data) for name,check in (('init',init),('control',control),('movies',movies),
                                               ('health',health),('actors',actors),('guards',guards))}
    sources=[];inputs={}
    for name in PARTS:
        path=Path(__file__).with_name(name);body=path.read_text()
        inputs[name]=hashlib.sha256(path.read_bytes()).hexdigest()
        body,count=re.subn(r'\nreturn [\w, ]+\s*$','\n',body)
        if count!=1:raise ValueError('Theresa helper return boundary changed: '+name)
        sources.append(body)
    prefix='''-- DISABLED Theresa candidate: full native dispatcher and merged host validation pending.
-- Entity state: DoneIntro = native byte1D; AskedForPresent = native byte1C.
local entityFields = {}
local entityState = {
    GetStateBool = function(_, key) return entityFields[key] end,
    SetStateBool = function(_, key, value) entityFields[key] = value end,
}
'''
    entrypoints='''
function Init(quest, me)
    quest:WithRetailResources(function(resources)
        initializeTheresa(quest, me, resources, entityState)
    end)
end

function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:WithRetailResources(function(resources)
        runTheresaMainAfterCondition(quest, me, resources, entityState)
    end)
end
'''
    result=prefix+'\n'.join(sources)+entrypoints
    LuaRuntime().execute('assert(load(...))',result)
    report=dict(status='disabled-composed-candidate',enabled=False,gameplayComplete=False,
        draftSha256=DRAFT_SHA,candidateSha256=hashlib.sha256(result.encode()).hexdigest(),
        inputs=inputs,entryCondition=entry,evidence=evidence,
        runtimeProposal='work/theresa_resource_integration/proposal.json',
        remaining=['Original-byte full outer dispatcher comparison',
                   'Actual assembled phase composition and error paths',
                   'Merged conscious-condition host/scheduler behavior',
                   'Entity/parent state and coroutine integration; DLL and live gameplay validation'])
    output=Path(output);output.mkdir(parents=True,exist_ok=True)
    (output/'NOVI_Theresa.resource_candidate.lua').write_text(result)
    (output/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    return result,report


if __name__=='__main__':print(generate()[1]['status'])
