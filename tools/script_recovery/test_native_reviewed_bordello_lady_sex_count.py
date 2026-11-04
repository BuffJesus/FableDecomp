"""Retail getter-to-setter evidence for BordelloLady's sex count."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_sex_count import NEW, OLD, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter.native('0x00e3eb10'), unit['reviewedBordelloLadySexCount']['0x00e3eb10'], converter.rdata


def test_setter_gets_incremented_getter_result(evidence):
    fn, row, rdata = evidence
    assert restore(OLD, [row['diagnostic']], fn, row, rdata) == (NEW, [])
    assert restore(OLD, [row['diagnostic']], fn, None, rdata) == (OLD, [row['diagnostic']])


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeInstructions'].__setitem__('0x00E3FD8A', '00'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady sex count'):
        restore(OLD, [row['diagnostic']], fn, changed, rdata)


def test_other_setter_is_not_rewritten(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted count call changed'):
        restore(OLD.replace('r12 = ', 'r13 = '), [row['diagnostic']], fn, row, rdata)


def test_sidecar_registers_integer_getter_and_setter():
    base = ROOT / 'work/new-oakvale-original-fse-20260912/sidecar-abi-v2/FableScriptExtender'
    registration = (base / 'LuaManager.cpp').read_text()
    implementation = (ROOT / 'refs/script_recovery/new_oakvale_intro/runtime_evidence/session-start-20260912/forge-source/LuaQuestState.cpp').read_text()
    for name in ('GetNumberOfTimesHeroHasHadSex', 'SetNumberOfTimesHeroHasHadSex'):
        assert f'questState_type["{name}"] = &LuaQuestState::{name};' in registration
    assert 'int LuaQuestState::GetNumberOfTimesHeroHasHadSex()' in implementation
    assert 'void LuaQuestState::SetNumberOfTimesHeroHasHadSex(int count)' in implementation
