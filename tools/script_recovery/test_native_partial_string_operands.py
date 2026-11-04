"""A printed string operand can be matched to its native stack slot by its constructor."""
from copy import deepcopy
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter


ROOT = Path(__file__).resolve().parents[2]
BEER_SITES = ('0x00E428F1', '0x00E44477')


@pytest.fixture(scope='module')
def converter_and_main():
    converter = UnitConverter(ROOT / 'refs/script_recovery/bordello/translation_unit_typed.json')
    return converter, converter.native('0x00e40e80')


def beer_calls(source):
    return [line.strip() for line in source.splitlines() if '+ 0x1e4))(' in line]


def test_retail_beer_calls_recover_count_and_flag(converter_and_main):
    converter, main = converter_and_main
    calls = beer_calls(converter.recover_dropped_operands(main['decompile'], main))
    assert calls == [
        '(**(code **)(**(int **)((int)this + 4) + 0x1e4))(&xStack_2bc,-1,1);',
        '(**(code **)(**(int **)((int)this + 4) + 0x1e4))(&xStack_2b0,-1,1);',
    ]


@pytest.mark.parametrize('site', BEER_SITES)
@pytest.mark.parametrize('bad_slot', [None, -4])
def test_unproved_constructor_slot_keeps_printed_call(converter_and_main, site, bad_slot):
    converter, original = converter_and_main
    main = deepcopy(original)
    ctor = next(call for call in main['calls'] if call['site'] == site)
    ctor['ecxStack'] = bad_slot
    calls = beer_calls(converter.recover_dropped_operands(main['decompile'], main))
    index = BEER_SITES.index(site)
    assert calls[index].endswith(('(aCStack_320);', '(aCStack_324);')[index])
    assert calls[1 - index].endswith(('(&xStack_2bc,-1,1);', '(&xStack_2b0,-1,1);')[1 - index])
