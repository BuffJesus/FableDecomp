import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_evidence_lowering import LoweringSpec, lower, lower_after_annotate
from tools.script_recovery.native_reviewed_arena_wave import PLAY_WAVE, recover_arena_wave

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def wave(tmp_path_factory):
    unit = json.loads((ROOT / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(ROOT / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}
    def capture(source, fn, rdata):
        if int(fn['address'], 16) == PLAY_WAVE:
            seen['input'] = source, fn, rdata
        return recover_arena_wave(source, fn, rdata)
    out = tmp_path_factory.mktemp('wave')
    with patch('tools.script_recovery.native_reviewed_arena_wave.recover_arena_wave', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, out)
    return (*seen['input'], (out / 'FSE/Arena/Arena.lua').read_text())


def test_pending_spawn_pushes_restored(wave):
    source, fn, rdata, lua = wave
    out = recover_arena_wave(source, fn, rdata)
    assert 'uVar9 = CScriptThing::GetPos(LOCALLIST_At(xStack_118, (iVar4) / 0xc));' in out
    assert 'uVar9,1.0,xStack_78,false);' in out
    assert 'xStack_f4 = iVar6;' in source
    assert '1.0, "ArenaEnemy")' in lua
    assert 'CScriptThing::operator=((CScriptThing *)xStack_f4,iVar6)' not in lua


def test_spawn_operand_and_native_drift_remain_unresolved(wave):
    source, fn, rdata, _ = wave
    for old, new in [('0x3f800000,xStack_78,0', '0x40000000,xStack_78,0'),
                     ('xStack_30,creatureType,uVar9', 'xStack_30,other,uVar9')]:
        changed = source.replace(old, new)
        assert recover_arena_wave(changed, fn, rdata) == changed
    moved = copy.deepcopy(fn); moved['address'] = '0x00F1EED4'
    assert recover_arena_wave(source, moved, rdata) == source
    class Drifted:
        def bytes_at(self, at, size):
            raw = bytearray(rdata.bytes_at(at, size)); raw[0] ^= 0xff; return bytes(raw)
    assert recover_arena_wave(source, fn, Drifted()) == source


@pytest.mark.parametrize('arg', ['thing', '(int)thing', '(CScriptThing *)thing'])
def test_plain_local_thing_copy_accepts_a_bare_return_pointer(arg):
    unit = json.loads((ROOT / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    spec = LoweringSpec(unit, 'Q_Arena', entity=False, thing_slots={}); spec.call_labels = {}
    text = ('{\n  thing = GSI->CreateCreatureNearby(hidden,kind,pos);\n'
            '  CScriptThing::operator=((CScriptThing *)xStack_f4,' + arg + ');\n  Use(xStack_f4);\n}')
    out = lower_after_annotate(lower(text, spec)[0])
    assert 'xStack_f4 = thing;' in out
    # An unknown destination type does not become a handle copy.
    assert 'operator=' in lower_after_annotate(lower(text.replace('(CScriptThing *)', '(Unknown *)', 1), spec)[0])


@pytest.mark.parametrize('call', ['UnknownConstructor(hidden)', 'GSI->CreateCreatureNearby(hidden,kind,pos);\n  thing = other'])
def test_bare_copy_of_an_unresolved_or_overwritten_register_stays_unresolved(call):
    unit = json.loads((ROOT / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    spec = LoweringSpec(unit, 'Q_Arena', entity=False, thing_slots={}); spec.call_labels = {}
    text = ('{\n  thing = ' + call + ';\n'
            '  CScriptThing::operator=((CScriptThing *)xStack_f4,thing);\n  Use(xStack_f4);\n}')
    assert 'operator=' in lower_after_annotate(lower(text, spec)[0])
