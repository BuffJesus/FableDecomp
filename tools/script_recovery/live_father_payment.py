"""Movie-owned deed-payment dialogue, with original signed32 state ordering."""
from tools.script_recovery.live_father_hit import recover as hit_proof
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_call_setup_ir import read_call_window
import json
from tools.script_recovery.signed_int32_lua import SOURCE as SIGNED_INT32

SOURCE=SIGNED_INT32+'''function LiveFatherPaymentSpeak(quest, resources, control, key)
    local actor = resources:NewThingFromResource(control)
    local ok, positive = xpcall(function()
        return resources:ThingHealth(actor) > 0.0
    end, function(err) return err end)
    local closed, err = pcall(function() resources:DestroyThing(actor) end)
    if not ok then error(positive, 0) end
    if not closed then error(err, 0) end
    if positive then
        resources:Speak(control, quest:GetHero(), key, 0, false, true, false)
        while resources:IsPerformingScriptTask(control) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return false end
        end
        if quest:IsActiveThreadTerminating() then return false end
    end
    return true
end

function LiveFatherPaymentDialogue(quest, me, resources, control, state)
    local goodDeeds = wrapSignedInt32(quest:GetStateInt("GoodDeedsPerformed"))
    if goodDeeds == 0 and quest:GetStateInt("BadDeedsPerformed") == 0 then
        if quest:IsActiveThreadTerminating() then return false end
        return LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_DONE_NOTHING_YET")
    end
    if goodDeeds > wrapSignedInt32(state:GetStateInt("PenniesGiven")) then
        if quest:IsActiveThreadTerminating() then return false end
        local paid = wrapSignedInt32(state:GetStateInt("PenniesGiven"))
        local total = wrapSignedInt32(quest:GetStateInt("GoodDeedsPerformed"))
        local amount = wrapSignedInt32(total - paid)
        state:SetStateInt("PenniesGiven", wrapSignedInt32(paid + amount))
        resources:GiveRawHeroGold(amount)
        local badDeeds = quest:GetStateInt("BadDeedsPerformed")
        if quest:IsActiveThreadTerminating() then return false end
        local key = badDeeds == 0 and "TEXT_QST_048_DAD_GIVE_REWARD_JUST_GOOD" or "TEXT_QST_048_DAD_GIVE_REWARD_PART_BAD"
        if not LiveFatherPaymentSpeak(quest, resources, control, key) then return false end
        if resources:LiveFatherHeroHasChocolate() then
            if quest:IsActiveThreadTerminating() then return false end
            if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_GIVE_PRESENT") then return false end
            resources:ClearRawInformation(me)
        else
            local gold = wrapSignedInt32(quest:GetHeroGold())
            if quest:IsActiveThreadTerminating() then return false end
            if gold > 3 then
                if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_YOU_HAVE_ENOUGH") then return false end
                resources:ClearRawInformation(me)
            else
                if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_IS_ENOUGH") then return false end
                resources:SetActiveQuestObjective("TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01")
            end
        end
    else
        local badDeeds = wrapSignedInt32(quest:GetStateInt("BadDeedsPerformed"))
        if quest:IsActiveThreadTerminating() then return false end
        if badDeeds > 0 then
            return LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_ANTISOCIAL")
        end
        if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_DO_MORE") then return false end
        if resources:LiveFatherHeroHasChocolate() then
            if quest:IsActiveThreadTerminating() then return false end
            resources:ClearRawInformation(me)
            if not LiveFatherPaymentSpeak(quest, resources, control, "TEXT_QST_048_DAD_GIVE_PRESENT_ALT") then return false end
        end
    end
    return true
end
'''

SITES=[(0xdb8c89,60),(0xdb8d97,160),(0xdb8e56,184),(0xdb8f47,208),(0xdb9023,232),(0xdb90e9,196),(0xdb923f,172),(0xdb92ef,220),(0xdb93ef,148)]
LITERALS={0x12d96f0:'TEXT_QST_048_DAD_DONE_NOTHING_YET',0x12d96c8:'TEXT_QST_048_DAD_GIVE_REWARD_JUST_GOOD',0x12d96a0:'TEXT_QST_048_DAD_GIVE_REWARD_PART_BAD',0x12d9680:'TEXT_QST_048_DAD_GIVE_PRESENT',0x12d965c:'TEXT_QST_048_DAD_YOU_HAVE_ENOUGH',0x12d9640:'TEXT_QST_048_DAD_IS_ENOUGH',0x12d9624:'TEXT_QST_048_DAD_ANTISOCIAL',0x12d9608:'TEXT_QST_048_DAD_DO_MORE',0x12d95e4:'TEXT_QST_048_DAD_GIVE_PRESENT_ALT',0x12d8f24:'OBJECT_CHOCOLATE_BOX_UNGIVEABLE',0x12d8244:'TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_01'}
API_SLOTS={0x1f8:0x898f40,0x1fc:0x898fa0,0x2e0:0x897220,0xa3c:0x891880,0x4a0:0x896a30}
def recover(data=None):
    data=data or RData();w=hit_proof(data)[1]
    for site,output in SITES:
        window=read_call_window(data,w['mainAddress'],w['mainSize'],site,argument_count=1)
        if window is None or window.ecx!=('stack',16) or window.stack_arguments[0]!=('stack',output):raise ValueError('LiveFather payment resource/output changed')
    for address,value in LITERALS.items():
        if data.string_at(address)!=value:raise ValueError('LiveFather payment text changed')
    for slot,target in API_SLOTS.items():
        if int.from_bytes(data.bytes_at(0x1260f0c+slot,4),'little')!=target:raise ValueError('LiveFather payment API target changed')
    return SOURCE,{'mainSha256':w['mainSha256'],'start':0xdb8c53,'end':0xdb9483,'cancelStops':[0xdb8d23,0xdb974b],
        'healthSites':SITES,'apiSlots':API_SLOTS,'fields':{'parentGood':0x54,'parentBad':0x58,'entityPenniesGiven':0x1c},
        'gold':'signed32 comparison and subtraction; PenniesGiven store precedes GiveHeroGold; post-payment gold is queried independently',
        'objective':'Construct empty region2, empty region1, objective; get owned active-quest CString; SetQuestCardObjective; destroy result/objective/region1/region2.',
        'limits':['Caller owns the movie and control throughout this interior; cancellation returns before caller movie cleanup.',
                  'Raw gold, scoped possession and owned active-quest objective adapters are staged/compiled separately but remain unapplied; merged owner validation is pending.',
                  'Gold API internals and native quest state persistence are not implemented by this phase.']}
def generate():
    source,report=recover();out=ROOT/'work/live_father_converter';out.mkdir(parents=True,exist_ok=True)
    (out/'PAYMENT_PHASE.lua').write_text('-- Disabled movie-owned LiveFather payment phase.\n'+source);(out/'PAYMENT_EVIDENCE.json').write_text(json.dumps(report,indent=2)+'\n');return source,report

if __name__=='__main__':print(generate()[1]['mainSha256'])
