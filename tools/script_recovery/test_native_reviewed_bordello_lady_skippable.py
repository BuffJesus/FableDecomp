"""Retail and sidecar evidence for BordelloLady's four skip flags."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_lady_skippable import AFTER, BEFORE, restore


ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter.native('0x00e3eb10'), unit['reviewedBordelloLadySkippable']['0x00e3eb10'], converter.rdata


def test_skip_flags_follow_retail_pushes(evidence):
    fn, row, rdata = evidence
    old = '\n'.join(BEFORE)
    todo = [row['diagnostic']] * 3
    assert restore(old, todo, fn, row, rdata) == ('\n'.join(AFTER), [])
    assert restore(old, todo, fn, None, rdata) == (old, todo)


@pytest.mark.parametrize('mutation', [
    lambda row: row.__setitem__('nativeSha256', '0' * 64),
    lambda row: row.__setitem__('sourceSha256', '0' * 64),
    lambda row: row['nativeCallsInSourceOrder'].__setitem__(0, '0x00E3F4B9'),
    lambda row: row.__setitem__('diagnostic', 'wrong'),
])
def test_stale_evidence_fails(evidence, mutation):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    mutation(changed)
    with pytest.raises(ValueError, match='reviewed BordelloLady skip flags'):
        restore('\n'.join(BEFORE), [row['diagnostic']] * 3, fn, changed, rdata)


def test_wrong_lift_shape_is_rejected(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted skip calls changed'):
        restore('\n'.join([BEFORE[0], BEFORE[1], 'quest:SetCutsceneSkippable(false)', BEFORE[3]]),
                [row['diagnostic']] * 3, fn, row, rdata)


def test_sidecar_registers_boolean_binding():
    base = ROOT / 'work/new-oakvale-original-fse-20260912/sidecar-abi-v2/FableScriptExtender'
    registration = (base / 'LuaManager.cpp').read_text()
    implementation = (ROOT / 'refs/script_recovery/new_oakvale_intro/runtime_evidence/session-start-20260912/forge-source/LuaQuestState.cpp').read_text()
    assert 'questState_type["SetCutsceneSkippable"] = &LuaQuestState::SetCutsceneSkippable;' in registration
    assert 'void LuaQuestState::SetCutsceneSkippable(bool bIsSkippable)' in implementation
    assert 'SetCutsceneSkippable_API(m_pGameInterface, bIsSkippable)' in implementation
