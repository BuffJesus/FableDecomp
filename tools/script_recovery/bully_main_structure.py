"""Structured outer Bully Main with explicit native normal cleanup joins."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.bully_item_structure import generate as item_candidate
from tools.script_recovery.lift_native_lua import ROOT

ACQUIRE='''local function BullyAcquirePrepared(quest, me, acquire)
    while not acquire() do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function BullyRunoffControls(quest, resources, me, control, retainedVictim)
    local hero = resources:NewResource()
    resources:PrepareResource(hero)
    local victim
    local function run()
        if not BullyAcquirePrepared(quest, me, function()
            return resources:TryAcquire(hero, quest:GetHero(), 4)
        end) then return false end
        victim = resources:NewResource()
        resources:PrepareResource(victim)
        if not BullyAcquirePrepared(quest, me, function()
            return resources:TryAcquireThing(victim, retainedVictim, 4)
        end) then return false end
        return BullyRunoffMovie(quest, resources, hero, victim, control, retainedVictim)
    end
    local completed = run()
    if victim ~= nil then resources:ReleaseResource(victim) end
    resources:ReleaseResource(hero)
    if completed then
        quest:SetStateBool("BullyRanOff", true)
        require("NewOakValeIntro.native_quest_helpers").AddGoodDeed(quest, me)
        quest:RemoveThing(me, false, true)
    end
end

'''
BODY='''    quest:RegisterBoundConsciousCondition()
    quest:NewScriptFrame(me)
    if quest:IsActiveThreadTerminating() then return end
    bully_control = resources:NewResource()
    resources:PrepareResource(bully_control)
    local victim
    local function acquire_self()
        return BullyAcquirePrepared(quest, me, function()
            return resources:TryAcquire(bully_control, me, 4)
        end)
    end
    local function run()
        if not acquire_self() then return end
        if not BullyReturnHomePhase(quest, me, resources, bully_control) then return end
        if quest:IsActiveThreadTerminating() then return end
        victim = resources:NewThingFromScriptName("NOVI_Victim")
        while not quest:IsActiveThreadTerminating() do
            resources:PrepareResource(bully_control)
            if not acquire_self() then return end
            if __native_entity_state:GetStateBool("DoneIntro") then
                if not handle_bully_item() then return end
            end
            release_presented()
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then return end
                resources:PrepareResource(bully_control)
                if not acquire_self() then return end
                bully_movie = resources:StartMovie("")
                resources:Pause(true)
                local completed = BullyMainDialogue(quest, resources, bully_control, __native_entity_state)
                finish_bully_movie()
                if not completed then return end
            end
            if resources:IsHitByHeroExceptAbility(me, 14) then
                if quest:IsActiveThreadTerminating() then return end
                if quest:GetStateInt("GUIBullyHealthCounter") == -999 then
                    if quest:IsActiveThreadTerminating() then return end
                    local counter = resources:AddBullyHealthBar(__native_entity_state:GetStateInt("InitialHealth"))
                    quest:SetStateInt("GUIBullyHealthCounter", counter)
                end
                BullySetHeroAlliance(quest, resources, me)
                local hits = __native_entity_state:GetStateInt("HitsTaken")
                __native_entity_state:SetStateInt("HitsTaken", hits + 1)
                if __native_entity_state:GetStateInt("InitialHealth") <= hits + 1 then
                    if quest:IsActiveThreadTerminating() then return end
                    quest:SetStateBool("BullySubdued", true)
                    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBullyHealthCounter"))
                    resources:PrepareResource(bully_control)
                    if acquire_self() then
                        BullyRunoffControls(quest, resources, me, bully_control, victim)
                    end
                    return
                end
                if quest:IsActiveThreadTerminating() then return end
                BullyHitConversation(resources, me, victim)
                quest:UpdateQuestInfoBar(quest:GetStateInt("GUIBullyHealthCounter"),
                    __native_entity_state:GetStateInt("InitialHealth") - __native_entity_state:GetStateInt("HitsTaken"), -1.0, -1.0)
            end
            if not BullyProximity(quest, resources, me, victim, bully_control, __native_entity_state) then return end
            quest:NewScriptFrame(me)
        end
    end
    run()
    release_presented()
    if victim ~= nil then resources:DestroyThing(victim) end
    resources:ReleaseResource(bully_control)
end

'''

def lower(source):
    start=source.index('    local aVar26, aVar5,')
    end=source.index('function GivenTeddy(',start)
    witness=json.loads(Path(__file__).with_name('bully_main_structure_witness.json').read_text())
    if hashlib.sha256(source[start:end].encode()).hexdigest()!=witness['regionSha256']:
        raise ValueError('Bully outer Main source correspondence changed')
    source=ACQUIRE+source[:start]+BODY+source[end:]
    return source,witness

def generate(**kwargs):
    source,report=item_candidate(**kwargs);source,evidence=lower(source)
    out=ROOT/'work/bully_converter/complete_structure';out.mkdir(parents=True,exist_ok=True)
    (out/'NOVI_Bully.lua').write_text(source)
    (out/'quests.lua').write_text('Quests = {} -- Disabled offline candidate.\n')
    report['mainStructure']=evidence;report['sourceSha256']=hashlib.sha256(source.encode()).hexdigest()
    (out/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    return source,report

if __name__=='__main__':generate()
