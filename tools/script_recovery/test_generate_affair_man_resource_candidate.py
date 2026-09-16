import json
import re
import tempfile
import unittest
from unittest.mock import patch
from pathlib import Path

from lupa.lua54 import LuaRuntime

from tools.script_recovery.generate_affair_man_resource_candidate import (
    DRAFT, EXTENSION_METHODS, generate, native_inputs, verified_movies,
)

EXPECTED_REWRITES = {
    'hit movie pause': 1, 'dead byte casts': 3,
    'movie start': 2, 'movie end': 33, 'movie pause': 35,
    'hit string scope': 1, 'hit mask reload': 1,
    'cached Thing lookup': 2, 'cached Thing alive': 2, 'cached Thing distance': 3,
    'cached Thing facing default': 2, 'cached Thing facing': 4,
    'cached Thing conversation person': 2, 'cached Thing conversation line': 6,
    'cached Thing destruction': 1,
    'resource address take': 1, 'preparation': 3, 'acquisition': 6, 'speech': 8, 'task query': 19,
    'clear actions': 1, 'clear commands': 1, 'animation': 3, 'animation byte': 3, 'movement': 1,
    'health Thing': 8, 'home distance Thing': 2, 'construction': 1, 'destruction': 1}

# Every mock records its call so the test can assert the exact resource order the candidate drives.
HARNESS = '''
return function(chunk, scenario)
    local events = {}
    local function rec(name, ...)
        local parts = {name}
        for i = 1, select('#', ...) do parts[#parts + 1] = tostring((select(i, ...))) end
        events[#events + 1] = table.concat(parts, ' ')
    end
    local frames, polls, nextId, acquires, animationReads, terminationChecks = 0, 0, 0, 0, 0, 0
    local resources = {}
    local entries, paused = {}, false
    local function add(kind)
        nextId = nextId + 1
        entries[nextId] = kind
        return nextId
    end
    local function destroy(id, kind, name)
        assert(entries[id] == kind, 'wrong kind or double destruction')
        entries[id] = false
        rec(name, id)
    end
    function resources:StartMovie(name)
        assert(name == '', 'native movie class must be empty')
        for _, kind in pairs(entries) do assert(kind ~= 'movie', 'overlapping movies') end
        local id = add('movie'); rec('StartMovie', id); return id
    end
    function resources:DestroyMovie(id) destroy(id, 'movie', 'DestroyMovie') end
    function resources:Pause(flag) paused = flag; rec('Pause', flag) end
    function resources:Close()
        if paused then self:Pause(false) end
        for id = nextId, 1, -1 do
            if entries[id] == 'thing' then self:DestroyThing(id)
            elseif entries[id] == 'movie' then self:DestroyMovie(id)
            elseif entries[id] == 'resource' then self:ReleaseResource(id) end
        end
        rec('ScopeClosed')
    end
    function resources:NewResource() local id = add('resource'); rec('NewResource', id); return id end
    function resources:PrepareResource(id) rec('PrepareResource', id) end
    function resources:TryAcquire(id, actor, priority)
        acquires = acquires + 1
        rec('TryAcquire', id, actor.name, priority)
        if scenario.retryTalk and acquires == 2 then return false end
        return scenario.acquire
    end
    function resources:ClearAllActions(id) rec('ClearAllActions', id) end
    function resources:ClearCommands(id) rec('ClearCommands', id) end
    function resources:IsHitByHeroExceptAbility(actor, ability)
        assert(actor.name == 'husband' and ability == 14)
        return scenario.hit or (scenario.anySpecial and not scenario.excludedSpecial)
    end
    function resources:ReadAnimationArgument5()
        animationReads = animationReads + 1
        local flag = animationReads % 2 == 0
        rec('ReadAnimationArgument5', flag)
        return flag
    end
    function resources:PlayAnimation(id, name, b1, b2, b3, b4, b5, b6, b7)
        assert(type(b5) == 'boolean')
        rec('PlayAnimation', id, name, b1, b2, b3, b4, b5, b6, b7)
    end
    function resources:ReleaseResource(id) destroy(id, 'resource', 'ReleaseResource') end
    function resources:Speak(id, target, key, selection, listen, sound2D, overFade)
        rec('Speak', id, target.name, key, selection, listen, sound2D, overFade)
        if scenario.failSpeak then error('injected speech error') end
    end
    function resources:IsPerformingScriptTask(id)
        polls = polls + 1; rec('IsPerformingScriptTask', id)
        return not scenario.idle and polls % 2 == 1
    end
    function resources:NewThingFromResource(id) local thing = add('thing'); rec('NewThingFromResource', id, thing); return thing end
    function resources:NewThingFromScriptName(name)
        local id = add('thing'); rec('NewThingFromScriptName', name, id)
        if scenario.failLookup and name == 'NOVI_AffairWife' then error('injected lookup error') end
        return id
    end
    function resources:ThingAlive(id) assert(entries[id] == 'thing'); return true end
    function resources:ThingsAreWithinDistance(actor, id, distance)
        assert(actor.name == 'husband' and entries[id] == 'thing')
        assert(distance == 5.0 or distance == 2.0)
        return true
    end
    function resources:FaceThing() end
    function resources:ThingHealth(thing) rec('ThingHealth', thing); if scenario.failHealth then error('injected health error') end; return 1.0 end
    function resources:DestroyThing(thing) destroy(thing, 'thing', 'DestroyThing') end
    local quest = {}
    function quest:NewScriptFrame(actor) frames = frames + 1; rec('NewScriptFrame', frames, actor.name); return true end
    function quest:IsActiveThreadTerminating()
        terminationChecks = terminationChecks + 1
        return frames >= scenario.terminateAtFrame or
            (scenario.terminateAtCheck and terminationChecks >= scenario.terminateAtCheck)
    end
    function quest:GetThingWithScriptName(name) rec('GetThingWithScriptName', name); return {name = name} end
    function quest:GetHero() return {name = 'hero'} end
    function quest:IsDistanceBetweenThingsUnder() return true end
    function quest:EntitySetFacingAngleTowardsThing() end
    function quest:RetailRandModulo() return 0 end
    function quest:WithRetailResources(callback)
        rec('WithRetailResources')
        local ok, err = pcall(callback, resources)
        resources:Close()
        if not ok then error(err) end
    end
    local me = setmetatable({name = 'husband'}, {__index = function(_, key)
        error('candidate used cached entity control: me:' .. tostring(key))
    end})
    me.MsgIsHitByHero = function() return scenario.hit end
    me.MsgIsHitByAnySpecialAbilityFromHero = function() return false end
    me.IsTalkedToByHero = function() return scenario.talk end
    package.preload['NewOakValeIntro.native_quest_helpers'] = function()
        return {AddBadDeed = function(_, actor, amount) rec('AddBadDeed', actor.name, amount) end}
    end
    local env = setmetatable({}, {__index = _G})
    local loaded, err = load(chunk, '@candidate', 't', env)
    if not loaded then error(err) end
    loaded()
    local ok, err = pcall(env.Main, quest, me)
    if scenario.failSpeak or scenario.failHealth or scenario.failLookup then
        assert(not ok, 'injected error did not propagate')
        assert(tostring(err):find('injected'), tostring(err))
        rec('ErrorPropagated')
    elseif not ok then error(err) end
    return events
end
'''


class AffairManResourceCandidateTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        with tempfile.TemporaryDirectory() as directory:
            cls.source, cls.report = generate(directory)
            out = Path(directory)
            cls.written = (out / 'NOVI_AffairMan.resource_candidate.lua').read_text(encoding='utf-8')
            cls.written_report = json.loads((out / 'NOVI_AffairMan.resource_candidate.json').read_text())

    def run_candidate(self, **scenario):
        lua = LuaRuntime(unpack_returned_tuples=True)
        harness = lua.execute(HARNESS)
        return list(harness(self.source, lua.table_from(scenario)).values())

    def test_every_rewrite_matches_reviewed_native_or_expanded_layout_counts(self):
        self.assertEqual(self.report['rewrites'], EXPECTED_REWRITES)
        self.assertEqual(self.report['resourceEvents']['speak'], 8)
        self.assertEqual(self.report['resourceEvents']['task_query'], 19)
        self.assertEqual(self.report['temporaryThings'], 10)
        self.assertTrue(self.report['syntax']['ok'])
        self.assertEqual(self.report['status'], 'disabled-candidate-not-registered')
        self.assertEqual(self.written, self.source)
        self.assertEqual(self.written_report['rewrites'], self.report['rewrites'])
        self.assertEqual(self.report['requiresExtension'], list(EXTENSION_METHODS))

    def test_candidate_has_one_resource_and_no_cached_control(self):
        self.assertEqual(self.source.count('resources:NewResource()'), 1)
        self.assertEqual(self.source.count('resources:ReleaseResource(man_resource)'), 1)
        self.assertEqual(len(re.findall(r'\bme:(AcquireControl|Speak|IsPerformingScriptTask|PlayAnimation|MoveToPosition|ClearAllActions|ClearCommands)\b', self.source)), 0)
        self.assertNotIn('quest:GetHealth(me)', self.source)
        self.assertNotIn('DAT_01375748', self.source)
        # The 0.1 m home-leave check runs on the cached entity in retail (0xCBE45C on me), so it stays.
        self.assertEqual(self.source.count('me:IsDistanceFromPositionOver(pCVar12, fVar25)'), 1)
        self.assertEqual(self.source.count('resources:ThingIsDistanceFromPositionOver(man_thing, native_arg_man_home_position, 2.0)'), 2)
        self.assertEqual(self.source.count('resources:NewThingFromResource(man_resource)'), 10)
        self.assertEqual(self.source.count('resources:DestroyThing(man_thing)'), 10)
        self.assertIn('quest:WithRetailResources(function(resources)', self.source)
        self.assertTrue(self.source.startswith('-- DISABLED CANDIDATE'))
        self.assertNotIn('Quests', self.source)

    def test_hit_path_drives_one_resource_from_construction_to_release(self):
        events = self.run_candidate(acquire=True, hit=True, terminateAtFrame=3)
        self.assertEqual(events[:2], ['WithRetailResources', 'NewScriptFrame 1 husband'])
        self.assertEqual(events[2:5], ['NewResource 1', 'PrepareResource 1', 'TryAcquire 1 husband 4'])
        self.assertEqual(events[5:7], ['NewThingFromScriptName NOVI_AffairWoman 2', 'NewThingFromScriptName NOVI_AffairWife 3'])
        self.assertEqual(events[7:11], ['StartMovie 4', 'Pause true',
                                        'PrepareResource 1', 'TryAcquire 1 husband 4'])
        self.assertEqual(events[11:14], ['NewThingFromResource 1 5', 'ThingHealth 5', 'DestroyThing 5'])
        self.assertEqual(events[14], 'Speak 1 hero TEXT_QST_048_AFFAIRMAN_ON_HIT 0 false true false')
        self.assertEqual(events[15:18], ['IsPerformingScriptTask 1', 'NewScriptFrame 2 husband', 'IsPerformingScriptTask 1'])
        self.assertEqual(events[18:21], ['AddBadDeed husband 2', 'Pause false', 'DestroyMovie 4'])
        self.assertEqual(events[21:], ['NewScriptFrame 3 husband', 'DestroyThing 3', 'DestroyThing 2', 'ReleaseResource 1', 'ScopeClosed'])
        self.assertEqual(events.count('NewResource 1'), 1)
        self.assertEqual(events.count('ReleaseResource 1'), 1)

    def test_talk_success_and_retry_reach_speech_and_cancel_cleanly(self):
        for retry in (False, True):
            with self.subTest(retry=retry):
                events = self.run_candidate(acquire=True, hit=False, talk=True,
                                            retryTalk=retry, terminateAtFrame=3 if retry else 2)
                self.assertIn('ClearAllActions 1', events)
                self.assertIn('ClearCommands 1', events)
                self.assertTrue(any('TEXT_QST_048_AFFAIRMAN_HOW_FIND_OUT' in e for e in events))
                self.assertEqual(events.count('TryAcquire 1 husband 4'), 3 if retry else 2)
                self.assertEqual(events[-6:], ['Pause false',
                                              'DestroyMovie 4', 'DestroyThing 3', 'DestroyThing 2', 'ReleaseResource 1', 'ScopeClosed'])
                self.assertEqual(events.count('ReleaseResource 1'), 1)
        self.assertNotIn('SUB41(', self.source)

    def test_talk_acquisition_cancellation_cleans_up_movie_and_resource(self):
        events = self.run_candidate(acquire=True, hit=False, talk=True,
                                    retryTalk=True, terminateAtFrame=2)
        self.assertNotIn('ClearAllActions 1', events)
        self.assertEqual(events[-6:], ['Pause false',
                                      'DestroyMovie 4', 'DestroyThing 3', 'DestroyThing 2', 'ReleaseResource 1', 'ScopeClosed'])

    def test_movie_scope_replaces_all_host_movie_and_pause_calls(self):
        self.assertNotRegex(self.source, r'quest:(StartMovieSequence|EndMovieSequence|PauseAllNonScriptedEntities)\(')
        self.assertEqual(self.report['movies']['starts'], ['0xdb0c71', '0xdb0e3d'])
        self.assertEqual(self.report['movies']['nativeDestructors'], 12)
        self.assertEqual(self.source.count('man_movie = resources:StartMovie("")'), 2)

    def test_errors_unpause_destroy_live_temporaries_and_movie_then_release(self):
        for talk in (False, True):
            for failure in ('failSpeak', 'failHealth'):
                with self.subTest(talk=talk, failure=failure):
                    events = self.run_candidate(acquire=True, hit=not talk, talk=talk,
                                                terminateAtFrame=10, **{failure: True})
                    expected = ['Pause false']
                    if failure == 'failHealth':
                        expected.append('DestroyThing 5')
                    expected += ['DestroyMovie 4', 'DestroyThing 3', 'DestroyThing 2', 'ReleaseResource 1', 'ScopeClosed', 'ErrorPropagated']
                    self.assertEqual(events[-len(expected):], expected)
                    self.assertEqual(events.count('DestroyMovie 4'), 1)
                    self.assertEqual(events.count('DestroyThing 5'), 1)
                    self.assertEqual(events.count('ReleaseResource 1'), 1)

    def test_repeated_hit_movies_destroy_each_id_once(self):
        events = self.run_candidate(acquire=True, hit=True, terminateAtFrame=5)
        self.assertEqual([e for e in events if e.startswith('StartMovie ')],
                         ['StartMovie 4', 'StartMovie 6'])
        self.assertEqual([e for e in events if e.startswith('DestroyMovie ')],
                         ['DestroyMovie 4', 'DestroyMovie 6'])
        self.assertEqual(events.count('ReleaseResource 1'), 1)

    def test_animation_reads_current_boolean_each_time_and_reaches_cleanup(self):
        events = self.run_candidate(acquire=True, hit=False, talk=False, idle=True, terminateAtFrame=3)
        reads = [e for e in events if e.startswith('ReadAnimationArgument5')]
        self.assertEqual(reads, ['ReadAnimationArgument5 false', 'ReadAnimationArgument5 true'])
        animations = [e for e in events if e.startswith('PlayAnimation')]
        self.assertEqual(animations, [
            'PlayAnimation 1 ST_OPINION_FEAR_IDLE_COWERING false false false true false false false',
            'PlayAnimation 1 ST_OPINION_FEAR_IDLE_COWERING false false false true true false false'])
        self.assertEqual(events[-2:], ['ReleaseResource 1', 'ScopeClosed'])
        self.assertNotIn('__native_byte_01375748', self.source)

    def test_cached_things_cleanup_before_main_loop_and_on_lookup_error(self):
        before = self.run_candidate(acquire=True, terminateAtFrame=10, terminateAtCheck=2)
        self.assertFalse(any(e.startswith('NewThingFromScriptName') for e in before))
        self.assertEqual(before[-2:], ['ReleaseResource 1', 'ScopeClosed'])
        after = self.run_candidate(acquire=True, terminateAtFrame=10, terminateAtCheck=3)
        self.assertEqual(after[-4:], ['DestroyThing 3', 'DestroyThing 2', 'ReleaseResource 1', 'ScopeClosed'])
        failure = self.run_candidate(acquire=True, terminateAtFrame=10, failLookup=True)
        self.assertEqual(failure[-5:], ['DestroyThing 3', 'DestroyThing 2', 'ReleaseResource 1', 'ScopeClosed', 'ErrorPropagated'])
        self.assertEqual([t['stackSlot'] for t in self.report['cachedThings']['things']], [32, 44])
        self.assertNotIn('quest:GetThingWithScriptName', self.source)
        self.assertNotRegex(self.source, r'\br[12]:')

    def test_changed_movie_bytes_and_lifetime_reject(self):
        fn, rdata, _ = native_inputs()

        class ChangedMovieDestructor:
            def bytes_at(self, address, size):
                raw = rdata.bytes_at(address, size)
                return b'\x00' * size if address == 0x6E7B80 else raw

        with self.assertRaisesRegex(ValueError, 'movie native bytes changed'):
            verified_movies(fn, ChangedMovieDestructor())
        with patch('tools.script_recovery.generate_affair_man_resource_candidate.check_single_resource_lifetime',
                   return_value=False):
            with self.assertRaisesRegex(ValueError, 'movie native lifetimes overlap or fail cleanup'):
                verified_movies(fn, rdata)

    def test_live_byte_cast_result_rejects_before_writing(self):
        with tempfile.TemporaryDirectory() as directory:
            changed = Path(directory) / 'draft.lua'
            changed.write_text(DRAFT.read_text(encoding='utf-8').replace(
                'uVar6 = SUB41(me,0)', 'uVar6 = SUB41(me,0)\n                    print(uVar6)'), encoding='utf-8')
            with self.assertRaisesRegex(ValueError, 'no longer a dead temporary'):
                generate(directory, draft_path=changed)
            self.assertFalse((Path(directory) / 'NOVI_AffairMan.resource_candidate.lua').exists())

    def test_changed_native_pause_rejects(self):
        with patch('tools.script_recovery.generate_affair_man_resource_candidate.read_call_window', return_value=None):
            with self.assertRaisesRegex(ValueError, 'native hit movie pause operands changed'):
                generate()

    def test_invalid_syntax_rejects_before_writing(self):
        with tempfile.TemporaryDirectory() as directory:
            with patch('tools.script_recovery.generate_affair_man_resource_candidate.LuaSyntaxChecker.check',
                       return_value={'ok': False}):
                with self.assertRaisesRegex(ValueError, 'candidate syntax rejected'):
                    generate(directory)
            self.assertFalse((Path(directory) / 'NOVI_AffairMan.resource_candidate.lua').exists())

    def test_termination_inside_acquisition_loop_still_releases_once(self):
        events = self.run_candidate(acquire=False, hit=False, terminateAtFrame=2)
        self.assertEqual(events, ['WithRetailResources', 'NewScriptFrame 1 husband', 'NewResource 1',
                                  'PrepareResource 1', 'TryAcquire 1 husband 4', 'NewScriptFrame 2 husband',
                                  'ReleaseResource 1', 'ScopeClosed'])

    def test_termination_before_construction_creates_no_resource(self):
        events = self.run_candidate(acquire=True, hit=True, terminateAtFrame=1)
        self.assertEqual(events, ['WithRetailResources', 'NewScriptFrame 1 husband', 'ScopeClosed'])

    def test_changed_draft_rejects_instead_of_partial_conversion(self):
        draft = DRAFT.read_text(encoding='utf-8')
        with tempfile.TemporaryDirectory() as directory:
            changed = Path(directory) / 'NOVI_AffairMan.lua'
            changed.write_text(draft.replace('cVar5 = me:AcquireControl(4)', 'cVar5 = true', 1), encoding='utf-8')
            with self.assertRaisesRegex(ValueError, 'acquisition: draft has 5 sites, native witness has 6'):
                generate(directory, draft_path=changed)
            self.assertFalse((Path(directory) / 'NOVI_AffairMan.resource_candidate.lua').exists())


if __name__ == '__main__':
    unittest.main()
