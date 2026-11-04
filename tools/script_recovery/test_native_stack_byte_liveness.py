from pathlib import Path
import sys
import pytest
W=Path(__file__).resolve().parent;sys.path.insert(0,str(W))
from tools.script_recovery.native_stack_byte_liveness import prove
from capstone.x86 import X86_REG_EBX
from tools.script_recovery.test_native_resource_cfg import Code

STORE='88442418'
KILL='c744241800000000'
RETURN='5d5b83c408c20800'
HELPERS={
    0x2000:dict(kind='constructor-overwrite',width=4),
    0x3000:dict(kind='nonstack-receiver',stackArguments=0,callerStackAccess='none'),
    0x4000:dict(kind='receiver-object',receiverBytes=4,stackArguments=0,callerStackAccess='none'),
}
def call(c,target):
    site=0x1000+len(c.data)
    c.emit('e8'+(target-site-5).to_bytes(4,'little',signed=True).hex())
def check(c,helpers=HELPERS):
    return prove(c.bytes(),0x1000,0x1000,helpers,nonstack_registers=[X86_REG_EBX])
def code(tail):
    c=Code();c.emit(STORE+' '+tail);return c

@pytest.mark.parametrize('tail',[KILL,RETURN,'90 '+KILL,'8d4c2414 '+KILL])
def test_full_overwrite_or_discard_proves(tail):
    assert check(code(tail))['visitedStates']>0

@pytest.mark.parametrize('tail',[
    '8b442418 '+KILL,
    'c644241800 '+KILL,
    '837c241800 '+KILL,
    '8d4c2418 8b01 '+KILL,
    '8d4c2418 894c2410 '+KILL,
    '8d442418 50 '+KILL,
    '8be0 '+KILL,
    '5d5b83c408c3',
    'ffe0',
    '8b07 '+KILL,
    '66b90000 '+KILL,
    '8d442418 '+RETURN,
])
def test_read_escape_partial_store_unknown_origin_or_live_return_rejects(tail):
    with pytest.raises(ValueError):check(code(tail))

def test_both_branches_are_checked():
    c=code('85c0');c.jump('74','bad');c.emit(KILL);c.jump('eb','exit')
    c.label('bad');c.emit('8b442418');c.label('exit');c.emit(RETURN)
    with pytest.raises(ValueError,match='read before'):check(c)

def test_native_constructor_boundary_proves():
    c=code('8d4c2418');call(c,0x2000);c.emit('c3')
    assert check(c)['overwriteBoundaries'][0]['kind']=='constructor-overwrite'

def test_constructor_targets_wrong_slot_rejects():
    c=code('8d4c2414');call(c,0x2000);c.emit('c3')
    with pytest.raises(ValueError,match='different slot'):check(c)

def test_unknown_call_rejects():
    c=code('8bcb');call(c,0x5000);c.emit(KILL)
    with pytest.raises(ValueError,match='unknown helper'):check(c)

def test_unproven_receiver_rejects():
    c=code('8bcd');call(c,0x3000);c.emit(KILL)
    with pytest.raises(ValueError,match='unknown/stack pointer'):check(c)

def test_immediate_does_not_certify_nonstack_receiver():
    c=code('b918000002');call(c,0x3000);c.emit(KILL)
    with pytest.raises(ValueError,match='unknown/stack pointer'):check(c)

def test_callee_saved_alias_survives_calls_and_remains_observable():
    c=code('8d7c2418 8bcb');call(c,0x3000);c.emit('8b07 '+KILL)
    with pytest.raises(ValueError,match='read before'):check(c)

def test_disjoint_receiver_contract_then_discard_proves():
    c=code('8d4c2414');call(c,0x4000);c.emit(RETURN)
    assert check(c)['returnBoundaries']

@pytest.mark.parametrize('offset',[0x15,0x16,0x17,0x18,0x19,0x1a,0x1b])
def test_overlapping_receiver_contract_rejects(offset):
    c=code('8d4c24'+bytes([offset]).hex());call(c,0x4000);c.emit(RETURN)
    with pytest.raises(ValueError,match='aliases spill'):check(c)

def test_caller_stack_access_contract_cannot_be_guessed():
    c=code('8bcb');call(c,0x3000);c.emit(KILL)
    helpers={**HELPERS,0x3000:{**HELPERS[0x3000],'callerStackAccess':'unknown'}}
    with pytest.raises(ValueError,match='caller stack'):check(c,helpers)

def test_stable_loop_reaches_fixed_point():
    c=code('');c.label('again');c.emit('85c0');c.jump('75','again');c.emit(KILL)
    assert check(c)['visitedStates']==3

def test_unbounded_stack_loop_rejects():
    c=code('');c.label('again');c.emit('6a00');c.jump('eb','again')
    with pytest.raises(ValueError,match='unbounded state'):check(c)
