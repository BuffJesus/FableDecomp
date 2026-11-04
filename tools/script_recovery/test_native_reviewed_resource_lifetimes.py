import copy,json
from pathlib import Path
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_resource_lifetimes import restore
ROOT=Path(__file__).resolve().parents[2]

@pytest.fixture(scope='module')
def evidence():
    c=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    fn=c.native('0x00e5b3b0')
    row=json.loads((ROOT/'refs/script_recovery/beggar_and_child/units/V_BeggarAndChild.json').read_text())['reviewedResourceLifetimes']['0x00e5b3b0']
    source=row['constructorHandle']+' = resources:NewResource()\n'
    for method,handles in row['sourceHandles'].items():
        for handle in handles:source+=f'resources:{method}({handle})\n'
    source+='r1 = quest:GetThingWithScriptName("separate actor")\n'
    return source,fn,row,c.rdata

def test_reviewed_handle_survives_other_object_assignment(evidence):
    source,fn,row,data=evidence;out=restore(source,fn,row,data)
    assert 'local entityControl = xStack_164' in out
    assert out.count('resources:ScriptThing(entityControl)')==9
    assert out.count('resources:TryAcquire(entityControl)')==2
    assert 'resources:ReleaseResource(entityControl)' in out
    assert 'r1 = quest:GetThingWithScriptName("separate actor")' in out
    assert restore(source,fn,None,data)==source

@pytest.mark.parametrize('kind',['body','source','slot','delta','anchor','target','alias','getter_inventory','source_inventory','collision'])
def test_changed_or_incomplete_evidence_rejected(evidence,kind):
    source,fn,row,data=evidence;row=copy.deepcopy(row)
    if kind=='body':row['nativeSha256']='0'*64
    if kind=='source':row['sourceSha256']='0'*64
    if kind=='slot':row['entrySlot']-=4
    if kind=='delta':row['receivers']['ScriptThing'][0]['entryEspDelta']+=4
    if kind=='anchor':row['nativeInstructions']['0x00E5B407']='00'
    if kind=='target':row['receivers']['ReleaseResource'][0]['call']='0x00E5CE3E'
    if kind=='alias':row['alias']='r1'
    if kind=='getter_inventory':row['receivers']['ScriptThing'].pop()
    if kind=='source_inventory':source=source.replace('resources:ScriptThing(xStack_150)','resources:ScriptThing(wrong)',1)
    if kind=='collision':source+='entityControl = 3\n'
    with pytest.raises(ValueError,match='reviewed resource lifetime'):restore(source,fn,row,data)


@pytest.fixture(scope='module')
def guarded_evidence():
    c=UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    fn=c.native('0x00e58d40')
    row=json.loads((ROOT/'refs/script_recovery/beggar_and_child/units/V_BeggarAndChild.json').read_text())['reviewedResourceLifetimes']['0x00e58d40']
    source=row['constructorHandle']+' = resources:NewResource()\n'
    source+='resources:PrepareResource('+row['sourceHandles']['PrepareResource'][0]+')\n'
    for method in ('TryAcquire','ScriptThing'):
        for handle in row['sourceHandles'][method]:source+=f'resources:{method}({handle})\n'
    handle=row['prepareGuards'][0]['handle']
    source+=f'if not resources:ScriptThing({handle}):IsNull() then\nresources:PrepareResource({handle})\nend\nresources:ReleaseResource({handle})\n'
    return source,fn,row,c.rdata


def test_native_prepare_guard_keeps_its_resource(guarded_evidence):
    source,fn,row,data=guarded_evidence;out=restore(source,fn,row,data)
    assert out.count('resources:ScriptThing(entityControl)')==14
    assert 'not resources:ScriptThing(entityControl):IsNull()' in out
    assert out.count('resources:PrepareResource(entityControl)')==2
    assert 'resources:ReleaseResource(entityControl)' in out


@pytest.mark.parametrize('kind',['missing','wrong_index','wrong_handle','intervening_call','guard_shape'])
def test_ambiguous_prepare_guard_is_rejected(guarded_evidence,kind):
    source,fn,row,data=guarded_evidence;row=copy.deepcopy(row)
    if kind=='missing':row.pop('prepareGuards')
    if kind=='wrong_index':row['prepareGuards'][0]['prepareIndex']=0
    if kind=='wrong_handle':row['prepareGuards'][0]['handle']='wrong'
    if kind=='intervening_call':source=source.replace(':IsNull() then',':IsNull() then\nresources:Unknown()')
    if kind=='guard_shape':source=source.replace('not resources:ScriptThing(', 'resources:ScriptThing(', 1)
    with pytest.raises(ValueError,match='reviewed resource lifetime'):restore(source,fn,row,data)
