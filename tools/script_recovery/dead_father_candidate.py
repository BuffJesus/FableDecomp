"""Complete disabled DeadFather lifecycle from original retail bytes."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData
from tools.script_recovery.native_new_oakvale_conditions import verify as verify_condition
DRAFT=ROOT/'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/OVI_DeadFather.lua'
SOURCE='''function DeadFatherInit(quest, me)
    quest:WithRetailResources(function(resources)
        resources:InitializeDeadFatherActor(me)
    end)
end

function DeadFatherMain(quest, me)
    quest:RegisterBoundAliveCondition(me)
    quest:NewScriptFrame()
    if quest:IsActiveThreadTerminating() then return end
    quest:WithRetailResources(function(resources)
        local control = resources:NewResource()
        resources:PrepareResource(control)
        while not resources:TryAcquire(control, me, 4) do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:PlaceDeadFatherAtMarker(me)
        resources:PlayDeadFatherPose(control)
        while not quest:GetStateBool("DadFound") do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:RemoveDeadFatherMarker(me)
        repeat quest:NewScriptFrame() until quest:IsActiveThreadTerminating()
    end)
end

function DeadFatherOnPredicateFail(quest, me)
end
'''
def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('dead_father_witness.json').read_text())
    for row in w['functions']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('DeadFather native bytes changed')
    if hashlib.sha256(DRAFT.read_text().encode()).hexdigest()!=w['draftSha256']:raise ValueError('DeadFather source correspondence changed')
    for slot,target in w['slots'].items():
        if int.from_bytes(data.bytes_at(w['vtable']+int(slot,16),4),'little')!=int(target,16):raise ValueError('DeadFather API target changed')
    for address,value in w['strings'].items():
        if data.string_at(int(address,16))!=value:raise ValueError('DeadFather literal changed')
    condition=verify_condition('OVI_DeadFather',data)
    if condition is None or condition['method']!='RegisterBoundAliveCondition':raise ValueError('DeadFather condition contract changed')
    w['conditionEvidence']=condition
    return w
def generate():
    w=prove();out=ROOT/'work/dead_father_converter';out.mkdir(parents=True,exist_ok=True)
    source='-- Disabled complete DeadFather lifecycle; no quest registration.\n'+SOURCE
    (out/'CANDIDATE.lua').write_text(source);(out/'EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');(out/'quests.lua').write_text('Quests = {} -- Disabled recovery.\n');return source,w
if __name__=='__main__':generate()
