import copy
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_quest_vector_fields as fixtures
from tools.script_recovery.lift_native_lua import RData, Lifter, load_entity_state, load_entity_parent_state
from tools.script_recovery.native_quest_vector_fields import new_oakvale_vectors
from tools.script_recovery.native_self_wrapper import fold_self_wrapper_arguments
from tools.script_recovery.native_barrel_init import recover_barrel_init


class BarrelInitTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest, sigs = fixtures.QuestVectorTests().inputs('0x00DB5260', True)
        source, _ = fold_self_wrapper_arguments(source)
        data = RData()
        parent = load_entity_parent_state('NOVI_BarrelMan')
        parent.update(new_oakvale_vectors(data)[1])
        return fn, source, data, manifest, parent, sigs

    def test_timer_identity_copied_home_and_explicit_flags(self):
        fn, source, data, manifest, parent, sigs = self.inputs()
        source, evidence = recover_barrel_init(fn, source, data, manifest, parent)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lifter = Lifter(manifest, load_entity_state('NOVI_BarrelMan'), 'quest', True, '', data,
                        parent_state=parent, thing_sigs=sigs, state_receiver='entity_state')
        emitted = '\n'.join(lifter.lift('Init', source))
        self.assertEqual(lifter.todo, [])
        for timer, coordinates in ((17, (2.5, -99, 7)), (-3, (-1, 0, 0.25))):
            lua, events, quest_state, entity_fields = LuaRuntime(), [], {}, {}
            position = lua.table_from(dict(zip('xyz', coordinates)))
            me = lua.table_from({'GetHomePos': lambda _: position})
            def actor_api(name):
                def call(_q, actor, *args):
                    self.assertTrue(lua.eval('rawequal')(actor, me))
                    events.append((name, *args))
                return call
            quest = lua.table_from({'GetStateInt': lambda _q, key: timer if key == 'WatchTimer' else None,
                'SetTimer': lambda _q, *args: events.append(('timer', *args)),
                'SetStateFloat': lambda _q, key, value: quest_state.__setitem__(key, value),
                **{name: actor_api(name) for name in ('EntitySetAsDamageable', 'EntitySetAsKillable',
                    'EntitySetAsToAddToComboMultiplierWhenHit', 'SetThingHasInformation', 'EntitySetSightRadius')}})
            entity_state = lua.table_from({name: lambda _s, key, value: entity_fields.__setitem__(key, value)
                                           for name in ('SetStateBool', 'SetStateInt')})
            lua.execute('return function(quest,me,entity_state)\n' + emitted + '\nend')(quest, me, entity_state)
            position.x = 10000
            self.assertEqual(tuple(quest_state['WarehouseMeetPoint_' + axis] for axis in 'xyz'), coordinates)
            self.assertEqual(entity_fields, dict(ComplainedAboutStock=False, MyPhase=0,
                                                HeroLetMeDown=False, OverheardYet=False))
            self.assertEqual(events, [('timer', timer, 0), ('EntitySetAsDamageable', False),
                ('EntitySetAsKillable', False, False), ('EntitySetAsToAddToComboMultiplierWhenHit', False),
                ('SetThingHasInformation', False, True, False), ('EntitySetSightRadius', 10.0)])

    def test_changed_bytes_contract_or_parent_ownership_reject(self):
        fn, source, data, manifest, parent, _ = self.inputs()
        for site in (0xDB527C, 0xDB52A0, 0xDB52BC, 0xDB52D5, 0xDB52EC):
            def changed(address, size):
                raw = data.bytes_at(address, size)
                if address <= site < address + size:
                    raw = bytearray(raw)
                    raw[site - address] ^= 1
                    return bytes(raw)
                return raw
            output, evidence = recover_barrel_init(fn, source, SimpleNamespace(bytes_at=changed), manifest, parent)
            self.assertEqual(output, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        for offset in ('0x108', '0x85', '0x89', '0x8d'):
            changed_parent = dict(parent)
            del changed_parent[offset]
            self.assertEqual(recover_barrel_init(fn, source, data, manifest, changed_parent)[1][0]['status'], 'rejected')
        changed_manifest = copy.deepcopy(manifest)
        changed_manifest['SetTimer']['parameters'].reverse()
        self.assertEqual(recover_barrel_init(fn, source, data, changed_manifest, parent)[1][0]['status'], 'rejected')
        self.assertEqual(recover_barrel_init(fn, source + '\n', data, manifest, parent)[1][0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
