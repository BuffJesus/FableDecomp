import copy
import json
from pathlib import Path

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_temporary_cleanup import restore

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def evidence():
    c = UnitConverter(ROOT/'refs/script_recovery/beggar_and_child/translation_unit_typed.json')
    fn = c.native('0x00e58d40')
    row = json.loads((ROOT/'refs/script_recovery/beggar_and_child/units/V_BeggarAndChild.json').read_text())['reviewedTemporaryCleanup']['0x00e58d40']
    source = '\n'.join(g['flagSet'] + f'\nlive_message_{i}();\n' + g['guardBlock']
                       for i, g in enumerate(row['groups']))
    return source, fn, row, c.rdata


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_reviewed_cleanup_preserves_each_message_and_surrounding_source(evidence, newline):
    source, fn, row, data = evidence
    source = 'before();\n' + source + '\nafter();'
    result = restore(source.replace('\n', newline), fn, row, data)
    assert result == newline.join(['before();', '', 'live_message_0();', '', '',
                                   'live_message_1();', '', '', 'live_message_2();', '', 'after();'])


@pytest.mark.parametrize('group', range(3))
@pytest.mark.parametrize('kind', ['anchor', 'missing', 'duplicate', 'effect', 'reverse'])
def test_stale_or_ambiguous_cleanup_group_rejected(evidence, group, kind):
    source, fn, row, data = evidence
    row = copy.deepcopy(row)
    g = row['groups'][group]
    if kind == 'anchor':
        g['nativeInstructions'][next(iter(g['nativeInstructions']))] = '00'
    elif kind == 'missing':
        source = source.replace(g['flagSet'], '', 1)
    elif kind == 'duplicate':
        source += '\n' + g['guardBlock']
    elif kind == 'effect':
        source = source.replace(g['guardBlock'], g['guardBlock'].replace('{', '{ live_effect();', 1))
    elif kind == 'reverse':
        source = source.replace(g['flagSet'], 'PLACEHOLDER', 1).replace(g['guardBlock'], g['flagSet'], 1).replace('PLACEHOLDER', g['guardBlock'])
    with pytest.raises(ValueError, match='reviewed temporary cleanup'):
        restore(source, fn, row, data)


def test_overlapping_groups_rejected(evidence):
    source, fn, row, data = evidence
    row = copy.deepcopy(row)
    row['groups'].append(copy.deepcopy(row['groups'][0]))
    with pytest.raises(ValueError, match='overlapping cleanup groups'):
        restore(source, fn, row, data)
