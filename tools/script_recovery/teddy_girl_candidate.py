"""Generate a disabled whole-body TeddyGirl candidate from proven phases."""
import hashlib,json
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.teddy_girl_health import DRAFT,SOURCE as HEALTH,prove
from tools.script_recovery.teddy_girl_question import recover as question
from tools.script_recovery.teddy_girl_talk import recover as talk
from tools.script_recovery.teddy_girl_predicates import SOURCE as PREDICATES,prove as predicates
from tools.script_recovery.teddy_girl_presented_phase import recover as presented
from tools.script_recovery.teddy_girl_hit import recover as hit
from tools.script_recovery.teddy_girl_movement import recover as movement
from tools.script_recovery.teddy_girl_lifecycle import SOURCE as LIFECYCLE,prove as lifecycle
from tools.script_recovery.teddy_girl_lifetimes import inventory
from tools.script_recovery.native_new_oakvale_conditions import verify

MAIN='''local function resourceBody(quest, me, resources)
    local control = resources:NewResource()
    local bully = resources:NewThingFromScriptName("NOVI_Bully")
    if quest:IsActiveThreadTerminating() then return end
    local function given()
        TeddyGirlGiven(quest, resources, me, __native_entity_state, function()
            require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        end)
    end
    while true do
        local output = resources:NewPresentedItemOutput(me)
        if resources:TeddyGirlTalkedWithTeddy(me) then
            if quest:IsActiveThreadTerminating() then return end
            if not TeddyGirlAcquire(quest, resources, me, control) then return end
            if not TeddyGirlWithMovie(quest, resources, function()
                return TeddyGirlQuestion(quest, resources, control, __native_entity_state, given)
            end) then return end
        else
            local kind = TeddyGirlPresentedKind(resources, output)
            if not TeddyGirlPresentedResponse(quest, resources, me, control, __native_entity_state, kind, given) then return end
        end
        if not TeddyGirlDeparture(quest, resources, me, control, bully) then return end
        if resources:IsTalkedToByHero(me) then
            if quest:IsActiveThreadTerminating() then return end
            if not TeddyGirlAcquire(quest, resources, me, control) then return end
            if not TeddyGirlWithMovie(quest, resources, function()
                return TeddyGirlTalk(quest, me, resources, control, __native_entity_state)
            end) then return end
        end
        if resources:IsHitByHeroExceptAbility(me, 14) then
            if not TeddyGirlHit(quest, resources, me, control, __native_entity_state, function(amount)
                require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, amount)
            end) then return end
        end
        resources:PrepareResource(control)
        resources:DestroyPresentedItemOutput(output)
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return end
    end
end

function Main(quest, me)
    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        resourceBody(quest, me, resources)
    end)
end

function Init(quest, me)
    quest:WithRetailResources(function(resources)
        TeddyGirlInitialize(quest, resources, me, __native_entity_state)
    end)
end

function GivenTeddy(quest, me)
    quest:WithRetailResources(function(resources)
        TeddyGirlGiven(quest, resources, me, __native_entity_state, function()
            require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        end)
    end)
end
'''

def generate(*,draft_path=DRAFT):
    w=prove();draft=Path(draft_path).read_text()
    if hashlib.sha256(draft.encode()).hexdigest()!=w['draftSha256']:raise ValueError('TeddyGirl candidate source correspondence changed')
    scopes=inventory()
    if not scopes['cfgLifetimeProved']:raise ValueError('TeddyGirl lifetime proof failed')
    entry=verify('NOVI_TeddyGirl');reports={'health':w,'lifetimes':scopes,'entry':entry,'predicates':predicates(),'lifecycle':lifecycle()}
    chunks=[HEALTH,PREDICATES,LIFECYCLE]
    for name,recover in (('question',question),('talk',talk),('presented',presented),('hit',hit),('movement',movement)):
        source,evidence=recover();chunks.append(source);reports[name]=evidence
    prefix=draft[:draft.index('function Init(')]
    source='-- DISABLED TeddyGirl native candidate: pending host/state/scheduler integration.\n'+prefix+'\n'.join(chunks)+MAIN
    LuaRuntime().execute('return function()\n'+source+'\nend')
    reports.update(enabled=False,gameplayComplete=False,sourceSha256=hashlib.sha256(source.encode()).hexdigest(),
        runtimeProposal='work/teddy_girl_converter/runtime_proposal/proposal.json',
        mergedOwnerProposal='work/teddy_girl_converter/owner_proposal/proposal.json',
        limits=['Merged TeddyGirl owner/adapters are an offline unapplied proposal; runtime integration remains pending.',
                'Native phases and the outer dispatcher have separate comparisons; phase interiors are abstracted in the dispatcher oracle, not an unabstracted whole-engine replay.',
                'Entity fields preserve generated local storage; parent teardown, state persistence and restore ordering remain integration gates.',
                'Enclosing WithRetailResources must destroy outstanding entries in reverse construction order after cancellation or callback errors.'])
    out=ROOT/'work/teddy_girl_converter/candidate';out.mkdir(parents=True,exist_ok=True)
    (out/'NOVI_TeddyGirl.lua').write_text(source);(out/'REPORT.json').write_text(json.dumps(reports,indent=2)+'\n');(out/'quests.lua').write_text('Quests = {}\n')
    return source,reports

if __name__=='__main__':print(generate()[1]['sourceSha256'])
