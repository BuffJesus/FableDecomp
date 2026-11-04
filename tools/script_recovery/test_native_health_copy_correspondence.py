from pathlib import Path
import ast,copy,json,sys
import pytest
W=Path(__file__).resolve().parent;R=W.parents[1];sys.path.insert(0,str(R))
from tools.script_recovery.native_health_copy_correspondence import prove
from tools.script_recovery.lift_native_lua import RData
P=W/'fixtures/register_resource_health'
Q=P
FN=json.loads((P/'phase.json').read_text())['fn']
BEFORE=json.loads((Q/'before.json').read_text())
AFTER=json.loads((Q/'after.json').read_text())
PHASE=(P/'phase.lua').read_text()
@pytest.fixture(autouse=True)
def no_retail_image(monkeypatch):
    monkeypatch.setattr(RData,'__init__',lambda *a,**k:(_ for _ in ()).throw(AssertionError('retail image opened')))
def test_actual_copy_replay():
    result=prove(FN,BEFORE,AFTER,PHASE)
    assert result['nativeSites']==2 and result['emittedUses']==3
    assert [s['native']['call']['site'].lower() for s in result['scopes']]==['0x00ec6627','0x00ec63fe','0x00ec6627']
    assert [len(s['distinctThingAliases']) for s in result['scopes']]==[2,1,2]
    assert result['scopes'][2]['copiedContinuation']=='LAB_00ec64df'
@pytest.mark.parametrize('fault',['before-drop','before-actor','before-extra','after-drop','after-actor','after-tail','lua-drop','lua-actor','lua-resource-alias','lua-result','lua-compare','native-slot','getter-identity','cleanup-identity','call-order'])
def test_changed_correspondence_rejected(fault):
    fn=copy.deepcopy(FN);before=list(BEFORE);after=list(AFTER);phase=PHASE;kw={}
    if fault.startswith('before-'):
        index=next(i for i,s in enumerate(before) if 'GSI->GetHealth' in s)
        if fault=='before-drop':before.pop(index)
        elif fault=='before-actor':before[index]=before[index].replace('pCVar7','otherActor')
        else:before.insert(index,before[index])
    elif fault.startswith('after-'):
        index=next(i for i,s in enumerate(after) if 'GSI->GetHealth' in s)
        if fault=='after-drop':after.pop(index)
        elif fault=='after-actor':after[index]=after[index].replace('pCVar7','otherActor')
        else:after.append('quest:Observe(unproved);')
    elif fault=='lua-drop':phase=phase.replace('quest:GetHealth(', 'quest:OtherHealth(',1)
    elif fault=='lua-actor':phase=phase.replace('quest:GetHealth(pCVar7)', 'quest:GetHealth(otherActor)',1)
    elif fault=='lua-resource-alias':phase=phase.replace('resources:ScriptThing(xStack_78)', 'resources:ScriptThing(pCVar7)',1)
    elif fault=='lua-result':phase=phase.replace('fret_01 = quest:GetHealth', 'fret_unknown = quest:GetHealth',1)
    elif fault=='lua-compare':phase=phase.replace('fVar12 < fret_01','fVar12 > fret_01',1)
    elif fault=='native-slot':kw['health_slot']=0x424
    elif fault=='getter-identity':kw['getter_target']=0x7e7491
    elif fault=='cleanup-identity':kw['cleanup_target']=0x4aa841
    else:
        health=next(c for c in fn['indirectCalls'] if c.get('slot')=='0x420')
        health['site']='0x00ec6628'
    with pytest.raises(ValueError):prove(fn,before,after,phase,**kw)
