"""Full Main decision paths using bounded native expectations and explicit API doubles."""
from pathlib import Path
import json,struct
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_reviewed_register_resource_health_lifetimes import recover
from tools.script_recovery.test_native_reviewed_interface_table_staging import FixtureImage
from tools.script_recovery.lua_local_versions import split_hoisted_locals
from tools.script_recovery.build_readable_unit import readable_file
F=Path(__file__).resolve().parent/'fixtures/register_resource_health'
read=lambda name:json.loads((F/name).read_text())
TRIALS=read('native_predicate_trials.json')
DOMAIN={row['bits']:row['positive'] for row in TRIALS['rows']}
@pytest.fixture(scope='module')
def stages():
    phase=read('phase.json');row=read('row.json');baseline=(F/'phase.lua').read_text()
    data=dict(source=baseline,todo=phase['todo'],calls=phase['calls'],fn=phase['fn'],row=row,unit=read('unit.json'),owner=row['owner'],image=FixtureImage([(int(p['address'],16),bytes.fromhex(p['bytes'])) for p in read('image_parts.json')]),manifest={'GetHealth':row['hostContract']},before=read('before.json'),after=read('after.json'))
    source=recover(**data)[0]
    assert TRIALS['nativeSha256']==row['nativeSha256']
    return dict(baseline=baseline,draft=source,split=split_hoisted_locals(source)[0],unstyled=readable_file(source,style=False,rel='FSE/V_SickChild/Entities/SickChild.lua')[0],readable=readable_file(source,style=True,rel='FSE/V_SickChild/Entities/SickChild.lua')[0])
def execute(text,target,bits,baseline=False):
    token='quest:GetHealth(' if baseline else 'resources:ResourceThingHealthAbove('
    lines=[i+1 for i,line in enumerate(text.splitlines()) if token in line];assert len(lines)==3
    health=struct.unpack('<f',struct.pack('<I',int(bits,16)))[0]
    vm=LuaRuntime(unpack_returned_tuples=True);vm.globals().NATIVE_DECISION=lambda:health>0
    return vm.execute((F/'harness.lua').read_text(),text,target,lines[target],health)[0]
@pytest.mark.parametrize('stage',['draft','split','unstyled','readable'])
@pytest.mark.parametrize('target',[0,1,2])
@pytest.mark.parametrize('bits',list(DOMAIN))
def test_full_main_native_ordered_decision(stages,stage,target,bits):
    assert execute(stages[stage],target,bits)==DOMAIN[bits]
@pytest.mark.parametrize('bits',[bits for bits in DOMAIN if (int(bits,16)&0x7f800000)==0x7f800000 and (int(bits,16)&0x7fffff)])
def test_baseline_inverse_nan_control_exposes_old_fault(stages,bits):
    assert DOMAIN[bits] is False
    assert execute(stages['baseline'],1,bits,baseline=True) is True
