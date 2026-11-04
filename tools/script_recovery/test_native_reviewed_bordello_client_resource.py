"""Retail and sidecar evidence for BordelloClient's controlled resource."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_client_resource import (
    NEW_ASSIGN, NEW_THING, OLD_ASSIGN, OLD_THING, restore,
)


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ('xStack_118 = resources:NewResource()\n'
          'resources:TryAcquire(xStack_118, me, 4)\n'
          f'{OLD_ASSIGN}\n'
          f'local thing = {OLD_THING}')


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e44ea0'),
            unit['reviewedBordelloClientResource']['0x00e44ea0'], converter.rdata)


def test_member_assignment_uses_constructed_handle(evidence):
    fn, row, rdata = evidence
    result, todo = restore(SOURCE, [row['diagnostic']], fn, row, rdata)
    assert NEW_ASSIGN in result and NEW_THING in result and 'xStack_120' not in result
    assert todo == []
    assert restore(SOURCE, [row['diagnostic']], fn, None, rdata) == (
        SOURCE, [row['diagnostic']])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E45FC7', '00'),
    lambda row: row['nativeCalls'].__setitem__('0x00E45FCD', '0x00000000'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_resource_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloClient resource'):
        restore(SOURCE, [row['diagnostic']], fn, changed, rdata)


def test_other_resource_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted resource alias changed'):
        restore(SOURCE.replace('seh_Whore', 'seh_Madam'),
                [row['diagnostic']], fn, row, rdata)


def test_sidecar_assign_and_get_use_same_handle(evidence):
    fn, row, rdata = evidence
    result, _ = restore(SOURCE, [row['diagnostic']], fn, row, rdata)
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute('''
        events = {}
        resources = {}
        function resources:NewResource() return 17 end
        function resources:TryAcquire(id, me, mode) return true end
        function resources:MemberResource(name) return 29 end
        function resources:AssignResource(dst, src)
            table.insert(events, {"assign", dst, src})
        end
        function resources:ScriptThing(id)
            table.insert(events, {"thing", id})
            return id
        end
        me = 3
    ''')
    lua.execute(result)
    assert [(lua.globals().events[i][1], lua.globals().events[i][2],
             lua.globals().events[i][3]) for i in (1, 2)] == [
                 ('assign', 29, 17), ('thing', 17, None)]


def test_sidecar_has_member_and_assignment_bindings():
    source = ROOT / 'work/codex_lua_marathon48_20260930/sidecar_candidate/FableScriptExtender/LuaRetailResources.h'
    text = source.read_text()
    assert 'type["MemberResource"] = &LuaRetailResources::MemberResource;' in text
    assert 'type["AssignResource"] = &LuaRetailResources::AssignResource;' in text
