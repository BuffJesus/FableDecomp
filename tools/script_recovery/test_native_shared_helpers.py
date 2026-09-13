import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_new_oakvale import convert
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.lift_native_lua import known_callee_aliases


class SharedHelperTests(unittest.TestCase):
    def test_reviewed_logbook_alias_uses_ecx_string_before_noise_filter(self):
        source = '''{
CCharString::CCharString((CCharString *)&stack0xfffffffc,"TEXT_QST_LOG_BASICS_MAP",-1);
CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffffc,(CFontBank *)ppVar4,pCVar5,unaff_EDI,unaff_ESI,pCVar3,param_1,0,0,0);
}'''
        lifter = make()
        lifter.callee_names = known_callee_aliases({'calls': [{'currentName': 'CSubtitleRenderer::SetText',
                                                            'target': '0x00CBE9EE'}]})
        body = '\n'.join(lifter.lift('AddBadDeed', source))
        lua, events = LuaRuntime(), []
        q = lua.table_from({'AddLogbookTutorialEntry': lambda _q, key: events.append(key)})
        lua.execute('return function(Quest)\n' + body + '\nend')(q)
        self.assertEqual(events, ['TEXT_QST_LOG_BASICS_MAP'])
        self.assertEqual(lifter.todo, [])
        unknown = source.replace('&stack0xfffffffc,(CFontBank', '&stack0xffffff00,(CFontBank')
        body = '\n'.join(lifter.lift('AddBadDeed', unknown))
        self.assertNotIn(':AddLogbookTutorialEntry(', body)
        self.assertTrue(lifter.todo)
        self.assertEqual(known_callee_aliases({'calls': [{'currentName': 'CSubtitleRenderer::SetText',
                                                       'target': '0x1234'}]}), {})

    def test_separate_equal_string_arguments_are_not_deduplicated(self):
        lifter = make()
        source = '''{
CCharString::CCharString((CCharString *)&stack0xfffffff0,"",-1);
CCharString::CCharString((CCharString *)&stack0xffffffec,"",-1);
CCharString::CCharString((CCharString *)&stack0xffffffe8,"OBJECTIVE",-1);
uVar4 = GSI->GetActiveQuestName();
GSI->SetQuestCardObjective(uVar4);
}'''
        body = '\n'.join(lifter.lift('Main', source))
        lua, events = LuaRuntime(), []
        q = lua.table_from({'GetActiveQuestName': lambda _q: 'Oakvale',
                            'SetQuestCardObjective': lambda _q, *args: events.append(args)})
        lua.execute('return function(Quest)\n' + body + '\nend')(q)
        self.assertEqual(events, [('Oakvale', 'OBJECTIVE', '', '')])
        self.assertEqual(lifter.todo, [])

    def test_parent_receiver_and_argument_must_be_proven(self):
        for receiver, argument, accepted in (
            ('*(Owner **)(this + 0x14)', ',3', True),
            ('*(Owner **)(param_1 + 0x14)', ',3', True),
            ('*(Owner **)(this + 0x18)', ',3', False),
            ('this', ',3', False), ('*(Owner **)(this + 0x14)', '', False),
        ):
            lifter = make(entity=True)
            lifter.parent_helpers = {'AddBadDeed': {'module': 'test.helpers', 'name': 'AddBadDeed', 'arity': 1}}
            source = '\n'.join(lifter.lift('GivenTeddy', '{\nAddBadDeed(' + receiver + argument + ');\n}'))
            if accepted:
                lua = LuaRuntime()
                lua.execute('package.preload["test.helpers"] = function() return {AddBadDeed=function(q,me,v) '
                            'assert(q == "parent"); assert(me == "entity"); assert(v == 3) end} end')
                lua.execute('return function(quest, me)\n' + source + '\nend')('parent', 'entity')
                self.assertEqual(lifter.todo, [])
            else:
                self.assertNotIn('require(', source)
                self.assertTrue(lifter.todo)

    def test_generated_bully_calls_shared_module_in_separate_entity_vm(self):
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory)
            report = convert(output)
            helpers = report['sharedHelpers']
            self.assertEqual(helpers['functions'], ['AddBadDeed', 'AddGoodDeed'])
            self.assertTrue(helpers['syntax']['ok'])
            module = (output / helpers['path']).read_text()
            self.assertIn('local AddBadDeed, AddGoodDeed', module)
            self.assertIn('quest:NewScriptFrame(me)', module)
            self.assertNotIn('quest:NewScriptFrame()', module)
            row = next(r for r in report['functions'] if r['owner'] == 'NOVI_Bully' and r['function'] == 'GivenTeddy')
            source = (output / row['path']).read_text().split('function GivenTeddy(', 1)[1]
            source = 'function GivenTeddy(' + source
            self.assertEqual(row['todo'], [])
            for _ in range(2):
                lua, events = LuaRuntime(), []
                lua.execute('package.preload["NewOakValeIntro.native_quest_helpers"] = function() '
                            'return {AddBadDeed=function(q,me,v) assert(me == "bully"); q:RecordDeed(v) end} end')
                quest = lua.table_from({name: (lambda _q, *args, name=name: events.append((name, *args)))
                                        for name in ('GiveHeroGold', 'TakeObjectFromHero', 'SetStateBool', 'RecordDeed')})
                lua.execute(source)
                lua.globals().GivenTeddy(quest, 'bully')
                self.assertEqual(events, [('GiveHeroGold', 1), ('TakeObjectFromHero', 'OBJECT_TEDDY_BEAR_UNGIVEABLE'),
                                          ('SetStateBool', 'SpokeAboutFindingTeddy', True),
                                          ('SetStateBool', 'TeddyRuined', True), ('RecordDeed', 3)])
                self.assertIsNone(lua.globals().AddBadDeed)
            # Loading the real generated module must not execute its still-incomplete bodies.
            lua = LuaRuntime()
            loaded = lua.execute(module)
            self.assertIsNotNone(loaded.AddBadDeed)
            self.assertIsNotNone(loaded.AddGoodDeed)
            self.assertIsNone(lua.globals().AddBadDeed)


if __name__ == '__main__':
    unittest.main()
