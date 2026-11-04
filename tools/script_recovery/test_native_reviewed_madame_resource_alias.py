"""Pinned resource receiver sites and stable Lua handle for Madame."""
from copy import deepcopy
import json
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_madame_resource_alias import RECEIVERS, restore


ROOT = Path(__file__).resolve().parents[2]
SOURCE = (ROOT / 'work/codex_lua_marathon62_20260930/pre_promotion/refs/script_recovery/'
          'lifted/Bordello/draft/FSE/V_Bordello/Entities/Madame.lua')


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter.native('0x00e3bb70'), unit['reviewedMadameResourceAlias']['0x00e3bb70'], converter.rdata


def test_all_native_getters_retain_constructed_resource(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text()
    changed = restore(source, fn, row, rdata)
    assert changed.count('xStack_174 = resources:NewResource()\n    local madameResource = xStack_174') == 1
    assert changed.count('resources:ScriptThing(madameResource)') == len(RECEIVERS) == 22
    assert changed.count('resources:ReleaseResource(madameResource)') == 2
    assert 'resources:ScriptThing(xStack_' not in changed
    assert restore(source, fn, None, rdata) == source
    LuaRuntime().compile(changed)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['getterSites'].__setitem__(0, '0x00E3BFBA'),
    lambda row: row.__setitem__('getterTarget', '0x00000000'),
])
def test_stale_resource_evidence_rejected(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed Madame resource alias'):
        restore(SOURCE.read_text(), fn, changed, rdata)


def test_unrelated_resource_source_rejected(evidence):
    fn, row, rdata = evidence
    source = SOURCE.read_text().replace('resources:ScriptThing(xStack_124)',
                                        'resources:ScriptThing(xStack_150)', 1)
    with pytest.raises(ValueError, match='lifted GetScriptThing receivers changed'):
        restore(source, fn, row, rdata)
