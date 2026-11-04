import copy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter, _align, _text_order_sites, restore_stack_operands
from tools.script_recovery.native_me_call_operands import restore

ROOT = Path(__file__).resolve().parents[2]


def fixture(body='8d7e08', pushes='6a006a0057', operand='alias'):
    raw = bytes.fromhex('568bf157' + body + pushes + '8b4e048b01ff90b00500005f5ec3')
    address = 0x1000 + raw.index(bytes.fromhex('ff90b0050000'))
    text = f'(**(code **)(**(int **)((int)this + 4) + 0x5b0))({operand},0,0);'
    start = text.index(f'({operand},') + 1
    site = dict(site=hex(address), slot='0x5b0', ecxValue=4, pushedStack=[None, None, 8])
    entries = [(start, len(text)-2, [operand, '0', '0'], site, '0x5b0', True)]
    fn = dict(address='0x1000', bodyEndExclusive=hex(0x1000+len(raw)))
    class Bytes:
        def bytes_at(self, start, size):
            return raw[start-0x1000:start-0x1000+size]
    return text, fn, entries, Bytes()


def test_me_operand_uses_native_definition_not_the_merged_source_name():
    text, fn, entries, data = fixture()
    assert restore(text, fn, entries, data) == text.replace('(alias,', '((CScriptThing *)((int)this + 8),')


@pytest.mark.parametrize('body,pushes', [
    ('8d7e04', '6a006a0057'),                         # interface pointer
    ('8d7e0885c074038d7e04', '6a006a0057'),           # ambiguous incoming definition
    ('8d7e088bf0', '6a006a0057'),                    # owner register overwritten
    ('8d7e08', '6a00e8000000006a0057'),              # intervening call
    ('8d4608', '6a006a0050'),                        # volatile register is not proven
    ('8d7e08ffe0', '6a006a0057'),                    # unbounded indirect branch
    ('8d7e08eb04', '6a006a0057'),                    # branch enters partial push sequence
])
def test_ambiguous_native_operand_stays_unresolved(body, pushes):
    text, fn, entries, data = fixture(body, pushes)
    assert restore(text, fn, entries, data) == text


@pytest.mark.parametrize('operand', ['side()', '*pointer', 'pointer[0]', 'pointer + 1'])
def test_source_effects_and_memory_reads_are_not_erased(operand):
    text, fn, entries, data = fixture(operand=operand)
    assert restore(text, fn, entries, data) == text


@pytest.mark.parametrize('change', ['receiver', 'arity', 'slot', 'offset', 'not_vtable'])
def test_argument_and_call_inventory_must_agree(change):
    text, fn, entries, data = fixture()
    site = entries[0][3]
    if change == 'receiver':site['ecxValue'] = 8
    if change == 'arity':site['pushedStack'].append(None)
    if change == 'slot':site['slot'] = '0x5b4'
    if change == 'offset':site['pushedStack'][-1] = 12
    if change == 'not_vtable':entries[0] = (*entries[0][:-1], False)
    assert restore(text, fn, entries, data) == text


def test_vtable_edx_feeds_stack_push_not_an_extra_register_parameter():
    site = dict(slot='0x120', ecxValue=4, edxStack=-512, pushedStack=[-484, -512])
    assert _align(site, ['&out', '&name'], True) == [(-512, None), (-484, None)]
    # Direct fastcall needs both register operands; their identity is retained.
    direct = dict(ecxStack=-20, edxStack=-24, pushedStack=[-28])
    assert _align(direct, ['self', 'arg', 'other']) == [(-20, None), (-24, None), (-28, None)]
    assert _align(site, ['&out', '&name', 'unrecorded', 'extra'], True) is None


def test_retail_cached_bully_and_me_operands_are_distinct():
    converter = UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    fn = converter.native('0x00e58d40')
    source = fn['decompile']
    recovered = restore(source, fn, _text_order_sites(source, fn), converter.rdata)
    assert recovered.count('(CScriptThing *)((int)this + 8)') - source.count('(CScriptThing *)((int)this + 8)') == 18
    entries = _text_order_sites(recovered, fn)
    calls = {int(site['site'], 16): args for _, _, args, site, _, _ in entries}
    for site, index in [(0xe5aac8, 0), (0xe5ab2c, 3), (0xe5ab89, 3), (0xe5abe6, 3), (0xe5ac2b, 3)]:
        assert calls[site][index] == '(CScriptThing *)((int)this + 8)'
    aligned = restore_stack_operands(recovered, fn)
    calls = {int(site['site'], 16): args for _, _, args, site, _, _ in _text_order_sites(aligned, fn)}
    assert calls[0xe58e5e][0] == '&xStack_200'
    assert calls[0xe5aadb][1] == '&xStack_200'


def test_unreviewed_vector_return_keeps_its_existing_alignment():
    # Broad EDX recovery at D450B6 split the vector's later element uses.
    # This layout requires separate whole-lifetime recovery, not partial repair.
    site = dict(slot='0x150', ecxValue=4, edxStack=-148, pushedStack=[-148])
    assert _align(site, ['&vector'], True) is None


@pytest.mark.parametrize('loaded', [False, True])
def test_printed_receiver_and_early_unrelated_push_are_not_confused(loaded):
    _, fn, original, data = fixture(pushes='6a016a006a0057')
    prefix = 'iVar9 = *iface;\n' if loaded else ''
    base = 'iVar9' if loaded else '*iface'
    text = prefix + f'(**(code **)({base} + 0x5b0))(iface,actor,0,0);'
    site = original[0][3]
    site['pushedStack'] = [None, None, None, 8]
    start = text.index('(iface,') + 1
    entries = [(start, len(text)-2, ['iface','actor','0','0'], site, '0x5b0', True)]
    assert restore(text, fn, entries, data) == text
