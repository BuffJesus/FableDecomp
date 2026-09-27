"""String member aliases preserve entity initialization and name-dependent prices."""
import json
from pathlib import Path
import re
import tempfile

import pytest
from lupa.lua54 import LuaRuntime

from tools.script_recovery.convert_quest_unit import UnitConverter, unwrap_statements, write_reports
from tools.script_recovery.build_readable_unit import build
from tools.script_recovery.native_evidence_lowering import normalise_typed_decompile
from tools.script_recovery.native_string_members import fold_string_member_aliases
from tools.script_recovery.lift_native_lua import RData

ROOT = Path(__file__).resolve().parents[2]
EVIDENCE = ROOT / 'refs/script_recovery/bordello'
MEMBER = r'\(CCharString \*\)\(this \+ 0x28\)'


@pytest.fixture(scope='module')
def native_source():
    tu = json.loads((EVIDENCE / 'translation_unit_typed.json').read_text())
    fn = next(f for f in tu['functions'] if int(f['address'], 16) == 0xe3ac70)
    source = normalise_typed_decompile(unwrap_statements(fn['decompile']))
    return source.replace('&DAT_012e3ae4', '"HEDWIG"')


def fold(source):
    return fold_string_member_aliases(source, MEMBER, 'ENTITYSTATE_GetString("Name")',
                                     lambda value: f'ENTITYSTATE_SetString("Name", {value})')


def test_member_alias_write_and_all_comparisons_are_preserved(native_source):
    result = fold(native_source)
    assert 'ENTITYSTATE_SetString("Name", pOther);' in result
    for name in ('POLLY', 'AMELIA', 'LUCREZIA', 'SOPHIA', 'HEDWIG'):
        assert f'ENGINE_StrCmp(this_00, "{name}")' in result


@pytest.mark.parametrize('extra', ['Escape(this_00);', 'Escape(&this_00);', 'this_00 = other;',
                                 'this_00 = this_00 + 1;', 'Mutate(this);',
                                 'Mutate((void *)this);', 'anotherReceiver = (void *)this;',
                                 'otherAlias = (CCharString *)(this + 0x28);'])
def test_pointer_escape_or_rebinding_preserves_original_evidence(native_source, extra):
    source = native_source.replace('  pOther = ', '  ' + extra + '\n  pOther = ', 1)
    assert fold(source) == source


def test_arbitrary_native_buffer_identity_branch_is_not_a_value_test():
    source = '''{
  CCharString *alias;
  alias = (CCharString *)(this + 0x28);
  if (*(undefined4 **)alias == (undefined4 *)0x0) {
    MutateWorld();
  }
}'''
    assert fold(source) == source


def test_price_stores_and_data_string_source_agree_with_retail_instructions():
    from capstone import Cs, CS_ARCH_X86, CS_MODE_32
    cs = Cs(CS_ARCH_X86, CS_MODE_32)
    code = list(cs.disasm(RData().bytes_at(0xe3ac70, 0x1e2), 0xe3ac70))
    prices = [int(i.op_str.split(', ')[1], 0) for i in code
              if i.mnemonic == 'mov' and i.op_str.startswith('dword ptr [ebp + 0x20], ')]
    assert prices == [0, 50, 100, 200, 1000, 2000]
    instructions = {i.address: (i.mnemonic, i.op_str) for i in code}
    assert instructions[0xe3ac96] == ('lea', 'ebx, [ebp + 0x28]')
    assert instructions[0xe3ac99] == ('call', 'dword ptr [eax + 0xc]')
    assert instructions[0xe3ac9f] == ('call', '0x99efb0')  # CCharString::operator=


@pytest.fixture(scope='module')
def generated():
    with tempfile.TemporaryDirectory(prefix='test_string_member_', dir=ROOT / 'work') as directory:
        root = Path(directory)
        assert root.resolve().is_relative_to((ROOT / 'work').resolve())
        converter = UnitConverter(EVIDENCE / 'translation_unit_typed.json')
        unit = json.loads((EVIDENCE / 'units/V_Bordello.json').read_text())
        report = converter.convert(unit, root / 'draft')
        write_reports([report], root / 'draft', 'String member regression')
        build('bordello', draft=root / 'draft', out=root / 'readable')
        yield root


@pytest.mark.parametrize('name,price', [('POLLY', 50), ('AMELIA', 100), ('LUCREZIA', 200),
                                      ('SOPHIA', 1000), ('HEDWIG', 2000), ('UNKNOWN', 0), ('', 0)])
@pytest.mark.parametrize('nunnery', [False, True])
@pytest.mark.parametrize('stage,helper', [('draft', 'helper_E403D0'), ('readable', 'GetLHTSTag')])
def test_init_keeps_name_price_and_dialogue_in_agreement(generated, stage, helper, name, price, nunnery):
    source = (generated / stage / 'FSE/V_Bordello/Entities/BordelloLady.lua').read_text()
    lua = LuaRuntime()
    lua.execute("package.preload['V_Bordello.native_quest_helpers'] = function() return {} end")
    fields = ('__native_entity_state:GetStateString("Name"), __native_entity_state:GetStateInt("GoldRequired")'
              if stage == 'draft' else 'name, goldRequired')
    inspect = lua.execute(source + '\nreturn function() return ' + fields + ' end')
    reads, information = [], []
    def data(_me):
        reads.append(True)
        return name
    me = lua.table_from({'GetDataString': data})
    quest = lua.table_from({
        'GetStateBool': lambda _q, key: nunnery,
        'SetThingHasInformation': lambda _q, target, value: information.append(value),
        'EntitySetAsKillable': lambda *args: None,
        'EntitySetAsToAddToComboMultiplierWhenHit': lambda *args: None,
        'EntitySetOpinionReactionMask': lambda *args: None,
    })
    lua.globals().Init(quest, me)
    assert reads == [True]
    assert information == ([] if nunnery else [True])
    assert inspect() == (name, price)
    assert lua.globals()[helper](quest, me, 'HAPPY') == 'TEXT_QST_B13_' + name + '_HAPPY'
