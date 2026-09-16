"""Structured Victim talk, movie/health lifetimes and one-time instruction UI."""
import json
from tools.script_recovery.victim_subdued import recover as proof
from tools.script_recovery.native_call_setup_ir import read_call_window
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function VictimSpeak(quest, resources, control, key, selection)
    local actor = resources:NewThingFromResource(control)
    local ok, positive = xpcall(function() return resources:ThingHealth(actor) > 0.0 end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(actor) end)
    if not ok then error(positive, 0) end
    if not closed then error(err, 0) end
    if positive then
        resources:Speak(control, quest:GetHero(), key, selection or 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
    return true
end

function VictimTalk(quest, me, resources, control, bully, state)
    if not resources:IsTalkedToByHero(me) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    local subdued = quest:GetStateBool("BullySubdued")
    if quest:IsActiveThreadTerminating() then return false end
    if not subdued then
        resources:SetRawScared(me, false)
        resources:VictimFaceHero(me, false)
    end
    local movie = resources:NewMovie()
    local pauseAttempted = false
    local ok, complete = xpcall(function()
        resources:StartOwnedMovie(movie, "")
        pauseAttempted = true
        quest:PauseAllNonScriptedEntities(true)
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        local attacked = quest:GetStateBool("HeroAttackedVictim")
        if quest:IsActiveThreadTerminating() then return false end
        local key
        if subdued then
            key = attacked and "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT" or "TEXT_QST_048_VICTIM_THANKS"
        else
            key = attacked and "TEXT_QST_048_VICTIM_PLEA_AFTER_ATTACK" or "TEXT_QST_048_VICTIM_PLEA"
        end
        if not VictimSpeak(quest, resources, control, key) then return false end
        if not subdued then
            resources:SetRawScared(me, true)
            resources:FaceTowardsRetainedThing(me, bully, false)
        end
        return true
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
    if not complete then return false end
    if not state:GetStateBool("DisplayedGameInfo") then
        if quest:IsActiveThreadTerminating() then return false end
        local xbox = quest:IsXbox()
        if quest:IsActiveThreadTerminating() then return false end
        resources:DisplayRawGameInfo(xbox and "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS" or "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS_PC")
        while not quest:MsgIsGameInfoClickedPast() do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
        state:SetStateBool("DisplayedGameInfo", true)
    end
    return true
end
'''
SITES=((0xdbd0b0,280),(0xdbd184,244),(0xdbd348,220),(0xdbd41f,232))
LITERALS={0x12d99b4:'TEXT_QST_048_VICTIM_THANKS_AFTER_HIT',0x12d9998:'TEXT_QST_048_VICTIM_THANKS',0x12d9c7c:'TEXT_QST_048_VICTIM_PLEA_AFTER_ATTACK',0x12d9c60:'TEXT_QST_048_VICTIM_PLEA',0x12d9c34:'TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS',0x12d9c08:'TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS_PC'}
def recover(data=None):
    data=data or RData();w=proof(data)[1]
    for site,output in SITES:
        window=read_call_window(data,w['mainAddress'],w['mainSize'],site,argument_count=1)
        if window is None or window.ecx!=('stack',16) or window.stack_arguments[0]!=('stack',output):raise ValueError('Victim controlled health output changed')
    for address,value in LITERALS.items():
        if data.string_at(address)!=value:raise ValueError('Victim dialogue literal changed')
    if data.bytes_at(0x122dedc,4)!=b'\0'*4:raise ValueError('Victim health threshold changed')
    return SOURCE,{'mainSha256':w['mainSha256'],'start':0xdbcf76,'end':0xdbd60f,'cancel':0xdbde18,'healthSites':SITES,'movies':[144,160],
        'limits':['Caller retains self control and Bully Thing; helper closes only movie and returned health Things.',
                  'Six actor capabilities are staged/compiled separately but remain unapplied and need merged owner validation.',
                  'Existing resource/movie/getter rollback/info capabilities require merged owner validation.']}
def generate():
    source,w=recover();out=ROOT/'work/victim_converter';out.mkdir(parents=True,exist_ok=True);(out/'TALK_PHASE.lua').write_text('-- Disabled Victim talk phase.\n'+source);(out/'TALK_EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');return source,w
