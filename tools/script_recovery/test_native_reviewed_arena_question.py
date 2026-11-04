"""Native evidence gates and emitted question argument regression."""
from copy import deepcopy
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery import native_reviewed_arena_question as recovery
from tools.script_recovery.lift_native_lua import RData

ROOT = next(p for p in Path(__file__).resolve().parents if (p / 'tools/script_recovery').is_dir())
FUNCTION = {'address': '0x00F1EED0', 'bodyEndExclusive': '0x00F212FE'}
# Captured pre-recovery caller excerpt. Full-function/native execution receipts:
# work/codex_lua_arena_round_question_w208_20261018/{question,native}_result.json.
SOURCE = ('function PlayWave(quest)\n'
          '    local pQuestionText = GetEndRoundQuestion(quest)\n'
          '    quest:GiveHeroYesNoQuestion("TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES", '
          '"TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO", "", pvVar8, (pQuestionText ~= 0))\nend\n')

class ChangedImage:
    def __init__(self, address, absent=False):
        self.address, self.absent, self.original = address, absent, RData()
    def bytes_at(self, address, size):
        raw = self.original.bytes_at(address, size)
        if address == self.address:
            return None if self.absent else bytes([raw[0] ^ 1]) + raw[1:]
        return raw
    def string_at(self, address):
        return 'STALE' if address == self.address else self.original.string_at(address)

@pytest.mark.parametrize('newline', ['\n', '\r\n'])
def test_exact_caller_and_single_evaluation(newline):
    source = SOURCE.replace('\n', newline)
    result = recovery.recover_arena_question(source, FUNCTION, RData())
    assert result != source
    assert result.count('pQuestionText = GetEndRoundQuestion(quest)') == 1
    assert 'quest:GiveHeroYesNoQuestion(pQuestionText, ' in result
    assert ', "", true)' in result
    assert recovery.recover_arena_question(result, FUNCTION, RData()) == result

@pytest.mark.parametrize('address', [0xf1eed0, 0xf21590])
@pytest.mark.parametrize('absent', [False, True])
def test_stale_native_body_is_rejected(address, absent):
    assert recovery.recover_arena_question(SOURCE, FUNCTION, ChangedImage(address, absent)) == SOURCE

@pytest.mark.parametrize('address', [0x12fa5d8, 0x12fa608, 0x122d70e])
def test_stale_literal_is_rejected(address):
    assert recovery.recover_arena_question(SOURCE, FUNCTION, ChangedImage(address)) == SOURCE

@pytest.mark.parametrize('changed', [
    SOURCE.replace('pvVar8,', 'different,'),
    SOURCE.replace('GetEndRoundQuestion(quest)', 'Different(quest)'),
    SOURCE.replace('(pQuestionText ~= 0))', '(pQuestionText ~= 0)), 7'),
    SOURCE + SOURCE,
], ids=['operand', 'helper', 'trailing-expression', 'duplicate'])
def test_stale_source_is_rejected(changed):
    assert recovery.recover_arena_question(changed, FUNCTION, RData()) == changed

def test_unrelated_function_does_not_read_image():
    fn = deepcopy(FUNCTION)
    fn['address'] = '0x00F21590'
    assert recovery.recover_arena_question(SOURCE, fn, None) == SOURCE

@pytest.mark.parametrize('multiplier', [-2147483648, -1, *range(10), 2147483647])
def test_generated_helper_and_call_preserve_five_native_arguments(multiplier):
    source = (ROOT / 'refs/script_recovery/lifted/Arena/readable/FSE/Arena/Arena.lua').read_text(encoding='utf-8')
    calls = [line.strip() for line in source.splitlines() if 'quest:GiveHeroYesNoQuestion(' in line and 'ARENA_END_ROUND_QUESTION_YES' in line]
    assert len(calls) == 1
    lua = LuaRuntime(unpack_returned_tuples=True)
    lua.execute(source)
    q = lua.execute('''return {
        GetStateInt=function(self,name) assert(name=='GoldMultiplier'); self.reads=(self.reads or 0)+1; return self.multiplier end,
        GiveHeroYesNoQuestion=function(self,...) self.arguments=table.pack(...) end
    }''')
    q['multiplier'] = multiplier
    runner = lua.execute('return function(quest) local pQuestionText = GetEndRoundQuestion(quest)\n' + calls[0] + '\nend')
    runner(q)
    args = q['arguments']
    assert args['n'] == 5 and q['reads'] == 1
    question = 'TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION'
    if 0 <= multiplier < 8:
        question += '_' + str(multiplier + 1)
    assert tuple(args[i] for i in range(1, 6)) == (
        question, 'TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_YES',
        'TEXT_QST_005_V2_ARENA_END_ROUND_QUESTION_NO', '', True)
