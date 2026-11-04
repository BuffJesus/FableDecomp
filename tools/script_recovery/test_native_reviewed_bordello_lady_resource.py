"""Guard BordelloLady's stack resource and parent assignment sites."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_resource import ASSIGN, restore


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'work/codex_lua_marathon63_20260930/baseline/BordelloLady_pre.lua'


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return (converter.native('0x00e3eb10'),
            unit['reviewedBordelloLadyResource']['0x00e3eb10'], converter.rdata)


def test_two_parent_copies_and_scripted_thing_reads_keep_constructed_handle(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text()
    changed, todo = restore(source, [row['diagnostic']] * 2, fn, row, rdata)
    assert changed.count('local ladyResource = x_stk_188') == 1
    assert changed.count('resources:AssignResource(resources:MemberResource("seh_Whore"), ladyResource)') == 2
    assert changed.count('resources:ScriptThing(ladyResource)') == 3
    assert changed.count(ASSIGN) == 0 and todo == []
    assert restore(source, [row['diagnostic']] * 2, fn, None, rdata) == (
        source, [row['diagnostic']] * 2)
    LuaRuntime().compile(changed)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3F35B', '00'),
    lambda row: row['nativeCalls'].__setitem__('0x00E3FB8A', '0x00000000'),
    lambda row: row['getterSites'].__setitem__(0, '0x00E3EE13'),
])
def test_stale_native_evidence_rejected(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady resource'):
        restore(SOURCE.read_text(), [row['diagnostic']] * 2, fn, changed, rdata)


def test_mismatched_lifted_resource_rejected(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted resource uses changed'):
        restore(SOURCE.read_text().replace('resources:ScriptThing(xStack_1a8)',
                                           'resources:ScriptThing(xStack_1a0)'),
                [row['diagnostic']] * 2, fn, row, rdata)
