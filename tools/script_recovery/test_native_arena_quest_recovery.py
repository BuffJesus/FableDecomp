import copy
import json
from pathlib import Path
from unittest.mock import patch

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.native_evidence_lowering import finish_lua, fold_resource_objects
from tools.script_recovery.native_reviewed_crowd_checker import CROWD_CHECKER, recover_crowd_checker

LABEL = 'NHeroInformationScreens::CBase::CBase__at99a2e0'


def test_vtable_reset_dropped_only_while_its_phantom_result_is_dead():
    text = (f'  xStack_14c = QUESTTHING_Empty();\n  iVar4 = {LABEL}((CBase *)xStack_14c);\n  return;\n'
            f'  iVar4 = {LABEL}((CBase *)xStack_14c);\n  iVar4 = 0;\n')
    out = fold_resource_objects(text, {LABEL: 0x99A2E0})
    assert LABEL not in out
    live = f'  iVar4 = {LABEL}((CBase *)xStack_14c);\n  f(iVar4);\n'
    assert fold_resource_objects(live, {LABEL: 0x99A2E0}) == live
    # another target under the same spelling is untouched
    assert fold_resource_objects(text, {LABEL: 0x99A2E4}) == text


def test_local_array_expansions_run_as_lua():
    body = finish_lua('local t = ENGINE_IntArray(0, 200, 400)\n'
                      'local z = ENGINE_ZeroArray(3)\n'
                      'ENGINE_ArraySet(z, 1, ENGINE_ArrayAt(t, 2))\n'
                      'return ENGINE_ArrayAt(t, 0), ENGINE_ArrayAt(z, 1), ENGINE_ArrayAt(z, 2), #z\n')
    assert LuaRuntime().execute(body) == (0, 400, 0, 3)


@pytest.fixture(scope='module')
def crowd(tmp_path_factory):
    root = Path(__file__).resolve().parents[2]
    unit = json.loads((root / 'refs/script_recovery/arena/units/Q_Arena.json').read_text())
    converter = UnitConverter(root / 'refs/script_recovery/arena/translation_unit_typed.json')
    seen = {}

    def capture(source, fn, rdata):
        if int(fn['address'], 16) == CROWD_CHECKER:
            seen['crowd'] = (source, fn, rdata)
        return recover_crowd_checker(source, fn, rdata)

    with patch('tools.script_recovery.native_reviewed_crowd_checker.recover_crowd_checker', capture), \
            patch.object(converter, 'constructor_defaults', return_value={}):
        converter.convert(unit, tmp_path_factory.mktemp('crowd'))
    return seen['crowd']


def test_crowd_checker_arrays(crowd):
    source, fn, rdata = crowd
    out = recover_crowd_checker(source, fn, rdata)
    assert 'aiStack_c = ENGINE_IntArray(0, 200, 400);' in out
    assert 'ENGINE_ArrayAt(aiStack_c, iVar3) <= ' in out
    assert 'xStack_12c = ENGINE_ZeroArray(LOCALLIST_Count(xStack_150));' in out
    assert 'ENGINE_ArraySet(xStack_12c, uVar9, uVar5);' in out and 'aiStack_c[' not in out


def test_crowd_checker_drift_leaves_source_unchanged(crowd):
    source, fn, rdata = crowd
    moved = copy.deepcopy(fn)
    next(c for c in moved['calls'] if int(c['target'], 16) == 0xA96100)['ecxStack'] = -0x128
    assert recover_crowd_checker(source, moved, rdata) == source
    assert recover_crowd_checker(source.replace('aiStack_c[2] = 400;', 'aiStack_c[2] = 401;'), fn, rdata) \
        == source.replace('aiStack_c[2] = 400;', 'aiStack_c[2] = 401;')

    class Drifted:
        def bytes_at(self, at, size):
            data = bytearray(rdata.bytes_at(at, size)); data[0] ^= 0xff; return bytes(data)
    assert recover_crowd_checker(source, fn, Drifted()) == source
