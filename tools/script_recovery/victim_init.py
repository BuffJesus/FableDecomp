"""Victim Init roles and isolated pointer/by-value actor adapter proposal."""
import hashlib,json
from pathlib import Path
from tools.script_recovery.victim_subdued import recover
from tools.script_recovery.lift_native_lua import ROOT,RData
SOURCE='''function VictimInit(quest, me, state)
    state:SetStateBool("DoneThanks", false)
    state:SetStateBool("DisplayedGameInfo", false)
    quest:WithRetailResources(function(resources)
        resources:InitializeVictimActor(me)
    end)
end
'''
ADAPTER='''// Unapplied Victim methods inside LuaRetailResources. No IsNull filtering.
struct VictimNativeText {
    CCharString value{};
    explicit VictimNativeText(const char* key) { CCharString_Construct_Literal(&value,key,-1); }
    ~VictimNativeText() { CCharString_Destroy(&value); }
    VictimNativeText(const VictimNativeText&)=delete;
};
void AddVictimRepeatConversationLines(int conversation,CScriptThing* speaker,unsigned listener) {
    auto& target=Get(listener,Kind::Thing);
    if(!AddLineToConversation_API) throw std::runtime_error("Victim repeat conversation API unavailable");
    {
        VictimNativeText key("TEXT_QST_048_VICTIM_EVIL_BROS");
        AddLineToConversation_API(m_game,conversation,&key.value,false,speaker,&target.thing);
    }
    {
        VictimNativeText key("TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM");
        AddLineToConversation_API(m_game,conversation,&key.value,false,speaker,&target.thing);
    }
}
void SetRawScared(CScriptThing* actor,bool scared) {
    CheckOpen();
    if(!EntitySetAsScared_API) throw std::runtime_error("Victim scared API unavailable");
    EntitySetAsScared_API(m_game,actor,scared);
}
void FaceTowardsRetainedThing(CScriptThing* actor,unsigned target,bool snap) {
    auto& e=Get(target,Kind::Thing);
    if(!EntitySetFacingAngleTowardsThing_API) throw std::runtime_error("Victim facing API unavailable");
    EntitySetFacingAngleTowardsThing_API(m_game,actor,&e.thing,snap);
}
void VictimFaceHero(CScriptThing* actor,bool snap) {
    CheckOpen();
    if(!GetHero_API || !EntitySetFacingAngleTowardsThing_API) throw std::runtime_error("Victim Hero facing APIs unavailable");
    auto* hero=GetHero_API(m_game);
    EntitySetFacingAngleTowardsThing_API(m_game,actor,hero,snap);
}
void VictimSetPushable(CScriptThing* actor,bool value) {
    CScriptThing argument=*actor;
    argument.pVTable=g_pCScriptThingVTable;
    if(argument.pImp.Info) ++argument.pImp.Info->RefCount;
    SetIsPushableByHero_API(m_game,argument,value); // Callee consumes copied reference.
}
void SetVictimReleasedState(CScriptThing* actor) {
    CheckOpen();
    if(!actor || !g_pCScriptThingVTable || !SetIsPushableByHero_API || !EntitySetAsUseMovementInActions_API || !ClearThingHasInformation_API)
        throw std::runtime_error("Victim release-state APIs unavailable");
    VictimSetPushable(actor,true);
    EntitySetAsUseMovementInActions_API(m_game,actor,true);
    ClearThingHasInformation_API(m_game,actor);
}
void InitializeVictimActor(CScriptThing* actor) {
    CheckOpen();
    if(!actor || !g_pCScriptThingVTable || !EntitySetAsDamageable_API || !EntitySetAsKillable_API || !EntitySetAsToAddToComboMultiplierWhenHit_API ||
       !SetThingHasInformation_API || !SetIsPushableByHero_API || !EntitySetAsUseMovementInActions_API || !EntitySetAsScared_API)
        throw std::runtime_error("Victim Init APIs unavailable");
    EntitySetAsDamageable_API(m_game,actor,false);
    EntitySetAsKillable_API(m_game,actor,false,false);
    EntitySetAsToAddToComboMultiplierWhenHit_API(m_game,actor,false);
    SetThingHasInformation_API(m_game,actor,false,false,false);
    VictimSetPushable(actor,false);
    EntitySetAsUseMovementInActions_API(m_game,actor,false);
    EntitySetAsScared_API(m_game,actor,true);
}
'''
def prove(data=None):
    data=data or RData();recover(data);w=json.loads(Path(__file__).with_name('victim_init_witness.json').read_text())
    if hashlib.sha256(data.bytes_at(w['address'],w['size'])).hexdigest()!=w['sha256']:raise ValueError('Victim Init bytes changed')
    return w
def generate():
    w=prove();out=ROOT/'work/victim_converter/runtime_proposal';out.mkdir(parents=True,exist_ok=True);(out/'victim_methods.inc').write_text(ADAPTER);return SOURCE,w
