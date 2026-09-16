"""Complete disabled marker-thread helper from original retail instructions."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function ManageQuestCoreMarkers(quest)
    quest:WithRetailResources(function(resources)
        local father = resources:NewThingFromScriptName("NOVI_LiveFather")
        local trader = resources:NewThingFromScriptName("NOVI_BookTrader")
        local theresa = resources:NewThingFromScriptName("NOVI_Theresa")
        local function wait_until(predicate)
            while not predicate() do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return false end
            end
            return not quest:IsActiveThreadTerminating()
        end
        local function run()
            resources:AddCoreQuestMarker(father)
            if not wait_until(function() return quest:GetHeroGold() >= 3 end) then return end
            if not wait_until(function() return quest:IsHeroControlledByPlayer() end) then return end
            resources:RemoveCoreQuestMarker(father)
            resources:AddCoreQuestMarker(trader)
            if quest:DisplayTutorial(19) then
                if quest:IsActiveThreadTerminating() then return end
                if not wait_until(function() return quest:MsgIsTutorialClickedPast() end) then return end
            end
            if not wait_until(function() return quest:GetStateBool("GivenSweets") end) then return end
            resources:RemoveCoreQuestMarker(trader)
            resources:AddCoreQuestMarker(theresa)
            if not wait_until(function() return quest:GetStateBool("GivenTheresaChocs") end) then return end
            resources:RemoveCoreQuestMarker(theresa)
            resources:AddCoreQuestMarker(father)
        end
        run()
        resources:DestroyThing(theresa)
        resources:DestroyThing(trader)
        resources:DestroyThing(father)
    end)
end
'''

def prove(data=None):
    d=data or RData();w=json.loads(Path(__file__).with_name('manage_quest_core_markers_witness.json').read_text())
    for row in w['regions']:
        if hashlib.sha256(d.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('ManageQuestCoreMarkers native bytes changed')
    unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text())
    fn=next(f for f in unit['functions'] if f['address']=='0x00DBE4E0')
    if hashlib.sha256(fn['decompile'].encode()).hexdigest()!=w['decompileSha256']:raise ValueError('ManageQuestCoreMarkers source correspondence changed')
    for address,key in w['literals'].items():
        if d.string_at(int(address,16))!=key:raise ValueError('ManageQuestCoreMarkers literal changed')
    for slot,target in w['slots'].items():
        if int.from_bytes(d.bytes_at(0x1260f0c+int(slot,16),4),'little')!=target:raise ValueError('ManageQuestCoreMarkers API slot changed')
    return w

def generate():
    w=prove();out=ROOT/'work/manage_quest_core_markers';out.mkdir(parents=True,exist_ok=True)
    (out/'CANDIDATE.lua').write_text(SOURCE);(out/'EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');(out/'quests.lua').write_text('Quests = {} -- Disabled helper.\n');return SOURCE,w

if __name__=='__main__':generate()
