"""Executable checks for recovered Wasp combat transitions and native copy shapes."""
import unittest
from pathlib import Path

from lupa.lua54 import LuaRuntime
from tools.script_recovery.native_evidence_lowering import (
    fold_local_thing_copies, normalise_typed_decompile,
    preserve_repeated_script_name_fills,
)

ROOT = Path(__file__).resolve().parents[2]


class NativeCopyTests(unittest.TestCase):
    def test_only_known_interface_fields_are_normalised(self):
        text = ('g = *(CCharString_bv **)(this + 0x40);\n'
                'other = *(CCharString **)(this + 0x44);\n')
        output = normalise_typed_decompile(text)
        self.assertIn('g = *(int **)(this + 0x40);', output)
        self.assertIn('other = *(CCharString **)(this + 0x44);', output)

    def test_retained_copy_requires_proven_destination_and_matching_parts(self):
        text = ('held = QUESTTHING_Empty();\n'
                'info = *(int **)(found + 0x8);\n'
                'data = *(int **)(found + 0x4);\n'
                'if (held != info) {\nheld = data;\nheld = info;\n'
                'if (info != (int *)0x0) {\n*info = *info + 1;\n}\n}\n')
        self.assertEqual(fold_local_thing_copies(text),
                         'held = QUESTTHING_Empty();\nheld = found;\n')
        for changed in (text.replace('QUESTTHING_Empty', 'Unproven'),
                        text.replace('found + 0x4', 'other + 0x4'),
                        text.replace('held = data', 'other = data')):
            self.assertEqual(fold_local_thing_copies(changed), changed)

    def test_aggregation_does_not_cross_branch_or_vector_reset(self):
        first = 'v = GSI->GetAllThingsWithScriptName(&first);\n'
        last = 'v = GSI->GetAllThingsWithScriptName(&last);\n'
        self.assertIn('LOCALLIST_Append(v,', preserve_repeated_script_name_fills(first + last))
        for boundary in ('v = 0;\n', '}\nelse {\n', 'Clear(&v);\n'):
            text = first + boundary + last
            self.assertEqual(preserve_repeated_script_name_fills(text), text)


class WaspCombatTests(unittest.TestCase):
    def runtime(self, stage='readable'):
        lua = LuaRuntime(unpack_returned_tuples=True)
        source = ROOT / f'refs/script_recovery/lifted/WaspBoss/{stage}/FSE/WaspBoss/WaspBoss.lua'
        lua.execute(source.read_text(encoding='utf-8'))
        return lua

    def test_five_drones_and_all_three_groups_gate_dynamic_queen_spawn(self):
        for stage, survivor in ((stage, group) for stage in ('draft', 'readable')
                               for group in ('HornetDrone', 'WaspChaser', 'WaspAttacker')):
            with self.subTest(stage=stage, survivor=survivor):
                lua = self.runtime(stage)
                lua.globals().survivor = survivor
                lua.execute(r'''
                    local groups, alive, spawns, flags = {}, true, {}, {}
                    for _, name in ipairs({'HornetDrone', 'WaspChaser', 'WaspAttacker'}) do
                        groups[name] = {{IsAlive = function() return name == survivor and alive end,
                                         IsUnconscious = function() return false end}}
                    end
                    local quest = setmetatable({}, {__index = function() return function() end end})
                    function quest:IsActiveThreadTerminating() return false end
                    function quest:ReadGlobalGameDataString(offset)
                        return offset == 0xe3c and 'MOD_DRONE' or 'MOD_QUEEN'
                    end
                    function quest:GetThingWithScriptName(name)
                        return {GetPos = function() return {marker = name} end}
                    end
                    function quest:CreateCreature(def, pos, name)
                        spawns[#spawns + 1] = {def, pos.marker, name}
                        return {}
                    end
                    function quest:GetAllThingsWithScriptName(name) return groups[name] end
                    function quest:SetStateBool(name, value) flags[name] = value end
                    function quest:GetStateBool(name) return flags[name] or false end
                    function quest:CreateThread(name) assert(name == 'GuildmasterHelp') end
                    function quest:NewScriptFrame() coroutine.yield(); return true end
                    WaspIntro = function() end
                    helper_E12F20 = WaspIntro
                    helper_E13310 = function() flags.healthPhase = true end
                    local co = coroutine.create(function() DoMission(quest) end)
                    local function resume() local ok, err = coroutine.resume(co); assert(ok, err) end
                    resume()
                    assert(#spawns == 5 and not flags.QueenHornetAttacks)
                    for i = 1, 5 do
                        assert(spawns[i][1] == 'MOD_DRONE')
                        assert(spawns[i][2] == 'QueenDepositPos' .. i)
                        assert(spawns[i][3] == 'HornetDrone')
                    end
                    alive = false
                    resume()
                    assert(#spawns == 6 and flags.QueenHornetAttacks)
                    assert(spawns[6][1] == 'MOD_QUEEN' and spawns[6][2] == 'MK_WQ_STARTING')
                    assert(spawns[6][3] == 'QueenHornet')
                    resume()
                    assert(flags.healthPhase and coroutine.status(co) == 'dead')
                ''')

    def test_queen_cutscene_faces_the_retained_queen_handle(self):
        lua = self.runtime()
        lua.execute(r'''
            local hero, queen, facing = {}, {}, false
            local noop = function() end
            local resources = setmetatable({}, {__index = function() return noop end})
            local quest = setmetatable({}, {__index = function() return noop end})
            function quest:GetHero() return hero end
            function quest:RetailResources() return resources end
            function quest:GetStateBool() return true end
            function quest:IsActiveThreadTerminating() return false end
            function quest:GetThingWithScriptName(name) assert(name == 'QueenHornet'); return queen end
            function quest:EntitySetFacingAngleTowardsThing(actor, target, snap)
                assert(actor == hero and target == queen and snap == true)
                facing = true
            end
            WatchForCutscene(quest)
            assert(facing)
        ''')
