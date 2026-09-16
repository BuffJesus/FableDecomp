"""Reproducibly emit disabled Guard Main composition; runtime proposals pending."""
import hashlib
import json
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.guard_health import DRAFT,SOURCE as HEALTH,prove
from tools.script_recovery.guard_lecture import recover as lecture
from tools.script_recovery.guard_approach import recover as approach
from tools.script_recovery.guard_interactions import recover as interactions
from tools.script_recovery.guard_lifetime_inventory import inventory
from tools.script_recovery.guard_init import SOURCE as INIT,prove as init_proof
from tools.script_recovery.guard_capabilities import prove as capability_proof
from tools.script_recovery.lift_native_lua import ROOT

MAIN='''local function resourceBody(quest, me, resources)
    local control = resources:NewResource()
    if quest:IsActiveThreadTerminating() then return end
    while true do
        local phase = GuardApproach(quest, me, resources, control)
        if phase == "cancel" then return end
        if phase == "lecture" then
            quest:SetStateInt("GuardsDealtWithBadDeeds", quest:GetStateInt("BadDeedsPerformed"))
            if not GuardAcquire(quest, me, resources, control) then return end
            while not quest:IsHeroControlledByPlayer() do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return end
            end
            if quest:IsActiveThreadTerminating() then return end
            local movie = resources:StartMovie("")
            resources:Pause(true)
            resources:GuardFaceHero(me, false, true)
            local active = GuardLecture(quest, resources, control)
            resources:Pause(false)
            resources:DestroyMovie(movie)
            if not active then return end
            resources:PrepareResource(control)
        elseif phase == "claimed" then
            resources:PrepareResource(control)
        end
        if not GuardTalk(quest, me, resources, control) then return end
        if not GuardHit(quest, me, resources, control) then return end
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
end

function Main(quest, me)
    quest:WithRetailResources(function(resources)
        resourceBody(quest, me, resources)
    end)
end
'''

def generate(*,draft_path=DRAFT):
    w=prove();draft=Path(draft_path).read_text()
    if hashlib.sha256(draft.encode()).hexdigest()!=w['draftSha256']:raise ValueError('Guard candidate draft changed')
    lifetime=inventory()
    if not lifetime['cfgLifetimeProved']:raise ValueError('Guard ownership proof failed')
    a,ar=approach();l,lr=lecture();i,ir=interactions()
    init=init_proof();capabilities=capability_proof()
    source='-- Disabled native Guard candidate. Pending runtime integration and lifecycle review.\n'+HEALTH+a+l+i+INIT+MAIN
    LuaRuntime().execute('return function()\n'+source+'\nend')
    report={'enabled':False,'gameplayComplete':False,'nativeHealth':w,'nativeLifetime':lifetime,
            'approach':ar,'lecture':lr,'interactions':ir,'init':init,'capabilities':capabilities,
            'runtimeProposal':'work/guard_converter/runtime_proposal/proposal.json',
            'sourceSha256':hashlib.sha256(source.encode()).hexdigest(),
            'entryCondition':None,'limits':['Main is composed from native-backed phases; whole-dispatcher differential gate is still pending.',
                'Init native copied-Thing consumers and eight staged Guard adapters are verified offline; merged runtime binding remains pending.',
                'Guard adapters compile against actual FSE types and real Lua; proposal remains unapplied.',
                'State persistence, helpers, engine scheduling and teardown are not established by offline phase tests.']}
    out=ROOT/'work/guard_converter/candidate';out.mkdir(parents=True,exist_ok=True)
    (out/'NOVI_Guard.lua').write_text(source);(out/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    (out/'quests.lua').write_text('-- Disabled incomplete Guard review candidate\nQuests = {}\n')
    return source,report

if __name__=='__main__':print(generate()[1]['sourceSha256'])
