"""Converter Lua versus independently executed native CPU trace fixtures."""
import unittest
from unittest.mock import patch

from tools.script_recovery.maze_native import recover
from tools.script_recovery.maze_converter import convert
from tools.script_recovery.lift_native_lua import RData, ROOT
from tools.script_recovery import test_maze_research_reference as oracle


def candidate_bridge(lua, h, entry, source_override=None, quest_setup=None):
    # Reuse only the existing native-fixture event doubles/assertions. No
    # reconstructed/reference Lua entrypoint is called by this bridge.
    quest, resources, me, flags = (lua.table() for _ in range(4))
    direct = {'GetStateBool': 'get_bool', 'SetStateBool': 'set_bool',
        'GetActiveQuestName': 'active_quest', 'SetQuestCardObjective': 'objective',
        'IsActiveThreadTerminating': 'terminating', 'SetThingAsUsable': 'usable',
        'MiniMapAddMarker': 'add_marker', 'MiniMapRemoveMarker': 'remove_marker',
        'GetThingWithScriptName': 'lookup', 'DisplayGameInfo': 'display',
        'MsgIsGameInfoClickedPast': 'clicked', 'GetMasterGameState': 'master',
        'EntitySetInLimbo': 'limbo', 'EntitySetAlpha': 'alpha', 'GetHero': 'hero',
        # Historical oracle labels native slot 5CC "skippable". The checked
        # current host API names that exact slot FixMovieSequenceCamera.
        'FixMovieSequenceCamera': 'skippable', 'DeactivateQuestLater': 'deactivate'}
    for api, name in direct.items():
        def forward(_, *args, name=name):
            args = tuple('me' if lua.eval('rawequal')(value, me) else value for value in args)
            return h[name](h, *args)
        quest[api] = forward
    quest.NewScriptFrame = lambda _, *args: h.frame(h)
    for api, name in {'NewResource': 'new_resource', 'NewActorMap': 'new_map',
        'SetActor': 'map_actor', 'StartMovie': 'start_movie', 'Pause': 'pause',
        'DestroyMovie': 'destroy_movie', 'DestroyActorMap': 'destroy_map', 'ReleaseResource': 'release'}.items():
        resources[api] = lambda _, *args, name=name: h[name](h, *args)
    resources.TryAcquire = lambda _, resource, actor, priority: h.acquire(h, actor, resource, priority)
    resources.RunMacroWithFlags = lambda _, name, actors, passed, setup, skip: h.run_macro(h, name, actors, setup, skip)
    quest.RetainRetailThing = lambda _, key, sword: h.retain_sword(h, sword)
    quest.GetRetainedRetailThing = lambda _, key: 'sword'
    quest.RetailFlags = lambda _, key: flags
    flags.Get = lambda _, name: h.flag(h, 'flags', name)
    flags.Set = lambda _, name, value: None
    def spawn(_, name, args):
        assert name == 'UnLimboSword' and args['region'] == '' and args['args'] is None
        h.spawn_unlimbo(h)
    quest.CreateThread = spawn
    quest.WithRetailResources = lambda _, callback: callback(resources)
    me.MsgIsUsedByHero = lambda _: h.used(h, 'me')
    quest.AddMiniMapMarkerByScriptName = lambda _, name, marker: h.add_marker(h, h.lookup(h, name), marker)
    bodies, _ = recover()
    if quest_setup is not None:
        quest_setup(quest,me)
    key = {'history': 'HistoryBookcase.Main', 'grave': 'EmptyGrave.Main', 'unlimbo': 'MazeResearch.UNLIMBO'}[entry]
    if source_override is None:
        function = lua.execute('return function(quest, me)\n' + bodies[key] + '\nend')
    else:
        lua.execute(source_override)
        function = lua.globals().UnLimboSword if entry == 'unlimbo' else lua.globals().Main
    function(quest, me)


class MazeConverterNativeTraceTests(oracle.MazeResearchReferenceTests):
    staged = True

    def setUp(self):
        self.bridge = patch.object(oracle, 'staged_bridge', candidate_bridge)
        self.bridge.start()
        self.addCleanup(self.bridge.stop)


class MazeConverterGuards(unittest.TestCase):
    def test_candidate_is_disabled_and_reports_semantic_gaps(self):
        report = convert()
        self.assertFalse(report['registrationEnabled'])
        self.assertEqual(len(report['syntax']), 3)
        self.assertIn('AddMiniMapMarkerByScriptName', report['pendingCapabilities'])
        self.assertEqual(report['rootDiagnostics']['OnPersist'], [])
        self.assertEqual(len(report['lifecycleAudit']['integrationGaps']), 4)
        with self.assertRaises(ValueError):
            convert(ROOT / 'refs/script_recovery/lifted/MazeResearch')

    def test_changed_bytes_and_literals_reject(self):
        real = RData()
        _, witness = recover(real)
        for address in [fn['address'] for fn in witness['functions'].values()] + [0xea8672, 0xea7b70]:
            class Changed:
                string_at = real.string_at
                def bytes_at(self, start, size):
                    raw = real.bytes_at(start, size)
                    if raw is not None and start <= address < start + size:
                        raw = bytearray(raw); raw[address-start] ^= 1; raw = bytes(raw)
                    return raw
            with self.subTest(address=hex(address)), self.assertRaisesRegex(ValueError, 'native function changed'):
                recover(Changed())
        class ChangedLiteral:
            bytes_at = real.bytes_at
            def string_at(self, address):
                return 'CHANGED' if address == 0x12ee628 else real.string_at(address)
        with self.assertRaisesRegex(ValueError, 'literal changed'):
            recover(ChangedLiteral())

    def test_init_resets_shared_flag_without_replacing_map(self):
        from lupa.lua54 import LuaRuntime
        lua = LuaRuntime()
        bodies, _ = recover()
        lua.execute('flags = {UNLIMBO=true, OTHER=true}; function flags:Set(k,v) self[k]=v end; '
                    'quest={RetailFlags=function(self,name) assert(name=="MazeResearch"); return flags end}')
        lua.execute(bodies['EmptyGrave.Init'])
        self.assertFalse(lua.globals().flags.UNLIMBO)
        self.assertTrue(lua.globals().flags.OTHER)

    def test_failed_acquire_output_survives_until_map_then_local_cleanup(self):
        from lupa.lua54 import LuaRuntime
        bodies, _ = recover()
        for populated in (False, True):
            for cancel in (False, True):
                with self.subTest(populated=populated, cancel=cancel):
                    lua = LuaRuntime()
                    lua.globals().populated = populated
                    lua.globals().cancel = cancel
                    lua.execute('''
                        events={}; refs=0; attempts=0; queries=0; taken=false
                        function emit(s) events[#events+1]=s end
                        r={}
                        function r:NewResource() return {value=nil} end
                        function r:TryAcquire(resource, actor, priority)
                            assert(priority==4 and actor=="hero"); attempts=attempts+1
                            if populated then resource.value="fallback"; refs=refs+1 end
                            return false -- false does not imply empty native output
                        end
                        function r:NewActorMap() return {} end
                        function r:SetActor(map,name,resource)
                            assert(name=="HERO"); map.value=resource.value
                            if map.value then refs=refs+1 end
                        end
                        function r:StartMovie(name) assert(name==""); return "movie" end
                        function r:Pause(value) end
                        function r:RunMacroWithFlags(name,map,flags,setup,skip)
                            assert(map.value==(populated and "fallback" or nil))
                            assert(refs==(populated and 2 or 0) and not setup and skip)
                            emit("macro")
                        end
                        function r:DestroyMovie(movie) emit("movie.destroy") end
                        function r:DestroyActorMap(map)
                            if map.value then refs=refs-1 end; emit("map.destroy")
                        end
                        function r:ReleaseResource(resource)
                            assert(refs==(populated and 1 or 0))
                            if resource.value then refs=refs-1 end; emit("resource.release")
                        end
                        q={}
                        function q:GetStateBool(key) return key=="BookRead" end
                        function q:SetStateBool(key,value) assert(key=="SwordTaken"); taken=value end
                        function q:GetMasterGameState(key)
                            if key=="PostSavePosition" then return 1701 end
                            if key=="JackBossBattleResult" then return 2 end
                            return true
                        end
                        function q:IsActiveThreadTerminating()
                            queries=queries+1; return queries>5 or (cancel and queries==5)
                        end
                        function q:GetThingWithScriptName(name) assert(name=="GoodSword");return "sword" end
                        function q:RetainRetailThing(key,sword) end
                        function q:EntitySetInLimbo(sword,a,b) end
                        function q:MiniMapAddMarker(me,key) end
                        function q:MiniMapRemoveMarker(me) end
                        function q:WithRetailResources(callback) callback(r) end
                        function q:GetHero() return "hero" end
                        function q:FixMovieSequenceCamera(value) end
                        function q:CreateThread(name,args) assert(name=="UnLimboSword" and args.region=="") end
                        function q:RetailFlags(name) return {} end
                        function q:GetActiveQuestName() return "MazeResearch" end
                        function q:DeactivateQuestLater(name,delay) end
                        function q:NewScriptFrame() end
                        me={MsgIsUsedByHero=function() return true end}
                    ''')
                    fn = lua.execute('return function(quest,me)\n' + bodies['EmptyGrave.Main'] + '\nend')
                    fn(lua.globals().q, lua.globals().me)
                    self.assertEqual(lua.globals().attempts, 1)
                    self.assertEqual(lua.globals().refs, 0)
                    self.assertEqual(lua.globals().taken, not cancel)
                    expected = ([] if cancel else ['macro']) + ['movie.destroy', 'map.destroy', 'resource.release']
                    self.assertEqual(list(lua.globals().events.values()), expected)

    def test_compiled_marker_lifetimes(self):
        from tools.script_recovery.maze_prepare_runtime_proposal import prepare
        report = prepare()
        self.assertTrue(report['compiledTest'].startswith('PASS:'))
        self.assertIn('confirmed host ownership gaps', report['compiledLifecycleTest'])
        self.assertTrue(all(row['exitCode'] == 0 for row in report['commands']))

    def test_persistence_native_cases(self):
        import json
        from lupa.lua54 import LuaRuntime
        convert()
        cases = json.loads((ROOT / 'refs/script_recovery/maze_research/runtime_evidence/native-port-persistence-20260912.json').read_text())['cases']
        for case in cases:
            if case['entry'] != '0xea80a0': continue
            with self.subTest(case=case):
                lua = LuaRuntime()
                lua.execute((ROOT / 'work/maze_converter/draft/MazeResearch.lua').read_text())
                state = dict(zip(('SwordTaken', 'BookRead'), case['initial']))
                storage = dict(case['stored']); calls = []; context = object()
                quest = lua.table()
                quest.GetStateBool = lambda _, name: state[name]
                quest.SetStateBool = lambda _, name, value: state.__setitem__(name, value)
                def transfer(_, passed, name, value, default):
                    self.assertIs(passed, context)
                    after = storage.get(name, default) if case['mode'] == 'read' else value
                    if case['mode'] == 'write': storage[name] = value
                    calls.append(dict(name=name, offset='0x48' if name == 'SwordTaken' else '0x49', before=value, default=default, after=after))
                    return after
                quest.PersistTransferBool = transfer
                lua.globals().OnPersist(quest, context)
                self.assertEqual(calls, case['calls'])
                self.assertEqual(list(state.values()), case['final'])
                self.assertEqual(storage, case['storage'])

    def test_root_order_and_initial_state(self):
        from lupa.lua54 import LuaRuntime
        convert(); lua = LuaRuntime()
        lua.execute((ROOT / 'work/maze_converter/draft/MazeResearch.lua').read_text())
        quest = lua.table(); events = []
        quest.SetStateBool = lambda _, name, value: events.append(['state', name, value])
        quest.AddEntityBinding = lambda _, name, file: events.append(['bind', name, file])
        quest.FinalizeEntityBindings = lambda _: events.append(['finalize'])
        quest.GetActiveQuestName = lambda _: 'MazeResearch'
        quest.SetQuestCardObjective = lambda _, *args: events.append(['objective', *args])
        lua.globals().Init(quest); lua.globals().Main(quest)
        self.assertEqual(events, [['state','SwordTaken',False],['state','BookRead',False],
            ['bind','EmptyGrave','MazeResearch/Entities/EmptyGrave'],
            ['bind','HistoryBookcase','MazeResearch/Entities/HistoryBookcase'],['finalize'],
            ['objective','MazeResearch','TEXT_QUEST_MAZE_RESEARCH_OBJECTIVE_01','HeroGuildComplexInside','']])

    def test_lifecycle_bytes_reject(self):
        from tools.script_recovery.maze_lifecycle_audit import audit
        real = RData()
        class Changed:
            string_at = real.string_at
            def bytes_at(self, address, size):
                raw = real.bytes_at(address, size)
                if address <= 0xea86ce < address + size:
                    raw = bytearray(raw); raw[0xea86ce-address] ^= 1; raw = bytes(raw)
                return raw
        with self.assertRaisesRegex(ValueError, 'native evidence changed: destructor'):
            audit(Changed())


if __name__ == '__main__':
    unittest.main()
