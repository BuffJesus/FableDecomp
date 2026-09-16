"""Conservative structured item phase over the byte-verified Bully candidate."""
import hashlib
import json
from pathlib import Path
from tools.script_recovery.bully_full_resource_candidate import generate as original
from tools.script_recovery.lift_native_lua import ROOT

START='        if __native_entity_state:GetStateBool("DoneIntro") then\n'
END='        ::finishBullyItemBranch::\n'
HELPER='''    local function handle_bully_item()
        if quest:IsActiveThreadTerminating() then return false end
        bully_presented = resources:NewPresentedItemOutput(me)
        local function speak_if_healthy(key)
            if BullyControlledHealthAboveThreshold(resources, bully_control) then
                resources:Speak(bully_control, quest:GetHero(), key, 0, false, true, false)
                while resources:IsPerformingScriptTask(bully_control) do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then return false end
                end
                if quest:IsActiveThreadTerminating() then return false end
            end
            return true
        end
        local function wait_for_complaint()
            quest:SetStateBool("VictimComplainsAboutLosingTeddy", true)
            while quest:GetStateBool("VictimComplainsAboutLosingTeddy") do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then return false end
            end
            return not quest:IsActiveThreadTerminating()
        end
        local function start_movie()
            bully_movie = resources:StartMovie("")
            resources:Pause(true)
        end
        local function accept_teddy()
            if not wait_for_complaint() then return false end
            if not speak_if_healthy("TEXT_QST_048_BULLY_FOUND_TEDDY_TWO") then return false end
            GivenTeddy(quest, me)
            quest:ClearThingHasInformation(me)
            return true
        end
        if resources:BullyTalkedWithTeddy(me) then
            if quest:IsActiveThreadTerminating() then return false end
            start_movie()
            if not speak_if_healthy("TEXT_QST_048_BULLY_FOUND_TEDDY_ONE") then
                finish_bully_movie()
                return false
            end
            resources:GiveBullyTeddyQuestion()
            local answer = quest:MsgIsQuestionAnsweredYesOrNo()
            while answer < 0 do
                quest:NewScriptFrame(me)
                if quest:IsActiveThreadTerminating() then
                    finish_bully_movie()
                    return false
                end
                answer = quest:MsgIsQuestionAnsweredYesOrNo()
            end
            if quest:IsActiveThreadTerminating() then
                finish_bully_movie()
                return false
            end
            local completed = true
            if answer == 1 then
                completed = not quest:IsActiveThreadTerminating()
                if completed then completed = accept_teddy() end
            end
            finish_bully_movie()
            return completed
        end
        if resources:PollPresentedItem(bully_presented) and
                resources:PresentedItemMatches(bully_presented, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
            if quest:IsActiveThreadTerminating() then return false end
            start_movie()
            local completed = accept_teddy()
            finish_bully_movie()
            return completed
        end
        if not resources:PollPresentedItem(bully_presented) or
                resources:PresentedItemMatches(bully_presented, "OBJECT_TEDDY_BEAR_UNGIVEABLE") then
            return true
        end
        if quest:IsActiveThreadTerminating() then return false end
        resources:PrepareResource(bully_control)
        while not resources:TryAcquire(bully_control, me, 4) do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        start_movie()
        local completed = speak_if_healthy("TEXT_QST_048_BULLY_DONT_WANT")
        finish_bully_movie()
        return completed
    end
'''

def lower(source):
    witness=json.loads(Path(__file__).with_name('bully_item_structure_witness.json').read_text())
    start=source.index(START);end=source.index(END,start)
    body=source[start:end]
    if hashlib.sha256(body.encode()).hexdigest()!=witness['regionSha256']:
        raise ValueError('Bully item source correspondence changed')
    anchor='    local aVar26, aVar5,'
    if source.count(anchor)!=1:raise ValueError('Bully item helper insertion changed')
    source=source[:start]+'''        if __native_entity_state:GetStateBool("DoneIntro") then
            if not handle_bully_item() then goto LAB_00dbccdd end
        end
'''+source[end:]
    source=source.replace(anchor,HELPER+anchor,1)
    # This native cleanup label was reachable only from the replaced region.
    source=source.replace('    ::LAB_00dbc7cd::\n    finish_bully_movie()\n','')
    return source,witness

def generate(**kwargs):
    source,report=original(**kwargs);source,evidence=lower(source)
    out=ROOT/'work/bully_converter/structured_candidate';out.mkdir(parents=True,exist_ok=True)
    (out/'NOVI_Bully.lua').write_text(source)
    (out/'quests.lua').write_text('Quests = {} -- Disabled offline candidate.\n')
    report['itemStructure']=evidence
    report['sourceSha256']=hashlib.sha256(source.encode()).hexdigest()
    (out/'REPORT.json').write_text(json.dumps(report,indent=2)+'\n')
    return source,report

if __name__=='__main__':generate()
