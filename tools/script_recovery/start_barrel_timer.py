"""Structured disabled StartBarrelTimer from the complete original function."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function StartBarrelTimer(quest)
    quest:WithRetailResources(function(resources)
        while resources:ReadBarrelWatchTimer(quest:GetStateInt("WatchTimer")) <= 0 do
            quest:NewScriptFrame()
            if quest:IsActiveThreadTerminating() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        resources:AddBarrelTimerBar(function(id) quest:SetStateInt("GUIBarrelCounter", id) end)
        local guard = resources:NewThingFromScriptName("M_WHouse_GuardPoint")
        local function run()
            while not quest:GetStateBool("BarrelManSpokenToHeroOnReturn") do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then return end
                local near = resources:IsHeroNearBarrelGuard(guard)
                resources:ColourBarrelTimer(quest:GetStateInt("GUIBarrelCounter"), near)
                resources:UpdateBarrelTimer(function() return quest:GetStateInt("WatchTimer") end,
                    function() return quest:GetStateInt("GUIBarrelCounter") end)
            end
            if not quest:IsActiveThreadTerminating() then
                resources:RemoveBarrelTimer(quest:GetStateInt("GUIBarrelCounter"))
            end
        end
        run()
        resources:DestroyThing(guard)
    end)
end
'''
def prove(data=None):
    d=data or RData();w=json.loads(Path(__file__).with_name('start_barrel_timer_witness.json').read_text())
    for r in w['regions']:
        if hashlib.sha256(d.bytes_at(r['address'],r['size'])).hexdigest()!=r['sha256']:raise ValueError('StartBarrelTimer native bytes changed')
    for address,key in w['strings'].items():
        if (d.bytes_at(int(address,16),1)!=b'\0' if key=='' else d.string_at(int(address,16))!=key):raise ValueError('StartBarrelTimer string changed')
    for slot,target in w['slots'].items():
        if int.from_bytes(d.bytes_at(0x1260f0c+int(slot,16),4),'little')!=target:raise ValueError('StartBarrelTimer slot changed')
    unit=json.loads((ROOT/'refs/script_recovery/new_oakvale_intro/translation_unit.json').read_text());f=next(f for f in unit['functions'] if f['address']=='0x00DB4F70')
    if hashlib.sha256(f['decompile'].encode()).hexdigest()!=w['sourceSha256']:raise ValueError('StartBarrelTimer source changed')
    return w

def generate():
    w=prove();out=ROOT/'work/start_barrel_timer';out.mkdir(parents=True,exist_ok=True)
    (out/'CANDIDATE.lua').write_text(SOURCE);(out/'EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');(out/'quests.lua').write_text('Quests = {} -- Disabled helper.\n');return SOURCE,w
if __name__=='__main__':generate()
