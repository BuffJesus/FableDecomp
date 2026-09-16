"""Recover Victim's opening shared-state phase from original native bytes."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData
DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Victim.lua'
SOURCE='''function VictimUpdateSubdued(quest, me, resources, control, bully, state)
    if quest:GetStateBool("BullySubdued") then
        if quest:IsActiveThreadTerminating() then return false end
        if not state:GetStateBool("DoneThanks") then
            if quest:IsActiveThreadTerminating() then return false end
            state:SetStateBool("DoneThanks", true)
            resources:PrepareResource(control)
            resources:SetRawScared(me, false)
            while not quest:GetStateBool("BullyRanOff") do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            if quest:IsActiveThreadTerminating() then return false end
            resources:SetVictimReleasedState(me)
        end
    elseif quest:GetStateBool("VictimShake") then
        if quest:IsActiveThreadTerminating() then return false end
        quest:SetStateBool("VictimShake", false)
        resources:FaceTowardsRetainedThing(me, bully, false)
    end
    return true
end
'''

def recover(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('victim_subdued_witness.json').read_text())
    for address,size,sha in ((w['mainAddress'],w['mainSize'],w['mainSha256']),(w['calleeAddress'],w['calleeSize'],w['calleeSha256'])):
        if hashlib.sha256(data.bytes_at(address,size)).hexdigest()!=sha:raise ValueError('Victim phase/callee bytes changed')
    if hashlib.sha256(DRAFT.read_text().encode()).hexdigest()!=w['draftSha256']:raise ValueError('Victim draft source correspondence changed')
    for slot,target in w['slots'].items():
        if int.from_bytes(data.bytes_at(w['vtable']+int(slot,16),4),'little')!=int(target,16):raise ValueError('Victim API target changed')
    if data.bytes_at(0x122d70e,1)!=b'\0':raise ValueError('Victim empty movie key changed')
    return SOURCE,w

def generate():
    source,w=recover();out=ROOT/'work/victim_converter';out.mkdir(parents=True,exist_ok=True)
    (out/'SUBDUED_PHASE.lua').write_text('-- Disabled Victim phase; caller retains control and Bully Thing.\n'+source)
    (out/'SUBDUED_EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');(out/'quests.lua').write_text('Quests = {} -- Disabled isolated recovery.\n');return source,w

if __name__=='__main__':generate()
