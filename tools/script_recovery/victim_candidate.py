"""Reproducible disabled Victim Init/Main composition with explicit owners."""
import json
from tools.script_recovery.lift_native_lua import ROOT
from tools.script_recovery.victim_init import generate as init
from tools.script_recovery.victim_subdued import generate as subdued
from tools.script_recovery.victim_talk import generate as talk
from tools.script_recovery.victim_hit import generate as hit
from tools.script_recovery.victim_repeat import generate as repeat_hit
from tools.script_recovery.victim_complaint import generate as complaint

BODY='''function VictimMain(quest, me, state, addBadDeed)
    quest:RegisterBoundConsciousCondition(me)
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        if not VictimAcquire(quest, resources, control, function() return me end) then return end
        local bully = resources:NewThingFromScriptName("NOVI_Bully")
        if quest:IsActiveThreadTerminating() then return end
        repeat
            if not VictimUpdateSubdued(quest, me, resources, control, bully, state) then return end
            if not VictimTalk(quest, me, resources, control, bully, state) then return end
            local hit = VictimBeginHit(quest, me, resources, control, bully, addBadDeed)
            if hit == "cancel" then return end
            if hit == "repeat" and not VictimRepeatHit(quest, me, resources, control) then return end
            if not VictimComplaint(quest, me, resources, control) then return end
            quest:NewScriptFrame()
        until quest:IsActiveThreadTerminating()
    end)
end
'''

def generate():
    initial,iw=init();opening,ow=subdued();dialogue,tw=talk()
    hits,hw=hit();repeats,rw=repeat_hit();complaints,cw=complaint()
    source='-- Disabled Victim Init/Main library; entity state and quest helper are injected. No registration.\n'+initial+opening+dialogue+hits+repeats+complaints+BODY
    report={'status':'disabled; native phases composed; host integration pending','nativeMainSha256':ow['mainSha256'],'init':iw,'subdued':ow,'talk':tw,'hit':hw,'repeat':rw,'complaint':cw,
        'pending':['Outer comparison abstracts separately compared phase bodies; no single unabstracted whole-engine run.',
                   'Six actor capabilities are compiled in an unapplied proposal; merge resource/movie/conversation/info support and validate owner.',
                   'Entity/parent state persistence, condition and scheduler teardown integration.']}
    out=ROOT/'work/victim_converter';(out/'CANDIDATE.lua').write_text(source);(out/'CANDIDATE_REPORT.json').write_text(json.dumps(report,indent=2)+'\n');return source,report

if __name__=='__main__':generate()
