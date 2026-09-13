"""Native trace replay through the actual Lua-to-runtime API adapter.

Resource/Quest entry points are test doubles. The separate C++ smoke exercises
the implementation behind the resource entry points, including error cleanup.
"""
import json
import unittest
from lupa import LuaRuntime
from tools.script_recovery.test_meet_sister_reference import SOURCE, HOST

FACADE = r'''
function facade(h)
    local quest, resources, count = {}, {}, 0
    function quest:AddEntityBinding(...) return h:bind(...) end
    function quest:FinalizeEntityBindings(...) return h:finalize_bindings(...) end
    function quest:SetQuestCardObjective(...) return h:set_objective(...) end
    function quest:GetActiveQuestName(...) return h:active_quest(...) end
    function quest:IsRegionLoaded(...) return h:region_loaded(...) end
    function quest:IsActiveThreadTerminating(...) return h:terminating(...) end
    function quest:GetHero(...) return h:hero(...) end
    function quest:RemoveThing(...) return h:remove(...) end
    function quest:SetQuestAsCompleted(...) return h:complete(...) end
    function quest:DeactivateQuestLater(...) return h:deactivate_later(...) end
    function quest:FadeScreenIn(...) return h:fade_in(...) end
    function quest:WithRetailResources(callback)
        callback(resources)
        for _, refs in pairs(h.refs) do assert(refs == 0) end
        assert(not h.paused)
    end
    function resources:NewResource()
        count = count + 1
        return h:new_resource(count == 1 and 'bound' or 'hero')
    end
    function resources:PrepareResource(...) return h:prepare(...) end
    function resources:TryAcquire(id, actor, priority) return h:acquire(actor,id,priority) end
    function resources:ReleaseResource(...) return h:release(...) end
    function resources:Frame(...) return h:frame(...) end
    function resources:NewActorMap(...) return h:new_map(...) end
    function resources:SetActor(...) return h:map_actor(...) end
    function resources:DestroyActorMap(...) return h:destroy_map(...) end
    function resources:RunMacro(name,map,setup,skippable)
        return h:run_macro(name,map,nil,nil,setup,skippable)
    end
    function resources:StartMovie(...) return h:start_movie(...) end
    function resources:Pause(...) return h:pause(...) end
    function resources:DestroyMovie(...) return h:destroy_movie(...) end
    return quest
end
'''


class MeetSisterRuntimeAdapterTests(unittest.TestCase):
    def test_all_native_scenarios_through_runtime_adapter(self):
        self.replay(False)

    def test_all_native_scenarios_through_staged_package(self):
        self.replay(True)

    def replay(self, package):
        proof = json.loads((SOURCE.parent.parent / 'runtime_evidence/native-lua-flow-comparison-20260912.json').read_text())
        for case in proof['cases']:
            with self.subTest(entry=case['entry'], scenario=case['scenario']):
                lua = LuaRuntime(unpack_returned_tuples=True)
                lua.execute(HOST + FACADE)
                options = dict(region_wait=0, acquire_wait=0, stop_frame=0,
                               initial_stop=False, hero_ok=True, stop_in_macro=False)
                options.update(case['options'])
                host = lua.globals().make_host(*options.values())
                quest = lua.globals().facade(host)
                if package:
                    fse = SOURCE.parents[2] / 'reconstructed/MeetSister/FSE'
                    lua.globals().package.path = fse.as_posix() + '/?.lua;' + lua.globals().package.path
                    entry = {'main': 'MeetSister', 'messenger': 'Entities/MeetSisterMessenger',
                             'sister': 'Entities/MeetSisterSister'}[case['entry']]
                    lua.execute((fse / 'MeetSister' / (entry + '.lua')).read_text())
                    lua.globals().Init(quest, 'bound-instance-not-a-name-lookup')
                    lua.globals().Main(quest, 'bound-instance-not-a-name-lookup')
                else:
                    reference = lua.execute(SOURCE.read_text())
                    adapter = lua.execute((SOURCE.parent / 'runtime_adapter.lua').read_text())
                    adapter.run(reference, case['entry'], quest, 'bound-instance-not-a-name-lookup')
                actual = [[row[i] for i in range(1, len(row)+1)] for row in host.events.values()]
                self.assertEqual(actual, case['events'])


if __name__ == '__main__':
    unittest.main()
