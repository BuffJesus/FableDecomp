"""A parent handle copy requires matching fields, lifetime operations and source type."""
import json
from pathlib import Path
import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.convert_quest_unit import UnitConverter
from tools.script_recovery.build_readable_unit import readable_file
from tools.script_recovery.native_evidence_lowering import fold_parent_thing_copies, normalise_typed_decompile

ROOT = Path(__file__).resolve().parents[2]


@pytest.fixture(scope='module')
def source():
    functions = json.loads((ROOT/'refs/script_recovery/arena/translation_unit_typed.json').read_text())['functions']
    fn = next(f for f in functions if int(f['address'], 16) == 0xf19bb0)
    return normalise_typed_decompile(fn['decompile'])


def test_arena_village_member_is_recovered_from_complete_copy(source):
    out = fold_parent_thing_copies(source, {0xc0: 'CellsVillage'})
    assert 'QUESTTHING_Set("CellsVillage", pCVar6);' in out
    assert 'if (**(int **)(iVar11 + 0xc8) == 0)' not in out
    # A later new string lifetime in the same stack slot must remain.
    assert 'CCharString::CCharString((CCharString *)&CStack_a8,"GuardingDoorMarkerLeft",-1);' in out


@pytest.mark.parametrize('before,after', [
    ('(iVar11 + 0xc4) = CStack_a8', '(iVar11 + 0xc0) = CStack_a8'),
    ('operator_delete(*(void **)(iVar11 + 0xc8))', 'operator_delete(*(void **)(iVar11 + 0xcc))'),
    ('*piVar1 = *piVar1 + 1', '*piVar1 = *piVar1 + 2'),
    ('CScriptThing *pCVar6;', 'Unknown *pCVar6;'),
])
def test_inconsistent_evidence_is_not_folded(source, before, after):
    assert before in source
    changed = source.replace(before, after)
    assert fold_parent_thing_copies(changed, {0xc0: 'CellsVillage'}) == changed


def test_unknown_member_and_escaping_pointer_are_not_folded(source):
    assert fold_parent_thing_copies(source, {}) == source
    changed = source.replace('  auStack_a4._0_4_', '  UseRawPointer(piVar1);\n  auStack_a4._0_4_', 1)
    assert fold_parent_thing_copies(changed, {0xc0: 'CellsVillage'}) == changed


@pytest.fixture(scope='module')
def guard_source(tmp_path_factory):
    evidence = ROOT/'refs/script_recovery/arena'
    unit = json.loads((evidence/'units/Q_Arena.json').read_text())
    out = tmp_path_factory.mktemp('arena_village_copy')
    UnitConverter(evidence/'translation_unit_typed.json').convert(unit, out)
    return (out/'FSE/Arena/Entities/ArenaCellDoorGuard2.lua').read_text()


@pytest.mark.parametrize('readable', [False, True])
def test_generated_guard_stores_village_before_waiting_and_terminates_cleanly(guard_source, readable):
    source = readable_file(guard_source)[0] if readable else guard_source
    lua = LuaRuntime()
    runner = lua.execute('''return function(source)
        local me, village, hero, quest, resources = {}, {}, {}, {}, {}
        local frame, saved, lookups = 0, nil, 0
        function quest:RetailResources() return resources end
        function resources:NewResource() return {} end
        function quest:GetHero() return hero end
        function quest:GetNearestWithDefName(relative, name)
            assert(relative == me and name == "VILLAGE_ARENA_CELLS")
            lookups = lookups + 1; return village
        end
        function quest:SetStateThing(name, value)
            assert(frame == 0 and name == "CellsVillage" and value == village)
            saved = value
        end
        function quest:GetStateInt(name) assert(name == "ArenaState"); return 2 end
        function quest:NewScriptFrame(entity)
            assert(entity == me and saved == village); frame = frame + 1; return false
        end
        function quest:IsActiveThreadTerminating() return frame >= 1 end
        function quest:EntitySetOpinionReactionsEnabled(entity, enabled) assert(entity == me and not enabled) end
        function quest:EntitySetInFaction(entity, faction) assert(entity == me and faction == "FACTION_HERO") end
        function quest:EntitySetAsKillable(entity, a, b) assert(entity == me and not a and not b) end
        function quest:EntitySetAsToAddToComboMultiplierWhenHit(entity, enabled) assert(entity == me and not enabled) end
        assert(load(source))(); Main(quest, me)
        return lookups == 1 and frame == 1 and saved == village
    end''')
    assert runner(source)
