import unittest
import tempfile
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_goto_scopes import supported_jumps
from tools.script_recovery.test_lift_native_lua import make
from tools.script_recovery.convert_new_oakvale import convert


class NativeGotoTests(unittest.TestCase):
    def test_actual_watch_barrels_frame_join_is_preserved(self):
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory)
            report = convert(out)
            row = next(r for r in report['functions'] if r['function'] == 'WatchBarrels')
            self.assertIn('if (iVar4 <= iVar3 + -4) goto LAB_00dbeabd;', row['nativeJumps'])
            source = (out / row['path']).read_text()
            self.assertIn('if iVar4 <= iVar3 + -4 then goto LAB_00dbeabd end', source)
            self.assertIn('::LAB_00dbeabd::', source)
            # Its cleanup label lies inside a sibling block; this rule must not certify it.
            self.assertNotIn('LAB_00dbeb07', row['nativeLabels'])
            self.assertTrue(row['todo'])

    def lift(self, source, parameters=None):
        lifter = make()
        lifter.native_gotos = True
        body = '\n'.join(lifter.lift('Main', source, parameters=parameters))
        return lifter, body

    def test_forward_join_skips_assignment_and_preserves_native_local_scope(self):
        source = '''{
iVar1 = 0;
if (skip) goto LAB_00123456;
iVar1 = 9;
LAB_00123456:
return iVar1;
}'''
        lifter, body = self.lift(source)
        lua = LuaRuntime()
        run = lua.execute('return function(skip)\n' + body + '\nend')
        self.assertEqual(run(True), 0)
        self.assertEqual(run(False), 9)
        self.assertIsNone(lua.globals().iVar1)
        self.assertEqual(lifter.todo, [])

    def test_backward_jump_reexecutes_body_instead_of_ending_thread(self):
        source = '''{
iVar1 = 0;
LAB_00123456:
iVar1 = iVar1 + 1;
GSI->SetTimer(17,iVar1);
if (iVar1 < 3) goto LAB_00123456;
return iVar1;
}'''
        lifter, body = self.lift(source)
        lua, events = LuaRuntime(), []
        q = lua.table_from({'SetTimer': lambda _q, timer, value: events.append(value)})
        self.assertEqual(lua.execute('return function(Quest)\n' + body + '\nend')(q), 3)
        self.assertEqual(events, [1, 2, 3])
        self.assertEqual(lifter.todo, [])

    def test_jump_out_of_branch_to_loop_tail_keeps_following_iterations(self):
        source = '''{
iVar1 = 0;
while (iVar1 < 3) {
if (iVar1 < 2) {
goto LAB_00123456;
}
GSI->SetTimer(17,iVar1);
LAB_00123456:
iVar1 = iVar1 + 1;
}
return iVar1;
}'''
        lifter, body = self.lift(source)
        lua, events = LuaRuntime(), []
        q = lua.table_from({'SetTimer': lambda _q, timer, value: events.append(value)})
        self.assertEqual(lua.execute('return function(Quest)\n' + body + '\nend')(q), 3)
        self.assertEqual(events, [2])
        self.assertEqual(lifter.todo, [])

    def test_nested_scope_entry_and_ambiguous_repeated_statement_are_rejected(self):
        for statements in (
            ['goto LAB_00123456;', 'if (x) {', 'LAB_00123456:', '}'],
            ['if (x) {', 'goto LAB_00123456;', '}', 'if (y) {', 'LAB_00123456:', '}'],
            ['goto LAB_00123456;', 'if (x) {', 'goto LAB_00123456;', 'LAB_00123456:', '}'],
        ):
            self.assertEqual(supported_jumps(statements), (set(), set()))


if __name__ == '__main__':
    unittest.main()
