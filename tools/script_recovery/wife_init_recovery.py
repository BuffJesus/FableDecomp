"""Exact Wife Init pointer calls, copied Thing and intervening state store."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function WifeRecoveredInit(quest, me, state)
    state:SetStateBool("GoingForHusband", false)
    state:SetStateBool("ForceFirstTimeSpeak", true)
    quest:WithRetailResources(function(resources)
        resources:InitializeWifeActor(me)
        state:SetStateBool("SaidRunningLine", false)
        resources:SetWifeDeedReactionsDisabled(me)
    end)
end
'''

def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('wife_init_recovery_witness.json').read_text())
    for row in w['functions']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('Wife Init/copy-consumer bytes changed')
    for slot,target in w['slots'].items():
        if int.from_bytes(data.bytes_at(0x1260f0c+int(slot,16),4),'little')!=target:raise ValueError('Wife Init API slot changed')
    return w

def generate():
    w=prove();out=ROOT/'work/wife_complete_converter';out.mkdir(parents=True,exist_ok=True)
    (out/'INIT.lua').write_text(SOURCE);(out/'INIT_EVIDENCE.json').write_text(json.dumps(w,indent=2)+'\n');return SOURCE,w

if __name__=='__main__':generate()
