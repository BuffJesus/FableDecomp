"""Discarding a native operator+ pointer must not discard its result string."""
import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.native_evidence_lowering import fold_engine_helpers, finish_lua
from tools.script_recovery.lift_native_lua import Lifter, load_manifest, RData


@pytest.mark.parametrize('address', [0x99F570, 0x99F600, 0x99F690])
def test_ignored_result_pointer_still_constructs_destination(address):
    source = '''{
    local_68 = 0;
    CCharString_OperatorPlus_API(&local_68,"TEXT_QST_B17_SPECTATOR_","3");
    return local_68;
}'''
    lowered = fold_engine_helpers(source, {'CCharString_OperatorPlus_API': address})
    assert 'local_68 = ENGINE_Concat(' in lowered
    lifter = Lifter(load_manifest(), {}, 'quest', False, '', RData())
    body = finish_lua('\n'.join(lifter.lift('Main', lowered)))
    assert LuaRuntime().execute('return function()\n' + body + '\nend')() == 'TEXT_QST_B17_SPECTATOR_3'


def test_assigned_pointer_result_keeps_its_existing_value_path():
    source = '    result = Concat(&slot, left, right);'
    assert fold_engine_helpers(source, {'Concat': 0x99F690}) == '    result = ENGINE_Concat(left, right);'


def test_member_destination_is_not_assumed_to_be_a_lua_local():
    source = '    Concat((CCharString *)(this + 0x14), left, right);'
    assert fold_engine_helpers(source, {'Concat': 0x99F690}) == source


def test_new_string_does_not_overwrite_the_slots_previous_result():
    lifter = Lifter(load_manifest(), {}, 'quest', False, '', RData())
    lifter.slot_results['xStack_58'] = 'previousResult'
    lifter.statement('Main', 'xStack_58 = ENGINE_Concat("book_", "3");')
    assert 'xStack_58' not in lifter.slot_results
    assert 'previousResult =' not in '\n'.join(lifter.out)
    assert 'xStack_58 =' in '\n'.join(lifter.out)


@pytest.mark.parametrize('declaration', ['CCharString xStack_310;', 'CCharString xStack_310 [2];'])
def test_recovered_stack_destination_uses_its_string_declaration(declaration):
    source = '''{
    CCharString xStack_310;
    xStack_310 = 0;
    Concat(xStack_310,"TEXT_QST_B13_MAGICMAN_CHATTER_0","3");
    return xStack_310;
}'''
    source = source.replace('CCharString xStack_310;', declaration)
    lowered = fold_engine_helpers(source, {'Concat': 0x99F690})
    assert 'xStack_310 = ENGINE_Concat(' in lowered
    lifter = Lifter(load_manifest(), {}, 'quest', False, '', RData())
    body = finish_lua('\n'.join(lifter.lift('Main', lowered)))
    assert LuaRuntime().execute('return function()\n'+body+'\nend')() == 'TEXT_QST_B13_MAGICMAN_CHATTER_03'


def test_pointer_parameter_is_not_mistaken_for_local_string_storage():
    source = '    CCharString *result;\n    Concat(result, left, right);'
    assert 'result = ENGINE_Concat(' not in fold_engine_helpers(source, {'Concat': 0x99F690})


@pytest.mark.parametrize('prefix', ['result =\n', 'return\n'])
@pytest.mark.parametrize('address', ['slot', '&slot'])
def test_multiline_expression_does_not_gain_a_second_assignment(prefix, address):
    source = 'CCharString slot;\n' + prefix + f'    Concat({address}, left, right);'
    out = fold_engine_helpers(source, {'Concat': 0x99F690})
    assert 'slot = ENGINE_Concat(' not in out
    assert prefix + '    ENGINE_Concat(left, right);' in out
