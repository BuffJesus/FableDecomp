import re
import struct
import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_new_oakvale import convert, ENTITY_STATE, ROOT
from tools.script_recovery.test_lift_native_lua import make


class TeddyCallbackTests(unittest.TestCase):
    def test_master_string_requires_reviewed_offset_and_entity_receiver(self):
        native = '{\nCCharString::operator=((CCharString *)(*(int *)(this + 0x18) + 0x54),"B");\n}'
        for entity, mapping, source, accepted in (
            (True, {'0x54': 'TeddySolution'}, native, True),
            (True, {}, native, False), (False, {'0x54': 'TeddySolution'}, native, False),
            (True, {'0x54': 'TeddySolution'}, native.replace('0x18', '0x14'), False),
        ):
            lifter = make(entity=entity)
            lifter.master_strings = mapping
            body = '\n'.join(lifter.lift('GivenTeddy', source))
            self.assertEqual(':SetMasterGameState(' in body, accepted)
            self.assertEqual(bool(lifter.todo), not accepted)

    def test_only_immediately_overwritten_receiver_slot_is_removed(self):
        source = '{\nlocal_4 = this;\nCCharString::CCharString((CCharString *)&local_4,"TEDDY",-1);\n}'
        lifter = make(entity=True)
        lifter.lift('GivenTeddy', source)
        self.assertEqual(lifter.todo, [])
        for changed in (source.replace('local_4,"', 'local_8,"'),
                        source.replace('this;\n', 'this;\nuse(local_4);\n')):
            lifter.lift('GivenTeddy', changed)
            self.assertTrue(lifter.todo)

    def test_actual_teddy_callbacks_and_generated_deed_dependencies_match_port(self):
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory)
            report = convert(out)
            helpers = (out / report['sharedHelpers']['path']).read_text()
            callbacks = {}
            for owner in ('NOVI_Bully', 'NOVI_TeddyGirl'):
                row = next(r for r in report['functions'] if r['owner'] == owner and r['function'] == 'GivenTeddy')
                self.assertEqual(row['todo'], [])
                text = (out / row['path']).read_text()
                callbacks[owner] = ENTITY_STATE + text[text.index('function GivenTeddy('):]
        port = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'

        def run(owner, native, repeat, delay, cancel):
            lua, events, fields = LuaRuntime(), [], {'GoodDeedsPerformed': repeat, 'BadDeedsPerformed': repeat,
                'GivenSweets': False, 'GUIGoodDeedCounter': 17, 'SpokeAboutFindingTeddy': False, 'TeddyRuined': False}
            fields.update({f'WhichBadDeedsPerformed_{i}': bool(repeat) for i in range(5)})
            lua.globals().package.path = port.as_posix() + '/?.lua;' + lua.globals().package.path
            if native:
                lua.globals().native_helpers = lua.execute(helpers)
                lua.execute('package.preload["NewOakValeIntro.native_quest_helpers"] = function() return native_helpers end')
                source = callbacks[owner] + '\nreturn GivenTeddy, function() return __native_entity_state:GetStateBool("FoundTeddy") end'
            else:
                text = (port / f'NewOakValeIntro/Entities/{owner}.lua').read_text()
                start = text.index('local function given_teddy(')
                end = re.search(r'\nlocal function \w+\(', text[start + 1:]).start() + start + 1
                source = text[:end] + '\nreturn given_teddy, function() return FoundTeddy end'
            callback, found_teddy = lua.execute(source)
            frames, gold = [0], [0]
            def write(_q, key, value):
                fields[key] = value
                events.append(('state', key, value))
            def advance(_q, me):
                self.assertEqual(me, 'entity')
                frames[0] += 1
                events.append(('frame',))
                return not cancel
            def give_gold(_q, amount):
                gold[0] += amount
                events.append(('gold', amount))
            methods = {'GetStateInt': lambda _q, key: fields[key], 'GetStateBool': lambda _q, key: fields[key],
                'SetStateInt': write, 'SetStateBool': write, 'NewScriptFrame': advance,
                'IsActiveThreadTerminating': lambda _q: cancel and frames[0] > 0,
                'MsgIsGameInfoClickedPast': lambda _q: frames[0] >= delay,
                'GetHeroGold': lambda _q: gold[0], 'GetActiveQuestName': lambda _q: 'NewOakValeIntro',
                'GiveHeroGold': give_gold,
                'GiveHeroMorality': lambda _q, value: events.append(('morality', struct.pack('<f', value)))}
            for name in ('TakeObjectFromHero', 'DisplayGameInfo', 'AddLogbookTutorialEntry',
                         'SetQuestCardObjective', 'UpdateQuestInfoCounter', 'ClearThingHasInformation', 'SetMasterGameState'):
                methods[name] = lambda _q, *args, name=name: events.append((name, *args))
            callback(lua.table_from(methods), 'entity')
            if owner == 'NOVI_TeddyGirl':
                self.assertTrue(found_teddy())
            return events, fields

        for owner in callbacks:
            for scenario in ((0, 0, False), (0, 2, False), (1, 0, False), (0, 2, True)):
                with self.subTest(owner=owner, scenario=scenario):
                    self.assertEqual(run(owner, True, *scenario), run(owner, False, *scenario))


if __name__ == '__main__':
    unittest.main()
