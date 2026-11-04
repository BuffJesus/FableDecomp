import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_reviewed_arena_audience import ANIMATE_CROWD, recover_arena_audience


@pytest.fixture(scope='module')
def audience(tmp_path_factory):
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}

    def capture(source, fn, rdata):
        if int(fn['address'], 16) == ANIMATE_CROWD:
            seen['input'] = source, fn, rdata
        return recover_arena_audience(source, fn, rdata)

    out = tmp_path_factory.mktemp('audience')
    with patch('tools.script_recovery.native_reviewed_arena_audience.recover_arena_audience', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, out)
    return (*seen['input'], (out / 'FSE/Arena/Arena.lua').read_text())


def test_audience_native_and_source_drift_preserves_unresolved_input(audience):
    source, fn, rdata, _ = audience
    assert 'xStack_18 = LOCALLIST_At(xStack_c, iVar5 / 0xc);' in recover_arena_audience(source, fn, rdata)
    for old, new in [('iVar5 + 4 +', 'iVar5 + 8 +'), ('iVar5 = iVar5 + 0xc;', 'iVar5 = iVar5 + 4;'),
                     ('GetAllThingsWithScriptName', 'GetAllThingsWithDefName')]:
        changed = source.replace(old, new)
        assert recover_arena_audience(changed, fn, rdata) == changed
    moved = copy.deepcopy(fn); moved['address'] = '0x00F1EC74'
    assert recover_arena_audience(source, moved, rdata) == source

    class Drifted:
        def bytes_at(self, at, size):
            raw = bytearray(rdata.bytes_at(at, size)); raw[0] ^= 0xff; return bytes(raw)
    assert recover_arena_audience(source, fn, Drifted()) == source


@pytest.mark.parametrize('count', [0, 1, 2, 7, 16])
@pytest.mark.parametrize('draw', [0, 1, 2, 3, 4, 5, 6, 32767])
def test_audience_actor_order_random_count_and_animation(audience, count, draw):
    _, _, _, body = audience
    lua = LuaRuntime()
    lua.execute(body)
    check = lua.execute('''return function(count, draw)
        local animations = {"CHEER_LOOP_01", "CHEER_LOOP_02", "CLAP_LOOP_02",
                            "SHOUT_LOOP_01", "THUMBS_DOWN_LOOP_01", "WAVE_LOOP_01"}
        local list, cursor, draws = {}, 1, 0
        for i = 1, count do list[i] = {id=i} end
        local q = {}
        function q:GetAllThingsWithScriptName(n) assert(n == "AudienceMember"); return list end
        function q:RetailRandModulo(n) assert(n == 32768); draws = draws + 1; return draw end
        function q:EntityPlayObjectAnimation(t, name, loop)
            assert(t == list[cursor], "wrong actor at " .. cursor)
            assert(name == animations[(draw % 6) + 1] and loop == true)
        end
        function q:EntitySetCutsceneBehaviour(t, mode)
            assert(t == list[cursor] and mode == 2); cursor = cursor + 1
        end
        CUTSCENE_BEHAVIOUR_NOT_PAUSED = 2
        AnimateCrowd(q)
        assert(cursor == count + 1 and draws == count)
    end''')
    check(count, draw)
