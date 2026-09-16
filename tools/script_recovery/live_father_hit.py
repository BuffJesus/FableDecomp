"""Recover LiveFather's hit phase; movie begins before control reacquisition."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData,ROOT
from tools.script_recovery.native_call_setup_ir import read_call_window
DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_LiveFather.lua'
SOURCE='''function LiveFatherHandleHit(quest, me, resources, control, addBadDeed)
    if not resources:IsHitByHeroExceptAbility(me, 14) then return true end
    if quest:IsActiveThreadTerminating() then return false end
    resources:SetThingAsAlly(me, quest:GetHero())
    resources:SetThingAsAlly(quest:GetHero(), me)
    addBadDeed(2)
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
        local actor = resources:NewThingFromResource(control)
        local healthOk, positive = xpcall(function()
            return resources:ThingHealth(actor) > 0.0
        end, function(err) return err end)
        local closed, err = pcall(function() resources:DestroyThing(actor) end)
        if not healthOk then error(positive, 0) end
        if not closed then error(err, 0) end
        if positive then
            resources:Speak(control, quest:GetHero(), "TEXT_QST_048_DAD_TEMPER", 0, false, true, false)
            while resources:IsPerformingScriptTask(control) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
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
    return complete
end
'''

def recover(data=None,*,draft_path=DRAFT):
    data=data or RData();w=json.loads(Path(__file__).with_name('live_father_hit_witness.json').read_text())
    if hashlib.sha256(data.bytes_at(w['mainAddress'],w['mainSize'])).hexdigest()!=w['mainSha256']:raise ValueError('LiveFather Main bytes changed')
    if hashlib.sha256(Path(draft_path).read_text().encode()).hexdigest()!=w['draftSha256']:raise ValueError('LiveFather draft correspondence changed')
    if data.bytes_at(w['thresholdAddress'],4)!=bytes.fromhex(w['thresholdHex']):raise ValueError('LiveFather health threshold changed')
    for address,value in w['literals'].items():
        if data.string_at(int(address))!=value:raise ValueError('LiveFather hit literal changed')
    if data.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('LiveFather empty movie key changed')
    window=read_call_window(data,w['mainAddress'],w['mainSize'],w['healthGetter'],argument_count=1)
    if window is None or window.ecx!=('stack',16) or window.stack_arguments[0]!=('stack',128):raise ValueError('LiveFather health output/control changed')
    return SOURCE,dict(w,status='disabled structured hit phase; full Main remains incomplete',
        limits=['Caller retains original control; false returns require caller-owned control cleanup.',
                'Normal/cancellation paths are compared with native bytes; Lua callback-error unwind is an adapter policy.',
                'NewThingFromResource requires reviewed pre-ID getter rollback; movie/resource methods remain staged capabilities.',
                'Other LiveFather phases, full Main composition, Init/persistence and engine scheduler integration remain open.'])

def generate():
    source,report=recover();out=ROOT/'work/live_father_converter';out.mkdir(parents=True,exist_ok=True)
    (out/'HIT_PHASE.lua').write_text('-- Disabled LiveFather phase library; no quest registration.\n'+source)
    (out/'HIT_EVIDENCE.json').write_text(json.dumps(report,indent=2)+'\n');(out/'quests.lua').write_text('Quests = {}\n');return source,report

if __name__=='__main__':print(generate()[1]['mainSha256'])
