"""Hidden string results retain the input text and behave as ordinary Lua values."""
import copy
import json
import re
import struct
import tempfile
from pathlib import Path

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter, _text_order_sites, callee_stack_words, write_reports
from tools.script_recovery.native_string_returns import recover_string_returns
from tools.script_recovery.build_readable_unit import build
from tools.script_recovery.lift_native_lua import Lifter, RData, load_manifest

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'refs/script_recovery/bordello'
EXPECTED = ['HAPPY', 'MAGICIAN_HINT', 'PARTY_PAID_INTRODUCTION', 'PARTY_PAID_REMINDER',
            'PARTY_AGAIN', 'PARTY_PAID_QUESTION', 'PARTY_PAID_FOLLOW_ME', 'PARTY_PAID_PLEASED',
            'PARTY_PAID_TOO_EXPENSIVE', 'PARTY_PAID_DECLINED', 'PARTY_FREE', 'PARTY_FREE_QUESTION',
            'PARTY_FREE_FOLLOW_ME', 'PARTY_FREE_FINISHING_UP', 'PARTY_FREE_DECLINED']


@pytest.fixture(scope='module')
def native_dialogue_key():
    """Run the retail helper; mock only the three reviewed string primitives."""
    import unicorn as uc
    from unicorn import x86_const as x86
    vm = uc.Uc(uc.UC_ARCH_X86, uc.UC_MODE_32)
    for address in (0xe40000, 0x99e000, 0x99f000, 0x200000, 0x300000, 0x400000):
        vm.mem_map(address, 4096)
    rdata = RData()
    vm.mem_write(0xe403d0, rdata.bytes_at(0xe403d0, 0x4f))
    for address in (0x99f690, 0x99f600, 0x99f570):
        vm.mem_write(address, b'\xc2\x04\x00')
    vm.mem_write(0x99eae0, b'\xc3')
    strings = {}
    def primitive(machine, address, _size, _data):
        if address not in (0x99f690, 0x99f600, 0x99f570):
            return
        dest = machine.reg_read(x86.UC_X86_REG_ECX)
        left = machine.reg_read(x86.UC_X86_REG_EDX)
        right = struct.unpack('<I', machine.mem_read(machine.reg_read(x86.UC_X86_REG_ESP) + 4, 4))[0]
        left = rdata.string_at(left) if address == 0x99f690 else strings[left]
        right = rdata.string_at(right) if address == 0x99f600 else strings[right]
        strings[dest] = left + right
        machine.reg_write(x86.UC_X86_REG_EAX, dest)
    vm.hook_add(uc.UC_HOOK_CODE, primitive)
    def run(name, suffix):
        strings.clear()
        strings.update({0x200028: name, 0x200100: suffix})
        vm.mem_write(0x300f00, struct.pack('<III', 0x400000, 0x200200, 0x200100))
        vm.reg_write(x86.UC_X86_REG_ESP, 0x300f00)
        vm.reg_write(x86.UC_X86_REG_ECX, 0x200000)
        vm.emu_start(0xe403d0, 0x400000, count=1000)
        assert vm.reg_read(x86.UC_X86_REG_EIP) == 0x400000
        assert vm.reg_read(x86.UC_X86_REG_EAX) == 0x200200
        assert vm.reg_read(x86.UC_X86_REG_ESP) == 0x300f0c
        return strings[0x200200]
    return run


@pytest.fixture(scope='module')
def generated():
    # The unit builder records repository-relative draft paths.
    with tempfile.TemporaryDirectory(prefix='test_string_return_', dir=ROOT / 'work') as directory:
        root = Path(directory)
        assert root.resolve().is_relative_to((ROOT / 'work').resolve())
        converter = UnitConverter(EVIDENCE / 'translation_unit_typed.json')
        unit = json.loads((EVIDENCE / 'units/V_Bordello.json').read_text())
        report = converter.convert(unit, root / 'draft')
        write_reports([report], root / 'draft', 'Hidden string return regression')
        build('bordello', draft=root / 'draft', out=root / 'readable')
        yield root


@pytest.mark.parametrize('stage,helper', [('draft', 'helper_E403D0'), ('readable', 'GetLHTSTag')])
def test_all_fifteen_callers_keep_their_actual_suffix(generated, stage, helper):
    source = (generated / stage / 'FSE/V_Bordello/Entities/BordelloLady.lua').read_text()
    LuaRuntime().compile(source)
    calls = re.findall(r'(?<!function )\b' + helper + r'\(quest, me, "([^"]*)"\)', source)
    assert calls == EXPECTED
    assert 'in_stack_00000008' not in source
    assert 'hidden string destination' not in source


@pytest.mark.parametrize('stage,helper', [('draft', 'helper_E403D0'), ('readable', 'GetLHTSTag')])
@pytest.mark.parametrize('name,suffix', [('HEDWIG', 'HAPPY'), ('', ''), ('LADY_2', 'PARTY_FREE')])
def test_generated_dialogue_helper_builds_the_full_key(generated, stage, helper, name, suffix, native_dialogue_key):
    source = (generated / stage / 'FSE/V_Bordello/Entities/BordelloLady.lua').read_text()
    function = re.search(r'^function ' + helper + r'\(.*?(?=^function |\Z)', source, re.M | re.S)[0]
    lua = LuaRuntime()
    lua.globals().__native_entity_state = lua.table_from({'GetStateString': lambda _self, key: name})
    # The readable stage stores entity fields as file-local upvalues. Supply the
    # native Name member explicitly: Init's member assignment recovery is separate.
    lua.execute('local name = ...\n' + function, name)
    assert lua.globals()[helper](lua.table(), lua.table(), suffix) == 'TEXT_QST_B13_' + name + '_' + suffix
    assert native_dialogue_key(name, suffix) == 'TEXT_QST_B13_' + name + '_' + suffix


@pytest.mark.parametrize('failure', ['ret', 'prototype', 'escape', 'input_write', 'branch', 'name_collision',
                                    'missing_call_order', 'missing_stack'])
def test_incomplete_evidence_does_not_change_callee_or_callers(failure):
    tu = json.loads((EVIDENCE / 'translation_unit_typed.json').read_text())
    functions = {f['address'].lower(): f for f in tu['functions']}
    helper, caller = functions['0x00e403d0'], functions['0x00e3eb10']
    words = callee_stack_words
    if failure == 'ret':
        words = lambda target: 1
    elif failure == 'prototype':
        helper['decompile'] = helper['decompile'].replace('class CCharString __thiscall', 'void __thiscall')
    elif failure == 'escape':
        helper['decompile'] = helper['decompile'].replace('return param_1;', 'Escape(param_1); return param_1;')
    elif failure == 'input_write':
        caller['decompile'] = caller['decompile'].replace('"HAPPY",-1);', '"HAPPY",-1); Escape(&piStack_1c8);')
    elif failure == 'branch':
        caller['decompile'] = caller['decompile'].replace('"HAPPY",-1);', '"HAPPY",-1); if (flag) goto elsewhere;')
    elif failure == 'name_collision':
        helper['decompile'] = helper['decompile'].replace('CCharString_bv *pCVar1;', 'CCharString_bv *pCVar1; int dialogueSuffix;')
    elif failure == 'missing_call_order':
        caller.pop('callOrder', None)
    elif failure == 'missing_stack':
        next(c for c in caller['calls'] if int(c['target'], 16) == 0xe403d0)['pushedStack'] = []
    before = copy.deepcopy(functions)
    assert not recover_string_returns(functions, words, _text_order_sites)
    assert functions == before


@pytest.mark.parametrize('assigned', [False, True])
def test_output_slot_and_returned_pointer_have_the_same_value(assigned):
    lifter = Lifter(load_manifest(), {}, 'quest', False, '', RData())
    lifter.helper_names = {'MakeKey'}
    lifter.helper_parameters['MakeKey'] = ['suffix']
    lifter.hidden_string_helpers.add('MakeKey')
    lifter.slot_results['output'] = 'oldValue'
    prefix = 'result = ' if assigned else ''
    lifter.statement('Main', prefix + 'MakeKey(this, &output, "HAPPY");')
    assert not lifter.todo
    lua = LuaRuntime()
    lua.execute('function MakeKey(quest, suffix) return "KEY_" .. suffix end')
    returned = lifter.expr('&output')
    # Address aliases are resolved by call operand lowering, while bare storage
    # remains available when no pointer result was assigned.
    if assigned:
        assert lifter.slot_results['output'] == 'result'
        returned = 'result'
    else:
        returned = 'output'
    assert lua.execute('\n'.join(lifter.out) + '\nreturn ' + returned) == 'KEY_HAPPY'
