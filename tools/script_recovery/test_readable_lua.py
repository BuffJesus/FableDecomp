import unittest

from tools.script_recovery.readable_lua import rename_labels
from pathlib import Path

from lupa.lua54 import LuaRuntime

from tools.script_recovery.readable_lua import readable_source, rename_identifiers


class ReadableLuaTests(unittest.TestCase):
    def test_labels_change_only_control_tokens_and_remain_reversible(self):
        source = '''function Main()
    local LAB_123 = "LAB_123"
    -- goto LAB_123
    if LAB_123 then goto LAB_123 end
    ::LAB_123::
    return LAB_123
end
'''
        output = rename_labels(source, {'LAB_123': 'releaseActors'})
        self.assertIn('if LAB_123 then goto releaseActors end', output)
        self.assertIn('::releaseActors::', output)
        self.assertIn('-- goto LAB_123', output)
        self.assertIn('return LAB_123', output)
        self.assertEqual(rename_labels(output, {'releaseActors': 'LAB_123'}), source)
        with self.assertRaises(ValueError):
            rename_labels(source, {'missing': 'cleanup'})

    def test_owned_script_lookup_has_actor_name(self):
        output, maps = readable_source('''function Main(resources)
    local r1 = resources:NewThingFromScriptName("NOVI_AffairWoman")
    resources:DestroyThing(r1)
end
''')
        self.assertIn('local affairWoman =', output)
        self.assertIn('DestroyThing(affairWoman)', output)

    def test_timer_loop_preserves_calls_values_and_uses_semantic_name(self):
        source = '''function Main(quest)
    local iVar2, uVar1
    uVar1 = quest:RegisterTimer()
    iVar2 = quest:GetTimer(uVar1)
    while iVar2 > 0 do
        iVar2 = quest:GetTimer(uVar1)
    end
    quest:DeregisterTimer(uVar1)
    return iVar2
end
'''
        output, mappings = readable_source(source)
        self.assertEqual(mappings[0]['locals']['iVar2']['name'], 'timeRemaining')
        self.assertEqual(mappings[0]['locals']['uVar1']['name'], 'timerId')
        def run(chunk):
            lua = LuaRuntime()
            events = []
            values = iter([2, 1, 0])
            def timer(_q, timer):
                events.append(('get', timer))
                return next(values)
            q = lua.table_from({'RegisterTimer': lambda _: 42, 'GetTimer': timer,
                                'DeregisterTimer': lambda _, timer: events.append(('remove', timer))})
            lua.execute(chunk)
            return lua.globals().Main(q), events
        self.assertEqual(run(output), run(source))

    def test_reused_result_is_not_given_one_of_its_conflicting_roles(self):
        source = '''function Main(quest)
    local cVar2
    cVar2 = quest:IsXbox()
    cVar2 = quest:MsgIsGameInfoClickedPast()
    return cVar2
end
'''
        output, mappings = readable_source(source, split_reused=False)
        self.assertEqual(mappings[0]['locals']['cVar2']['basis'], 'reused or unresolved native temporary')
        self.assertIn('return scratchValue', output)
        versioned, mappings = readable_source(source)
        self.assertIn('local isXbox', versioned)
        self.assertIn('return instructionDismissed', versioned)
        self.assertIn('cVar2', mappings[0]['splitLocals'])

    def test_renaming_preserves_literals_comments_members_and_table_keys(self):
        source = '''local uVar1, uVar2 = 5, 7
local t = {uVar1 = uVar1, uVar2 = uVar2}
--[==[ uVar1 ]==]
print([=[uVar1]=], "uVar1", t.uVar1)
return uVar1, uVar2
'''
        result = rename_identifiers(source, {'uVar1': 'first', 'uVar2': 'second'})
        self.assertIn('local first, second = 5, 7', result)
        self.assertIn('{uVar1 = first, uVar2 = second}', result)
        self.assertIn('--[==[ uVar1 ]==]', result)
        self.assertIn('print([=[uVar1]=], "uVar1", t.uVar1)', result)
        self.assertEqual(LuaRuntime().execute(result), LuaRuntime().execute(source))

    def test_concatenation_is_not_mistaken_for_member_access(self):
        source = 'local uVar1 = 7\nreturn "value=" .. uVar1'
        result = rename_identifiers(source, {'uVar1': 'count'})
        self.assertIn('.. count', result)
        self.assertEqual(LuaRuntime().execute(result), 'value=7')

    def test_actual_husband_readable_output_preserves_event_traces(self):
        from tools.script_recovery.generate_affair_man_resource_candidate import generate
        from tools.script_recovery.test_generate_affair_man_resource_candidate import HARNESS
        source, _ = generate()
        readable, _ = readable_source(source)
        def run(chunk, scenario):
            lua = LuaRuntime(unpack_returned_tuples=True)
            return list(lua.execute(HARNESS)(chunk, lua.table_from(scenario)).values())
        scenarios = [dict(acquire=True, hit=True, terminateAtFrame=5),
                     dict(acquire=True, talk=True, retryTalk=True, terminateAtFrame=3),
                     dict(acquire=False, terminateAtFrame=2),
                     dict(acquire=True, hit=True, failHealth=True, terminateAtFrame=10),
                     dict(acquire=True, talk=True, failSpeak=True, terminateAtFrame=10)]
        for scenario in scenarios:
            with self.subTest(scenario=scenario):
                self.assertEqual(run(readable, scenario), run(source, scenario))

    def test_actual_barrel_names_split_platform_prompt_and_shared_state(self):
        path = Path(__file__).resolve().parents[2] / 'refs/script_recovery/lifted/NewOakValeIntro/FSE/NewOakValeIntro/Entities/NOVI_Barrel.lua'
        source = path.read_text(encoding='utf-8')
        output, _ = readable_source(source)
        self.assertIn('while not instructionGivenBarrels do', output)
        self.assertIn('if not isXbox then', output)
        self.assertIn('while not instructionDismissed', output)
        self.assertNotIn('scratchValue = "TEXT_', output)
        harness = '''return function(source, xbox, shown, dismissed)
            local events, frames = {}, 0
            local quest = {}
            function quest:NewScriptFrame() frames=frames+1; events[#events+1]='frame'..frames; return true end
            function quest:IsActiveThreadTerminating() return frames>=3 end
            function quest:GetStateBool() return shown end
            function quest:SetStateBool(key,value) shown=value; events[#events+1]=key..tostring(value) end
            function quest:GetHero() return {} end
            function quest:IsDistanceBetweenThingsUnder() return true end
            function quest:IsXbox() return xbox end
            function quest:DisplayGameInfo(key) events[#events+1]=key end
            function quest:MsgIsGameInfoClickedPast() return dismissed end
            local env = setmetatable({}, {__index=_G})
            assert(load(source,'barrel','t',env))()
            env.Main(quest,{})
            return table.concat(events,'|')
        end'''
        lua = LuaRuntime()
        run = lua.execute(harness)
        for xbox in (False, True):
            for shown in (False, True):
                for dismissed in (False, True):
                    with self.subTest(xbox=xbox, shown=shown, dismissed=dismissed):
                        self.assertEqual(run(source, xbox, shown, dismissed), run(output, xbox, shown, dismissed))

    def test_function_maps_are_independent_and_avoid_collisions(self):
        source = '''function First(quest, hero)
    local pCVar1 = quest:GetHero()
    return pCVar1
end
function Second(quest)
    local pCVar1 = quest:GetPos()
    return pCVar1
end
'''
        output, mappings = readable_source(source)
        self.assertEqual(mappings[0]['locals']['pCVar1']['name'], 'hero2')
        self.assertEqual(mappings[1]['locals']['pCVar1']['name'], 'position')
        self.assertIn('return hero2', output)
        self.assertIn('return position', output)

    def test_shadowed_local_and_multiline_literal_are_not_interpreted_as_one_binding(self):
        source = '''function Main(quest)
    local pCVar1 = quest:GetHero()
    do
        local pCVar1 = 5
        print(pCVar1)
    end
    print([=[
local uVar1 = quest:RegisterTimer()
]=])
    return pCVar1
end
'''
        output, maps = readable_source(source)
        self.assertEqual(output, source)
        self.assertEqual(maps[0]['locals'], {})

    def test_function_text_in_long_strings_and_comments_is_not_a_function_boundary(self):
        source = '''--[=[
function Fake(quest)
    local uVar1 = quest:RegisterTimer()
end
]=]
function Main(quest)
    local text = [==[
function AlsoFake(quest)
    local uVar1 = quest:RegisterTimer()
end
]==]
    local pCVar1 = quest:GetHero()
    return pCVar1, text
end
'''
        output, maps = readable_source(source)
        self.assertEqual([m['function'] for m in maps], ['Main'])
        self.assertIn('local pCVar1', source)
        self.assertIn('local hero', output)
        self.assertEqual(output.count('local uVar1 = quest:RegisterTimer()'), 2)


class UseRoleTests(unittest.TestCase):
    """A temporary whose assignments say nothing is named by the API that consumes it."""

    def name_of(self, body, declared='uVar1'):
        source = 'function Main(quest, me, resources, hero)\n    local %s\n%s\nend\n' % (declared, body)
        output, maps = readable_source(source, split_reused=False)
        return maps[0]['locals'].get(declared, {}).get('name')

    def test_the_consuming_call_names_the_value(self):
        self.assertEqual(self.name_of('    uVar1 = 0\n    quest:DeregisterTimer(uVar1)'), 'timerId')
        self.assertEqual(self.name_of('    uVar1 = 0\n    quest:AddPersonToConversation(uVar1, hero)'), 'conversationId')
        self.assertEqual(self.name_of('    uVar1 = 0\n    resources:TryAcquire(uVar1, hero, 4)'), 'heroControl')
        self.assertEqual(self.name_of('    uVar1 = 0\n    quest:StateListErase("AllCreatures", uVar1)'), 'allCreaturesIndex')

    def test_a_list_walked_by_byte_offset_is_an_offset_not_an_index(self):
        self.assertEqual(self.name_of('    uVar1 = 0\n    quest:GetStateListAt("AllCreatures", uVar1 / 12)'),
                         'allCreaturesOffset')

    def test_consumers_of_one_kind_agree_and_the_first_rule_wins(self):
        # acquired, released and destroyed are three views of the same resource
        body = ('    uVar1 = 0\n    resources:TryAcquire(uVar1, hero, 4)\n'
                '    resources:ReleaseResource(uVar1)\n    resources:DestroyMovie(uVar1)')
        self.assertEqual(self.name_of(body), 'heroControl')

    def test_consumers_of_different_kinds_decline(self):
        # one slot, two values: naming it either way would be a lie
        body = '    uVar1 = 0\n    resources:ReleaseResource(uVar1)\n    quest:DeregisterTimer(uVar1)'
        self.assertEqual(self.name_of(body), 'scratchValue')

    def test_a_bit_field_is_not_a_scratch_value(self):
        body = '    uVar1 = 0\n    uVar1 = uVar1 | 1\n    uVar1 = uVar1 & 0xfffffffe\n    quest:F(uVar1)'
        self.assertEqual(self.name_of(body), 'flags')

    def test_plain_literals_are_not_a_bit_field(self):
        body = '    uVar1 = 0\n    uVar1 = 1\n    quest:F(uVar1)'
        self.assertEqual(self.name_of(body), 'scratchValue')
