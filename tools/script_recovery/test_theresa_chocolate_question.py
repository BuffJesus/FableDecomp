"""Original question CString order, signed answer polling and cancellation."""
import itertools
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime
from tools.script_recovery.lift_native_lua import RData
from tools.script_recovery.native_theresa_control import verify
from tools.script_recovery.test_native_affair_wife_hit_scopes import Uc, UC_ARCH_X86, UC_MODE_32, UC_HOOK_CODE
from unicorn.x86_const import UC_X86_REG_ESP, UC_X86_REG_EIP, UC_X86_REG_ECX, UC_X86_REG_EAX, UC_X86_REG_EBP

KEYS = ('', 'TEXT_OBJECT_HERO_ANSWER_NO', 'TEXT_OBJECT_HERO_ANSWER_YES', 'TEXT_QST_048_GIVE_CHOCOLATE_BOX')


class Scenario:
    def __init__(self, answers, cancel):
        self.answers, self.cancel = answers, cancel
        self.events, self.terms, self.polls = [], 0, 0
    def term(self):
        self.terms += 1
        result = self.terms == self.cancel
        self.events.append(('term', result))
        return result
    def answer(self):
        result = self.answers[min(self.polls, len(self.answers)-1)]
        self.polls += 1
        self.events.append(('answer', result))
        return result


def native(data, scenario, offer=False):
    uc = Uc(UC_ARCH_X86, UC_MODE_32)
    targets = (0x99ebf0, 0x99eae0, 0xf35b30, 0x203000, 0x203010, 0x203020)
    for address, size in ((0xdb9000, 0x3000), (0x99e000, 0x1000), (0xf35000, 0x1000),
                          (0x100000, 0x10000), (0x200000, 0x4000)):
        uc.mem_map(address, size)
    uc.mem_write(0xdb97a0, data.bytes_at(0xdb97a0,7013))
    for target in targets: uc.mem_write(target, b'\xc3')
    def put(a, v): uc.mem_write(a, v.to_bytes(4, 'little'))
    def get(a): return int.from_bytes(uc.mem_read(a, 4), 'little')
    stack, thread, game, table = 0x108000, 0x200000, 0x201000, 0x202000
    put(thread+4, game); put(game, table)
    for slot, target in ((0x1c8, 0x203000), (0x9c, 0x203010), (0x1c, 0x203020)): put(table+slot, target)
    strings, finished = {}, []
    exits = {0xdb9c87: True, 0xdb9e86: False, 0xdbaedf: None, 0xdbaefa: None, 0xdbaeed: None}
    if offer: exits = {0xdba10f:True,0xdba31a:False,0xdbaf2e:None,0xdbaf53:None}
    def hook(machine, address, size, user):
        if address in exits: finished.append(exits[address]); machine.emu_stop(); return
        if address not in targets: return
        esp, receiver = machine.reg_read(UC_X86_REG_ESP), machine.reg_read(UC_X86_REG_ECX)
        count, result = 0, 0xabcd0000
        if address == 0xf35b30:
            assert receiver == thread
            result |= int(scenario.term())
        elif address == 0x99ebf0:
            assert get(esp+8) == 0xffffffff
            key = data.bytes_at(get(esp+4), 80).split(b'\0')[0].decode()
            assert receiver not in strings
            strings[receiver] = key; scenario.events.append(('construct', key)); count = 2
        elif address == 0x99eae0:
            scenario.events.append(('destroy', strings.pop(receiver)))
        elif address == 0x203000:
            assert receiver == game
            assert [strings[get(esp+4+4*i)] for i in range(4)] == [KEYS[3], KEYS[2], KEYS[1], KEYS[0]]
            assert get(esp+20) == 1
            scenario.events.append(('question',)); count = 5
        elif address == 0x203010:
            assert receiver == game
            result = scenario.answer() & 0xffffffff
        else:
            assert receiver == game
            scenario.events.append(('frame',))
        machine.reg_write(UC_X86_REG_EAX, result)
        machine.reg_write(UC_X86_REG_ESP, esp+4+4*count)
        machine.reg_write(UC_X86_REG_EIP, get(esp))
    uc.reg_write(UC_X86_REG_ESP, stack); uc.reg_write(UC_X86_REG_EBP, thread)
    uc.hook_add(UC_HOOK_CODE, hook); uc.emu_start(0xdba004 if offer else 0xdb9b7f, 0xdba110 if offer else 0xdb9c88, count=2000)
    assert len(finished) == 1 and not strings and uc.reg_read(UC_X86_REG_ESP) == stack
    return finished[0]


def lua_phase(scenario, offer=False):
    lua = LuaRuntime()
    phase = lua.execute(Path(__file__).with_name('theresa_offer_choice.lua' if offer else 'theresa_chocolate_question.lua').read_text())
    quest, resources = lua.table(), lua.table()
    quest.IsActiveThreadTerminating = lambda self: scenario.term()
    quest.MsgIsQuestionAnsweredYesOrNo = lambda self: scenario.answer()
    quest.NewScriptFrame = lambda self, me: scenario.events.append(('frame',))
    def show(self):
        scenario.events.extend(('construct', key) for key in KEYS)
        scenario.events.append(('question',))
        scenario.events.extend(('destroy', key) for key in reversed(KEYS))
    resources.ShowTheresaChocolateQuestion = show
    return phase(quest, 17, resources)


class TheresaQuestionTests(unittest.TestCase):
    def test_later_offer_has_two_post_answer_cancellation_queries(self):
        data=RData();verify(data)
        for pending,answer,cancel in itertools.product((0,1,3),(0,1,2,0x7fffffff),range(1,9)):
            answers=(-0x80000000,)*pending+(answer,)
            with self.subTest(pending=pending,answer=answer,cancel=cancel):
                expected,actual=Scenario(answers,cancel),Scenario(answers,cancel)
                self.assertEqual(lua_phase(actual,True),native(data,expected,True))
                self.assertEqual(actual.events,expected.events)

    def test_native_signed_answers_and_all_cancellation_positions(self):
        data = RData(); verify(data)
        for pending, answer, cancel in itertools.product((0, 1, 3), (0, 1, 2, 0x7fffffff), range(1, 9)):
            answers = (-0x80000000,)*pending + (answer,)
            with self.subTest(pending=pending, answer=answer, cancel=cancel):
                expected, actual = Scenario(answers, cancel), Scenario(answers, cancel)
                self.assertEqual(lua_phase(actual), native(data, expected))
                self.assertEqual(actual.events, expected.events)
