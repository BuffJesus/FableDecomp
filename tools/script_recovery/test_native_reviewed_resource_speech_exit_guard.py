import copy
import json
import re
from pathlib import Path
from unittest.mock import patch

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_resource_speech import _recover, restore
from tools.script_recovery.native_wide_assign_receivers import recover_wide_assign_receivers

ADDRESS = '0x00f1b9c0'


@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}

    class Captured(Exception):
        pass

    def speech(source, fn, row, rdata):
        if row is not None and fn['address'].lower() == ADDRESS:
            seen['speech'] = (source, fn, row, rdata)
        return restore(source, fn, row, rdata)

    def wide(source, fn, rdata):
        if fn['address'].lower() == ADDRESS:
            seen['wide'] = (source, fn, rdata)
            raise Captured()
        return recover_wide_assign_receivers(source, fn, rdata)

    # Constructor emulation (Unicorn) is unrelated to operand recovery and can
    # fault inside pytest on this Windows host; keep the real native evidence.
    with patch('tools.script_recovery.native_reviewed_resource_speech.restore', speech), \
            patch('tools.script_recovery.native_wide_assign_receivers.recover_wide_assign_receivers', wide), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        with pytest.raises(Captured):
            converter.convert(unit, tmp_path_factory.mktemp('exit_guard_input'))
    return seen


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_exit_guard_speech_operands_and_waits(captured, newline):
    source, fn, row, rdata = captured['speech']
    result = restore(source.replace('\n', newline), fn, row, rdata)
    assert result.count('RESOURCE_Speak(') == 10
    assert result.count('RESOURCE_IsPerformingScriptTask(') == 24
    assert result.count('RESOURCE_ClearCommands(') == 2
    assert result.count(', 0, false, true, false)') == 10
    assert result.count('RESOURCE_Speak(xStack_12c, (CScriptThing *)pCVar11,') == 10
    assert {r['listenerKind'] for r in row['calls'] if r['method'] == 'Speak'} == {'hero'}


def test_hero_listener_must_be_latest_get_hero_result(captured):
    source, fn, _, rdata = captured['speech']
    head = source.index('pCVar11 = GSI->GetHero();')
    # an intervening reassignment of the listener variable breaks the native EAX witness
    stale = source[:head] + 'pCVar11 = GSI->GetHero();\npCVar11 = pCVar13;' + source[head + len('pCVar11 = GSI->GetHero();'):]
    with pytest.raises(ValueError, match='witness mismatch'):
        _recover(stale, fn, rdata)
    renamed = source.replace('pCVar11 = GSI->GetHero();', 'pCVar99 = GSI->GetHero();', 1)
    with pytest.raises(ValueError, match='witness mismatch'):
        _recover(renamed, fn, rdata)


@pytest.mark.parametrize('kind', ['loweredSha256', 'listener', 'key', 'missingCall'])
def test_exit_guard_drift_is_rejected(captured, kind):
    source, fn, original, rdata = captured['speech']
    row = copy.deepcopy(original)
    speech = next(r for r in row['calls'] if r['method'] == 'Speak')
    if kind == 'loweredSha256':
        row[kind] = '0' * 64
    elif kind == 'missingCall':
        row['calls'].pop()
    else:
        speech[kind] = 'wrong'
    with pytest.raises(ValueError):
        restore(source, fn, row, rdata)


def test_wide_assign_destination_recovered_from_native(captured):
    source, fn, rdata = captured['wide']
    assert source.count('CCharString__AssignFromWide("");') == 2
    result = recover_wide_assign_receivers(source, fn, rdata)
    assert 'AssignFromWide' not in result
    assert 'CCharString::CCharString((CCharString *)&xStack_ec,"",-1);' in result
    assert 'CCharString::CCharString((CCharString *)&xStack_fc,"",-1);' in result


@pytest.mark.parametrize('kind', ['literal', 'extraPrinted', 'missingSite', 'slot', 'depth', 'order', 'unnamed'])
def test_wide_assign_disagreement_leaves_source_unchanged(captured, kind):
    source, original_fn, rdata = captured['wide']
    fn = copy.deepcopy(original_fn)
    sites = [c for c in fn['calls'] if int(c['target'], 16) == 0x99B6B0]
    if kind == 'literal':
        source = source.replace('CCharString__AssignFromWide("");', 'CCharString__AssignFromWide("x");', 1)
    elif kind == 'extraPrinted':
        source = source + '\nCCharString__AssignFromWide("");\n'
    elif kind == 'missingSite':
        fn['calls'].remove(sites[0])
    elif kind == 'slot':
        sites[0]['ecxStack'] -= 4
    elif kind == 'depth':
        sites[1]['depth'] += 4
    elif kind == 'order':
        fn['callOrder'] = [s for s in fn['callOrder'] if s.lower() != sites[0]['site'].lower()]
    elif kind == 'unnamed':
        source = re.sub(r'\bxStack_ec\b', 'xStack_zz', source)
    assert recover_wide_assign_receivers(source, fn, rdata) == source
