"""Native CTimer result stores, including clobbered and ambiguous evidence."""
from pathlib import Path
from types import SimpleNamespace
import json
import pytest
from tools.script_recovery.native_timer_operands import result_stack_slot,recover


def spill(tail,depth=36,call='ff915c010000'):
    start=0x400000;raw=bytes.fromhex('83ec20 53 '+call+' '+tail)
    fn={'address':hex(start),'size':len(raw)}
    site={'site':hex(start+4),'depth':depth}
    data=SimpleNamespace(bytes_at=lambda a,n:raw[a-start:a-start+n])
    return result_stack_slot(fn,site,data)

@pytest.mark.parametrize('tail', ['89442408','6a28 50 89442410','8b0df8e84301 89442408','83ec08 89442410'])
def test_stored_timer_id_accounts_for_pending_pushes(tail):
    assert spill(tail)==-28

@pytest.mark.parametrize('tail', ['31c0 89442408','66b80100 89442408','e800000000 89442408','eb00 89442408','c3','58 89442408','83c404 89442408','89442440'])
def test_unproven_result_store_stays_unknown(tail):
    assert spill(tail) is None


def test_wrong_depth_or_vtable_slot_is_not_accepted():
    assert spill('89442408',depth=40) is None
    assert spill('89442408',call='ff9160010000') is None

@pytest.fixture(scope='module')
def magicman():
    root=Path(__file__).resolve().parents[2]
    f=next(f for f in json.loads((root/'refs/script_recovery/bordello/translation_unit_typed.json').read_text())['functions'] if int(f['address'],16)==0xe40e80)
    return f


def test_real_destructors_resolve_to_two_distinct_registered_ids(magicman):
    from tools.script_recovery.convert_quest_unit import _text_order_sites,_register_operand_slot
    source=magicman['decompile']
    out=recover(source,magicman,_text_order_sites(source,magicman),_register_operand_slot)
    ending=out[out.index('LAB_00e4450d:'):out.index('LAB_00e4451f:')]
    assert 'CTimer::~CTimer((CTimer *)&timerId);' in ending
    assert 'CTimer::~CTimer((CTimer *)&iStack_378);' in ending


def test_reassigned_timer_variable_is_not_used_as_stable_identity(magicman):
    from tools.script_recovery.convert_quest_unit import _text_order_sites,_register_operand_slot
    source=magicman['decompile']+'\n  timerId = 123;\n'
    out=recover(source,magicman,_text_order_sites(source,magicman),_register_operand_slot)
    ending=out[out.index('LAB_00e4450d:'):out.index('LAB_00e4451f:')]
    assert 'CTimer::~CTimer((CTimer *)&timerId);' not in ending
