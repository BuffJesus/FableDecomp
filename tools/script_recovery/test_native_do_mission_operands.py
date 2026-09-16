import copy
import unittest
from types import SimpleNamespace

from lupa.lua54 import LuaRuntime

from tools.script_recovery import test_native_quest_vector_fields as fixtures
from tools.script_recovery.lift_native_lua import RData, Lifter
from tools.script_recovery.native_do_mission_operands import recover_do_mission_operands, recover_attack_stuff_operands


class DoMissionOperandTests(unittest.TestCase):
    def inputs(self):
        fn, source, manifest, sigs = fixtures.QuestVectorTests().inputs('0x00DBDE40')
        return fn, source, RData(), manifest, sigs

    def run_mission(self, already_over=False, cancel_at=None):
        fn, source, data, manifest, sigs = self.inputs()
        source, evidence = recover_do_mission_operands(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lifter = Lifter(manifest, {'0x50': ('AttackOver', 'Bool')}, 'quest', False, '', data,
                        live_termination=True, native_gotos=True, thing_sigs=sigs)
        lifter.helper_names = {'AttackStuff', 'PostAttackStuff'}
        body = '\n'.join(lifter.lift('DoMission', source))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        state = dict(over=already_over, check=0, hero=0, name=0)
        def terminate(_q):
            state['check'] += 1
            return state['check'] == cancel_at
        def frame(_q):
            state['over'] = True
            return True
        def hero(_q):
            state['hero'] += 1
            return 'hero' + str(state['hero'])
        def name(_q):
            state['name'] += 1
            return 'quest' + str(state['name'])
        def record(method):
            return lambda _q, *args: events.append((method, *args))
        methods = {name: record(name) for name in (
            'FadeScreenOutUntilNextCallToFadeScreenIn', 'TurnCreatureInto', 'CreateThread',
            'CacheMusicSet', 'ActivateQuest', 'EntitySetAsKillable', 'SetTimeAsStopped',
            'SetTimeOfDay', 'SetHeroSleepingAsEnabled', 'DisplayMoneyBag',
            'OverrideAutomaticHouseLocking', 'OpenHouseDoors', 'KickOffQuestStartScreen',
            'OverrideMusic', 'SetQuestAsCompleted', 'DeactivateQuestLater')}
        methods.update(IsRegionLoaded=lambda _q, region: region == 'StartOakVale',
                       GetStateBool=lambda _q, key: state['over'], NewScriptFrame=frame,
                       IsActiveThreadTerminating=terminate, GetHero=hero, GetActiveQuestName=name,
                       GetThingWithScriptName=lambda _q, key: 'house:' + key)
        lua.globals().AttackStuff = lambda _q: events.append(('attack',))
        lua.globals().PostAttackStuff = lambda _q: events.append(('postAttack',))
        lua.execute('return function(quest)\n' + body + '\nend')(lua.table_from(methods))
        return events

    def test_initial_setup_and_completion_keep_distinct_actors_and_names(self):
        events = self.run_mission()
        self.assertEqual(events, [
            ('FadeScreenOutUntilNextCallToFadeScreenIn', 2.0, 0.0),
            ('TurnCreatureInto', 'hero1', 'CREATURE_HERO_CHILD'),
            ('CreateThread', 'WatchBarrels'), ('CreateThread', 'WatchForGotGold'),
            ('CreateThread', 'ManageQuestCoreMarkers'), ('CacheMusicSet', 46),
            ('ActivateQuest', 'Q_NewOakValeIntro_PreAttack'),
            ('EntitySetAsKillable', 'hero2', False, False), ('SetTimeAsStopped', True),
            ('SetTimeOfDay', 12.0), ('SetHeroSleepingAsEnabled', False), ('DisplayMoneyBag', True),
            ('OverrideAutomaticHouseLocking', 'house:HerosOldHouse', True),
            ('OpenHouseDoors', 'house:HerosOldHouse'), ('KickOffQuestStartScreen', 'quest1', False, True),
            ('OverrideMusic', 19, False, True), ('attack',), ('postAttack',),
            ('FadeScreenOutUntilNextCallToFadeScreenIn', 0.5, 0.0),
            ('EntitySetAsKillable', 'hero3', True, False), ('SetHeroSleepingAsEnabled', True),
            ('SetQuestAsCompleted', 'quest2', False, False, False), ('DeactivateQuestLater', 'quest3', 0)])

    def test_loaded_post_attack_and_cancellation_do_not_replay_setup(self):
        events = self.run_mission(already_over=True)
        self.assertEqual(events, [('attack',), ('postAttack',),
            ('FadeScreenOutUntilNextCallToFadeScreenIn', 0.5, 0.0),
            ('EntitySetAsKillable', 'hero1', True, False), ('SetHeroSleepingAsEnabled', True),
            ('SetQuestAsCompleted', 'quest1', False, False, False), ('DeactivateQuestLater', 'quest2', 0)])
        for index in range(1, 5):
            with self.subTest(cancel=index):
                events = self.run_mission(cancel_at=index)
                self.assertNotIn(('attack',), events)
                self.assertFalse(any(event[0] == 'SetQuestAsCompleted' for event in events))

    def test_changed_operands_getter_abi_or_contract_reject(self):
        fn, source, data, manifest, _ = self.inputs()
        for site in (0xDBDFFC, 0xDBE0BF, 0xDBE13A, 0xDBE191, 0xDBE1B9, 0xDBE258,
                     0xDBE282, 0x891D4C, 0x8918A3, 0x1261024):
            def changed(address, size):
                raw = data.bytes_at(address, size)
                if address <= site < address + size:
                    raw = bytearray(raw)
                    raw[site - address] ^= 1
                    return bytes(raw)
                return raw
            with self.subTest(site=hex(site)):
                result, evidence = recover_do_mission_operands(fn, source, SimpleNamespace(bytes_at=changed), manifest)
                self.assertEqual(result, source)
                self.assertEqual(evidence[0]['status'], 'rejected')
        changed = copy.deepcopy(manifest)
        changed['KickOffQuestStartScreen']['parameters'].reverse()
        self.assertEqual(recover_do_mission_operands(fn, source, data, changed)[1][0]['status'], 'rejected')
        self.assertEqual(recover_do_mission_operands(fn, source + ' ', data, manifest)[1][0]['status'], 'rejected')


class AttackStuffOperandTests(unittest.TestCase):
    def test_native_transition_order_and_numeric_values(self):
        fn, source, manifest, sigs = fixtures.QuestVectorTests().inputs('0x00DBE3C0')
        data = RData()
        source, evidence = recover_attack_stuff_operands(fn, source, data, manifest)
        self.assertEqual(evidence[0]['status'], 'recovered')
        lifter = Lifter(manifest, {}, 'quest', False, '', data, thing_sigs=sigs)
        body = '\n'.join(lifter.lift('AttackStuff', source))
        self.assertEqual(lifter.todo, [])
        lua, events = LuaRuntime(), []
        def record(name):
            return lambda _q, *args: events.append((name, *args))
        quest = lua.table_from({name: record(name) for name in ('ActivateQuest', 'DeactivateQuest',
            'SetTimeOfDay', 'TransitionToTheme', 'SetQuestCardObjective')})
        quest.GetActiveQuestName = lambda _q: 'active-quest-at-call'
        lua.execute('return function(quest)\n' + body + '\nend')(quest)
        self.assertEqual(events, [('ActivateQuest', 'Q__OakValeIntro_PostAttack'),
            ('DeactivateQuest', 'Q_NewOakValeIntro_PreAttack', 0), ('SetTimeOfDay', 23.0),
            ('TransitionToTheme', 'ENVIRONMENT_OV_POSTATTACK', 0.0),
            ('SetQuestCardObjective', 'active-quest-at-call', 'TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06', '', '')])

    def test_changed_transition_or_literal_rejects(self):
        fn, source, manifest, _ = fixtures.QuestVectorTests().inputs('0x00DBE3C0')
        data = RData()
        for site in (0xDBE405, 0xDBE421, 0xDBE440, 0xDBE491, 0x12D9D7C, 0x122D70E):
            def changed(address, size):
                raw = data.bytes_at(address, size)
                if address <= site < address + size:
                    raw = bytearray(raw)
                    raw[site - address] ^= 1
                    return bytes(raw)
                return raw
            with self.subTest(site=hex(site)):
                output, evidence = recover_attack_stuff_operands(fn, source, SimpleNamespace(bytes_at=changed), manifest)
                self.assertEqual(output, source)
                self.assertEqual(evidence[0]['status'], 'rejected')


if __name__ == '__main__':
    unittest.main()
