import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_quest_markers as fixtures
from tools.script_recovery.native_quest_markers import recover_quest_markers
from tools.script_recovery.native_marker_cleanup import recover_marker_cleanup, OWNERSHIP_LIMITATION
from tools.script_recovery.lift_native_lua import ROOT, Lifter, RData


class MarkerCleanupTests(unittest.TestCase):
    def generated(self):
        fn, source, manifest = fixtures.QuestMarkerTests().inputs()
        lifter = Lifter(manifest, {'0x94': ('GivenSweets', 'Bool'), '0x95': ('GivenTheresaChocs', 'Bool')},
                        'quest', False, '', RData(), live_termination=True, native_gotos=True)
        body = '\n'.join(lifter.lift('ManageQuestCoreMarkers', source, native_function=fn))
        self.assertEqual(lifter.todo, [OWNERSHIP_LIMITATION])
        return body

    def run_scenario(self, body, cancel_at=None, tutorial=True, ready=False, port=False, preterminated=False):
        lua = LuaRuntime()
        if port:
            root = ROOT / 'refs/script_recovery/reconstructed/NewOakValeIntro/FSE'
            lua.globals().root = root.as_posix()
            lua.execute('package.path = root .. "/?.lua;" .. package.path')
            lua.execute((root / 'NewOakValeIntro/NewOakValeIntro.lua').read_text())
            function = lua.globals().ManageQuestCoreMarkers
        else:
            function = lua.execute('return function(quest)\n' + body + '\nend')
        frames, events, terminating = [0], [], [preterminated]
        def frame(_q):
            frames[0] += 1
            self.assertLess(frames[0], 7, 'marker wait did not finish')
            events.append(('frame',))
            if frames[0] == cancel_at:
                terminating[0] = True
            return not terminating[0]
        def lookup(_q, name):
            events.append(('lookup', name))
            return name
        def show(_q, tutorial_id):
            self.assertEqual(tutorial_id, 19)
            events.append(('tutorial', tutorial_id))
            return tutorial
        quest = lua.table_from({
            'GetThingWithScriptName': lookup,
            'MiniMapAddMarker': lambda _q, who, texture: events.append(('add', who, texture)),
            'MiniMapRemoveMarker': lambda _q, who: events.append(('remove', who)),
            'GetHeroGold': lambda _q: 3 if ready or frames[0] >= 1 else 2,
            'IsHeroControlledByPlayer': lambda _q: ready or frames[0] >= 2,
            'DisplayTutorial': show, 'MsgIsTutorialClickedPast': lambda _q: ready or frames[0] >= 3,
            'GetStateBool': lambda _q, key: ready or frames[0] >= {'GivenSweets': 4, 'GivenTheresaChocs': 5}[key],
            'NewScriptFrame': frame, 'IsActiveThreadTerminating': lambda _q: terminating[0],
        })
        function(quest)
        return events

    def test_full_generated_script_effects_match_port_through_every_wait(self):
        body = self.generated()
        for cancel_at in (None, 1, 2, 3, 4, 5):
            with self.subTest(cancel_at=cancel_at):
                events = self.run_scenario(body, cancel_at=cancel_at)
                self.assertEqual(events, self.run_scenario(body, cancel_at=cancel_at, port=True))
                if cancel_at is not None:
                    self.assertEqual(events[-1], ('frame',))
                else:
                    self.assertEqual(events[-1], ('add', 'NOVI_LiveFather', 'HUD_ORB_QUEST_CORE'))
        for options in ({'tutorial': False}, {'ready': True}):
            self.assertEqual(self.run_scenario(body, **options), self.run_scenario(body, port=True, **options))
        # Additional retail termination checks are retained even where the
        # port's already-satisfied predicate helpers do not query termination.
        events = self.run_scenario(body, ready=True, preterminated=True)
        self.assertEqual(len(events), 4)
        self.assertEqual(events[-1], ('add', 'NOVI_LiveFather', 'HUD_ORB_QUEST_CORE'))

    def test_changed_cleanup_body_or_source_is_not_discarded(self):
        fn, source, _ = fixtures.QuestMarkerTests().inputs()
        data = RData()
        source = recover_quest_markers(fn, source, data)[0]
        for changed_address in (0xDBE4E0, 0x4AA840, 0x99A2E0):
            def read(address, size):
                raw = data.bytes_at(address, size)
                return bytes([raw[0] ^ 1]) + raw[1:] if address == changed_address else raw
            result, evidence = recover_marker_cleanup(fn, source, SimpleNamespace(bytes_at=read))
            self.assertEqual(result, source)
            self.assertEqual(evidence[0]['status'], 'rejected')
        altered = source.replace('operator_delete(local_24[0]);', 'GSI->NewScriptFrame();')
        self.assertNotEqual(altered, source)
        self.assertEqual(recover_marker_cleanup(fn, altered, data)[1][0]['status'], 'rejected')
