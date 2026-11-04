"""Reviewed native GetScriptThing receivers must fail closed when evidence drifts."""
from copy import deepcopy
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter, _text_order_sites, restore_stack_operands
from tools.script_recovery.native_resource_thing_receivers import GET_THING_RECEIVER, restore


@pytest.fixture(scope='module')
def magicman():
    root = Path(__file__).resolve().parents[2]
    converter = UnitConverter(root / 'refs/script_recovery/bordello/translation_unit_typed.json')
    fn = converter.native('0x00e40e80')
    unit = __import__('json').loads((root / 'refs/script_recovery/bordello/units/V_Bordello.json').read_text())
    row = unit['reviewedResourceThingReceivers']['0x00e40e80']
    source = restore_stack_operands(fn['decompile'], fn)
    sites = _text_order_sites(fn['decompile'], fn)
    return source, fn, row, converter.rdata, sites


def test_reviewed_receivers_change_only_the_getter_argument(magicman):
    source, fn, row, rdata, sites = magicman
    changed = restore(source, fn, row, rdata, sites)
    assert changed.count('(CScriptGameResourceObjectScriptedThingBase *)&xStack_364') == 30
    assert source.count('(CScriptGameResourceObjectScriptedThingBase *)&xStack_364') < 30
    scrub = lambda text: GET_THING_RECEIVER.sub(lambda match: match['head'] + '<receiver>', text)
    assert scrub(source) == scrub(changed)
    assert restore(source, fn, None, rdata, sites) == source


@pytest.mark.parametrize('field,value', [
    ('nativeSha256', '0' * 64),
    ('sourceSha256', '0' * 64),
    ('anchorSlot', -852),
    ('getterReceiverDisplacement', 44),
])
def test_stale_evidence_is_rejected(magicman, field, value):
    source, fn, row, rdata, sites = magicman
    bad = deepcopy(row)
    bad[field] = value
    with pytest.raises(ValueError):
        restore(source, fn, bad, rdata, sites)


def test_missing_site_or_printed_call_is_rejected(magicman):
    source, fn, row, rdata, sites = magicman
    bad = deepcopy(row)
    bad['sites'].pop()
    with pytest.raises(ValueError):
        restore(source, fn, bad, rdata, sites)
    with pytest.raises(ValueError):
        restore(source.replace('_GetScriptThing_', '_WrongGetter_', 1), fn, row, rdata, sites)
