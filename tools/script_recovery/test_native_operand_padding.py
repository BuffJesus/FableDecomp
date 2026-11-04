"""Register-preserving LEA padding must not consume operand tracing depth."""
from pathlib import Path
from types import SimpleNamespace
import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter


def recover(padding, setup=bytes.fromhex('8be9 8d5d08')):
    # this -> ebp; me -> ebx; padding; push me; call [eax+0x1234].
    code=setup+padding+b'\x53'+bytes.fromhex('ff9034120000')
    entry=0x400000;site=entry+len(code)-6
    converter=UnitConverter.__new__(UnitConverter)
    converter.rdata=SimpleNamespace(bytes_at=lambda a,n:code[a-entry:a-entry+n],string_at=lambda a:None)
    converter.slots={0x1234:'ActorCall'};converter.thing_slots={}
    converter.manifest={'ActorCall':dict(returnType='void',parameters=[dict(name='actor',type='CScriptThing*')])}
    fn=dict(address=hex(entry),bodyEndExclusive=hex(entry+len(code)),calls=[],callOrder=[hex(site)],
            indirectCalls=[dict(site=hex(site),kind='vtable',slot='0x1234',ecxValue=4,depth=4)])
    return converter.recover_dropped_operands('(**(code **)(vtable + 0x1234))();',fn)


@pytest.mark.parametrize('count',[0,1,4,8,32])
@pytest.mark.parametrize('padding',[bytes.fromhex('8d1b'),bytes.fromhex('8d5b00'),bytes.fromhex('8d9b00000000')])
def test_identity_lea_retains_actor_at_any_padding_length(count,padding):
    assert recover(padding*count)=='(**(code **)(vtable + 0x1234))((this + 8));'


def test_offset_lea_is_not_discarded():
    assert recover(bytes.fromhex('8d5b04'))=='(**(code **)(vtable + 0x1234))(((this + 8) + 4));'


def test_real_alias_depth_limit_is_preserved():
    assert '__unknown_push' in recover(bytes.fromhex('8d5b04')*8)


def test_indexed_lea_is_not_mistaken_for_padding():
    assert '__unknown_push' in recover(bytes.fromhex('8d1c1b'))


def test_padding_does_not_invent_an_uninitialized_actor():
    assert '__unknown_push' in recover(bytes.fromhex('8d1b')*32,setup=b'')


def test_padding_does_not_restore_a_clobbered_register():
    assert recover(bytes.fromhex('8d1b')*8+bytes.fromhex('bb00100000')+bytes.fromhex('8d1b')*8).endswith('(0x1000);')


def test_retail_magicman_recovers_all_six_camera_actor_operands():
    root=Path(__file__).resolve().parents[2]
    converter=UnitConverter(root/'refs/script_recovery/bordello/translation_unit_typed.json')
    fn=converter.native('0x00e40e80')
    source=converter.recover_dropped_operands(fn['decompile'],fn)
    calls=[line for line in source.splitlines() if '+ 0x678)' in line]
    assert len(calls)==6
    assert all(',(this + 8),-1.0,0,-1);' in line for line in calls)
