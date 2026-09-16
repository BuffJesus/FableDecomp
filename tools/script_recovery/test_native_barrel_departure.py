import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime
from tools.script_recovery import test_native_barrel_health as fixtures
from tools.script_recovery.native_book_trader_health import recover_barrel_health, recover_barrel_health_boolean
from tools.script_recovery.native_barrel_departure import recover_barrel_departure
from tools.script_recovery.lift_native_lua import RData, Lifter, strip_declarations


class BarrelDepartureTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest = fixtures.BarrelHealthTests().inputs()
        source, _ = recover_barrel_health(fn, source, RData(), manifest)
        source, _ = recover_barrel_health_boolean(fn, source, RData())
        return fn, source, manifest

    def test_fade_and_teleports_preserve_actor_order_flags_and_seconds(self):
        fn, source, manifest = self.inputs()
        result, evidence = recover_barrel_departure(fn, source, RData(), manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        statements = [s.strip() for s in strip_declarations(result)]
        start = next(i for i, s in enumerate(statements) if s == 'GSI->FadeScreenOut(1.0,1.0);')
        end = next(i for i, s in enumerate(statements) if s == 'GSI->FadeScreenIn();')
        # This test exercises the transition calls; state/timer commits are outside
        # this operand recovery and are not silently treated as validated here.
        calls = [s for s in statements[start:end + 1] if any(name in s for name in
                 ['GSI->FadeScreen', 'GSI->Pause(', 'GSI->GetThingWithScriptName',
                  'GSI->GetHero(', 'GSI->EntityTeleportToThing'])]
        self.assertEqual(len(calls), 8)
        lifter = Lifter(manifest, {}, 'quest', True, '', RData())
        body = '\n'.join(lifter.lift('Departure', '{\n' + '\n'.join(calls) + '\n}'))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        quest = lua.table_from({
            'FadeScreenOut': lambda _, fade, hold: events.append(('out', fade, hold)),
            'Pause': lambda _, seconds: events.append(('wait', seconds)),
            'GetThingWithScriptName': lambda _, name: events.append(('lookup', name)) or name,
            'GetHero': lambda _: events.append('hero') or 'hero',
            'EntityTeleportToThing': lambda _, actor, target, flag: events.append(('teleport', actor, target, flag)),
            'FadeScreenIn': lambda _: events.append('in')})
        lua.execute('return function(quest,me)\n' + body + '\nend')(quest, 'barrel_man')
        self.assertEqual(events, [('out', 1.0, 1.0), ('wait', 2.0), ('lookup', 'M_WHouse_GuardPoint'),
            'hero', ('teleport', 'hero', 'M_WHouse_GuardPoint', False),
            ('lookup', 'M_BarrelManHiddenPos'), ('teleport', 'barrel_man', 'M_BarrelManHiddenPos', False), 'in'])

    def test_changed_float_or_marker_rejects_whole_transition(self):
        fn, source, manifest = self.inputs()
        data = RData()
        def changed(address, size):
            raw = data.bytes_at(address, size)
            if address == 0xDB63CA:
                raw = bytearray(raw)
                raw[0xDB63FA - address] = 0x3f
                return bytes(raw)
            return raw
        for reader in [SimpleNamespace(bytes_at=changed, string_at=data.string_at),
                       SimpleNamespace(bytes_at=data.bytes_at, string_at=lambda _: 'wrong_marker')]:
            result, evidence = recover_barrel_departure(fn, source, reader, manifest)
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
