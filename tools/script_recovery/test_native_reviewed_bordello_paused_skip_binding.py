"""Verify the reviewed Bordello paused cutscene skip binding evidence."""
from copy import deepcopy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_bordello_paused_skip_binding import COMMENT, DIAGNOSTIC, restore


ROOT = Path(__file__).resolve().parents[2]
SOURCE = (f'    {COMMENT}\n    quest:SetCutsceneSkippableWhilePaused(true)\n'
          f'    {COMMENT}\n    quest:SetCutsceneSkippableWhilePaused(false)\n')


@pytest.fixture(scope='module')
def evidence():
    unit = json.loads((ROOT / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter.native('0x00e3e720'), unit['reviewedBordelloPausedSkipBinding']['0x00e3e720'], converter.rdata


def test_binding_clears_only_stale_comments_and_diagnostics(evidence):
    fn, row, rdata = evidence
    updated, todo = restore(SOURCE, [DIAGNOSTIC, 'other', DIAGNOSTIC], fn, row, rdata)
    assert COMMENT not in updated
    assert updated.count('quest:SetCutsceneSkippableWhilePaused(') == 2
    assert todo == ['other']
    assert restore(SOURCE, [DIAGNOSTIC, DIAGNOSTIC], fn, None, rdata) == (SOURCE, [DIAGNOSTIC, DIAGNOSTIC])


@pytest.mark.parametrize('field', ['nativeSha256', 'sourceSha256', 'bindingPatchSha256'])
def test_stale_evidence_fails(evidence, field):
    fn, row, rdata = evidence
    changed = deepcopy(row)
    changed[field] = '0' * 64
    with pytest.raises(ValueError, match='reviewed Bordello paused skip binding'):
        restore(SOURCE, [DIAGNOSTIC, DIAGNOSTIC], fn, changed, rdata)


def test_changed_lifted_call_fails(evidence):
    fn, row, rdata = evidence
    with pytest.raises(ValueError, match='lifted paused skip calls changed'):
        restore(SOURCE.replace('(false)', '(nil)'), [DIAGNOSTIC, DIAGNOSTIC], fn, row, rdata)
