"""Quest flag vectors retain byte semantics and exact native persistence identity."""
import json
from pathlib import Path
from types import SimpleNamespace

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_flag_vectors import lower_flag_vectors
from tools.script_recovery.native_evidence_lowering import LoweringSpec, finish_lua
from tools.script_recovery.convert_quest_unit import lift_persist_evidence, sidecar_bindings

ROOT = Path(__file__).resolve().parents[2]


def spec(entity=True):
    unit = json.loads((ROOT / 'refs/script_recovery/book_collecting/units/V_BookCollecting.json').read_text())
    s = LoweringSpec(unit, 'BS_Teacher' if entity else 'V_BookCollecting', entity=entity, thing_slots={})
    s.call_labels = {'resize': 0x467686, 'CPersistContext::Transfer<signed_char>': 0xcdcf80}
    s.resolve_string = lambda _: None
    return unit, s


@pytest.mark.parametrize('entity', [False, True])
@pytest.mark.parametrize('reverse', [False, True])
@pytest.mark.parametrize('value', [0, 1])
def test_byte_read_write_and_zero_comparison(entity, reverse, value):
    _, s = spec(entity)
    base = '*(int *)(this + 0x14)' if entity else 'this'
    begin = f'*(int *)({base} + 0xa0)'
    address = f'(int)index + {begin}' if reverse else f'{begin} + (int)index'
    text = f'*(undefined1 *)({address}) = {value};\nif (*(char *)({address}) == \'\\0\') {{'
    out = lower_flag_vectors(text, s)
    assert f'SetStateFlagVectorAt("BookDonated", index, {str(bool(value)).lower()})' in out
    assert 'ENGINE_FlagVectorByte("BookDonated", index)' in out
    lua = LuaRuntime()
    expr = finish_lua('ENGINE_FlagVectorByte("BookDonated", index)')
    fn = lua.execute('return function(quest,index) return ' + expr + ' end')
    q = lua.table_from({'GetStateFlagVectorAt': lambda _, name, index: bool(value)})
    assert fn(q, 3) == value


@pytest.mark.parametrize('source', [
    '*(undefined1 *)(*(int *)(*(int *)(this + 0x14) + 0xa0) + i) = 2;',
    '*(undefined1 *)(*(int *)(*(int *)(this + 0x14) + 0xa0) + i) += 1;',
    '*(int *)(*(int *)(*(int *)(this + 0x14) + 0xa0) + i) = 1;',
    '*(char *)(*(int *)(*(int *)(this + 0x18) + 0xa0) + i)',
    '*(char *)(*(int *)(*(int *)(this + 0x14) + 0xa4) + i)',
    '*(char *)(*(int *)(*(int *)(this + 0x14) + 0xa0) + i * 4)',
])
def test_unknown_store_width_owner_member_and_stride_remain_unresolved(source):
    assert lower_flag_vectors(source, spec()[1]) == source


@pytest.mark.parametrize('fill,between,target,expected', [
    (0, '', 0x467686, True), (1, '', 0x467686, False),
    (0, 'if (flag) {', 0x467686, False),
    (0, 'fill = 1;', 0x467686, False), (0, '', 0x123456, False)])
def test_resize_requires_native_callee_and_proven_zero_fill(fill, between, target, expected):
    _, s = spec(False)
    s.call_labels['resize'] = target
    source = f'fill = {fill};\n{between}\nresize((vector<unsigned_char> *)(this + 0xa0), count, (int)&fill);'
    assert ('StateFlagVectorResize("BookDonated", count)' in lower_flag_vectors(source, s)) == expected


@pytest.mark.parametrize('name,offset,target,expected', [
    ('BookDonated', '0xa0', 0xcdcf80, True), ('BookOwned', '0xac', 0xcdcf80, True),
    ('BookOwned', '0xa0', 0xcdcf80, False), ('BookOwned', '0xac', 0x123456, False),
    ('BookOwned', '0xa4', 0xcdcf80, False)])
def test_persist_requires_member_name_and_callee_agreement(name, offset, target, expected):
    unit, s = spec(False)
    s.call_labels['CPersistContext::Transfer<signed_char>'] = target
    source = f'CPersistContext::Transfer<signed_char>(context,"{name}",this + {offset});'
    out, calls, todo = lift_persist_evidence(source, unit, s, {})
    assert ('PersistStateFlagVector' in calls) == expected
    if expected:
        assert out == [f'    quest:PersistStateFlagVector(context, "{name}")'] and not todo


def test_exported_sidecar_patch_exposes_typed_getters():
    bindings = sidecar_bindings()
    assert bindings['GetStateFlagVectorAt']['returnType'] == 'bool'
    assert bindings['GetStateFlagVectorCount']['returnType'] == 'int'
    assert [p['name'] for p in bindings['PersistStateFlagVector']['parameters']] == ['context', 'name']
