"""Isolated LiveFather composition, gated on original retail Main bytes."""
import json
from pathlib import Path
from tools.script_recovery.live_father_hit import recover as hit
from tools.script_recovery.live_father_payment import recover as payment
from tools.script_recovery.live_father_init import generate as init
from tools.script_recovery.lift_native_lua import ROOT,RData

BODY='''function LiveFatherAcquire(quest, resources, control, target, priority, prepare)
    if prepare then resources:PrepareResource(control) end
    while not resources:TryAcquire(control, target(), priority) do
        quest:NewScriptFrame()
        if quest:IsActiveThreadTerminating() then return false end
    end
    return not quest:IsActiveThreadTerminating()
end

function LiveFatherMovie(quest, resources, body)
    local movie = resources:NewMovie()
    local pauseAttempted = false
    local ok, complete = xpcall(function()
        resources:StartOwnedMovie(movie, "")
        pauseAttempted = true
        quest:PauseAllNonScriptedEntities(true)
        return body()
    end, function(err) return err end)
    local cleanupError
    if pauseAttempted then
        local closed, err = pcall(function() quest:PauseAllNonScriptedEntities(false) end)
        if not closed then cleanupError = err end
    end
    local closed, err = pcall(function() resources:DestroyMovie(movie) end)
    if not closed and cleanupError == nil then cleanupError = err end
    if not ok then error(complete, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return complete
end

function LiveFatherRoutine(quest, me, resources, control, state, addBadDeed)
    if quest:IsActiveThreadTerminating() then return false end
    repeat
        if not LiveFatherAcquire(quest, resources, control, function() return me end, 3, true) then return false end
        if resources:IsTalkedToByHero(me) then
            if quest:IsActiveThreadTerminating() then return false end
            if not LiveFatherAcquire(quest, resources, control, function() return me end, 4, true) then return false end
            if not LiveFatherMovie(quest, resources, function()
                return LiveFatherPaymentDialogue(quest, me, resources, control, state)
            end) then return false end
        end
        if not LiveFatherHandleHit(quest, me, resources, control, addBadDeed) then return false end
        quest:NewScriptFrame()
    until quest:IsActiveThreadTerminating()
    return false
end

function LiveFatherIntro(quest, me, resources, control)
    if quest:IsActiveThreadTerminating() then return false end
    local hero = resources:NewResource()
    if not LiveFatherAcquire(quest, resources, hero, function() return quest:GetHero() end, 4, false) then
        resources:ReleaseResource(hero)
        return false
    end
    local actors = resources:NewActorMap()
    resources:SetActor(actors, "Hero", hero)
    resources:SetActor(actors, "Father", control)
    local complete = LiveFatherMovie(quest, resources, function()
        local cameraOk, cameraError = xpcall(function()
            quest:FixMovieSequenceCamera(true)
            resources:RunMacro("CS_OAKVALE_INTRO_FATHER", actors, false, true)
        end, function(err) return err end)
        local cameraClosed, closeError = pcall(function() quest:FixMovieSequenceCamera(false) end)
        if not cameraOk then error(cameraError, 0) end
        if not cameraClosed then error(closeError, 0) end
        quest:SetAllSoundsAsMuted(false)
        quest:SetStateBool("DadFinishedIntro", true)
        quest:Pause(1.0)
        quest:CameraResetToViewBehindHero(0.0)
        quest:CameraDefault()
        local xbox = quest:IsXbox()
        if quest:IsActiveThreadTerminating() then return false end
        resources:DisplayRawGameInfo(xbox and "TEXT_QST_048_INSTRUCTION_HIGHLIGHTING" or "TEXT_QST_048_INSTRUCTION_HIGHLIGHTING_PC")
        while not quest:MsgIsGameInfoClickedPast() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local counterKey = resources:NewLiteralText("HUD_DEED_GOOD_ICON")
        local counterOk, counterError = xpcall(function()
            local counter = resources:AddLiveFatherGoodDeedCounter(counterKey)
            quest:SetStateInt("GUIGoodDeedCounter", counter)
        end, function(err) return err end)
        local keyClosed, keyError = pcall(function() resources:DestroyText(counterKey) end)
        if not counterOk then error(counterError, 0) end
        if not keyClosed then error(keyError, 0) end
        quest:DisplayQuestInfo(true)
        return true
    end)
    resources:DestroyActorMap(actors)
    resources:ReleaseResource(hero)
    return complete
end

function LiveFatherMain(quest, me, state, addBadDeed)
    quest:RegisterBoundConsciousCondition(me)
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        if not LiveFatherAcquire(quest, resources, control, function() return me end, 4, true) then return end
        if not quest:GetStateBool("DadFinishedIntro") then
            if not LiveFatherIntro(quest, me, resources, control) then return end
        end
        LiveFatherRoutine(quest, me, resources, control, state, addBadDeed)
    end)
end
'''

def prove(data=None):
    data=data or RData();hit(data);payment(data)
    witness=json.loads(Path(__file__).with_name('live_father_composition_witness.json').read_text())
    for slot,target in witness['slots'].items():
        if int.from_bytes(data.bytes_at(witness['vtable']+int(slot,16),4),'little')!=int(target,16):raise ValueError('LiveFather composition API changed')
    for address,value in witness['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('LiveFather intro literal changed')
    return witness

def generate():
    witness=prove()
    hit_source,hw=hit();payment_source,pw=payment()
    init_source,iw=init()
    source='-- Disabled LiveFather composition: explicit state/helper injection, no quest registration.\n'+init_source+hit_source+payment_source+BODY
    report={'mainSha256':hw['mainSha256'],'routineStart':0xdb8aee,'introStart':0xdb8798,
        'priority':{'initialSelf':4,'introHero':4,'idleSelf':3,'talkSelf':4,'hitSelf':4},
        'initSha256':iw['functions'][0]['sha256'],'apiAndLiteralProof':witness,
        'limits':['Eight actor capabilities compiled in an unapplied standalone x86/real-Lua proposal; merged owner validation remains pending.',
                  'Payment and Intro adapters are staged but not integrated. Existing no-argument NewText must remain unchanged; NewLiteralText is separate.',
                  'Active-quest getter pre-return failure ownership is not established; the adapter requires successful output construction.',
                  'Entry, intro, recurring dispatcher, payment and hit have original-byte comparisons at explicit separately checked phase boundaries; a single unabstracted Main execution and composed host validation are pending.',
                  'State persistence, registered condition and scheduler teardown integration remain pending.']}
    out=ROOT/'work/live_father_converter';out.mkdir(parents=True,exist_ok=True)
    (out/'CANDIDATE.lua').write_text(source);(out/'COMPOSITION_EVIDENCE.json').write_text(json.dumps(report,indent=2)+'\n')
    return source,report

if __name__=='__main__':generate()
