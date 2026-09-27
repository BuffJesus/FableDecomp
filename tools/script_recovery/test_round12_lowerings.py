"""Round 12 (2026-09-27): the lowerings that took the remaining Lua syntax failures to parse, each on the native
shape that needed it."""
import ast
import json
import struct

from lupa.lua54 import LuaRuntime

from tools.script_recovery.quest_unit_evidence import ROOT
from tools.script_recovery.native_evidence_lowering import (
    LoweringSpec, brace_assigning_single_ifs, finish_lua, index_member_array_walks, lower_cifmod,
    lower_conversation_snapshots, lower_embedded_state_stores, lower_member_resources, name_ftol2_operand,
    nest_assigning_else_if, resolve_this_aliases,
)
from tools.script_recovery.native_condition_tree import has_call_assignment, parse
from tools.script_recovery.convert_quest_unit import restore_stack_operands, unwrap_statements


def quest_spec(**quest):
    unit = {'quest': {'fields': {}, 'unmappedFields': [], **quest}, 'entities': {}}
    return LoweringSpec(unit, 'Q', entity=False, thing_slots={})


def test_this_aliases_and_member_list_registers_become_the_member():
    spec = quest_spec(unmappedFields=[{'offset': '0x48', 'type': 'vector<CScriptThing,std::allocator<CScriptThing>_>', 'name': 'FirePoint'}])
    text = ('{\n  CS *local_14;\n  CS *pCVar11;\n  local_14 = this;\n  pCVar11 = this;\n  pOutThings = this + 0x48;\n'
            '  GSI->Fill(&name,pOutThings);\n  f(pCVar11 + 0x40);\n  pCVar11 = local_14;\n  g(*(int *)pOutThings);\n}\n')
    out = resolve_this_aliases(text, spec)
    assert 'GSI->Fill(&name,(this + 0x48));' in out and 'g(*(int *)(this + 0x48));' in out
    assert 'f(this + 0x40);' in out and 'pCVar11' not in out and 'local_14' not in out


def test_ftol2_duplicate_is_named_before_the_call_that_overwrites_its_input():
    # V_StatueMaster GetStatuePointingPosition 0x00ED4420: `fld st(0); call __ftol2`, then `extraout_ST0 - i`
    text = ('{\n  float10 extraout_ST0;\n  i = GSI->GetTimeOfDay();\n  i = __ftol2(((float10)(i % 100) * k + (float10)(i / 100)) * k2);\n'
            '  do {\n    if (a < (float)(extraout_ST0 - (float10)i)) {\n    }\n  } while (b);\n}\n')
    out = name_ftol2_operand(text)
    assert 'f_st0 = ((float10)(i % 100) * k + (float10)(i / 100)) * k2;\n  i = __ftol2(f_st0);' in out
    assert 'extraout_ST0' not in out and '(f_st0 - (float10)i)' in out


def test_member_float_array_walk_is_indexed_by_its_counter():
    spec = quest_spec(arrays=[{'name': 'AnglesToFaceList', 'element': 'float', 'base': '0x48', 'stride': 4, 'count': 4,
                               'members': {'0x0': ['', 'Float']}, 'things': {}}])
    text = ('  pCVar3 = this + 0x4c;\n  iVar2 = 1;\n  do {\n    if (*(float *)pCVar3 - c < x) {\n      return iVar2;\n    }\n'
            '    iVar2 = iVar2 + 1;\n    pCVar3 = pCVar3 + 4;\n  } while (iVar2 < 4);\n')
    out = index_member_array_walks(text, spec)
    assert '*(float *)(this + 0x48 + iVar2 * 4) - c' in out and 'pCVar3' not in out


def test_cifmod_operands_come_from_the_bytes_before_the_call():
    # V_StatueMaster Main 0x00ED3DC9: fld dword [esp+0x10]; fld qword [0x1237f00]; call _CIfmod
    image = {0x1000 - 10 + i: b for i, b in enumerate(b'\xd9\x44\x24\x10\xdd\x05\x00\x20\x00\x00')}
    image.update({0x2000 + i: b for i, b in enumerate(struct.pack('<d', 1.0))})
    spec = quest_spec()
    spec.byte_at = lambda va: image[va]
    spec.calls = [{'site': '0x00001000', 'target': '0x00BFEB28'}]
    out = lower_cifmod('  f_stk_40 = fVar6;\n  fVar6 = _CIfmod();\n', spec)
    assert 'fVar6 = ENGINE_Fmod(f_stk_40, 1.0);' in out


def test_conversation_snapshot_reads_come_from_the_definitions():
    spec = quest_spec(definitionSnapshots={'0x4c': {'name': 'BookReactions', 'global': '0x4c8', 'element': 'CConversation'}})
    text = ('  std_vector_CConversation_Assign(this + 0x4c,DAT_0143e90c + 0x4c8);\n'
            '  iVar2 = *(int *)(p * 0x5c + 0x28 + *(int *)(this + 0x4c));\n'
            '  iVar6 = r * 0x5c;\n  iVar8 = line * 4;\n'
            '  CCharString::CCharString(&local_14,(CCharString *)(*(int *)(iVar6 + 0x38 + *(int *)(this + 0x4c)) + iVar8));\n')
    out = lower_conversation_snapshots(text, spec)
    assert 'std_vector_CConversation_Assign' not in out
    assert 'iVar2 = ENGINE_ConversationLines(0x4c8, p);' in out
    assert 'local_14 = ENGINE_ConversationString(0x4c8, r, "Dialogue", line);' in out
    lua = finish_lua('local a = ENGINE_ConversationString(0x4c8, r, "Dialogue", line)\n')
    assert 'quest:GlobalConversations(0x4c8)[(r) + 1]["Dialogue"][(line) + 1]' in lua


def test_quest_flag_map_member_reads_are_booleans():
    spec = quest_spec(unmappedFields=[{'offset': '0x60', 'type': 'map<CCharString,bool,std::less<CCharString>>', 'name': 'OakValeFlag'}])
    spec.call_labels = {'std::map<CCharString,C2DVector>::operator[]': 0x8ADF10}
    text = ('  pcVar7 = std::map<CCharString,C2DVector>::operator[]((map<CCharString,C2DVector> *)(this + 0x60),&xStack_28);\n'
            '  cVar1 = *pcVar7;\n  while (cVar1 != \'\\x01\') {\n  }\n')
    out = lower_member_resources(text, spec)
    assert 'cVar1 = FLAGS_Get(FLAGS_Member("OakValeFlag"), xStack_28);' in out
    assert 'while (!cVar1)' in out


def test_comma_sequences_reach_the_condition_tree():
    else_if = '  if (a) {\n    x();\n  }\n  else if ((v = f(), v != 0)) {\n    y();\n  } else {\n    z();\n  }\n  after();\n'
    nested = nest_assigning_else_if(else_if)
    assert '} else {\nif ((v = f(), v != 0)) {' in nested
    assert '    z();\n  }\n}\n  after();' in nested          # the whole rest of the chain inside the nested if
    assert nested.count('{') == nested.count('}')
    single = '  if ((b) || (b = f(this), b)) break;\n'
    assert brace_assigning_single_ifs(single) == '  if ((b) || (b = f(this), b)) {\n    break;\n  }\n'
    store = '(QUESTSTATE_GetBool("BootyDugUp") = 1, !QUESTSTATE_GetBool("W"))'
    assert lower_embedded_state_stores(store) == '(QUESTSTATE_SetBool("BootyDugUp", 1), !QUESTSTATE_GetBool("W"))'
    # a store through any lvalue is a sequence statement, and any sequence needs the tree
    tree = parse('(!t && (*(undefined1 *)(*(int *)(this + 0x14) + 0x4d) = 1, x == 0))')
    assert tree[0] == 'and' and tree[1][1][0] == 'seq'
    assert has_call_assignment('(a = 1, a)')


def test_lua_math_spellings():
    lua = finish_lua('local a = fpatan(dx, dy)\nlocal b = ENGINE_Fmod(a, 1.0)\nlocal c = ENGINE_NotNil(name)\n')
    assert 'math.atan(dx, dy)' in lua and 'math.fmod(a, 1.0)' in lua and '(name ~= nil)' in lua
    runtime = LuaRuntime()
    assert abs(runtime.eval('math.atan(1, 0)') - 1.5707963) < 1e-6      # atan2(y, x), FPATAN's ST1/ST0 order


def test_counter_slot_keeps_its_name_through_operand_restoration():
    # OakValeFire 0x00EE8870: the byte index lives at [esp+0x10] (Ghidra `local_28`, the "fire" string's slot)
    tu = json.loads((ROOT / 'refs/script_recovery/oakvale_revisited/translation_unit_typed.json').read_text(encoding='utf-8'))
    fn = next(f for f in tu['functions'] if f['address'].lower() == '0x00ee8870')
    for key in ('calls', 'indirectCalls'):
        if isinstance(fn.get(key), str):
            fn[key] = ast.literal_eval(fn[key])
    out = restore_stack_operands(unwrap_statements(fn['decompile']), fn)
    assert 'local_28 = (CCharString_bv)0x0;' in out
    assert '(**(code **)(*(int *)(*(int *)pOutThings + (int)local_28) + 0x18))();' in out
    assert 'local_28 = (CCharString_bv)((int)local_28 + 0xc);' in out


def test_x87_return_is_proven_only_for_the_time_of_day_helper():
    from tools.script_recovery.ghidra_typing_spec import proven_float_returns
    assert proven_float_returns(ROOT / 'refs/script_recovery/statue_master') == {'0x00ed43d0': 0}
    assert proven_float_returns(ROOT / 'refs/script_recovery/oakvale_revisited') == {}


def test_c_bool_in_arithmetic_is_an_integer():
    # SummoningTheShip SummonerMinion Init 0x00DF2980: `WaveID = (cVar5 != '\0') + 1`
    from tools.script_recovery.native_evidence_lowering import lower
    lua = finish_lua('waveID = ENGINE_BoolToInt(cVar5) + 1\n')
    runtime = LuaRuntime()
    for value, expected in (('true', 2), ('false', 1), ('1', 2), ('0', 1), ('nil', 1)):
        assert runtime.execute(f'local cVar5 = {value}\n{lua}\nreturn waveID') == expected


def test_unprototyped_unit_helpers_get_their_stack_operands():
    # STS_BriarRose RemoveNeighbours 0x00DF2090: `ret 4`, called with `push 1` -- Ghidra typed it `void (void)`
    from tools.script_recovery.ghidra_typing_spec import stack_purges, unit_functions
    unit_dir = ROOT / 'refs/script_recovery/summoning_the_ship'
    assert stack_purges(unit_dir)['0x00df2090'] == 4
    proto = unit_functions(unit_dir)['0x00df2090']
    assert proto['ret'] == 'void' and len(proto['params']) == 1
