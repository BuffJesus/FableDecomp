"""Save/load each entity callback affected by the ownership correction."""
import os
from pathlib import Path
import re

import pytest
from lupa.lua54 import LuaRuntime
from tools.script_recovery.script_units import UNITS

ROOT = Path(__file__).resolve().parents[2]
CASES = [('arena', 'Arena/Entities/ArenaCellDoorGuard.lua'),
 ('arena', 'Arena/Entities/ArenaCellDoorGuard2.lua'),
 ('arena', 'Arena/Entities/Flick.lua'),
 ('arena', 'Arena/Entities/Needle.lua'),
 ('arena', 'Arena/Entities/Roth.lua'),
 ('arena', 'Arena/Entities/Shadow.lua'),
 ('bandit_camp', 'BanditCampBossBattle/Entities/BCGameMaster.lua'),
 ('bandit_camp', 'BanditCampBossBattle/Entities/BanditForger.lua'),
 ('bordello', 'V_Bordello/Entities/BordelloClient.lua'),
 ('bordello', 'V_Bordello/Entities/BordelloLady.lua'),
 ('bordello', 'V_Bordello/Entities/Madame.lua'),
 ('bordello', 'V_Bordello/Entities/Magicman.lua'),
 ('chicken_kicking', 'V_ChickenKicking/Entities/ChickenMaster.lua'),
 ('guild_training', 'GuildTraining/Entities/AppleGirl.lua'),
 ('guild_training', 'GuildTraining/Entities/ApprenticeSpeedTest.lua'),
 ('guild_training', 'GuildTraining/Entities/BirdKiller.lua'),
 ('heros_old_house', 'HerosOldHouse/Entities/FishermansWife.lua'),
 ('heros_old_house', 'HerosOldHouse/Entities/GhostFisherman.lua'),
 ('sick_child', 'V_SickChild/Entities/IngredientOwner.lua'),
 ('sick_child', 'V_SickChild/Entities/ManInLove.lua'),
 ('sick_child', 'V_SickChild/Entities/MansLover.lua'),
 ('sick_child', 'V_SickChild/Entities/SickChildsSister.lua'),
 ('sick_child', 'V_SickChild/Entities/Witch.lua'),
 ('sick_child', 'V_SickChild/Entities/WomanToAttract.lua'),
 ('trader_escort', 'TraderEscort/Entities/DarkwoodTrader.lua'),
 ('wasp_boss', 'WaspBoss/Entities/WaspHelper.lua')]


@pytest.mark.parametrize('unit,relative', CASES)
@pytest.mark.parametrize('stage', ['draft', 'readable'])
def test_entity_save_load_uses_local_state_and_preserves_types(unit, relative, stage):
    scratch = os.environ.get('ENTITY_PERSIST_CORPUS')
    if scratch:
        base = Path(scratch) / ('ab/comparison/after' if stage == 'draft' else 'readable') / unit
    else:
        package = ROOT / 'refs/script_recovery/lifted' / UNITS[unit]['package']
        selected = 'readable_converter' if stage == 'readable' and (package / 'readable_converter').exists() else stage
        base = package / selected
    source = (base / 'FSE' / relative).read_text()
    callback = re.search(r'^function OnPersist\([^\n]*\)\n.*?(?=^function |\Z)', source, re.M | re.S)[0]
    transfers = re.findall(r'PersistTransfer(Bool|Int)\(context, "([^"]+)"', callback)
    assert transfers
    runtimes = []
    for instance in range(2):
        lua = LuaRuntime()
        lua.execute('package.preload["' + relative.split('/')[0] + '.native_quest_helpers"] = function() return {} end')
        lua.execute(source)
        lua.globals().expected = lua.table_from(dict((key, kind) for kind, key in transfers))
        lua.globals().instance = instance
        lua.execute("""
            local ctx, me = {}, {}
            local reading, values, saved = true, {}, {}
            local quest = {}
            local function transfer(kind, context, key, value)
                assert(context == ctx and expected[key] == kind)
                -- This corpus fixture starts at load, without running each
                -- script's gameplay Init. Allow an uninitialized input only
                -- while loading; saved values must always have native types.
                if not reading or value ~= nil then
                    assert(type(value) == (kind == 'Bool' and 'boolean' or 'number'))
                    if kind == 'Int' then assert(math.type(value) == 'integer') end
                end
                if reading then return values[key] end
                saved[key] = value
                return value
            end
            function quest:PersistTransferBool(...) return transfer('Bool', ...) end
            function quest:PersistTransferInt(...) return transfer('Int', ...) end
            -- No quest GetState/SetState methods: persistence must use entity state.
            function roundtrip(round)
                for key, kind in pairs(expected) do
                    if kind == 'Bool' then values[key] = (round + instance) % 2 == 1
                    else values[key] = (round == 1 and -1 or 1234567) + instance end
                end
                reading = true
                OnPersist(quest, me, ctx)
                reading = false
                saved = {}
                OnPersist(quest, me, ctx)
                local count = 0
                for key, value in pairs(values) do assert(saved[key] == value); count = count + 1 end
                local actual = 0; for _ in pairs(saved) do actual = actual + 1 end
                assert(count == actual)
            end
            -- The callback and the entity's runtime functions must share an
            -- actual closure cell, not just independently named locals.
            local shared = false
            for i = 1, 100 do
                local name = debug.getupvalue(OnPersist, i)
                if not name then break end
                if name ~= '_ENV' then
                    for _, fn in ipairs({Main, Init}) do
                        for j = 1, 100 do
                            local other = debug.getupvalue(fn, j)
                            if not other then break end
                            if debug.upvalueid(OnPersist, i) == debug.upvalueid(fn, j) then shared = true end
                        end
                    end
                end
            end
            assert(shared, 'callback is disconnected from Main/Init state')
            roundtrip(1)
        """)
        runtimes.append(lua)
    for lua in reversed(runtimes):
        lua.execute('roundtrip(2); roundtrip(3)')
