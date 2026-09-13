import unittest
from dataclasses import replace
from unittest.mock import patch

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_post_attack_teleport as fixtures
from tools.script_recovery.lift_native_lua import Lifter, RData
from tools.script_recovery.native_post_attack_teleport import recover_post_attack_teleport
from tools.script_recovery.native_teddy_distance import recover_post_attack_distance
from tools.script_recovery.native_call_setup_ir import read_call_window


class PostAttackDistanceTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.PostAttackTeleportTests().inputs()
        return fn, recover_post_attack_teleport(fn, source, RData(), manifest)[0], manifest

    def test_actual_wait_caches_trigger_refreshes_hero_and_cancels_without_rechecking(self):
        fn, source, manifest = self.inputs()
        recovered, evidence = recover_post_attack_distance(fn, source, RData())
        self.assertEqual(evidence[0]['status'], 'recovered')
        self.assertEqual(len(evidence[0]['distanceSetups']), 2)
        start = recovered.rindex('CCharString::CCharString', 0, recovered.index('"MK_OVI_DADTRIGGER"'))
        end = recovered.index('      bVar3 = CScriptBase::IsActiveThreadTerminating', recovered.index('\n      }', start))
        native = '{\n' + recovered[start:end] + '\nreturn true;\nLAB_00dbef57:\nreturn false;\n}'
        lifter = Lifter(manifest, {}, 'quest', False, '', RData(), live_termination=True, native_gotos=True)
        body = '\n'.join(lifter.lift('FatherWait', native))
        self.assertEqual(lifter.todo, [])
        for near_at, cancel in ((0, None), (2, None), (2, 1)):
            lua, events, frame = LuaRuntime(), [], [0]
            def lookup(_q, name):
                events.append(('lookup', name))
                return 'trigger'
            def hero(_q):
                events.append(('hero', frame[0]))
                return 'hero' + str(frame[0])
            def distance(_q, who, target, amount):
                self.assertEqual((who, target, amount), ('hero' + str(frame[0]), 'trigger', 5.0))
                events.append(('distance', frame[0]))
                return frame[0] >= near_at
            def tick(_q):
                frame[0] += 1
                self.assertLess(frame[0], 4)
                events.append(('frame', frame[0]))
                return frame[0] != cancel
            quest = lua.table_from({'GetThingWithScriptName': lookup, 'GetHero': hero,
                'IsDistanceBetweenThingsUnder': distance, 'NewScriptFrame': tick,
                'IsActiveThreadTerminating': lambda _q: frame[0] == cancel})
            result = lua.execute('return function(quest)\n' + body + '\nend')(quest)
            self.assertIs(result, cancel is None)
            expected = [('lookup', 'MK_OVI_DADTRIGGER'), ('hero', 0), ('distance', 0)]
            for i in range(1, (cancel or near_at) + 1):
                expected.append(('frame', i))
                if i != cancel:
                    expected.extend([('hero', i), ('distance', i)])
            self.assertEqual(events, expected)

    def test_either_distance_operand_change_rejects_entire_recovery(self):
        fn, source, _ = self.inputs()
        for site in (0xDBED51, 0xDBED8D):
            for field, value in (('edx', ('stack', 12)), ('ecx', ('unknown',)),
                                 ('stack_arguments', (('constant', 0x41200000),))):
                def decode(*args, **kwargs):
                    result = read_call_window(*args, **kwargs)
                    return replace(result, **{field: value}) if args[3] == site else result
                with patch('tools.script_recovery.native_teddy_distance.read_call_window', side_effect=decode):
                    result, evidence = recover_post_attack_distance(fn, source, RData())
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
