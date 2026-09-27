"""Reviewed Arena stack lifetimes become separate Lua counter tables."""
import json
import re

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.quest_unit_evidence import ROOT
from tools.script_recovery.native_arena_counter_arrays import recover_counter_arrays
from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile, finish_lua
from tools.script_recovery.convert_quest_unit import unwrap_statements, UnitConverter
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.build_readable_unit import readable_file


@pytest.fixture(scope='module')
def native():
    tu = json.loads((ROOT/'refs/script_recovery/arena/translation_unit_typed.json').read_text())
    source = next(f['decompile'] for f in tu['functions'] if int(f['address'], 16) == 0xf1eed0)
    return normalise_typed_decompile(unwrap_statements(source))


@pytest.mark.parametrize('normalized_names', [False, True])
def test_only_the_counter_lifetime_changes(native, normalized_names):
    if normalized_names:
        native = re.sub(r'\bauStack_(e4|f4)\b', r'xStack_\1', native)
    start = native.index('  pCVar15 = this + 0xd0;')
    end = native.index('LAB_00f20ce5:')
    out = recover_counter_arrays(native)
    assert out != native
    assert out[:start] == native[:start]
    assert out[out.index('LAB_00f20ce5:'):] == native[end:]
    scope = out[start:out.index('LAB_00f20ce5:')]
    assert not re.search(r'\b(?:au|x)Stack_(?:e4|f4)\b', scope)
    assert 'ENGINE_SetListWord(creatureCounterIds, iVar4 / 4, -1);' in scope
    assert 'ENGINE_ListWord(creatureCounterIds, 0)' in scope


@pytest.mark.parametrize('extra', ['Escape(auStack_e4);', 'Escape(&auStack_f4);',
    'auStack_e4._4_4_ = 3;', 'fStack_f0 = 2.0;', 'xStack_dc = 5;',
    'creatureCounterIds = other;', 'replacementCounterId = 3;'])
def test_unproven_alias_or_name_collision_declines_all_changes(native, extra):
    source = native.replace('  pCVar15 = this + 0xd0;', '  pCVar15 = this + 0xd0;\n  '+extra, 1)
    assert recover_counter_arrays(source) == source


@pytest.mark.parametrize('old,new', [('iVar6 < 0xc', 'iVar6 < 0x10'),
    ('0xb < iVar4', '0xf < iVar4'), ('(int)auStack_f4);', '&auStack_f4);'),
    ('LAB_00f20ce5:', 'different_label:')])
def test_missing_extent_or_lifetime_evidence_declines_recovery(native, old, new):
    source = native.replace(old, new)
    assert source != native
    assert recover_counter_arrays(source) == source


def test_retail_stack_loads_prove_element_width_extent_and_first_id():
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    from tools.script_recovery.lift_native_lua import RData
    code = {i.address: (i.mnemonic, i.op_str) for i in
            Cs(CS_ARCH_X86, CS_MODE_32).disasm(RData().bytes_at(0xf1eed0, 0x1800), 0xf1eed0)}
    assert code[0xf20593] == ('mov', 'eax, dword ptr [esp + ebx + 0x88]')
    assert code[0xf205ba] == ('mov', 'eax, dword ptr [esp + ebx + 0x80]')  # after two pushes: base +78
    assert code[0xf205c8] == ('add', 'ebx, 4')
    assert code[0xf205ce] == ('cmp', 'ebx, 0xc')
    assert code[0xf205fe] == ('mov', 'eax, dword ptr [esp + 0x78]')
    assert code[0xf20604] == ('push', 'eax')
    assert code[0xf20605] == ('call', 'dword ptr [edx + 0x548]')


@pytest.mark.parametrize('readable', [False, True])
def test_counter_table_operations_preserve_three_ids_and_replace_only_first(readable):
    source = '''{
initialCreatureCounts = ENGINE_EmptyList();
creatureCounterIds = ENGINE_EmptyList();
iVar4 = 0;
while (iVar4 < 12) {
ENGINE_SetListWord(initialCreatureCounts, iVar4 / 4, iVar4 + 2);
ENGINE_SetListWord(creatureCounterIds, iVar4 / 4, iVar4 + 100);
iVar4 = iVar4 + 4;
}
ENGINE_SetListWord(initialCreatureCounts, 0, ENGINE_ListWord(initialCreatureCounts, 0) + 5);
ENGINE_SetListWord(creatureCounterIds, 0, 900);
return ENGINE_ListWord(initialCreatureCounts, bucket) * 1000 + ENGINE_ListWord(creatureCounterIds, bucket);
}'''
    lifter = make()
    lifter.accessor_kinds = True
    body = finish_lua('\n'.join(lifter.lift('Counters', source, parameters={'bucket': 'number'})))
    assert not lifter.todo
    lua_source = 'function Counters(quest,bucket)\n' + body + '\nend'
    if readable:
        lua_source = readable_file(lua_source)[0]
    lua = LuaRuntime()
    lua.execute(lua_source)
    lua.execute('debug.sethook(function() error("instruction budget exceeded") end,"",10000)')
    assert [lua.globals().Counters(lua.table(), i) for i in range(3)] == [7900, 6104, 10108]


def test_complete_converter_keeps_counter_tables_and_other_slot_lifetimes(tmp_path):
    evidence = ROOT/'refs/script_recovery/arena'
    unit = json.loads((evidence/'units/Q_Arena.json').read_text())
    UnitConverter(evidence/'translation_unit_typed.json').convert(unit, tmp_path)
    source = (tmp_path/'FSE/Arena/Arena.lua').read_text()
    assert 'initialCreatureCounts = {}' in source
    assert 'creatureCounterIds = {}' in source
    assert 'quest:RemoveQuestInfoElement(creatureCounterIds[(0) + 1])' in source
    assert not re.search(r'if 0 < \*\(xStack_e4', source)
    # Wave-vector operands now use the definition snapshot. This still is not
    # a complete PlayWave behavioral recovery.
    assert 'replacementCounterId = quest:AddQuestInfoCounterList(' in source
    start = source.index('function PlayWave(')
    end = source.find('\nfunction ', start + 1)
    LuaRuntime().compile(source[start:end if end != -1 else len(source)])
