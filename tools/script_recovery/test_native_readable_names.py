import unittest

from tools.script_recovery.lift_native_lua import load_manifest, Lifter, RData
from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_readable_names import readable_names, rename_lua_locals


class ReadableNameTests(unittest.TestCase):
    def test_emitter_uses_proven_timer_name_without_changing_calls(self):
        lifter = Lifter(load_manifest(), {}, 'quest', False, '', RData(), readable_locals=True)
        body = '\n'.join(lifter.lift('Main', '{\nuVar1 = GSI->RegisterTimer();\n'
            'GSI->SetTimer(uVar1,5);\nGSI->DeregisterTimer(uVar1);\nreturn uVar1;\n}'))
        self.assertEqual(lifter.readable_local_names, {'uVar1': 'timerId'})
        lua, events = LuaRuntime(), []
        quest = lua.table_from({'RegisterTimer': lambda _q: 71,
            'SetTimer': lambda _q, timer, value: events.append(('set', timer, value)),
            'DeregisterTimer': lambda _q, timer: events.append(('remove', timer))})
        self.assertEqual(lua.execute('return function(quest)\n' + body + '\nend')(quest), 71)
        self.assertEqual(events, [('set', 71, 5), ('remove', 71)])

    def test_timer_provenance_names_local_and_avoids_collisions(self):
        lines = ['uVar1 = GSI->RegisterTimer();', 'GSI->SetTimer(uVar1,5);', 'GSI->DeregisterTimer(uVar1);']
        self.assertEqual(readable_names(lines, load_manifest()), {'uVar1': 'timerId'})
        self.assertEqual(readable_names(lines, load_manifest(), {'timerId'}), {'uVar1': 'timerId2'})

    def test_reassigned_loop_or_unknown_use_is_not_renamed(self):
        for lines in (['uVar1 = GSI->RegisterTimer();', 'uVar1 = 7;', 'GSI->DeregisterTimer(uVar1);'],
                      ['while (running) {', 'uVar1 = GSI->RegisterTimer();', 'GSI->DeregisterTimer(uVar1);', '}'],
                      ['uVar1 = GSI->RegisterTimer();', 'Unknown(uVar1);', 'GSI->DeregisterTimer(uVar1);']):
            self.assertEqual(readable_names(lines, load_manifest()), {})

    def test_lua_rewrite_preserves_native_comments_and_strings(self):
        lines = ['local uVar1 = quest:RegisterTimer()', 'quest:SetTimer(uVar1,5) -- uVar1',
                 'print("uVar1")', '--[[ uVar1 ]]']
        self.assertEqual(rename_lua_locals(lines, {'uVar1': 'timerId'}),
                         ['local timerId = quest:RegisterTimer()', 'quest:SetTimer(timerId,5) -- uVar1',
                          'print("uVar1")', '--[[ uVar1 ]]'])
