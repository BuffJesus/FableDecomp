"""Conservative hidden-result recovery, including GuildMaster's omitted ABI argument."""
import copy
import json
from pathlib import Path

import pytest

from tools.script_recovery.native_string_returns import recover_void_string_returns
from tools.script_recovery.convert_quest_unit import callee_stack_words

ROOT = Path(__file__).resolve().parents[2]


def source(dropped=False, typed=False):
    typ = 'CCharString_bv' if typed else 'CCharString'
    argument = '' if dropped else f',{typ} *result'
    declaration = 'CCharString *in_stack_00000004;' if dropped else ''
    output = 'in_stack_00000004' if dropped else 'result'
    return f"""/* [bsim sim=0.8 <- ego_r]
class CCharString __thiscall Quest::Speech(void) */
{typ} * __thiscall Quest::Speech
    (Quest *this{argument})
{{
  {declaration}
  if (flag) {{
    CCharString::CCharString((CCharString *){output},"",-1);
    return {output};
  }}
  CCharString::CCharString({output},&local_4);
  return {output};
}}
"""


@pytest.mark.parametrize('dropped,typed', [(False,False),(False,True),(True,False)])
def test_hidden_output_becomes_a_value(dropped, typed):
    functions = {'0x1000': {'decompile': source(dropped,typed)}}
    assert recover_void_string_returns(functions, lambda _: 1) == {0x1000}
    text = functions['0x1000']['decompile']
    assert text.count('return hiddenStringResult;') == 2
    assert 'CCharString __thiscall Quest::Speech' in text
    assert 'in_stack_00000004' not in text
    assert 'CCharString::CCharString(&hiddenStringResult,' in text
    before = copy.deepcopy(functions)
    assert not recover_void_string_returns(functions, lambda _: 1)
    assert functions == before


@pytest.mark.parametrize('fault', ['purge','signature','escape','alternate_return','collision','missing_slot'])
def test_incomplete_evidence_is_preserved(fault):
    text = source(True)
    words = 1
    if fault == 'purge': words = 0
    if fault == 'signature': text = text.replace('class CCharString __thiscall','void __thiscall')
    if fault == 'escape': text = text.replace('return in_stack_00000004;', 'Escape(in_stack_00000004); return in_stack_00000004;',1)
    if fault == 'alternate_return': text = text.replace('return in_stack_00000004;', 'return other;',1)
    if fault == 'collision': text = text.replace('if (flag)', 'int hiddenStringResult; if (flag)')
    if fault == 'missing_slot': text = text.replace('CCharString *in_stack_00000004;', '')
    functions = {'0x1000': {'decompile': text}}
    before = copy.deepcopy(functions)
    assert not recover_void_string_returns(functions, lambda _: words)
    assert functions == before


def test_registered_guild_helper_has_one_hidden_output_word():
    tu = json.loads((ROOT / 'refs/script_recovery/guild_master_village/translation_unit_typed.json').read_text())
    helper = next(f for f in tu['functions'] if int(f['address'],16) == 0xe91f20)
    functions = {helper['address']: copy.deepcopy(helper)}
    assert callee_stack_words(0xe91f20) == 1
    assert recover_void_string_returns(functions, callee_stack_words) == {0xe91f20}
    text = functions[helper['address']]['decompile']
    assert text.count('return hiddenStringResult;') == 2


@pytest.fixture(scope='module')
def generated_guild():
    import tempfile
    import subprocess
    import sys
    # Isolate Unicorn constructor emulation from this process's native test state.
    with tempfile.TemporaryDirectory(prefix='guild_return_test_', dir=ROOT / 'work') as directory:
        root = Path(directory)
        subprocess.run([sys.executable, '-m', 'tools.script_recovery.convert_quest_unit',
                        '--unit', 'guild_master_village', '--out', str(root / 'draft')],
                       cwd=ROOT, check=True, capture_output=True, text=True)
        code = ('from pathlib import Path; import sys; '
                'from tools.script_recovery.build_readable_unit import build; '
                'p=Path(sys.argv[1]); '
                'build("guild_master_village", draft=p/"draft", out=p/"readable")')
        subprocess.run([sys.executable, '-c', code, str(root)], cwd=ROOT,
                       check=True, capture_output=True, text=True)
        yield root


@pytest.mark.parametrize('filename', ['native_quest_helpers.lua', 'V_GuildMaster.lua'])
@pytest.mark.parametrize('stage,name', [('draft','helper_E91F20'), ('readable','GetGuildMasterSpeech')])
@pytest.mark.parametrize('phase,key', [(100,'TRAINING'), (150,'WAITING_FOR_WASP_BOSS'),
    (200,'WASP_BOSS'), (300,'MAZE_MEETING'), (400,'WAITING_FOR_ORCHARD_FARM'),
    (450,'ORCHARD_FARM'), (500,'WAITING_FOR_TRADER_ESCORT'), (550,'TRADER_ESCORT'),
    (700,'BANDIT_CAMP'), (900,'ARENA'), (0,None)])
def test_recovered_result_reaches_lua_caller(generated_guild, stage, name, phase, key, filename):
    from lupa.lua54 import LuaRuntime
    lua = LuaRuntime()
    # The cache comparison remains unresolved in the registered export. Keep the
    # optional random information branch disabled; this test proves result transfer,
    # not cache equivalence or random-call counts.
    lua.execute('math.random = function() return 1 end')
    module = lua.execute((generated_guild / stage / 'FSE/V_GuildMaster' / filename).read_text()) or lua.globals()
    quest = lua.table_from({'GetMasterGameState': lambda _self, key: phase,
                           'GetStateString': lambda _self, key: '',
                           'SetStateString': lambda *_: None})
    assert module[name](quest, lua.table()) == ('' if key is None else 'TEXT_QST_081_' + key)


@pytest.mark.parametrize('filename', ['native_quest_helpers.lua', 'V_GuildMaster.lua'])
@pytest.mark.parametrize('stage,name', [('draft','helper_E91F20'), ('readable','GetGuildMasterSpeech')])
@pytest.mark.parametrize('phase', [600,800,1300])
def test_terminating_speech_returns_empty_string(generated_guild, stage, name, phase, filename):
    from lupa.lua54 import LuaRuntime
    lua = LuaRuntime()
    module = lua.execute((generated_guild / stage / 'FSE/V_GuildMaster' / filename).read_text()) or lua.globals()
    quest = lua.table_from({'GetMasterGameState': lambda _self, key: phase if key == 'PostSavePosition' else False,
                           'IsQuestCompleted': lambda *_: False,
                           'IsQuestActive': lambda *_: False,
                           'IsActiveThreadTerminating': lambda *_: True})
    assert module[name](quest, lua.table()) == ''
