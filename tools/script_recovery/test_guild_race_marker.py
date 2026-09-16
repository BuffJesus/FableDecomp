import itertools
import unittest

from lupa.lua54 import LuaRuntime

from tools.script_recovery.guild_race_marker import SOURCE, verify
from tools.script_recovery.guild_race_marker_native import execute
from tools.script_recovery.lift_native_lua import RData


def lua_case(near, cancel, initial):
    lua = LuaRuntime()
    lua.execute(SOURCE)
    quest = lua.table()
    trace = []
    queries = distances = 0
    reached = initial

    def terminating(_):
        nonlocal queries
        queries += 1
        result = queries >= cancel
        trace.append(('term', result))
        return result

    def distance(_, hero, marker, radius):
        nonlocal distances
        assert (hero, marker, radius) == ('hero', 'marker', 3.0)
        result = near[distances % len(near)]
        distances += 1
        trace.append(('distance', result))
        return result

    def state(_, key, value):
        nonlocal reached
        assert key == 'ReachedPlatform' and value is True
        reached = value
        trace.append(('state', key, value))

    def remove(_, marker):
        assert marker == 'marker'
        trace.append(('remove',))

    def frame(_, marker):
        assert marker == 'marker'
        trace.append(('frame',))
        return False  # Native frame result must not substitute for the next query.

    quest.IsActiveThreadTerminating = terminating
    quest.GetHero = lambda _: trace.append(('hero',)) or 'hero'
    quest.IsDistanceBetweenThingsUnder = distance
    quest.SetStateBool = state
    quest.MiniMapRemoveMarker = remove
    quest.NewScriptFrame = frame
    lua.globals().Init(quest, 'marker')
    lua.globals().Main(quest, 'marker')
    return reached, trace


class GuildRaceMarkerTests(unittest.TestCase):
    def test_complete_native_control_flow_matches_readable_lua(self):
        patterns = [(False,), (True,), (False, True), (True, False)]
        for near, cancel, initial, raw in itertools.product(patterns, range(1, 9), (False, True), (1, 255)):
            with self.subTest(near=near, cancel=cancel, initial=initial, raw=raw):
                self.assertEqual(execute(near, cancel, initial, raw), lua_case(near, cancel, initial))

    def test_native_change_rejected(self):
        original = RData()

        class Changed:
            def bytes_at(self, address, size):
                return bytes([original.bytes_at(address, size)[0] ^ 1]) + original.bytes_at(address, size)[1:]

        with self.assertRaisesRegex(ValueError, 'evidence changed'):
            verify(Changed())


if __name__ == '__main__':
    unittest.main()
