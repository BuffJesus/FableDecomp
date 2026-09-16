"""Lua state boundaries around the independently verified native gift tail."""
import unittest
from pathlib import Path
from lupa.lua54 import LuaRuntime


class TheresaGiftCommitTests(unittest.TestCase):
    def test_main_local_flag_precedes_info_clear_but_follows_objective(self):
        for failure in ('', 'update', 'clear'):
            lua = LuaRuntime()
            commit = lua.execute(Path(__file__).with_name('theresa_gift_commit.lua').read_text())
            make = lua.eval('''function(failure)
                local events, progress = {}, {givenChocolates=false}
                local quest = {SetStateBool=function(_,key,value)
                    assert(key=='GivenTheresaChocs' and value==true)
                    table.insert(events,'persistent')
                end}
                local resources = {
                    TakeTheresaChocolatesAndUpdateObjective=function()
                        assert(not progress.givenChocolates)
                        table.insert(events,'update')
                        if failure=='update' then error('update failed') end
                    end,
                    ClearTheresaInformation=function(_,actor)
                        assert(actor==17 and progress.givenChocolates)
                        table.insert(events,'clear')
                        if failure=='clear' then error('clear failed') end
                    end
                }
                return {quest=quest,resources=resources,progress=progress,events=events}
            end''')
            state = make(failure)
            if failure:
                with self.assertRaisesRegex(Exception, failure+' failed'):
                    commit(state.quest, 17, state.resources, state.progress)
            else: commit(state.quest, 17, state.resources, state.progress)
            self.assertEqual(state.progress.givenChocolates, failure != 'update')
            self.assertEqual(list(state.events.values()), ['persistent','update'] + ([] if failure=='update' else ['clear']))
