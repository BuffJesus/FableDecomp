"""Reviewed native spill operands and generated comment-only book behavior."""
import copy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import unwrap_statements, _text_order_sites
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_evidence_lowering import LoweringSpec
from tools.script_recovery.native_member_resource_calls import recover_member_resource_calls
from tools.script_recovery.test_book_specific_args import generated
from tools.script_recovery.test_book_saved_index import run_selection

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture
def inputs():
    base = ROOT / 'refs/script_recovery/book_collecting'
    unit = json.loads((base / 'units/V_BookCollecting.json').read_text())
    fn = next(f for f in json.loads((base / 'translation_unit_typed.json').read_text())['functions']
              if int(f['address'], 16) == 0xe57530)
    row = unit['reviewedResourceOperands']['0x00e57530']
    spec = LoweringSpec(unit, 'BS_Teacher', entity=True, thing_slots={})
    return fn, row, spec, RData()


def recover(inputs, mutate_text=None):
    fn, row, spec, image = inputs
    text = unwrap_statements(fn['decompile'])
    if mutate_text:
        text = mutate_text(text)
    return recover_member_resource_calls(text, fn, _text_order_sites(text, fn), spec, image, row)


def test_reviewed_dynamic_text_and_final_flag(inputs):
    output = recover(inputs)
    assert 'pCVar8,pvVar11,0,0,1,0)' in output
    assert '"TEXT_QST_B16_BOOK_COMMENT_"' in output
    assert 'local_18 = *pCVar12;' in output  # separate unresolved native spill stays visible


@pytest.mark.parametrize('fault', ['source', 'native', 'anchor', 'offset', 'spill',
                                  'frame', 'dispatch', 'push', 'text', 'helper', 'hero'])
def test_changed_or_ambiguous_evidence_is_rejected(inputs, fault):
    fn, row, spec, image = inputs
    fn, row = copy.deepcopy((fn, row))
    call = row['calls'][0]
    if fault == 'source': fn['decompile'] += '\n'
    if fault == 'native': row['nativeSha256'] = '0' * 64
    if fault == 'anchor': call['resourceAnchor'] = '0x00e57770'
    if fault == 'offset': call['offset'] += 4
    if fault == 'spill': call['vtableStack'][0]['bytes'] = '90'
    if fault == 'frame': call['vtableStack'][1]['entryEspDelta'] += 4
    if fault == 'dispatch': call['dispatch']['bytes'] = 'ff5068'
    if fault == 'push': call['flagPushes'][0]['bytes'] = '6a01'
    if fault == 'text': call['textToken']['start'] += 1
    if fault == 'helper': call['helpers'][0]['sha256'] = '0' * 64
    if fault == 'hero': call['heroSite'] = '0x00e57770'
    with pytest.raises(ValueError, match='resource operand evidence'):
        recover((fn, row, spec, image))


def test_transformed_text_cannot_silently_lose_reviewed_operand(inputs):
    with pytest.raises(ValueError, match='resource operand evidence'):
        recover(inputs, lambda s: s.replace('pCVar8,pvVar11,uVar15', 'pCVar8,unknown,uVar15'))


@pytest.mark.parametrize('index', [5, 6])
@pytest.mark.parametrize('seen', [(), (5, 6)])
@pytest.mark.parametrize('interrupt', [False, True])
def test_comment_book_is_spoken_without_donation(generated, index, seen, interrupt):
    for _, source in generated:
        speech = []
        result = run_selection(source, have=[index], seen=seen, interrupt=interrupt, speech=speech)
        assert result['spoken'] == [f'TEXT_QST_B16_BOOK_COMMENT_{index-5}']
        assert speech == [('hero', f'TEXT_QST_B16_BOOK_COMMENT_{index-5}', 0, False, True, False)]
        assert result['requests'] == []
        assert result['seen'] == sorted(set(seen) | {index})
        assert result['frames'] == 1


@pytest.mark.parametrize('index', [25, 26, 39])
def test_native_reference_comment_suffixes(generated, index):
    for _, source in generated:
        speech = []
        result = run_selection(source, have=[index], bounds=(20, 25, 40), count=40, speech=speech)
        assert speech == [('hero', f'TEXT_QST_B16_BOOK_COMMENT_{index-25}', 0, False, True, False)]
        assert result['requests'] == []


def test_comment_zero_health_does_not_speak_or_donate(generated):
    for _, source in generated:
        result = run_selection(source, have=[5], health=0)
        assert result['spoken'] == result['requests'] == []


def test_request_category_precedes_comment_category(generated):
    for _, source in generated:
        result = run_selection(source, have=[4, 5])
        assert result['requests'] == [(4, 'TEXT_QST_B16_BOOK_REFUSED')]
        assert result['spoken'] == ['TEXT_QST_B16_BOOK_REQUEST_4']
