import json
from pathlib import Path
from unittest.mock import patch

import pytest

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_evidence_lowering import fold_dword_colours
from tools.script_recovery.native_reviewed_whisper_ally import MAIN, recover_whisper_ally


@pytest.fixture(scope='module')
def captured(tmp_path_factory):
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}

    def capture(source, fn, rdata):
        if int(fn['address'], 16) == MAIN:
            seen['main'] = (source, fn, rdata)
        return recover_whisper_ally(source, fn, rdata)

    with patch('tools.script_recovery.native_reviewed_whisper_ally.recover_whisper_ally', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, tmp_path_factory.mktemp('whisper_ally'))
    return seen['main']


def test_lowering_already_resolves_pause_guard_and_colours(captured):
    source, _, _ = captured
    assert '+ 0x5ec))()' not in source
    assert 'ENGINE_Colour(255, 255, 0, 255)' in source       # ally health bar, 0xffffff00
    assert '0xffff0000' not in source                         # round bar, ENGINE_Colour(255, 0, 0, 255)


@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_creature_total_and_is_alive(captured, newline):
    source, fn, rdata = captured
    result = recover_whisper_ally(source.replace('\n', newline), fn, rdata).replace('\r\n', '\n')
    assert result.count('creatureTotal = creatureTotal + QUESTSTATE_GetInt(__key("TotalCreatures_" .. iVar12));') == 3
    assert result.count('} while (iVar12 < 3);') == 3
    assert 'while ((creatureTotal == 0 && ' in result and 'while ((creatureTotal != 0 && ' in result
    assert '0xd0' not in result.split('creatureTotal = 0;', 1)[1].split('while (iVar12 < 3)', 1)[0]
    assert result.count('CScriptThing::_IsAlive_CScriptThing__UBE_NXZ((CScriptThing *)xStack_e8)') >= 2
    assert '+ 0x12c))()' not in result
    # conversation ids and the timer keep the native slot
    assert 'xStack_108 = GSI->AddNewConversation' in result and 'GSI->DeregisterTimer(xStack_108)' in result


def test_native_or_vtable_drift_leaves_source_unchanged(captured):
    source, fn, rdata = captured

    class Drifted:
        def __init__(self, at):
            self.at = at

        def bytes_at(self, address, size):
            data = bytearray(rdata.bytes_at(address, size))
            if address <= self.at < address + size:
                data[self.at - address] ^= 0xff
            return bytes(data)
    for at in (MAIN + 0x10, 0x01238C8C + 0x12C, 0x004AB130 + 3):
        assert recover_whisper_ally(source, fn, Drifted(at)) == source


@pytest.mark.parametrize('edit', [
    ('iVar12 = iVar12 + 4;', 'iVar12 = iVar12 + 8;'),
    ('while (iVar12 < 0xdc)', 'while (iVar12 < 0xe0)'),
    ('LAB_00f241aa:\n', 'LAB_00f241ab:\n'),
])
def test_text_drift_is_all_or_nothing(captured, edit):
    source, fn, rdata = captured
    text = source.replace('\r\n', '\n')
    changed = text.replace(*edit, 1)
    assert changed != text
    assert recover_whisper_ally(changed, fn, rdata) == changed


def test_dword_colour_with_declared_type_cast():
    text = ('  CRGBColour xStack_c0;\n  xStack_c0 = (CRGBColour)0xffff0000;\n'
            '  iVar1 = GSI->AddQuestInfoBar(1.0,0.0,&xStack_c0,&xStack_c0);\n  xStack_c0 = (CRGBColour)0x0;\n')
    assert 'ENGINE_Colour(255, 0, 0, 255),ENGINE_Colour(255, 0, 0, 255)' in fold_dword_colours(text)
    # a non-colour slot still needs the pointer cast; any other use keeps the store
    text = '  xStack_d8 = (CCharString)0xffffff00;\n  f(&xStack_d8);\n'
    assert fold_dword_colours(text) == text
    text = '  xStack_d8 = (CCharString)0xffffff00;\n  f((CRGBColour *)&xStack_d8);\n  g(xStack_d8);\n'
    assert fold_dword_colours(text) == text
