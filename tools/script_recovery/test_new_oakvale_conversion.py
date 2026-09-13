import json
import tempfile
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_new_oakvale import convert, inventories, ENTITY_STATE
from tools.script_recovery.native_self_wrapper import fold_self_wrapper_arguments
from tools.script_recovery.lift_native_lua import ROOT, Lifter, RData, load_manifest


COPY = '''uVar4 = *(undefined4 *)(this + 0xc);
piVar3 = *(int **)(this + 0x10);
if (piVar3 != (int *)0x0) {
 *piVar3 = *piVar3 + 1;
}
GSI->SetIsPushableByHero(&PTR__scalar_deleting_destructor__01238c8c,uVar4,piVar3,0);'''


class SelfWrapperTests(unittest.TestCase):
    def test_vector_parameter_rejects_stale_scalar_staging(self):
        lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
        lifter.push_temp('uStack_c', '0')
        args = lifter.place_args([{'name': 'pos', 'type': 'sol::table', 'nativeKind': 'vector'}], [])
        self.assertEqual(args, ['nil --[[missing]]'])
        self.assertTrue(lifter.todo)

    def test_actual_guard_init_matches_port_host_effects(self):
        with tempfile.TemporaryDirectory() as directory:
            out = Path(directory)
            report = convert(out)
            row = next(r for r in report['functions'] if r['owner'] == 'NOVI_Guard' and r['function'] == 'Init')
            self.assertEqual(row['todo'], [])
            self.assertEqual(row['selfWrapperArguments'], 4)
            generated = (out / row['path']).read_text().split('\nfunction Main(', 1)[0]
        port = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'
        original = (port / 'NewOakValeIntro/Entities/NOVI_Guard.lua').read_text()
        def run(source):
            lua, events = LuaRuntime(), []
            lua.globals().package.path = port.as_posix() + '/?.lua;' + lua.globals().package.path
            position = lua.table_from({'x': 12.5, 'y': -7, 'z': 3})
            def home(_me):
                events.append(('GetHomePos',))
                return position
            me = lua.table_from({'GetHomePos': home})
            methods = {}
            for name in ('EntitySetAsDamageable', 'EntitySetAsToAddToComboMultiplierWhenHit',
                         'SetWanderMinDistance', 'SetWanderMaxDistance', 'SetScriptingStateGroup', 'EntitySheatheWeapons'):
                methods[name] = lambda _q, _me, *args, name=name: events.append((name, *args))
            methods['EntitySetAsKillable'] = lambda _q, _me, killable, force=False: events.append(('EntitySetAsKillable', killable, force))
            methods['SetWanderCentrePoint'] = lambda _q, _me, pos: events.append(('SetWanderCentrePoint', pos.x, pos.y, pos.z))
            lua.execute(source)
            lua.globals().Init(lua.table_from(methods), me)
            return events
        actual = run(generated)
        self.assertEqual(actual, run(original))
        self.assertIn(('SetWanderCentrePoint', 12.5, -7, 3), actual)
        self.assertIn(('SetWanderMaxDistance', 6.0), actual)

    def test_reused_copy_locals_preserve_wander_and_state_group_operands(self):
        copies = [COPY.replace('SetIsPushableByHero', method).replace('piVar3,0);', 'piVar3,' + value + ');')
                  for method, value in (('SetWanderMinDistance', '0'), ('SetWanderMaxDistance', '0x40c00000'),
                                        ('SetScriptingStateGroup', '4'))]
        source = '\n'.join(copies)
        lifted, count = fold_self_wrapper_arguments(source)
        self.assertEqual(count, 3)
        self.assertIn('SetWanderMinDistance((CScriptThing *)(this + 8),0.0);', lifted)
        self.assertIn('SetWanderMaxDistance((CScriptThing *)(this + 8),6.0);', lifted)
        self.assertIn('SetScriptingStateGroup((CScriptThing *)(this + 8),4);', lifted)
        live = source + '\nconsume(uVar4);'
        self.assertEqual(fold_self_wrapper_arguments(live), (live, 0))

    def test_staged_entity_alias_is_a_real_lua_entity_operand(self):
        source = '{\npCVar1 = this + 8;\nuStack_c = 0;\npCStack_10 = pCVar1;\nGSI->EntitySetAsDamageable();\n}'
        lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Init', source))
        lua, events = LuaRuntime(), []
        q = lua.table_from({'EntitySetAsDamageable': lambda _q, *args: events.append(args)})
        lua.execute('return function(quest,me)\n' + body + '\nend')(q, 'guard')
        self.assertEqual(events, [('guard', False)])
        self.assertEqual(lifter.todo, [])

    def test_copy_argument_executes_with_entity_receiver(self):
        for receiver in ('this', 'param_1'):
            lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
            body = '\n'.join(lifter.lift('Init', '{\n' + COPY.replace('this', receiver) + '\n}'))
            lua, calls, me = LuaRuntime(), [], object()
            quest = lua.table_from({'SetIsPushableByHero': lambda _q, entity, flag: calls.append((entity, flag))})
            lua.execute('return function(quest, me)\n' + body + '\nend')(quest, me)
            self.assertEqual(calls, [(me, False)])
            self.assertEqual(lifter.self_wrapper_arguments, 1)
            self.assertEqual(lifter.todo, [])

    def test_unproven_or_live_copy_is_not_removed(self):
        for source in (COPY + '\nuse(piVar3);', COPY + '\nuse(uVar4);', COPY.replace('+ 1;', '+ 2;'),
                       COPY.replace('0x10', '0x14'), COPY.replace('01238c8c', '01238c90'),
                       '/* ' + COPY + ' */', '"' + COPY + '"'):
            self.assertEqual(fold_self_wrapper_arguments(source), (source, 0))

    def test_entity_fields_are_isolated_and_parent_fields_remain_shared(self):
        lifter = Lifter(load_manifest(), {'0x24': ('DoneIntro', 'Bool')}, 'quest', True, '', RData(),
                        parent_state={'0x70': ('SpokeAboutFindingTeddy', 'Bool')},
                        state_receiver='__native_entity_state')
        init = '\n'.join(lifter.lift('Init', '{\nthis[0x24] = 0;\n}'))
        update = '\n'.join(lifter.lift('Main', '{\nthis[0x24] = 1;\n'
                    '*(undefined1 *)(*(int *)(this + 0x14) + 0x70) = 1;\n}'))
        read = lifter.expr('this[0x24]')
        lua, shared = LuaRuntime(), {}
        quest = lua.table_from({'SetStateBool': lambda _q, name, value: shared.update({name: value})})
        factory = lua.execute('return function(quest)\n' + ENTITY_STATE + init +
                              '\nreturn {update=function()\n' + update + '\nend,'
                              'read=function() return ' + read + ' end}\nend')
        first, second = factory(quest), factory(quest)
        first.update()
        self.assertIs(first.read(), True)
        self.assertIs(second.read(), False)
        self.assertEqual(shared, {'SpokeAboutFindingTeddy': True})

    def test_current_host_flags_are_retained(self):
        lifter = Lifter(load_manifest(), {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Init', '{\nGSI->EntitySetAsKillable((CScriptThing *)(this + 8),0,0);\n'
                  'GSI->SetThingHasInformation((CScriptThing *)(this + 8),0,1,0);\n}'))
        lua, events, me = LuaRuntime(), [], object()
        quest = lua.table_from({'EntitySetAsKillable': lambda _q, thing, *flags: events.append(('kill', thing, flags)),
                    'SetThingHasInformation': lambda _q, thing, *flags: events.append(('info', thing, flags))})
        lua.execute('return function(quest, me)\n' + body + '\nend')(quest, me)
        self.assertEqual(events, [('kill', me, (False, False)), ('info', me, (False, True, False))])


class FullConversionCoverageTests(unittest.TestCase):
    def test_generated_bully_init_matches_actual_port_host_effects(self):
        port_root = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'
        port = (port_root / 'NewOakValeIntro/Entities/NOVI_Bully.lua').read_text(encoding='utf-8')
        with tempfile.TemporaryDirectory() as directory:
            convert(Path(directory))
            generated = (Path(directory) / 'FSE/NewOakValeIntro/Entities/NOVI_Bully.lua').read_text()
        # Main is still an unsupported draft. Execute the complete generated Init plus its module
        # state, against the actual full port module loaded unchanged in another Lua VM.
        generated = generated[:generated.index('\nfunction Main(')]
        me, hero = object(), object()

        def run(source):
            lua, events = LuaRuntime(), []
            lua.globals().package.path = port_root.as_posix() + '/?.lua;' + lua.globals().package.path
            methods = {'GetHero': lambda _q: hero}
            for name in ('SetStateBool', 'EntitySetAsDamageable', 'EntitySetAsKillable',
                         'EntitySetAsToAddToComboMultiplierWhenHit', 'SetThingHasInformation',
                         'EntitySetThingAsAllyOfThing', 'SetIsPushableByHero'):
                def record(_q, *args, method=name):
                    events.append((method, *args))
                methods[name] = record
            lua.execute(source)
            lua.globals().Init(lua.table_from(methods), me)
            return events

        actual = run(generated)
        self.assertEqual(actual, run(port))
        self.assertIn(('SetIsPushableByHero', me, False), actual)
        self.assertIn(('EntitySetAsKillable', me, False, False), actual)
        self.assertIn(('SetThingHasInformation', me, False, False, False), actual)

    def test_all_inventoried_bodies_are_emitted_and_diagnostics_are_not_hidden(self):
        expected = {(e['entity'], name, f['address']) for e in inventories()
                    for name, f in e['functions'].items() if not f.get('coverageAlias')}
        with tempfile.TemporaryDirectory() as directory:
            output = Path(directory)
            report = convert(output)
            self.assertEqual(report['missing'], [])
            self.assertEqual({(r['owner'], r['function'], r['address']) for r in report['functions']}, expected)
            self.assertEqual(len(report['packages']), 17)
            self.assertEqual(len(report['functions']), 51)
            self.assertGreater(report['summary']['todo'], 0)
            self.assertFalse(report['syntax']['ok'])
            self.assertEqual((output / 'FSE/quests.lua').read_text().splitlines()[-1], 'Quests = {}')
            self.assertEqual(json.loads((output / 'CONVERSION_REPORT.json').read_text())['summary'], report['summary'])


if __name__ == '__main__':
    unittest.main()
