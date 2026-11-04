import copy

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.lift_native_lua import Lifter, converter_signatures


def manifest():
    return {'CreateCreature': {'scope': 'Quest', 'returnType': 'std::shared_ptr<CScriptThing>',
                              'parameters': [{'name': name, 'type': kind} for name, kind in (
                                  ('defName', 'const std::string&'), ('position', 'sol::table'),
                                  ('scriptName', 'const std::string&'))]}}


def test_signature_overlay_is_immutable_and_idempotent():
    source = manifest()
    before = copy.deepcopy(source)
    updated = converter_signatures(source)
    assert source == before
    assert updated['CreateCreature']['parameters'][-1] == {
        'name': 'nativeFlag', 'type': 'sol::optional<bool>', 'optional': True}
    assert converter_signatures(updated) == updated


@pytest.mark.parametrize('changed', ['scope', 'return', 'name', 'type', 'newer', 'absent'])
def test_other_signature_shapes_remain_untouched(changed):
    source = manifest()
    row = source['CreateCreature']
    if changed == 'scope':
        row['scope'] = 'Entity'
    elif changed == 'return':
        row['returnType'] = 'void'
    elif changed == 'name':
        row['parameters'][2]['name'] = 'customName'
    elif changed == 'type':
        row['parameters'][1]['type'] = 'C3DVector*'
    elif changed == 'newer':
        row['parameters'].append({'name': 'customFlag', 'type': 'bool'})
    else:
        source = {}
    updated = converter_signatures(source).get('CreateCreature')
    original = source.get('CreateCreature')
    if original is None:
        assert updated is None
    else:
        # Existing normalization annotates vector parameters independently.
        # The flag overlay must preserve this other argument contract.
        assert updated['scope'] == original['scope']
        assert updated['returnType'] == original['returnType']
        assert [(p['name'], p['type']) for p in updated['parameters']] == [
            (p['name'], p['type']) for p in original['parameters']]


@pytest.mark.parametrize('operand,expected', [('true', True), ('false', False), ('1', True), ('0', False), ('', False)])
def test_explicit_native_flag_survives_and_omission_keeps_default(operand, expected):
    lifter = Lifter(converter_signatures(manifest()), {}, 'quest', False, '', None)
    source = '{\nGSI->CreateCreature("CREATURE_BALVERINE_EASY",native_arg_position,"SurpriseBalverine"'
    source += (',' + operand if operand else '') + ');\n}'
    body = '\n'.join(lifter.lift('Spawn', source, parameters={'native_arg_position': 'vector'}))
    assert lifter.todo == []
    lua = LuaRuntime()
    calls = []
    def create(quest, definition, position, script, flag=False):
        calls.append((definition, position['x'], position['y'], position['z'], script, flag))
    quest = lua.table_from({'CreateCreature': create})
    position = lua.table_from({'x': 12.5, 'y': -7.25, 'z': 3.125})
    lua.execute('return function(quest,me,native_arg_position)\n' + body + '\nend')(quest, lua.table(), position)
    assert calls == [('CREATURE_BALVERINE_EASY', 12.5, -7.25, 3.125, 'SurpriseBalverine', expected)]
