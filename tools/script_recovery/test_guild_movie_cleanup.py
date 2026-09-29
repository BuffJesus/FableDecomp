"""GuildMaster Main movie lifetimes, normal continuation and termination."""
import json
import os
from pathlib import Path
import subprocess
import struct
import sys
import tempfile

import pytest
from lupa.lua54 import LuaRuntime

ROOT = Path(__file__).resolve().parents[2]
FILE = Path('FSE/V_GuildMaster/Entities/GuildMasterGameFlow.lua')


@pytest.fixture(scope='module')
def generated():
    explicit = os.environ.get('GUILD_CLEANUP_TEST_ARTIFACT')
    if explicit:
        yield Path(explicit).resolve()
        return
    with tempfile.TemporaryDirectory(prefix='guild_cleanup_', dir=ROOT / 'work') as directory:
        p = Path(directory)
        subprocess.run([sys.executable, '-m', 'tools.script_recovery.convert_quest_unit',
                        '--unit', 'guild_master_village', '--out', str(p / 'draft')],
                       cwd=ROOT, check=True, capture_output=True)
        code = 'from pathlib import Path; import sys; from tools.script_recovery.build_readable_unit import build; p=Path(sys.argv[1]); build("guild_master_village",draft=p/"draft",out=p/"readable")'
        subprocess.run([sys.executable, '-c', code, str(p)], cwd=ROOT, check=True, capture_output=True)
        yield p


def run_main(path, config):
    lua = LuaRuntime()
    events, movies = [], []
    state = dict(frames=0, checks=0, terminating=False, busy=0, hit=False, talk=False, answers=0,
                 post_termination=[])
    def record(*event):
        events.append(event)
        if state['terminating']:
            state['post_termination'].append(event)
    def frame(*_):
        record('frame')
        state['frames'] += 1
        state['busy'] = max(0, state['busy'] - 1)
        if state['frames'] >= config.get('stop_frame', 4):
            state['terminating'] = True
        return not state['terminating']
    def terminating(*_):
        state['checks'] += 1
        if state['checks'] == config.get('stop_check'):
            state['terminating'] = True
        return state['terminating']
    def hit(*_):
        value = config.get('hit', False) and not state['hit']
        state['hit'] = True
        return value
    def talk(*_):
        value = config.get('talk', False) and not state['talk']
        state['talk'] = True
        record('talk_check', value)
        return value
    def speak(_self, target, key, *args):
        record('speak', key)
        state['busy'] = config.get('busy', 0)
        # The FSE blocking Speak returns false when its wait is terminated.
        while state['busy'] and not state['terminating']:
            frame()
        state['busy'] = 0
        return not state['terminating']
    def start_movie(*_):
        handle = f'movie{len(movies) + 1}'
        movies.append(handle)
        record('start', handle)
        return handle
    def answer(*_):
        state['answers'] += 1
        return -1 if state['answers'] <= config.get('wait_answers', 0) else config.get('answer', 0)
    hero = lua.table()
    me = lua.table_from({'MsgIsHitByHero': hit, 'MsgIsHitByAnySpecialAbilityFromHero': lambda *_: False,
        'MsgIsHitByHeroSpecialAbility': lambda *_: False, 'IsTalkedToByHero': talk,
        'Speak': speak, 'IsPerformingScriptTask': lambda *_: state['busy'] > 0})
    resources = lua.table_from({'NewResource': lambda *_: 'resource',
        'PrepareResource': lambda *_: record('prepare'), 'TryAcquire': lambda *_: True,
        'ScriptThing': lambda *_: me, 'StartMovie': start_movie,
        'DestroyMovie': lambda _, handle: record('destroy', handle),
        'ReleaseResource': lambda _, handle: record('release', handle)})
    quest = lua.table_from({'GetHero': lambda *_: hero, 'RetailResources': lambda *_: resources,
        'NewScriptFrame': frame, 'IsActiveThreadTerminating': terminating,
        'IsQuestActive': lambda *_: False, 'GetMasterGameState': lambda *_: config.get('phase', 100),
        'PauseAllNonScriptedEntities': lambda _, value: record('pause', value),
        'GetHealth': lambda *_: config.get('health', 100),
        'GiveHeroYesNoQuestion': lambda *_: record('question'), 'MsgIsQuestionAnsweredYesOrNo': answer,
        'GiveHeroQuestCardDirectly': lambda _, *args: record('card', *args),
        'TakeObjectFromHero': lambda _, value: record('take', value)})
    lua.execute('package.preload["V_GuildMaster.native_quest_helpers"] = function() return {helper_E91F20=function() return "GENERIC" end, GetGuildMasterSpeech=function() return "GENERIC" end} end')
    lua.execute('debug.sethook(function() error("Main instruction budget exceeded") end, "", 100000)')
    lua.execute(path.read_text())
    lua.globals().Main(quest, me)
    return events, state


CASES = [dict(hit=True), dict(hit=True, health=0), dict(talk=True), dict(talk=True, health=0),
         *[dict(talk=True, phase=1450, answer=answer, health=health)
           for answer in (0, 1) for health in (0, 100)], dict(hit=True, talk=True),
         dict(hit=True, busy=2), dict(talk=True, busy=2),
         dict(talk=True, phase=1450, wait_answers=2, answer=1)]


@pytest.mark.parametrize('config', CASES)
@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('stop_check', [None, *range(1, 22)])
def test_every_started_movie_unpauses_and_destroys_once_before_resource_release(generated, stage, config, stop_check):
    events, state = run_main(generated / stage / FILE, dict(config, stop_check=stop_check))
    starts = [e[1] for e in events if e[0] == 'start']
    assert [e[1] for e in events if e[0] == 'destroy'] == starts
    assert [e[1] for e in events if e[0] == 'pause'] == [value for _ in starts for value in (True, False)]
    assert events.count(('release', 'resource')) == int(('prepare',) in events)
    for i, event in enumerate(events):
        if event[0] == 'destroy':
            assert events[i - 1] == ('pause', False)
    if ('prepare',) in events:
        assert events[-1] == ('release', 'resource')
    assert state['frames'] <= 4
    assert all(e[0] in ('pause', 'destroy', 'release') for e in state['post_termination'])


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('health', [0, 100])
def test_attack_cleanup_continues_into_talk_before_next_frame(generated, stage, health):
    events, _ = run_main(generated / stage / FILE, dict(hit=True, talk=True, health=health, stop_frame=2))
    first = events.index(('destroy', 'movie1'))
    assert events[first + 1:first + 3] == [('talk_check', True), ('start', 'movie2')]
    assert events[-3:] == [('destroy', 'movie2'), ('frame',), ('release', 'resource')]


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('answer', [0, 1])
def test_question_answer_controls_card_and_continues(generated, stage, answer):
    events, _ = run_main(generated / stage / FILE, dict(talk=True, phase=1450, answer=answer, stop_frame=2))
    assert ('question',) in events
    assert any(e[0] == 'card' for e in events) == bool(answer)
    assert (('take', 'OBJECT_MAZE_JOURNAL') in events) == bool(answer)
    assert events[-3:] == [('destroy', 'movie1'), ('frame',), ('release', 'resource')]


@pytest.mark.parametrize('stage', ['draft', 'readable'])
@pytest.mark.parametrize('config', [dict(hit=True, busy=3), dict(talk=True, busy=3),
                                  dict(talk=True, phase=1450, wait_answers=3)])
def test_termination_during_wait_cleans_up_and_does_not_resume(generated, stage, config):
    events, _ = run_main(generated / stage / FILE, dict(config, stop_frame=2))
    assert events[-4:] == [('frame',), ('pause', False), ('destroy', 'movie1'), ('release', 'resource')]
    assert not any(e[0] == 'card' for e in events)


def native_cleanup_paths():
    """Execute retail epilogues; mock only the three external cleanup calls.

    The endpoints are the next normal operation or a sentinel return address.
    Separate EDI/EBP receivers reflect the retail call sites.
    """
    import unicorn as uc
    from unicorn import x86_const as x
    from tools.script_recovery.lift_native_lua import RData
    image = RData()
    results = []
    for entry, stop, attack in [(0xe91898, 0xe918af, True), (0xe91db4, 0xe91dcc, False),
                                (0xe91ccb, 0x400000, False), (0xe91ee8, 0x400000, False),
                                (0xe91ef9, 0x400000, True)]:
        vm = uc.Uc(uc.UC_ARCH_X86, uc.UC_MODE_32)
        for base, size in [(0xe91000, 0x2000), (0x200000, 0x3000), (0x300000, 0x4000),
                           (0x400000, 0x1000), (0x600000, 0x1000),
                           (0x6e7000, 0x1000), (0x7e7000, 0x1000)]:
            vm.mem_map(base, size)
        vm.mem_write(0xe91000, image.bytes_at(0xe91000, 0x2000))
        def put(address, value):
            vm.mem_write(address, struct.pack('<I', value))
        put(0x200000, 0x201000)
        put(0x2015ec, 0x600100)
        put(0x3031c4, 0x400000)
        vm.mem_write(0x600100, b'\xc2\x04\x00')
        for address in (0x6e7b80, 0x7e74d0):
            vm.mem_write(address, b'\xc3')
        events = []
        def hook(machine, address, size, data):
            receiver = machine.reg_read(x.UC_X86_REG_ECX)
            if address == 0x600100:
                esp = machine.reg_read(x.UC_X86_REG_ESP)
                value = struct.unpack('<I', machine.mem_read(esp + 4, 4))[0]
                events.append(['pause', receiver, value])
            elif address == 0x6e7b80:
                events.append(['destroy', receiver])
            elif address == 0x7e74d0:
                events.append(['release', receiver])
        vm.hook_add(uc.UC_HOOK_CODE, hook)
        vm.reg_write(x.UC_X86_REG_ESP, 0x303000)
        vm.reg_write(x.UC_X86_REG_EDI, 0x200000)
        vm.reg_write(x.UC_X86_REG_EBP, 0 if attack else 0x200000)
        vm.reg_write(x.UC_X86_REG_ECX, 0x30310c)
        vm.emu_start(entry, stop, count=100)
        assert vm.reg_read(x.UC_X86_REG_EIP) == stop
        assert vm.reg_read(x.UC_X86_REG_ESP) == (0x3031c8 if stop == 0x400000 else 0x303000)
        results.append(dict(entry=entry, stop=stop, events=events))
    return results


def test_retail_cleanup_distinguishes_continue_from_return():
    # Keep native execution out of pytest's Windows fault handler.
    result = subprocess.run([sys.executable, '-m', 'tools.script_recovery.test_guild_movie_cleanup', '--native-worker'],
                            cwd=ROOT, check=True, capture_output=True, text=True, timeout=30)
    paths = json.loads(result.stdout)
    for path in paths:
        attack = path['entry'] in (0xe91898, 0xe91ef9)
        expected = [] if path['entry'] == 0xe91ef9 else [['pause', 0x200000, 0]]
        expected += [['destroy', 0x30310c if attack else 0x30302c]]
        if path['stop'] == 0x400000:
            expected += [['release', 0x303014]]
        assert path['events'] == expected


if __name__ == '__main__' and '--native-worker' in sys.argv:
    print(json.dumps(native_cleanup_paths()))
