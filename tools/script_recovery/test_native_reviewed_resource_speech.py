import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_resource_speech import restore


@pytest.fixture(scope='module')
def guard(tmp_path_factory):
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    captured = []

    class Captured(Exception):
        pass

    def capture(source, fn, row, rdata):
        if row is not None and fn['address'].lower() == '0x00f17c70':   # ArenaCellDoorGuard.Main
            captured.append((source, fn, row, rdata))
            raise Captured()
        return restore(source, fn, row, rdata)

    # Constructor emulation is unrelated to operand recovery and uses Unicorn,
    # which can fault inside pytest on this Windows host. Source lowering does
    # not depend on those runtime defaults; retain real native speech evidence.
    with patch('tools.script_recovery.native_reviewed_resource_speech.restore', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        with pytest.raises(Captured):
            converter.convert(unit, tmp_path_factory.mktemp('speech_input'))
    return captured[0]


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_native_speech_operands_and_all_resource_waits(guard, newline):
    source, fn, row, rdata = guard
    result = restore(source.replace('\n', newline), fn, row, rdata)
    assert result.count('RESOURCE_Speak(') == 28
    assert result.count('RESOURCE_IsPerformingScriptTask(') == 60
    assert result.count('RESOURCE_ClearCommands(') == 2
    assert result.count(', 0, false, true, false)') == 28
    assert 'RESOURCE_Speak(xStack_224, (CScriptThing *)(this + 8),' in result
    assert 'RESOURCE_IsPerformingScriptTask(xStack_1ec)' in result


def test_unreviewed_function_is_unchanged(guard):
    source, fn, _, rdata = guard
    assert restore(source, fn, None, rdata) == source


@pytest.mark.parametrize('kind', [
    'nativeSha256', 'sourceSha256', 'loweredSha256', 'site', 'receiver', 'depth',
    'lea', 'key', 'listener', 'pushes', 'zeroDefinitions', 'helperBytes',
    'nativeInstructions', 'missingCall', 'extraCall', 'sourceArguments',
    'listenerSlot', 'metadataDepth', 'metadataReceiver', 'metadataTarget',
])
def test_native_or_source_drift_is_rejected(guard, kind):
    source, original_fn, original_row, rdata = guard
    row, fn = copy.deepcopy(original_row), copy.deepcopy(original_fn)
    speech = next(r for r in row['calls'] if r['method'] == 'Speak')
    if kind.endswith('Sha256'):
        row[kind] = '0' * 64
    elif kind in ('site', 'receiver', 'depth', 'lea'):
        row['calls'][0][kind] += 4
    elif kind in ('key', 'listener'):
        speech[kind] = 'wrong'
    elif kind == 'pushes':
        speech[kind].reverse()
    elif kind == 'zeroDefinitions':
        speech[kind] = {}
    elif kind in ('helperBytes', 'nativeInstructions'):
        row[kind][next(iter(row[kind]))] = '00'
    elif kind == 'missingCall':
        row['calls'].pop()
    elif kind == 'extraCall':
        row['calls'].append(copy.deepcopy(row['calls'][0]))
    elif kind == 'sourceArguments':
        speech[kind][1] = 'me'
    elif kind == 'listenerSlot':
        next(r for r in row['calls'] if 'listenerSlot' in r)['listenerSlot'] += 4
    else:
        call = next(c for c in fn['calls'] if int(c['site'], 16) == row['calls'][0]['site'])
        if kind == 'metadataDepth':
            call['depth'] += 4
        elif kind == 'metadataReceiver':
            call['ecxStack'] += 4
        else:
            call['target'] = '0x007E7390'
    with pytest.raises(ValueError):
        restore(source, fn, row, rdata)
