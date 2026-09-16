"""LiveFather Init, including native by-value pushability consumption."""
import hashlib,json,struct
from pathlib import Path
from tools.script_recovery.lift_native_lua import RData,ROOT

SOURCE='''function LiveFatherInit(quest, me, state)
    state:SetStateInt("PenniesGiven", 0)
    quest:WithRetailResources(function(resources)
        resources:InitializeLiveFatherActor(me)
    end)
end
'''

ADAPTER='''// Unapplied. Include inside LuaRetailResources; actual FSE by-value ABI.
void InitializeLiveFatherActor(CScriptThing* actor) {
    CheckOpen();
    if (!actor || !g_pCScriptThingVTable || !EntitySetAsDamageable_API || !EntitySetAsKillable_API ||
        !EntitySetAsToAddToComboMultiplierWhenHit_API || !SetThingHasInformation_API ||
        !SetIsPushableByHero_API || !EntitySetDeedReactionsEnabled_API)
        throw std::runtime_error("LiveFather Init APIs unavailable");
    EntitySetAsDamageable_API(m_game,actor,false);
    EntitySetAsKillable_API(m_game,actor,false,false);
    EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
    SetThingHasInformation_API(m_game,actor,false,true,false);
    CScriptThing argument=*actor;
    argument.pVTable=g_pCScriptThingVTable;
    if(argument.pImp.Info) ++argument.pImp.Info->RefCount;
    // Retail callee consumes this copy. Do not add a caller-side destructor.
    SetIsPushableByHero_API(m_game,argument,false);
    EntitySetDeedReactionsEnabled_API(m_game,actor,false);
}
'''

def prove(data=None):
    data=data or RData();w=json.loads(Path(__file__).with_name('live_father_init_witness.json').read_text())
    for row in w['functions']:
        if hashlib.sha256(data.bytes_at(row['address'],row['size'])).hexdigest()!=row['sha256']:raise ValueError('LiveFather Init/callee changed')
        if 'slot' in row and data.bytes_at(w['vtable']+row['slot'],4)!=struct.pack('<I',row['address']):raise ValueError('LiveFather pushability target changed')
    return w

def generate():
    w=prove();out=ROOT/'work/live_father_converter/runtime_proposal';out.mkdir(parents=True,exist_ok=True)
    (out/'live_father_init.inc').write_text(ADAPTER);return SOURCE,w
